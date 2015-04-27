/**
 *
 *   WinBreakoutC++
 *
 *   breakout.cpp
 *
 *   2015 a collaboration of CS50x students
 *
 */

/**   include our programs declarations   **/
#include "breakout.h"
#include <iostream>
#include "convenienceMacros.h"
#include "assert.h"

/**   define some global object variables   **/
ball ballInfo;        // persistent ball information
RECT mwinRect;        // main window rectangle
RECT ballRect;        // ball rectangle
RECT ballRectInvaild; // ball rectangle for future and past positions
                      // needed to redraw only where needed
RECT textRect;        // text rectangle

/**   the brink array   **/
// TODO: quit taking stuff from main or use dynamic array
#define NUMBRICKS 50
rectangle* bricks[NUMBRICKS];
int numBricks = 0;

/** making paddle a pointer to heap memory       *
 ** is pointless but demonstrative               *
 ** we get to use pointy -> syntax and will need *
 ** free the memory                              */
rectangle paddle;

POINTS mpoint_x;  // POINTS structure to relay mouse data to paddle

/** need to keep track of paddle length beyond it's creation **
** maybe a paddle info structure like ball, but for now      **/
int paddlelength = 0;

void playSound(int sound)
{
    switch(sound)
    {
        case 1:
            PlaySound(MAKEINTRESOURCE(IDW_MYBALLSOUND),
                    GetModuleHandle(NULL), SND_ASYNC|SND_RESOURCE);
            break;
        default:;
    }
}

void createBall(int sizeDia, int x, int y)
{
    /**   set up the ball rectangles   **/
    int halfDia        = sizeDia / 2;
    ballRect.left      = x - halfDia;
    ballRect.top       = y - halfDia;
    ballRect.right     = x + halfDia;
    ballRect.bottom    = y + halfDia;
    ballInfo.sizeDia   = sizeDia;
    ballInfo.position.x = ballRect.left;
    ballInfo.position.y = ballRect.top;

    srand(GetTickCount());

    /**   provide a random values to the ball velocity   **/
    // TODO: this can result in zero
    ballInfo.velocity.x = randVlo('x');
    ballInfo.velocity.y = randVlo('y');

    /**   play a starting sound / a natural pause   **/
    playSound(1);
}

void createPaddle(int posy, int length, int height)
{
    /**   preserve the length of the paddle   **/
    paddlelength = length;

    /**   load paddle rectangle  **/
    paddle.boundingbox.lefttop.x = mwinRect.right / 2 - length / 2;
    paddle.boundingbox.lefttop.y = posy;
    paddle.boundingbox.rightbottom.x = paddle.boundingbox.lefttop.x + length;
    paddle.boundingbox.rightbottom.y  = paddle.boundingbox.lefttop.y + height;
}

rectangle* createRectangle(int x, int y, int length, int height,
        COLORREF pen, COLORREF brush)
{
    rect2 bb = rect2(x, y, x+length, y+height);
    rectangle* abrick = new rectangle(bb, pen, brush);

    return abrick;
}

void createWall(int x)
{
    for(int i = 0; i < x; i++, numBricks++)
    {
        assert(numBricks < NUMBRICKS);
        int32_t y = 30 + 15 * (i / 10);
        int32_t x = 10 + 35 * (i % 10);
        bricks[i] = createRectangle(x, y, 30, 10, bO_DDGRAY, bO_RED);
    }
}

void updatePaddle()
{
    /**   alter paddle position from mouse position   **/
    paddle.boundingbox.lefttop.x = mpoint_x.x - paddlelength / 2;
    paddle.boundingbox.rightbottom.x = paddle.boundingbox.lefttop.x + paddlelength;
    if (paddle.boundingbox.lefttop.x < 0)
    {
        paddle.boundingbox.lefttop.x = 0;
        paddle.boundingbox.rightbottom.x =
            paddle.boundingbox.lefttop.x + paddlelength;
    } else if (paddle.boundingbox.rightbottom.x > mwinRect.right)
    {
        paddle.boundingbox.rightbottom.x = mwinRect.right;
        paddle.boundingbox.lefttop.x =
            paddle.boundingbox.rightbottom.x - paddlelength;
    }
}

/* the whole game is a bunch of rectangles manipulated here */
void updateGame(HWND hwnd)
{
    if (numBricks)
    {
        /** MS's Offset rectangle function makes moving the rectangle easy **/
        OffsetRect(&ballRect, ballInfo.velocity.x, ballInfo.velocity.y);
        /**   copy new ball rectangle   **/
        ballRectInvaild = ballRect;

        /*********************************************************************
         * Adjust the RECT used to invalidate the part of the screen we need *
         * to redraw for the ball.  The region needed to be redrawn is where *
         * the ball is and where it is going to be on the next screen redraw *
        *********************************************************************/
        if(ballInfo.velocity.x > 0)
        {
            ballRectInvaild.left = ballRect.left - ballInfo.velocity.x;
            ballRectInvaild.right = ballRect.right + ballInfo.velocity.x;
        }
        else{
            ballRectInvaild.left = ballRect.left + ballInfo.velocity.x;
            ballRectInvaild.right = ballRect.right - ballInfo.velocity.x;
        }

        if(ballInfo.velocity.y > 0)
        {
            ballRectInvaild.top = ballRect.top - ballInfo.velocity.y;
            ballRectInvaild.bottom = ballRect.bottom  + ballInfo.velocity.y;
        }
        else{
            ballRectInvaild.top = ballRect.top + ballInfo.velocity.y;
            ballRectInvaild.bottom = ballRect.bottom - ballInfo.velocity.y;
        }

        handleCollisions(hwnd);
    }
}

