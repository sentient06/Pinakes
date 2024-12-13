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

typedef void (*FuncPtr)(void);

namespace PMenu {
  using namespace std;

  void menuAction_null();

  void menuAction_Apple(short theItem);

  void menuAction_FileNew();
  void menuAction_FileOpen();
  void menuAction_FileClose();
  void menuAction_FileQuit();

  map<short, map<short, FuncPtr> > menuActions;

  /**
   * @brief Gets the menu item reference, but also populates the gaps with a null callback.
   */
  FuncPtr& getMenuReference(short menu, short item) {
    // Check if the menu exists:
    if (menuActions.find(menu) == menuActions.end()) {
      menuActions[menu] = map<short, FuncPtr>();
    }

    // Check if the item exists in the menu:
    map<short, FuncPtr>& itemMap = menuActions[menu];
    if (itemMap.find(item) == itemMap.end()) {
      itemMap[item] = menuAction_null; // Assign the default action
    }

    return itemMap[item]; // Return the reference
  }

  /**
   * @brief Initialise menu item actions.
   */
  void InitMenuHandlers() {
    getMenuReference(MENU_FILE, MENU_NEW)   = menuAction_FileNew;
    getMenuReference(MENU_FILE, MENU_OPEN)  = menuAction_FileOpen;
    getMenuReference(MENU_FILE, MENU_CLOSE) = menuAction_FileClose;
    getMenuReference(MENU_FILE, MENU_QUIT)  = menuAction_FileQuit;
  }

  /**
   * @brief Handle the menu click actions.
   * @param theMenu The short ID of the parent-most menu item.
   * @param theItem The short ID of the specific menu item.
   */
  void HandleMenuClick(short theMenu, short theItem) {
    if (theMenu == MENU_APPLE) {
      menuAction_Apple(theItem);
    } else {
      menuActions[theMenu][theItem]();
    }

    // Switch off highlighting on the menu just used:
    HiliteMenu(0);
  }

  // ----------------------------------------------------------------------------------------------
  // Menu actions:

  /**
   * @brief En empty action for items not meant to be clicked.
   */
  void menuAction_null() {}

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Apple

  /**
   * @brief Handler for items in the Apple menu.
   * @param theItem
   *          The short ID of the specific menu item.
   *          It's only important when it's the "about" item.
   *          Everything else is delegated to the Desk Accessory process.
   */
  void menuAction_Apple(short theItem) {
    unsigned char accName[255];
    short         itemHit;
    short         refNum;
    DialogPtr     theDialog;
    GrafPtr       savePort;

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
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // File

  /**
   * @brief File > New is just a prototype and opens a silly window.
   * @todo  Maybe drop this item.
   */
  void menuAction_FileNew() {
    PIntro::OpenIntroWindow();
  }

  /**
   * @brief File > Open is for opening a directory.
   * @todo  Hand the folder's ID to some Git manager to check for the files.
   */
  void menuAction_FileOpen() {
    // StandardGetFolder:
    StandardFileReply myReply;
    Point  where;
    Str255 message = "\pSelect a folder";
    // Position window at the centre of the screen:
    where.h = -1;
    where.v = -1;

    // Select folder:
    LFiles::StandardGetFolder(where, message, &myReply);
    if (myReply.sfGood) {
      PIntro::OpenIntroWindow();
    }
  }

  /**
   * @brief File > Close closes the currently focused window.
   */
  void menuAction_FileClose() {
    CloseWindow(FrontWindow());
  }

  /**
   * @brief File > Quit quits the application.
   */
  void menuAction_FileQuit() {
    gShouldQuit = TRUE;
  }
}