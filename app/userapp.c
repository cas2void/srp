// INCLUDES ///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  // just say no to MFC

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <stdio.h>
#include <stdlib.h>
#include "assert_ig.h"
#include "malloc_ig.h"
#include "winport_srp.h"

// DEFINES ////////////////////////////////////////////////
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_BITS   32

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"

// FUNCTIONS //////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, 
                            UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
    // this is the main message handler of the system
    HDC hdc;
    PAINTSTRUCT ps;

    // what is the message 
    switch(msg)
    {   
    case WM_CREATE: 
    {
        // do initialization stuff here
        // return success
        return(0);
    } break;
  
    case WM_PAINT: 
    {
        // simply validate the window
        hdc = BeginPaint(hwnd,&ps);      
        // you would do all your painting here
        EndPaint(hwnd,&ps);

        // return success
        return(0);
    } break;

    case WM_DESTROY: 
    {
        // kill the application, this sends a WM_QUIT message 
        PostQuitMessage(0);

        // return success
        return(0);
    } break;

    default:break;

    } // end switch

    // process any messages that we didn't take care of 
    return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(     HINSTANCE hinstance,
                        HINSTANCE hprevinstance,
                        LPSTR lpcmdline,
                        int ncmdshow)
{

    WNDCLASSEX winclass; // this will hold the class we create
    HWND       hwnd;     // generic window handle
    MSG        msg;      // generic message
    DWORD      style, exStyle;
#ifdef FULLSCREEN
    DEVMODE dmScreenSettings;
#endif

    RECT		rcWindowRect;                       // Grabs Rectangle Upper Left / Lower Right Values
	  rcWindowRect.left	= (long)0;                // Set Left Value To 0
  	rcWindowRect.right	= (long)WINDOW_WIDTH;   // Set Right Value To Requested Width
  	rcWindowRect.top	= (long)0;                // Set Top Value To 0
	  rcWindowRect.bottom	= (long)WINDOW_HEIGHT;  // Set Bottom Value To Requested Height

    // first fill in the window class stucture
    winclass.cbSize         = sizeof(WNDCLASSEX);
    winclass.style          = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc    = WindowProc;
    winclass.cbClsExtra     = 0;
    winclass.cbWndExtra     = 0;
    winclass.hInstance      = hinstance;
    winclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    winclass.hCursor        = LoadCursor(NULL, IDC_ARROW); 
    winclass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName   = NULL;
    winclass.lpszClassName  = WINDOW_CLASS_NAME;
    winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

    // register the window class
    if (!RegisterClassEx(&winclass))
    {
        return(0);
    }

#ifdef FULLSCREEN
    {
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize         = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth    = WINDOW_WIDTH;
        dmScreenSettings.dmPelsHeight   = WINDOW_HEIGHT;
        dmScreenSettings.dmBitsPerPel   = WINDOW_BITS;
        dmScreenSettings.dmFields       = DM_BITSPERPEL | 
            DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != 
            DISP_CHANGE_SUCCESSFUL)
        {
            printf("Error: ChangeDisplaySettings Error.\n");
            return 0;
        }
        
        exStyle = WS_EX_APPWINDOW;
        style = WS_POPUP | WS_VISIBLE;
    }
#else
    {
        exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        style = WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE;
    }
#endif
    
    
    // Adjust Window To True Requested Size
    AdjustWindowRectEx(&rcWindowRect, style, FALSE, exStyle);

    // create the window
    if (!(hwnd = CreateWindowEx(exStyle,                  // extended style
                                WINDOW_CLASS_NAME,            // class
                                "SRP Demo",    // title
                                style,
                                0,0,                          // initial x,y
                                rcWindowRect.right - rcWindowRect.left,	// Calculate Window Width
		                            rcWindowRect.bottom - rcWindowRect.top,	// Calculate Window Height
                                NULL,          // handle to parent 
                                NULL,          // handle to menu
                                hinstance,     // instance of this application
                                NULL)))        // extra creation parms
    {
        return(0);
    }

    ShowWindow(hwnd, ncmdshow);
    UpdateWindow(hwnd);

    if(!SrpInit(hwnd, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BITS))
    {
        printf("SrpInit Error.\n");
        return(0);
    }

    // enter main event loop, but this time we use PeekMessage()
    // instead of GetMessage() to retrieve messages
    while(TRUE)
    {
        // test if there is a message in queue, if so get it
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        { 
            // test if this is a quit
            if (msg.message == WM_QUIT)
                break;
        
            // translate any accelerator keys
            TranslateMessage(&msg);

            // send the message to the window proc
            DispatchMessage(&msg);
        } // end if
    
        // main game processing goes here
        if (KEYDOWN(VK_ESCAPE))
        {
            SendMessage(hwnd, WM_CLOSE, 0,0);
        }

        SrpMain();
        
    } // end while

    SrpQuit(hwnd);

    // return to Windows like this
    return(msg.wParam);

} // end WinMain

///////////////////////////////////////////////////////////