/**   detect collisions of the ball
  with window walls, paddle and bricks        **/
void handleCollisions(HWND hwnd)
{

    /**   get main window rectangle   **/
    GetClientRect(hwnd, &mwinRect);

    /**   ball can collide in corner any one or   **/
    /**   two of the following are possible       **/

    /**   if collision left   **/
    if(ballRect.left <= mwinRect.left)
    {
        ballInfo.velocity.x = -ballInfo.velocity.x;
        ballRect.left = mwinRect.left + 1;
        ballRect.right = ballInfo.sizeDia + 1;
        playSound(1);
    }
    /**   if collision top   **/
    if(ballRect.top <= mwinRect.top)
    {
        ballInfo.velocity.y = -ballInfo.velocity.y;
        ballRect.top = mwinRect.top + 1;
        ballRect.bottom = ballInfo.sizeDia + 1;
        playSound(1);
    }
    /**   if collision right   **/
    if(ballRect.right >= mwinRect.right)
    {
        ballInfo.velocity.x = -ballInfo.velocity.x;
        ballRect.left = mwinRect.right - ballInfo.sizeDia - 1;
        ballRect.right = mwinRect.right - 1;
        playSound(1);
    }
    /**   if collision.rightbottom   **/
    // TODO (ebob): add code to flag event "ball lost"
    if(ballRect.bottom >= mwinRect.bottom)
    {
        ballInfo.velocity.y = -ballInfo.velocity.y;
        ballRect.bottom = mwinRect.bottom - 1;
        ballRect.top = ballRect.bottom - ballInfo.sizeDia - 1;
        playSound(1);
    }

    //  TODO:  this is messy and not complete
    /**   detect paddle collisions   **/
    rect2 ball;
    ball.lefttop.x = ballRect.left;
    ball.lefttop.y = ballRect.top;
    ball.rightbottom.x = ballRect.right;
    ball.rightbottom.y = ballRect.bottom;
    if (detectCollision(ball, paddle.boundingbox))
    {
        {
            ballInfo.velocity.y = -ballInfo.velocity.y;
            playSound(1);
        }
    }

    // detect brick collisions
    for (int i = 0; i < sizeof(bricks)/sizeof(rectangle*); i++)
    {
        if (bricks[i]) // if this brick exists
        {
            if (detectCollision(ball, bricks[i]->boundingbox))
            {
                // invalidate so background is drawn next frame
                rect2 bb = bricks[i]->boundingbox;
                RECT brickPos {bb.lefttop.x, bb.lefttop.y, bb.rightbottom.x,
                    bb.rightbottom.y};
                InvalidateRect(hwnd, &brickPos, TRUE);

                // release memory
                delete bricks[i];
                bricks[i] = 0; // mark as released
                // and update count
                // TODO: move bricks into last brick into first empty??
                numBricks--;

                // reverse ball
                ballInfo.velocity.y = -ballInfo.velocity.y;
            }
        }
    }
}

bool detectCollision(rect2 a, rect2 b)
{
    // TODO: return info about which _side_ collided (enum?)
    if (a.rightbottom.x > b.lefttop.x && // a's right inside b
        a.lefttop.x < b.rightbottom.x)   // a's left inside b
    {
        if (a.rightbottom.y > b.lefttop.y && // a's bottom inside b?
            a.rightbottom.y < b.rightbottom.y)
            return true;
        else if (a.lefttop.y > b.lefttop.y &&
                 a.lefttop.y < b.rightbottom.y)
            return true;
        return false;
    }
    return false;
}

void drawText(HDC hdc, LPCTSTR tsText, COLORREF color)
{
    /* TODO: This code needs work, our exact needs for text display are
       not completely defined yet                                  */

    COLORREF last = NULL;
    if(color)
        last = SetTextColor( hdc, color);

    textRect.top = 150; // offset from top

    // use draw text to calculate proper size
    DrawTextEx(hdc, (LPSTR)tsText, _tcslen(tsText),
            &textRect, DT_CENTER | DT_CALCRECT, 0);
    // adjust to center
    textRect.left = (mwinRect.right - (textRect.right - textRect.left)) / 2;
    /**   draw the text using rectangle   **/
    DrawTextEx(hdc, (LPSTR)tsText, _tcslen(tsText),
            &textRect, DT_CENTER, 0);

    if(color)
        SetTextColor(hdc, last);

}

