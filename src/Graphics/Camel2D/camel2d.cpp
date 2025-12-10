#include "camel2d.hpp"

Camel2D::Camel2D(Canvas *canvas) {
    this->canvas = canvas;
}

Canvas *Camel2D::getCanvas() {
    return canvas;
}

void Camel2D::drawPixel(Point point) {
    u8p base = (u8p) canvas->mode->info->baseAddress;

    u32 bpp  = canvas->mode->info->BBP;
    u32 pps  = canvas->mode->fullWidth;
    u32 pitch = pps * bpp;

    u32p pixelPtr = (u32p)(base
                           + point.yPos * pitch
                           + point.xPos * bpp);

    *pixelPtr = point.color;
}

void Camel2D::drawPixel(u32 xPos, u32 yPos, u32 color) {
    drawPixel(Point {xPos, yPos, color});
}

void Camel2D::drawChar(char character, Point point) {
    i8p fontp = (i8p) canvas->font->glyphBuffer +
                (character * canvas->font->header->charSize);

    for (u32 y = point.yPos; y < point.yPos + canvas->font->header->charSize; y++) {
        for (u32 x = point.xPos; x < point.xPos + 8; x++) {
            if ((*fontp & (0x80 >> (x - point.xPos))) != 0) {
                drawPixel(x, y, point.color);
            }
        }
        fontp++;
    }
}

void Camel2D::drawChar(char character, u32 xPos, u32 yPos, u32 color) {
    drawChar(character, Point {xPos, yPos, color});
}

void Camel2D::drawString(stringc str, Point point) {
    u32 offset = 0;
    stringc character = str;

    while (*character != 0) {
        drawChar(*character, point.xPos + offset, point.yPos, point.color);
        offset += 8;
        character++;
    }
}

void Camel2D::drawString(stringc str, u32 xPos, u32 yPos, u32 color) {
    drawString(str, Point {xPos, yPos, color});
}

void Camel2D::drawRect(u32 xPos, u32 yPos, u32 width, u32 height, u32 color) {
    for (u32 x = xPos; x < xPos + width; x++) {
        for (u32 y = yPos; y < yPos + height; y++) {
            drawPixel(x, y, color);
        }
    }
}

void Camel2D::clear(u32 color) {
    drawRect(
        0,
        0,
        canvas->mode->width,
        canvas->mode->height,
        color
    );
}
