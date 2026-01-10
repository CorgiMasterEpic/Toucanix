/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#include "console.hpp"

Console::Console(Citrus *citrus) {
  this->citrus = citrus;
  this->setConsoleColor(ConsoleColor::White, ConsoleColor::Black);
  this->setCursorPosition(0, 0);
  this->clear();
}

void Console::setConsoleColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor) {
  this->foregroundColor = foregroundColor;
  this->backgroundColor = backgroundColor;
}

void Console::setCursorPosition(u32 xPos, u32 yPos) {
  this->xPos = xPos;
  this->yPos = yPos;
}

void Console::clear() {
  this->citrus->clearCanvas((u32)this->backgroundColor);
  this->setCursorPosition(0, 0);
}

void Console::write(char character) {
  switch (character) {
    case '\n':
      this->writeNewline();
      break;

    case '\r':
      this->writeReturn();
      break;

    case '\t':
      this->writeTab();
      break;

    case '\b':
      this->writeBackspace();
      break;

    default:
      // Clarification: I check them sepretly.
      if (this->citrus->outOfBounds(this->xPos + this->citrus->getFt()->psfHeader->width, 0)) {
        this->xPos = 0;
        this->yPos += this->citrus->getFt()->psfHeader->height;
      } if (this->citrus->outOfBounds(0, this->yPos + this->citrus->getFt()->psfHeader->height)) {
        return;
      }

      this->citrus->drawRectangle(this->xPos, this->yPos, this->citrus->getFt()->psfHeader->width, this->citrus->getFt()->psfHeader->height, (u32)this->backgroundColor);
      this->citrus->drawCharacter(character, this->xPos, this->yPos, (u32)this->foregroundColor);
      this->xPos += this->citrus->getFt()->psfHeader->width;
      break;
  }
}

void Console::write(const char *string) {
  while (*string) {
    this->write(*string);
    string++;
  }
}

void Console::writeLine() {
  this->writeNewline();
}

void Console::writeLine(const char *string) {
  this->write(string);
  this->writeNewline();
}

void Console::log(const char *string, LogType logType) {
  ConsoleColor oldForeground = this->foregroundColor;

  this->setConsoleColor(ConsoleColor::White, this->backgroundColor);
  this->write("[");

  switch (logType) {
    case LogType::Failure:
      this->setConsoleColor(ConsoleColor::Red, this->backgroundColor);
      this->write("Failure");
      break;

    case LogType::Succsess:
      this->setConsoleColor(ConsoleColor::Mint, this->backgroundColor);
      this->write("Succsess");
      break;

    case LogType::Warning:
      this->setConsoleColor(ConsoleColor::Yellow, this->backgroundColor);
      this->write("Warning");
      break;

    case LogType::Info:
      this->setConsoleColor(ConsoleColor::Cyan, this->backgroundColor);
      this->write("Info");
      break;
  }

  this->setConsoleColor(ConsoleColor::White, this->backgroundColor);
  this->write("] >> ");

  this->setConsoleColor(oldForeground, this->backgroundColor);
  this->write(string);
}

// Private methods
void Console::writeNewline() {
  this->xPos = 0;
  this->yPos += this->citrus->getFt()->psfHeader->height;
}

void Console::writeReturn() {
  this->xPos = 0;
}

void Console::writeTab() {
  this->xPos += this->citrus->getFt()->psfHeader->width * 4;
}

void Console::writeBackspace() {
  if (this->xPos >= this->citrus->getFt()->psfHeader->width) {
    this->xPos -= this->citrus->getFt()->psfHeader->width;
  } else if (this->yPos >= this->citrus->getFt()->psfHeader->height) {
    this->yPos -= this->citrus->getFt()->psfHeader->height;
    this->xPos = ((this->citrus->getFb()->width / this->citrus->getFt()->psfHeader->width) - 1) * this->citrus->getFt()->psfHeader->width; // last column
  } else {
    return;
  }

  this->citrus->drawRectangle(this->xPos, this->yPos, this->citrus->getFt()->psfHeader->width, this->citrus->getFt()->psfHeader->height, (u32)this->backgroundColor);
}
