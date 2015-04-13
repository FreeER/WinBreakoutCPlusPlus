/**
*
*   WinBreakoutC++
*
*   main.cpp
*
*   2015 a collaboration of CS50x students
*
*/

#include "breakout.h"
#include "resource.h"

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nCmdShow)
{
    MSG messages;  /* Here messages to the application are saved */

    /**
     *   create the main window width, height
     ******************************/
    HWND hwnd = createMainWin(hThisInstance, 330, 500);

    /** if main window handle invalid alert and quit  */
    if(!hwnd){
        MessageBox(NULL, " can not create main window ",
                         " WinBreakoutC++ ", MB_OK);
        return 0;
    }

    /** Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /** Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /** The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

// TODO (#1#): fix this

    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint (hwnd, &ps);
            TextOut (hdc,
                     // Location of the text
                     80, // x
                     40, // y
                     // Text to print
                     "this is the main window...",
                     // Size of the text in characters
                     23);
            EndPaint (hwnd, &ps);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
