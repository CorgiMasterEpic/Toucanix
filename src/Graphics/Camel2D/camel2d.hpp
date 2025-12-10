#pragma once

#include "../../Camel/types.hpp"
#include "../gop.hpp"
#include "../font.hpp"

struct Canvas {
    GopMode *mode;
    PSFFont *font;
};

struct Point {
    u32 xPos;
    u32 yPos;
    u32 color;
};

class Camel2D {
    public:
        Camel2D(Canvas *canvas);
        Canvas *getCanvas();
        void drawPixel(Point point);
        void drawPixel(u32 xPos, u32 yPos, u32 color);
        void drawChar(char character, Point point);
        void drawChar(char character, u32 xPos, u32 yPos, u32 color);
        void drawString(stringc str, Point point);
        void drawString(stringc str, u32 xPos, u32 yPos, u32 color);
        void drawRect(u32 xPos, u32 yPos, u32 width, u32 height, u32 color);
        void clear(u32 color);
    private:
        Canvas *canvas;
};