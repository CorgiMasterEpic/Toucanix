#include "fbcon.hpp"

Console::Console(Camel2D *camel2d) {
    this->camel2d = camel2d;
    this->consolePoint = ConsolePoint { ConsoleColor::Black, ConsoleColor::White, 0, 0 };
    clear();
}

void Console::clear() {
    camel2d->clear(this->consolePoint.backgroundColor);
}

void Console::clear(ConsoleColor color) {
    camel2d->clear(color);
}

void Console::setConsolePoint(ConsolePoint point) {
    consolePoint = point;
}

void Console::setConsolePoint(ConsoleColor backgroundColor, ConsoleColor foregroundColor, u32 xPos, u32 yPos) {
    consolePoint = ConsolePoint { backgroundColor, foregroundColor, xPos, yPos };
}

void Console::setConsoleColor(ConsoleColor backgroundColor, ConsoleColor foregroundColor) {
    consolePoint.backgroundColor = backgroundColor;
    consolePoint.foregroundColor = foregroundColor;
}

void Console::setConsolePosition(u32 xPos, u32 yPos) {
    consolePoint.xPos = xPos;
    consolePoint.yPos = yPos;
}

void Console::write(char character) {
    if (consolePoint.xPos + 8 > camel2d->getCanvas()->mode->width && consolePoint.yPos + camel2d->getCanvas()->font->header->charSize > camel2d->getCanvas()->mode->height) {
        return; // TODO: add scrolling!
    }

    switch (character) {
        case '\n': {
            consolePoint.xPos = 0;
            consolePoint.yPos += camel2d->getCanvas()->font->header->charSize;
            return;
        }

        case '\r': {
            consolePoint.xPos = 0;
            return;
        }

        case '\t': {
            if (consolePoint.xPos + 32 > camel2d->getCanvas()->mode->width) {
                return;
            }
            consolePoint.xPos += 32;
            return;
        }

        case '\0': {
            return;
        }

        default: {
            if (consolePoint.xPos + 8 > camel2d->getCanvas()->mode->width) {
                consolePoint.xPos = 0;
                consolePoint.yPos += camel2d->getCanvas()->font->header->charSize;
            } 

            camel2d->drawRect(consolePoint.xPos, consolePoint.yPos, 8, camel2d->getCanvas()->font->header->charSize, consolePoint.backgroundColor);
            camel2d->drawChar(character, consolePoint.xPos, consolePoint.yPos, consolePoint.foregroundColor);
            consolePoint.xPos += 8;
        }
    }
}

void Console::write(stringc str) {
    stringc character = str;
    while (*character != 0) {
        write(*character);
        character++;
    }
}

void Console::writeLine() {
    write('\n');
}

void Console::writeLine(char character) {
    write(character);
    write('\n');
}

void Console::writeLine(stringc str) {
    write(str);
    write('\n');
}