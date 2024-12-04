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

#include "Main.h"
#include "LFiles.h"
#include "src/PMenu.h"

// ----------------------------------------------------------------------------
// Globals

Boolean     gShouldQuit;

// ----------------------------------------------------------------------------
// Prototypes

void PHandleMenuClick(long mResult);
Boolean PInit(void);

void POpenIntroWindow(void);
void PMainEventLoop(void);

// ----------------------------------------------------------------------------
// Functions

/**
 * Handle Menu: Handles menu clicks.
 */
void PHandleMenuClick(long mResult) {
  unsigned char accName[255];
  short         itemHit;
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

  theMenu = HiWord(mResult);
  theItem = LoWord(mResult);

  PMenu::HandleMenuClick(theMenu, theItem);
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
      PHandleMenuClick(MenuSelect(eventStrucPtr->where));
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
        PHandleMenuClick(
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