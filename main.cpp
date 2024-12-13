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
 * @brief Handles menu clicks.
 */
void PHandleMenuClick(long mResult) {
  short         theItem, theMenu;	

  theMenu = HiWord(mResult);
  theItem = LoWord(mResult);

  PMenu::HandleMenuClick(theMenu, theItem);
  // Small and neat!
}

/**
 * @brief Initialise libraries.
 */
Boolean PInit(void) {
  Handle theMenuBar;

  MaxApplZone(); // 1
  MoreMasters(); // 2
  MoreMasters(); // 3
  MoreMasters(); // 4
  MoreMasters(); // 5
  MoreMasters(); // 6
  MoreMasters(); // 7
  MoreMasters(); // 8
  MoreMasters(); // 9 times!

  // Initialise the various managers:
  InitGraf(&qd.thePort); // QuickDraw
  InitFonts();
  FlushEvents(everyEvent, 0); // Clears stray event queues
  InitWindows();
  InitMenus();
  TEInit(); // Text Edit
  InitDialogs(NIL);

  // Got our menu resources OK?
  if ((theMenuBar = GetNewMBar(MENU_BAR_ID)) == NIL) {
    return FALSE;
  }

  PMenu::InitMenuHandlers(); // Initialise menu actions
  SetMenuBar(theMenuBar);    // Add the menus to menu list
  DisposeHandle(theMenuBar);
  AppendResMenu(GetMenuHandle(MENU_APPLE), 'DRVR'); // Build Apple menu
  DrawMenuBar();

  InitCursor();
  return TRUE;
}

/**
 * @brief Handles mouse click events.
 */
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

/**
 * @brief Handles actions from system events.
 * @param eventStrucPtr Pointer to event struct.
 */
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
 * @brief Handles all system events.
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
 * @brief Point of entry for Pinax.
 */
int main() {
  if (PInit()) {
    PMainEventLoop();
  } else {
    SysBeep(30);
  }
}