/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#pragma once

#include <stdtypes.hpp>
#include "citrus.hpp"

enum class ConsoleColor : u32 {
  Black           = 0xff121212, // Charcoal (im fed up with vga style consoles)
  White           = 0xffe6e6e6,
  Red             = 0xffe74856,
  Green           = 0xff16c60c,
  Blue            = 0xff3b78ff,
  Yellow          = 0xfff9f1a5,
  Cyan            = 0xff61d6d6,
  Magenta         = 0xffb4009e,
  Orange          = 0xffff8c00,
  Pink            = 0xffff5fa2,

  LightBlue       = 0xff7aaeff,
  LightningBlue   = 0xff4dfcff,
  Crimson         = 0xffb11226,
  Scarlet         = 0xfffc2847,
  Burgundy        = 0xff800020,
  Maroon          = 0xff6f1d1b,

  Teal            = 0xff008080,
  Turquoise       = 0xff40e0d0,
  Aqua            = 0xff00ffff,
  Mint            = 0xff98ff98,
  Lime            = 0xffbfff00,
  Olive           = 0xff808000,
  ForestGreen     = 0xff0b6623,

  Emerald         = 0xff50c878,
  Jade            = 0xff00a86b,
  Sapphire        = 0xff0f52ba,
  Navy            = 0xff001f3f,
  Indigo          = 0xff4b0082,
  Violet          = 0xff8f00ff,
  Purple          = 0xff6a0dad,

  Lavender        = 0xffb57edc,
  Plum            = 0xff673147,
  Peach           = 0xffffcba4,
  Coral           = 0xffff7f50,
  Salmon          = 0xfffa8072,

  Copper          = 0xffb87333,
  Bronze          = 0xffcd7f32,
  Gold            = 0xffffd700,
  Silver          = 0xffc0c0c0,
  Platinum        = 0xffe5e4e2,

  Diamond         = 0xffeafcff,
  SapphireBlue    = 0xff0f52ba,
  Pearl           = 0xfff8f6f0,
  Ivory           = 0xfffffff0,
  Beige           = 0xfff5f5dc,
  Sand            = 0xffc2b280,

  Khaki           = 0xffc3b091,
  Chocolate       = 0xff7b3f00,
  Coffee          = 0xff6f4e37,
  Mocha           = 0xff967969,

  Charcoal        = 0xff36454f,
  Slate           = 0xff708090,
  Steel           = 0xff4682b4,
  Gunmetal        = 0xff2a3439,

  Smoke           = 0xff848884,
  Ash             = 0xffb2beb5,

  Ruby            = 0xffe0115f,
  Topaz           = 0xffffc87c,
  Amber           = 0xffffbf00,
  Citrine         = 0xffe4d00a,

  Onyx            = 0xff0f0f0f,
  Obsidian        = 0xff0b1215,
  Midnight        = 0xff191970,
  Sky             = 0xff87ceeb,
  Ice             = 0xffdff6ff,

  NeonRed         = 0xffff073a,
  NeonOrange      = 0xffff5f00,
  NeonYellow      = 0xffffff00,
  NeonLime        = 0xffccff00,
  NeonGreen       = 0xff39ff14,
  NeonMint        = 0xff00ff9c,
  NeonCyan        = 0xff00ffff,
  NeonTurquoise   = 0xff00ffd5,
  NeonBlue        = 0xff1b03ff,
  NeonSky         = 0xff4d9fff,
  NeonIndigo      = 0xff5f00ff,
  NeonPurple      = 0xffa020f0,
  NeonViolet      = 0xff8f00ff,
  NeonMagenta     = 0xffff00ff,
  NeonPink        = 0xffff1493,
  NeonRose        = 0xffff007f,

  NeonAmber       = 0xffffbf00,
  NeonGold        = 0xffffd700,
  NeonCopper      = 0xffff7a18,

  NeonGray        = 0xffe0e0e0,
};

enum class LogType : u8 {
  Failure = 0,
  Succsess = 1,
  Warning = 2,
  Info = 3,
};

class Console {
public:
  Console(Citrus *citrus);
  void setConsoleColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor);
  void setCursorPosition(u32 xPos, u32 yPos);
  void clear();
  void write(char character);
  void write(const char *string);
  void writeLine();
  void writeLine(const char *string);
  void log(const char *string, LogType logType);

private:
  // These could be used outside this class but its silly
  void writeNewline();
  void writeReturn();
  void writeTab();
  void writeBackspace();

  Citrus *citrus = nullptr;
  ConsoleColor foregroundColor;
  ConsoleColor backgroundColor;
  u32 xPos;
  u32 yPos;
};
