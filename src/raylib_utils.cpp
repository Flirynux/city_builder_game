#pragma once

#include "raylib.h"

Color mixColor(Color colorA,Color colorB){
    unsigned char r = (colorA.r+colorB.r)/2;
    unsigned char g = (colorA.g+colorB.g)/2;
    unsigned char b = (colorA.b+colorB.b)/2;
    unsigned char a = (colorA.a+colorB.a)/2;
    return {r,g,b,a};
}