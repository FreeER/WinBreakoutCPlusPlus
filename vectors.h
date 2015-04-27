/**
*
*   WinBreakoutC++
*
*   vectors.h
*
*   A set of vector functions and definitions
*
*   2015 a collaboration of CS50x students
*
*/

#pragma once
#include <stdint.h>

struct v2
{
    int32_t x;
    int32_t y;

    v2() {}

    v2(int32_t _x, int32_t _y)
    {
        x = _x;
        y = _y;
    }

};

v2 operator*(int32_t A, v2 B);

v2 operator-(v2 A);

v2 operator+(v2 A, v2 B);

v2 operator-(v2 A, v2 B);

struct rect2
{
    v2 lefttop;
    v2 rightbottom;
    rect2::rect2() {}

    rect2::rect2(v2 lt, v2 rb)
    {
        lefttop     = lt;
        rightbottom = rb;
    }

    rect2::rect2(int32_t left, int32_t top, int32_t right, int32_t bottom)
    {
        lefttop.x     = left;
        lefttop.y     = top;
        rightbottom.x = right;
        rightbottom.y = bottom;
    }
};

rect2 RECT2rect2(RECT theRECT);
