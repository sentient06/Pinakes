#include <Types.h>
#include <QuickDraw.h>
#include <Fonts.h>
#include <Windows.h>
#include <Memory.h>
#include <Devices.h>
#include <Events.h>
#include <ToolUtils.h>
#include <OSUtils.h>
#include <Sound.h>

// ----------------------------------------------------------------------------

#define NIL             0L
#define IN_FRONT        (-1)
#define IS_VISIBLE      TRUE
#define NO_CLOSE_BOX    FALSE

#define MENU_BAR_ID     128 // ID for MBAR resource

#define MENU_APPLE      128 // Menu ID for Apple menu
#define MENU_ABOUT      1

#define MENU_FILE       129 // Menu ID for File menu
#define MENU_OPEN       1
#define MENU_QUIT       2

#define WINDOW_ABOUT_ID 128
#define WINDOW_INTRO_ID 129

// ----------------------------------------------------------------------------
// Globals

Boolean     gUserDone;
EventRecord gEvent;
WindowPtr   gWhichWindow;
short       gWindowCode;

#define LONG_NAP       60L
#define NO_CURSOR       0L
#define CHAR_CODE_MASK 255

// ----------------------------------------------------------------------------
// Prototypes

Boolean PInit(void);
int PInitBaseWindow();
void PMainEventLoop(void);
Boolean PHandleMenu(long mResult);

// ----------------------------------------------------------------------------
// Functions

/**
 * Handle Menu: Handles menu clicks.
 */
Boolean PHandleMenu(long mResult) {
  unsigned char accName[255];
  short         itemHit;
  Boolean       quitApp;
  short         refNum;
  DialogPtr     theDialog;
  WindowPtr     introWindow;
  short         theItem, theMenu;	
  GrafPtr       savePort;

  quitApp = FALSE;
  theMenu = HiWord(mResult);
  theItem = LoWord(mResult);

  switch(theMenu) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case MENU_APPLE:
      if (theItem == MENU_ABOUT) {
        theDialog = GetNewDialog(WINDOW_ABOUT_ID, NIL, (WindowPtr) IN_FRONT);
        if (theDialog != NIL) {
          ModalDialog(NIL, &itemHit);
          DisposeDialog(theDialog);
        } else {
          SysBeep(30);
        }
      } else {
        // It's a Desk Accessory:
        GetPort(&savePort);
        GetMenuItemText(GetMenuHandle(MENU_APPLE), theItem, accName);
        refNum = OpenDeskAcc(accName);
        SetPort(savePort);
      }
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case MENU_FILE:
      switch(theItem) {
        case MENU_OPEN:
          introWindow = GetNewWindow(WINDOW_INTRO_ID, NIL, (WindowPtr) IN_FRONT);
          if (introWindow != NIL) {
            // Make the window visible
            ShowWindow(introWindow);
            // Optional: Select the window 
            SelectWindow(introWindow);
          } else {
            SysBeep(30);
          }
          break;
        case MENU_QUIT:
          quitApp = TRUE;
          break;
      }
      break;
  }
  // Switch off highlighting on the menu just used:
  HiliteMenu(0);

  return quitApp;
}

/**
 * Init: Initialise libraries.
 */
Boolean PInit(void) {
  Handle theMenuBar;

  MaxApplZone();
  MoreMasters();
  MoreMasters();
  MoreMasters();
  MoreMasters();
  MoreMasters();
  MoreMasters();
  MoreMasters();
  MoreMasters();

  // Initialise the various managers
  InitGraf(&qd.thePort); // QuickDraw
  InitFonts();
  FlushEvents(everyEvent, 0); // Clears stray event queues
  InitWindows();
  InitMenus();
  TEInit();
  InitDialogs(NIL);

  // Got our menu resources OK?
  if ((theMenuBar = GetNewMBar(MENU_BAR_ID)) == NIL) {
    return FALSE;
  }

  SetMenuBar(theMenuBar); // Add our menus to menu list
  DisposeHandle(theMenuBar);
  AppendResMenu(GetMenuHandle(MENU_APPLE), 'DRVR'); // Build Apple menu
  DrawMenuBar();

  InitCursor();
  return TRUE;
}

/**
 * Init Base Window: Draw a hello world window that quits on click.
 */
int PInitBaseWindow() {
  WindowPtr thisWindow;
  Rect      windowRect;

  /* Set up the window */
  windowRect.top    = 40;
  windowRect.left   = 40;
  windowRect.bottom = 200;
  windowRect.right  = 300;

  thisWindow = NewWindow(
    NIL,
    &windowRect,
    "\pHello world",
    IS_VISIBLE,
    documentProc,
    (WindowPtr) IN_FRONT,
    NO_CLOSE_BOX,
    NIL
  );

  if (thisWindow != NIL) {
    SetPort(thisWindow);
    MoveTo(20, 20);
    DrawString("\pHello world");
    InitCursor();

    while(!Button())
      ;

    DisposeWindow(thisWindow);
  } else {
    SysBeep(30);
  }

  return 0;
}

/**
 * Main Event Loop: Handle all system events.
 */
void PMainEventLoop(void) {
  Point where;

  // Clear out leftover events:
  FlushEvents(everyEvent, 0);

  gUserDone = FALSE;

  do {
    if (WaitNextEvent(everyEvent, &gEvent, LONG_NAP, NO_CURSOR)) {
      switch(gEvent.what) {
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Mouse down:
        case mouseDown:
            gWindowCode = FindWindow(gEvent.where, &gWhichWindow);

            switch(gWindowCode) {
              case inSysWindow:
                SystemClick(&gEvent, gWhichWindow);
                break;
              case inMenuBar:
                gUserDone = PHandleMenu(MenuSelect(gEvent.where));
                break;
            }
          break;
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Key down:
        case keyDown:
        case autoKey:
          if ((gEvent.modifiers & cmdKey) != 0) {
            gUserDone = PHandleMenu(
              MenuKey((char) (gEvent.message & CHAR_CODE_MASK))
            );
          }
          break;
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Update the window:
        case updateEvt:
        // Activate event:
        case activateEvt:
          gWhichWindow = (WindowPtr) gEvent.message;
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        default:
          break;
      }
    } else // Null event
      ;    // Do idle or background stuff here

  } while (gUserDone == FALSE);
}

/**
 * Main: Point of entry for Pinakes.
 */
int main() {
  if (PInit()) {
    PMainEventLoop();
  } else {
    SysBeep(30);
  }
}