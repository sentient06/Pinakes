// C++ headers
#include <map>
#include <memory>

// Toolbox headers
#include <Devices.h>
#include <Sound.h>

// Project headers
#include "../main.h"
#include "LFiles.h"
#include "PMenu.h"
#include "PIntro.h"

// ----------------------------------------------------------------------------
// Constants

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

// ----------------------------------------------------------------------------
// Code

typedef void (*FuncPtr)(void);

namespace PMenu {
  using namespace std;

  map<short, map<short, FuncPtr> > menuActions;

  /**
   * Initialise the menu.
   */
  void InitMenuHandlers() {
    // menuActions[MENU_FILE][MENU_NEW]   = FooAction;
    // menuActions[MENU_FILE][MENU_OPEN]  = BarAction;
    // menuActions[MENU_FILE][MENU_CLOSE] = BazAction;
    
    // i = 0;
    // menuActions[EDIT_MENU][++i] = FooAction;
  }

  /**
   * Handle the menu click actions.
   */
  void HandleMenuClick(short theMenu, short theItem) {
    // Desk accessory:
    unsigned char accName[255];
    short         itemHit;
    short         refNum;
    DialogPtr     theDialog;
    GrafPtr       savePort;

    // StandardGetFolder:
    StandardFileReply myReply;
    Point  where;
    Str255 message = "\pSelect a folder";
    // Position window at the centre of the screen:
    where.h = -1;
    where.v = -1;

    // To be used...
    const char * fileName = "Macintosh:Documents";
    bool exists = false;
    bool isFile = false;
    bool isDir  = false;

    // New-style:
    // menuActions[theMenu][theItem]();

    // Old-style:
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
            PIntro::OpenIntroWindow();
            break;

          case MENU_OPEN:
            // Select folder:
            LFiles::StandardGetFolder(where, message, &myReply);
            if (myReply.sfGood) {
              PIntro::OpenIntroWindow();
            }
            break;

          case MENU_CLOSE:
            CloseWindow(FrontWindow());
            break;

          case MENU_QUIT:
            gShouldQuit = TRUE;
            break;
        }
        break;
    }
    // Switch off highlighting on the menu just used:
    HiliteMenu(0);
  }
}