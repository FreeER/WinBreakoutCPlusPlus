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
#include "vectors.h"
v2 operator*(int32_t A, v2 B)
{
    v2 result;
    result.x = A*B.x;
    result.y = A*B.y;
    return result;
}

v2 operator-(v2 A)
{
    v2 result;
    result.x = -A.x;
    result.y = -A.y;
    return result;
}

v2 operator+(v2 A, v2 B)
{
    v2 result;
    result.x = A.x + B.x;
    result.y = A.y + B.y;
    return result;
}

v2 operator-(v2 A, v2 B)
{
    v2 result;
    result.x = A.x - B.x;
    result.y = A.y - B.y;
    return result;
}

rect2 RECT2rect2(theRECT)
{
     rect2 temp;
     temp.lefttop.x     = theRECT.left;
     temp.lefttop.y     = theRECT.top;
     temp.rightbottom.x = theRECT.right;
     temp.rightbottom.y = theRECT.bottom;
     return temp;
}
