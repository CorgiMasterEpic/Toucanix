/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#pragma once

#include <stdtypes.hpp>
#include <Shrimp/gop.hpp>

class Citrus {
public:
  // Constructor
  Citrus(const GOPFrameBuffer *fb, const PSFFont *ft);

  // Drawing
  inline void drawPixel(u32 x, u32 y, u32 color) const { // ZERO OVERHEAD
    if (outOfBounds(x, y)) return;
    ((u32*)fb->baseAddress)[x + y * fb->pitch] = color;
  }

  inline bool outOfBounds(u32 xPos, u32 yPos) const { // ZERO OVERHEAD
    return xPos >= this->fb->width || yPos >= this->fb->height;
  }

  inline void drawSpan(u32 x, u32 y, u32 len, u32 color) const { // Slightly faster
    if (y >= fb->height || x >= fb->width) return;

    u32* row = (u32*)fb->baseAddress + y * fb->pitch + x;

    if (x + len <= fb->width) {
      for (u32 i = 0; i < len; i++)
        row[i] = color;
    } else {
      u32 maxLen = fb->width - x;
      for (u32 i = 0; i < maxLen; i++)
        row[i] = color;
    }
  }

  void drawCharacter(char character, u32 xPos, u32 yPos, u32 color) const;
  void drawString(const char *string, u32 xPos, u32 yPos, u32 color) const;
  void drawLine(u32 x0, u32 y0, u32 x1, u32 y1, u32 color) const;
  void drawRectangle(u32 xPos, u32 yPos, u32 sizeX, u32 sizeY, u32 color) const;
  void drawRectangleHollow(u32 xPos, u32 yPos, u32 sizeX, u32 sizeY, u32 color) const;
  void drawRoundedRectangle(u32 xPos, u32 yPos, u32 sizeX, u32 sizeY, u32 radius, u32 color) const;
  void drawTopRoundedRectangle(u32 xPos, u32 yPos, u32 sizeX, u32 sizeY, u32 radius, u32 color) const;
  void clearCanvas(u32 color);

  // Getters
  const GOPFrameBuffer *getFb() const;
  const PSFFont *getFt() const;

private:
  void drawFilledCircleQuarter(u32 cx, u32 cy, u32 r, u32 color, bool left, bool top) const;

  static constexpr u32 tabSize = 4;

  // Cheeky nullptrs 💩
  const GOPFrameBuffer *fb = nullptr;
  const PSFFont *ft = nullptr;
};
