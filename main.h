#include <Types.h>

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

#define LONG_NAP       60L
#define NO_CURSOR       0L
#define CHAR_CODE_MASK 255

// ----------------------------------------------------------------------------
// Globals

extern Boolean     gShouldQuit;
