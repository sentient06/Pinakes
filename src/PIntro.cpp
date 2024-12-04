#include <QuickDraw.h>
#include <MacWindows.h>
#include <Sound.h>

#include "PIntro.h"
#include "../main.h"

namespace PIntro {
    /**
     * Shows a window.
     */
    void OpenIntroWindow(void) {
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
}
