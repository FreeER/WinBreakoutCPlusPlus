/**
*
*   WinBreakoutC++
*
*   breakout.h
*
*   2015 a collaboration of CS50x students
*
*/

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

// #define _WIN32_WINNT 0x0500

/**   declare timer ID constant   **/
#define ID_TIMER 1

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "resource.h"
#include "faiza.h"
#include "vectors.h"

/**   global rectangle structures               **/
/**   for our window, ball, bricks and paddle   **/
extern RECT mwinRect;

/**   global POINTS structure to tack mouse position   **/
extern POINTS mpoint_x;

/** structure for ball information size, position, speed  **/
typedef struct ball
{
    int   sizeDia;     //  size diameter
    v2    position;
    v2    velocity;
    rect2 boundingbox;
} ball;

typedef struct rectangle
{
    rect2    boundingbox;
    COLORREF pen;
    COLORREF brush;
    rectangle() {};
    rectangle(rect2 bb, COLORREF _pen, COLORREF _brush)
    {
        boundingbox = bb;
        pen   = _pen;
        brush = _brush;
    }
} rectangle;

/**    windows procedure   **/
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/**   createMainWin() returns a window handle
      winHandle = createMainWin(thisInstance, width, height)   **/
HWND createMainWin(HINSTANCE, int, int);

/**   drawText()  returns void
      drawText(window handle, text to paint)   **/
void drawText(HDC, LPCTSTR, COLORREF color = NULL);

/**   drawball() returns void   **/
void drawball(HDC hdc, COLORREF pen = NULL, COLORREF color = NULL);

/**   drawPaddle() returns void   **/
void drawPaddle(HDC hdc, COLORREF pen = NULL, COLORREF fill = NULL);

/**   refreshWindow() returns void
      refreshWindow(main window handle, optional text or NULL  **/
void refreshWindow(HWND hwnd, LPCTSTR lpOptionalText = NULL);

/**   createBall() returns void
      createBall(ball size, position x, position y)   **/
void createBall(int, int, int);

/**   updateGame() returns void
      updateGame(main window handle)  **/
void updateGame(HWND);

/**   handles collisions of ball   **/
void handleCollisions(HWND);

/**   detects collisions of two rectangles   **/
bool detectCollision(rect2 a, rect2 b);

/**   makePaddle() returns void
      makePaddle(position y, length, height)   **/
void createPaddle(int, int, int);

/**   paddle tracks mouse using POINTS structure   **/
void updatePaddle();

/**   add rectangles to the window's update region   **/
void setUpdateRegion(HWND);

/**   make a brick
      position x, position y, length, height, pen, brush   **/
rectangle* createRectangle(int, int, int, int, COLORREF, COLORREF);

/**  make the brick wall   **/
void createWall(int x);

/****/
void cleanUp();

/**   define some color constants for use in the game   **/
#define bO_RED RGB(255,0,0)
#define bO_BLUE RGB(0,0,255)
#define bO_GREEN RGB(0,255,0)
#define bO_WHITE RGB(255,255,255)
#define bO_DDGRAY RGB(50,50,50)
#define bO_BLACK RGB(1,1,1)    // because zero is zero

