#include <Types.h>
#include <QuickDraw.h>
#include <Fonts.h>
#include <MacWindows.h>
#include <Memory.h>
#include <Devices.h>
#include <Events.h>
#include <ToolUtils.h>
#include <OSUtils.h>
#include <Sound.h>
#include <Resources.h>

#include "LFiles.h"

// ----------------------------------------------------------------------------

#define NIL             0L
#define IN_FRONT        (-1)
#define IS_VISIBLE      TRUE
#define NO_CLOSE_BOX    FALSE

#define MENU_BAR_ID     128 // ID for MBAR resource

#define MENU_APPLE      128 // Menu ID for Apple menu
#define MENU_ABOUT      1

#define MENU_FILE       129 // Menu ID for File menu
#define MENU_NEW        1
#define MENU_OPEN       2
// Open recent          3
#define MENU_CLOSE      4
// -------------------- 5
#define MENU_QUIT       6

#define WINDOW_ABOUT_ID 128
#define WINDOW_INTRO_ID 129

// ----------------------------------------------------------------------------
// Globals

Boolean     gShouldQuit;

#define LONG_NAP       60L
#define NO_CURSOR       0L
#define CHAR_CODE_MASK 255

// ----------------------------------------------------------------------------
// Prototypes

Boolean PHandleMenuClick(long mResult);
Boolean PInit(void);

void POpenIntroWindow(void);
void PMainEventLoop(void);

// ----------------------------------------------------------------------------
// Functions

/**
 * Shows a window.
 */
void POpenIntroWindow(void) {
  WindowPtr     introWindow;
  
  introWindow = GetNewWindow(WINDOW_INTRO_ID, NIL, (WindowPtr) IN_FRONT);
  if (introWindow != NIL) {
    // Make the window visible
    ShowWindow(introWindow);
    // Optional: Select the window 
    SelectWindow(introWindow);
  } else {
    SysBeep(30);
  }
}

/**
 * Handle Menu: Handles menu clicks.
 */
Boolean PHandleMenuClick(long mResult) {
  unsigned char accName[255];
  short         itemHit;
  Boolean       quitApp;
  short         refNum;
  DialogPtr     theDialog;
  short         theItem, theMenu;	
  GrafPtr       savePort;

  // StandardGetFolder:
  StandardFileReply myReply;
  Point  where;
  Str255 message = "\pSelect a folder";

  // Centre:
  where.h = -1;
  where.v = -1;
  // -----------------

  // To be used...
  const char * fileName = "Macintosh:Documents";
  bool exists = false;
  bool isFile = false;
  bool isDir  = false;
  // -------------

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
        case MENU_NEW:
          POpenIntroWindow();
          break;

        case MENU_OPEN:
          // Select folder:
          LFiles::StandardGetFolder(where, message, &myReply);
          break;

        case MENU_CLOSE:
          CloseWindow(FrontWindow());
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

void PHandleMouseDownEvent(EventRecord *eventStrucPtr) {
  WindowRef      windowRef;
  WindowPartCode partCode;

  partCode = FindWindow(eventStrucPtr->where, &windowRef);

  switch(partCode) {
    case inSysWindow:
      SystemClick(eventStrucPtr, windowRef);
      break;

    case inMenuBar:
      gShouldQuit = PHandleMenuClick(MenuSelect(eventStrucPtr->where));
      break;

    case inDrag: {
        Rect greyRegionRect = (*GetGrayRgn())->rgnBBox; // Create a local copy of the Desktop rect
        DragWindow(windowRef, eventStrucPtr->where, &greyRegionRect);
      }
      break;

    case inGoAway:
      if (TrackGoAway(windowRef, eventStrucPtr->where) == TRUE) {
        DisposeWindow(windowRef);
      }
      break;

    case inContent:
      if (windowRef != FrontWindow()) {
        SelectWindow(windowRef);
      } else {
        // PHandleInContentEvent(eventStrucPtr);
      }
      break;
  }
}

void PHandleEvents(EventRecord *eventStrucPtr) {
  switch(eventStrucPtr->what) {
    case mouseDown:
      PHandleMouseDownEvent(eventStrucPtr);
      break;

    // Key down:
    case keyDown:
    case autoKey:
      if ((eventStrucPtr->modifiers & cmdKey) != 0) {
        gShouldQuit = PHandleMenuClick(
          MenuKey((char) (eventStrucPtr->message & CHAR_CODE_MASK))
        );
      }
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Update the window:
    case updateEvt:
    // Activate event:
    case activateEvt:
      // gWhichWindow = (WindowPtr) eventStrucPtr->message;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    default:
      break;
  }
}

/**
 * Main Event Loop: Handle all system events.
 */
void PMainEventLoop(void) {
  EventRecord eventStructure;
  Point where;

  // Clear out leftover events:
  FlushEvents(everyEvent, 0);

  gShouldQuit = FALSE;

  do {
    if (WaitNextEvent(everyEvent, &eventStructure, LONG_NAP, NO_CURSOR)) {
      PHandleEvents(&eventStructure);
    } else // Null event
      ;    // Do idle or background stuff here

  } while (gShouldQuit == FALSE);
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