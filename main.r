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
#define WINDOW_INTRO_ID     129
#define WINDOW_OPEN_FOLDER  2008

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
        "New...",               noicon, "N", nomark, plain;
        "Open...",              noicon, "O", nomark, plain;
        "Open recent",          noicon, nokey, nomark, plain;
        "Close",                noicon, "W", nomark, plain;
        "-",                    noicon, nokey, nomark, plain;
        "Quit",                 noicon, "Q", nomark, plain
    }
};

// ----------------------------------------------------------------------------
// Windows

resource 'DLOG' (WINDOW_ABOUT_ID, purgeable) {
    // 44 x 28 : 330 x 230
    {50, 50, 280, 380},
    noGrowDocProc,
    visible,
    noGoAway,
    0x0,						// No refCon
    WINDOW_ABOUT_ID,
    "",							// No window title
    1
};

resource 'WIND' (WINDOW_INTRO_ID, preload, purgeable) {
    {64, 60, 314, 460}, // initial window size and location
    zoomDocProc,        // window definition ID: 
                        // incorporates definition function 
                        //  and variation code
    invisible,          // window is initially invisible
    goAway,             // window has close box
    0x0,                // reference constant
    "untitled",         // window title
    staggerParentWindowScreen     
                        // optional positioning specification
};

resource 'DITL' (WINDOW_ABOUT_ID, purgeable) {
    {        
        // Item 1
        // Y    X    H    W
        //          21  100
        {154+18,  80+37, 175+18, 180+37}, Button { enabled, "OK" },
        // Item 2
        {  4+18,  68+37,  38+18, 193+37}, StaticText { disabled, "Pinakes 0.0.0a1\rby Giancarlo Mariot" },
        // Item 3
        //          16  239
        { 86+18,  11+37, 102+18, 250+37}, StaticText { disabled, "Copyright - 2024 Giancarlo Mariot." },
        // Item 4
        //          32   32
        { 44+18, 114+37,  76+18, 146+37}, Icon { disabled, 128 },
        // Item 5
        //          26  174
        {107+18,  43+37, 133+18, 217+37}, StaticText { disabled, "Written in Retro68" }
    }
};

/* Standard Get Folder */

/* Let's not use WINDOW_OPEN_FOLDER yet because 2008 is easy to find. */
/* Yes! These are the Rez definitions for StandardGetFolder!!! */

resource 'DLOG' (2008, "CustomGet-Select Directory", purgeable) {
	{0, 0, 226, 344},
	noGrowDocProc,
	invisible,
	noGoAway,
	0x0,
	2008,
	"",
	alertPositionMainScreen
};

resource 'DITL' (2008, "CustomGet-Select Directory", purgeable) {
	{	/* array DITLarray: 12 elements */
		/* [1] */
		{159, 252, 179, 332},
		Button {
			enabled,
			"Open"
		},
		/* [2] */
		{128, 252, 148, 332},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{-1, -1, -1, -1},
		HelpItem {
			disabled,
			HMScanhdlg {
				-6042
			}
		},
		/* [4] */
		{32, 235, 48, 337},
		UserItem {
			enabled
		},
		/* [5] */
		{56, 252, 76, 332},
		Button {
			enabled,
			"Eject"
		},
		/* [6] */
		{84, 252, 104, 332},
		Button {
			enabled,
			"Desktop"
		},
		/* [7] */
		{53, 12, 183, 230},
		UserItem {
			enabled
		},
		/* [8] */
		{31, 12, 50, 230},
		UserItem {
			enabled
		},
		/* [9] */
		{115, 251, 116, 333},
		Picture {
			disabled,
			11
		},
		/* [10] */
		{199, 35, 217, 210},
		Button {
			enabled,
			""
		},
		/* [11] */
		{7, 15, 23, 229},
		StaticText {
			disabled,
			""
		},
		/* [12] */
		{-1, -1, -1, -1},
		StaticText {
			disabled,
			"Select ^0"
		}
	}
};

resource 'dctb' (2008) {
	{	/* array ColorSpec: 1 elements */
		/* [1] */
		wContentColor, 56797, 56797, 56797
	}
};