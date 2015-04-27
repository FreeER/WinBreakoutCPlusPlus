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
#include "convenienceMacros.h"

HWND createMainWin(HINSTANCE hThisInstance, int sizeX, int sizeY);

/**   WinMain where we communicate with the system              **/
/**   after we define and instantiate our programs components   **/
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nCmdShow)
{
    UNREFERENCED(lpszArgument);
    UNREFERENCED(hPrevInstance);
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
      allows us to use printf and cout/printf to display data     **/
#ifdef _DEBUG
    if(!AllocConsole()){
        MessageBox(NULL, _T(" can not create console window "),
                         _T(" WinBreakoutC++ "), MB_OK);
    }
    freopen("CONOUT$","wb",stdout);  // reopen stout handle as console window output
#endif

    /**   get our windows rectangle so we can size things   **/
    GetClientRect(hwnd, &mwinRect);

    createBall(20, mwinRect.right / 2, mwinRect.bottom / 2);

    // TODO:  this needs to be from top with a re-sizable window
    createPaddle(mwinRect.bottom - 60, 40, 10);

    createWall(50);

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
        // when window is first created
        case WM_CREATE:
            // start timer running
            // with an ID of ID_TIMER (macro)
            // 20 millisecond intervals
            // and no callback function (thus we'll get a message instead)
            SetTimer(hwnd, ID_TIMER, 10, 0);
            break;
        // message from timer "ID_TIMER"
        case WM_TIMER:
            setUpdateRegion(hwnd); // add rectangle to update region
            updateGame(hwnd);     // do the game math update all rectangles
            UpdateWindow(hwnd);  // send WM_PAINT if update region is not empty
            break;
        // whenever the mouse moves over our window
        case WM_MOUSEMOVE:
            // store mouse position
            mpoint_x = MAKEPOINTS(lParam);
            // update paddle rectangle immediately
            updatePaddle();
            break;
        // sent when window is being closed
        case WM_DESTROY:
            cleanUp();
            PostQuitMessage(0);    // send a WM_QUIT a 0 to the message queue
            break;
        // the system message to paint the update region
        case WM_PAINT:
            // paint all rectangles to update region
            refreshWindow(hwnd);
            break;
        // for messages that we don't deal with use the default window procedure
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

/**   make the main window   **/
HWND createMainWin(HINSTANCE hThisInstance, int sizeX, int sizeY)
{

    /**   declare a class name. Why? Because.  **/
    TCHAR szClassName[ ] = _T("Student Project");

    /**   declare the main window handle   **/
    HWND hwnd = NULL;

    /**   declare data structure object variable for the window class   **/
    WNDCLASSEX wincl;

    /**   initialize the window structure   **/
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;   // This function is called by windows
    wincl.style = CS_DBLCLKS;     // Catch double-clicks
    wincl.cbSize = sizeof (WNDCLASSEX);

    /**  use default icon and mouse-pointer  **/
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);

    /**   no menu   **/
    wincl.lpszMenuName = NULL;

    /**   no extra bytes after the window class structure
          or the window instance                            */
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    /**   add a custom background color   */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    /**   Register the window class, if it fails quit the program   */
    if(!RegisterClassEx(&wincl))
        return 0;

    /**   the class is registered,
          let's create the programs main window   */
    hwnd = CreateWindowEx(
           0,                  // extended possibilities for variation
           szClassName,            // Class name
           _T(" WinBreakoutC++ "),   // Title Text
           WS_OVERLAPPEDWINDOW,     // default window style
           CW_USEDEFAULT,       // windows decides the position
           CW_USEDEFAULT,       // where the window ends up on the screen
           sizeX,             // the window width
           sizeY,             // and height in pixels
           HWND_DESKTOP,      // the window is a child-window to desktop
           NULL,              // no menu
           hThisInstance,     // the program instance handle
           NULL               // no window creation data
           );

    /**   if window creation successful return the main window handle
          any work in the window will need this handle                  **/
    if(hwnd)
        return hwnd;

    /**   if we get this far we have failed to create
          a window for our program, returning 0 will
          cause a message to the user and then quit the process.   **/

    return 0;
}

