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

/**   WinMain where we communicate with the system              **/
/**   after we define and instantiate our programs components   **/
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nCmdShow)
{
    /**   here messages to the application are saved   **/
    MSG messages;

   /**********************************************
    *                                            *
    *   create the main window - width, height   *
    *                                            *
    **********************************************/
    HWND hwnd = createMainWin(hThisInstance, 400, 600);

    /**   if main window handle is invalid alert and quit  **/
    if(!hwnd){
        // using wide string for the Unicode crowd
        MessageBox(NULL, _T(" can not create main window "),
                         _T(" WinBreakoutC++ "), MB_OK);

        /**   this in itself will end the program   **/
        PostQuitMessage(0);
    }

    /**   make the window visible on the screen   **/
    ShowWindow(hwnd, nCmdShow);

/**   some debugging code   **/
/**   with the following statements a Debug build target
      allows us to use printf and cout to display data     **/
#ifdef _DEBUG
    if(!AllocConsole()){
        MessageBox(NULL, _T(" can not create console window "),
                         _T(" WinBreakoutC++ "), MB_OK);
    }
    freopen("CONOUT$","wb",stdout);  // reopen stout handle as console window output
#endif

    /**   get our windows rectangle so we can size things   **/
    GetClientRect(hwnd, &mwinRect);

   /********************************************
    *   make the ball                          *
    *   size diameter, center x and center y   *
    ********************************************/
    createBall(20, mwinRect.right / 2, mwinRect.bottom / 2);

   /**********************************
    *   make the paddle              *
    *   y position, length, height   *
    **********************************/
    // TODO:  this needs to be from top with a re-sizable window
    createPaddle(mwinRect.bottom - 60, 40, 10);

   /***************************
    *   make the brick wall   *
    ***************************/
    createWall(0);

    /**   run the message loop                        **/
    /**   It will run until GetMessage() returns 0.   **/
    /**   argument two is null we are listening to    **/
    /**   the thread not the window.  we get all      **/
    /**   messages.  this loop is the heart of our    **/
    /**   program pumping messages not blood          **/

    /**   messages is MSG structure that windows passes       **/
    /**   messages to our program defined in top of WinMain   **/
    while(GetMessage(&messages, NULL, 0, 0))
    {
        /**   Translate virtual-key messages into character messages   **/
        TranslateMessage(&messages);

        /**   Send message to WindowProcedure   **/
        DispatchMessage(&messages);
    }

    /**   The program return-value is 0                   **/
    /**   - The value that PostQuitMessage() gave was 0   **/
    /**   that's all folks                                **/
    return messages.wParam;
}

/**   this function is called by the Windows function         **/
/**   DispatchMessage() inside the the main loop in WinMain   **/

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message,
                                 WPARAM wParam, LPARAM lParam)
{
    /**   process system messages   **/
    switch (message)
    {
        case WM_TIMER:    // message from timer "ID_TIMER"
            setUpdateRegion(hwnd);    // add rectangle to update region
            updateGame(hwnd);    // do the game math update all rectangles
            UpdateWindow(hwnd);    // send WM_PAINT if update region is not empty
            break;
        case WM_CREATE:
            SetTimer(hwnd, ID_TIMER, 20, 0);    // set timer running
            break;
        case WM_MOUSEMOVE:    // whenever the mouse moves over our window
            mpoint_x = MAKEPOINTS(lParam);    // store mouse position
            updatePaddle();    // update paddle rectangle immediately
            break;
        case WM_DESTROY:    // this turns off our program
            cleanUp();
            PostQuitMessage(0);    // send a WM_QUIT a 0 to the message queue
            break;
        /**   whenever some part of a window needs updating   **/
        case WM_PAINT:    // the system message to paint the update region
            refreshWindow(hwnd);    // paint all rectangles to update region
            break;
        default:      // for messages that we don't deal with
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