void drawball(HDC hdc, COLORREF pen, COLORREF fill)
{

    /**   using the device context we can change              **/
    /**   the color of pen and brush for the device context   **/
    /**   changing before drawing the ball causes the ball    **/
    /**   to change to red in this example                    **/

    /**   Note: if you don't select on object, PEN, BRUSH     **/
    /**         you can't change it                           **/

    COLORREF lastpen = NULL;
    COLORREF lastbrush =  NULL;
    if(pen || fill)
    {
        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        lastpen = SetDCPenColor(hdc, pen);
        lastbrush = SetDCBrushColor(hdc, fill);
    }

    Ellipse(hdc,
            ballRect.left,     // int nLeftRect
            ballRect.top,      // int nTopRect
            ballRect.right,    // int nRightRect
            ballRect.bottom);  // int .rightbottomRect

    if(pen || fill)
    {

        SetDCPenColor(hdc, lastpen);
        SetDCBrushColor(hdc, lastbrush);
    }

}

void drawPaddle(HDC hdc, COLORREF pen, COLORREF fill)
{

    COLORREF lastpen = NULL;
    COLORREF lastbrush =  NULL;

    if(pen || fill)
    {
        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        lastpen = SetDCPenColor(hdc, pen);
        lastbrush = SetDCBrushColor(hdc, fill);
    }

    Rectangle(hdc,
            paddle.boundingbox.lefttop.x,
            paddle.boundingbox.lefttop.y,
            paddle.boundingbox.rightbottom.x,
            paddle.boundingbox.rightbottom.y);

    if(pen || fill)
    {
        SetDCPenColor(hdc, lastpen);
        SetDCBrushColor(hdc, lastbrush);
    }
}

void drawWall(HDC hdc)
{

    COLORREF lastpen = NULL;
    COLORREF lastbrush =  NULL;

    SelectObject(hdc, GetStockObject(DC_PEN));
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    for (int i = 0; i < NUMBRICKS; i++)
    {
        if (bricks[i])
        {
            lastpen   = SetDCPenColor(hdc, bricks[i]->pen);
            lastbrush = SetDCBrushColor(hdc, bricks[i]->brush);

            Rectangle(hdc,
                    bricks[i]->boundingbox.lefttop.x,
                    bricks[i]->boundingbox.lefttop.y,
                    bricks[i]->boundingbox.rightbottom.x,
                    bricks[i]->boundingbox.rightbottom.y);

            SetDCPenColor(hdc, lastpen);
            SetDCBrushColor(hdc, lastbrush);
        }
    }
}

/** add rectangles to our window's update region **/
void setUpdateRegion(HWND hwnd)
{
    InvalidateRect(hwnd, &ballRectInvaild, TRUE);
    InvalidateRect(hwnd, &textRect, TRUE);
    RECT temp;
    // clear the height of the ball
    temp.bottom = paddle.boundingbox.rightbottom.y;
    temp.top    = paddle.boundingbox.lefttop.y;
    // for the width of the window
    temp.left   = mwinRect.left;
    temp.right  = mwinRect.right;
    InvalidateRect(hwnd, &temp, TRUE);
}

void refreshWindow(HWND hwnd, LPCTSTR lpOptionalText)
{

    /**   we need to deal with optional text           **/
    /**   calling function can omit the text           **/
    /**   this is demonstrative at this point           **/

    /**   declare a wide CHAR buffer for the Unicode crowd   **/
    /**   for some one you this is a string                  **/
    TCHAR tcText[256] = {0};
    // TODO: there is a wide character issue here, important ??
    sprintf_s(tcText, sizeof(tcText),
            "ball: (x = %d, y = %d)\npaddle: (x = %d, y = %d)"
            "\nRemaining Bricks: %d", // making use of concat of literals
            ballRect.left, ballRect.top,
            paddle.boundingbox.lefttop.x, paddle.boundingbox.lefttop.y,
            numBricks);

    /**   if calling function sends text, use it   **/
    if(lpOptionalText)
        lstrcpy((LPSTR)tcText, lpOptionalText);

    /**   declare the paint structure object variable   **/
    /**   needed by the device context                  **/
    PAINTSTRUCT ps;

    /**   get the handle of the device context for our window needed   **/
    /**   by functions that paint or draw to the window                **/
    HDC hdc = BeginPaint(hwnd, &ps);

    drawPaddle(hdc, bO_BLACK, bO_RED); // &paddle,
    drawText(hdc, (LPCTSTR)tcText, 0); // color 0 default
    drawball(hdc, NULL, bO_RED);       // draw a black ball

    drawWall(hdc);

    /**   much like free we use end paint to return        **/
    /**   the device context to the system.  if we         **/
    /**   don't release the DC the resource will be lost   **/
    /**   as a memory leak                                 **/
    EndPaint(hwnd, &ps);
}

void cleanUp()
{
    // release memory for any remaining bricks
    for (int i = 0; i < sizeof(bricks)/sizeof(rectangle*); i++)
    {
        if (bricks[i])
            delete bricks[i];
    }
}

