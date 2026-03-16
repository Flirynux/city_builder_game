#pragma once

#include "raylib.h"

enum ZoneType{
    RESIDENTIAL,
    SERVICES,
    COMMERCE,
    INDUSTRY,
};

//RGBA
enum ZoneColor : unsigned int{
    RESIDENTIAL = 0x00FF00FF,
    SERVICES    = 0xFF0000FF,
    COMMERCE    = 0xFF00FFFF,
    INDUSTRY    = 0x0000FFFF,
    NONE        = 0x999999FF,
};

Color ConvertZoneColorToColor(ZoneColor zoneColor){
    unsigned char r = (zoneColor >> 24) & 0xFF;
    unsigned char g = (zoneColor >> 16) & 0xFF;
    unsigned char b = (zoneColor >> 8) & 0xFF;
    unsigned char a = (zoneColor) & 0xFF;
    return {r,g,b,a};
}

ZoneColor GetZoneColorByType(ZoneType zoneType){
    
    switch (zoneType)
        {
        case ZoneType::RESIDENTIAL:
            return ZoneColor::RESIDENTIAL;
            break;
        case ZoneType::SERVICES:
            return ZoneColor::SERVICES;
            break;
        case ZoneType::COMMERCE:
            return ZoneColor::COMMERCE;
            break;
        case ZoneType::INDUSTRY:
            return ZoneColor::INDUSTRY;
            break;
        
        default:
            return ZoneColor::NONE;
            break;
        }
}

class ZoneArea {
public:
    static const float DISTANCE_FROM_ROAD = 50.0f;
    static const float CELL_WIDTH = 10.0f;

    bool residential = false;
    bool services = false;
    bool commerce = false;
    bool industry = false;

    float width;
    Vector2 pos;

    ZoneArea(float areaWidth, Vector2 pos) : pos(pos)
    {
        width = (float)(((int)(areaWidth))/CELL_WIDTH);
        
    }

    void toggleZoneType(ZoneType zoneType){
        switch (zoneType)
        {
        case ZoneType::RESIDENTIAL:
            residential = !residential;
            break;
        case ZoneType::SERVICES:
            services = !services;
            break;
        case ZoneType::COMMERCE:
            commerce = !commerce;
            break;
        case ZoneType::INDUSTRY:
            industry = !industry;
            break;
        
        default:
            break;
        }
    }

    void Draw(){
        DrawRectanglePro({pos.x,pos.y,width,DISTANCE_FROM_ROAD},pos,0,ConvertZoneColorToColor(ZoneColor::RESIDENTIAL));
    }
};
