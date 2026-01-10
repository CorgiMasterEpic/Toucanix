/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#include "citrus.hpp"

Citrus::Citrus(const GOPFrameBuffer *fb, const PSFFont *ft) : fb(fb), ft(ft) {}

// ------- //
// Drawing //
// ------- //

void Citrus::drawCharacter(char character, u32 xPos, u32 yPos, u32 color) const {
  u8 glyph = (u8)character;
  const i8 *fontPointer = (const i8*)this->ft->glyphBuffer + (glyph * this->ft->psfHeader->height);

  for (u32 y = yPos; y < yPos + this->ft->psfHeader->height; y++) {
    for (u32 x = xPos; x < xPos + this->ft->psfHeader->width; x++) {
      if ((*fontPointer & (0x80 >> (x - xPos))) > 0) { // PSF1 fonts are always 8px wide
        this->drawPixel(x, y, color); // Does the out of bounds checking for us.
      }
    }
    fontPointer++;
  }
}

void Citrus::drawString(const char *string, u32 xPos, u32 yPos, u32 color) const {
  u32 x = xPos, y = yPos;

  while (*string) {
    switch (*string) {
      case '\n':
        x = xPos;
        y += this->ft->psfHeader->height;
        string++;
        break;

      case '\r':
        x = xPos;
        string++;
        break;

      case '\t':
        x += this->ft->psfHeader->width * tabSize;
        string++;
        break;

      case ' ': // Mini optimization
        x += this->ft->psfHeader->width;
        string++;
        break;

      default:
        // Clarification: I check them sepretly.
        if (this->outOfBounds(x + this->ft->psfHeader->width, 0)) {
          x = xPos;
          y += this->ft->psfHeader->height;
        } if (this->outOfBounds(0, y + this->ft->psfHeader->height)) {
          return;
        }

        this->drawCharacter(*string, x, y, color);
        x += this->ft->psfHeader->width;
        string++;
        break;
    }
  }
}

void Citrus::drawLine(u32 x0, u32 y0, u32 x1, u32 y1, u32 color) const {
  i32 dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
  i32 sx = (x0 < x1) ? 1 : -1;

  i32 dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
  i32 sy = (y0 < y1) ? 1 : -1;

  i32 err = (dx > dy ? dx : -dy) / 2;
  i32 e2;

  while (true) {
    this->drawPixel(x0, y0, color);

    if (x0 == x1 && y0 == y1) break;

    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
}


void Citrus::drawRectangle(u32 xPos, u32 yPos, u32 sizeX, u32 sizeY, u32 color) const {
  if (sizeX == 0 || sizeY == 0) return;

  u32 yEnd = yPos + sizeY;

  for (u32 y = yPos; y < yEnd; y++) {
    this->drawSpan(xPos, y, sizeX, color);
  }
}

void Citrus::drawRectangleHollow(u32 xPos, u32 yPos, u32 sizeX, u32 sizeY, u32 color) const {
  if (sizeX == 0 || sizeY == 0) return;

  u32 yEnd = yPos + sizeY - 1;
  u32 xEnd = xPos + sizeX - 1;

  // Top & Bottom
  this->drawSpan(xPos, yPos, sizeX, color);
  this->drawSpan(xPos, yEnd, sizeX, color);

  // Left & Right
  for (u32 y = yPos + 1; y <= yEnd - 1; y++) {
    this->drawPixel(xPos, y, color);
    this->drawPixel(xEnd, y, color);
  }
}

void Citrus::drawRoundedRectangle(u32 xPos, u32 yPos, u32 sizeX, u32 sizeY, u32 radius, u32 color) const {
  if (sizeX == 0 || sizeY == 0) return;

  // Clamp radius
  if (radius * 2 > sizeX) radius = sizeX / 2;
  if (radius * 2 > sizeY) radius = sizeY / 2;

  u32 xEnd = xPos + sizeX;
  u32 yEnd = yPos + sizeY;

  // ---- Center rectangle ----
  this->drawRectangle(xPos + radius, yPos, sizeX - 2 * radius, sizeY, color);

  // ---- Left & right strips ----
  this->drawRectangle(xPos, yPos + radius, radius, sizeY - 2 * radius, color);

  this->drawRectangle(xEnd - radius, yPos + radius, radius, sizeY - 2 * radius, color);

  // ---- Corner circles ----
  // Top-left
  this->drawFilledCircleQuarter(xPos + radius, yPos + radius, radius, color, true, true);

  // Top-right
  this->drawFilledCircleQuarter(xEnd - radius - 1, yPos + radius, radius, color, false, true);

  // Bottom-left
  this->drawFilledCircleQuarter(xPos + radius, yEnd - radius - 1, radius, color, true, false);

  // Bottom-right
  this->drawFilledCircleQuarter(xEnd - radius - 1, yEnd - radius - 1, radius, color, false, false);
}

void Citrus::drawTopRoundedRectangle(u32 xPos, u32 yPos, u32 sizeX, u32 sizeY, u32 radius, u32 color) const {
  if (sizeX == 0 || sizeY == 0) return;

  // Clamp radius
  if (radius * 2 > sizeX) radius = sizeX / 2;
  if (radius > sizeY)     radius = sizeY;

  u32 xEnd = xPos + sizeX;

  // ---- Main rectangle (full bottom) ----
  this->drawRectangle(xPos, yPos + radius, sizeX, sizeY - radius, color);

  // ---- Top center strip ----
  this->drawRectangle(xPos + radius, yPos, sizeX - 2 * radius, radius, color);

  // ---- Top-left corner ----
  this->drawFilledCircleQuarter(xPos + radius, yPos + radius, radius, color, true, true);

  // ---- Top-right corner ----
  this->drawFilledCircleQuarter(xEnd - radius - 1, yPos + radius, radius, color, false, true);
}

void Citrus::clearCanvas(u32 color) {
  this->drawRectangle(0, 0, this->fb->width, this->fb->height, color); // Positioning starts at zero
}

// Private method (should not be used globally)
void Citrus::drawFilledCircleQuarter(u32 cx, u32 cy, u32 r, u32 color, bool left, bool top) const {
  // Convert ONCE. Geometry is signed.
  i32 scx = (i32)cx;
  i32 scy = (i32)cy;
  i32 sr  = (i32)r;

  i32 x = 0;
  i32 y = sr;
  i32 d = 1 - sr;

  while (y >= x) {
    // Horizontal span at y = x
    for (i32 i = 0; i <= y; i++) {
      i32 px = left ? scx - i : scx + i;
      i32 py = top  ? scy - x : scy + x;
      drawPixel((u32)px, (u32)py, color);
    }

    // Horizontal span at y = y
    for (i32 i = 0; i <= x; i++) {
      i32 px = left ? scx - i : scx + i;
      i32 py = top  ? scy - y : scy + y;
      drawPixel((u32)px, (u32)py, color);
    }

    if (d < 0) {
      d += 2 * x + 3;
    } else {
      d += 2 * (x - y) + 5;
      y--;
    }
    x++;
  }
}

// ------- //
// Getters //
// ------- //

const GOPFrameBuffer *Citrus::getFb() const {
  return this->fb;
}

const PSFFont *Citrus::getFt() const {
  return this->ft;
}
