#include "SysTypes.r"
#include "Types.r"

// 31 flags
#define AllItems    0b1111111111111111111111111111111
#define NoItems     0b0000000000000000000000000000000
#define MenuItem1   0b0000000000000000000000000000001
#define MenuItem2   0b0000000000000000000000000000010
#define MenuItem3   0b0000000000000000000000000000100
#define MenuItem4   0b0000000000000000000000000001000

#define MENU_BAR_ID         128
#define MENU_APPLE          128
#define MENU_FILE           129

#define WINDOW_ABOUT_ID     128

// ----------------------------------------------------------------------------
// Menu resources

resource 'MBAR' (MENU_BAR_ID, preload) {
    { MENU_APPLE, MENU_FILE };
};

resource 'MENU' (MENU_APPLE, preload) {
    MENU_APPLE, textMenuProc,
    AllItems & ~MenuItem2,	// Disable separator line, enable About Box and DAs
    enabled, apple,
    {
        "About Pinakes 0.0.0a", noicon, nokey, nomark, plain;
        "-",                    noicon, nokey, nomark, plain
    }
};

resource 'MENU' (MENU_FILE, preload) {
    MENU_FILE, textMenuProc,
    AllItems,
    enabled, "File",
    {
        "Quit",                 noicon, "Q", nomark, plain
    }
};

// ----------------------------------------------------------------------------
// Windows

resource 'DLOG' (WINDOW_ABOUT_ID, purgeable) {
    {31, 6, 224, 265},
    altDBoxProc,
    visible,
    noGoAway,
    0x0,						// No refCon
    WINDOW_ABOUT_ID,
    "",							// No window title
    1
};

resource 'DITL' (WINDOW_ABOUT_ID, purgeable) {
    {
        // Item 1
        {154,  80, 175, 180}, Button { enabled, "OK" },
        // Item 2
        {  4,  68,  38, 193}, StaticText { disabled, "Pinakes 0.0.0a1\rby Giancarlo Mariot" },
        // Item 3
        { 86,  11, 102, 250}, StaticText { disabled, "Copyright - 2024 Giancarlo Mariot." },
        // Item 4
        { 44, 114,  76, 146}, Icon { disabled, 128 },
        // Item 5
        {107,  43, 133, 217}, StaticText { disabled, "Written in Retro68" }
    }
};
