#include "../../../Graphics/Camel2D/camel2d.hpp"

enum ConsoleColor {
    // Grayscale / Neutral
    Black          = 0xff121212,
    White          = 0xffdfdfdf,
    Tan            = 0xffc3b09a,
    Brown          = 0xff9c6f59,
    BrightBlack    = 0xff2a2a2a,
    BrightWhite    = 0xffefefef,
    BrightTan      = 0xffd9c8b6,
    BrightBrown    = 0xffb9856f,

    // Reds / Warm Reds
    Red            = 0xffc45a5a,
    Crimson        = 0xffb34a5c,
    Rose           = 0xffc76f86,
    Orange         = 0xffd49a6a,
    Amber          = 0xffd8b06e,
    Yellow         = 0xffd8c477,
    BrightRed      = 0xffd47f7f,
    BrightCrimson  = 0xffc86373,
    BrightRose     = 0xffdea0ab,
    BrightOrange   = 0xffe6b689,
    BrightAmber    = 0xffecd09b,
    BrightYellow   = 0xffe8d48a,

    // Greens
    Green          = 0xff7fbf7b,
    Lime           = 0xffa8d38a,
    Olive          = 0xffb8b97a,
    BrightGreen    = 0xff97d39c,
    BrightLime     = 0xffbde8a0,
    BrightOlive    = 0xffcbc98a,

    // Blues
    Blue           = 0xff7aa4d8,
    Navy           = 0xff4a5f8e,
    Teal           = 0xff6fb7b1,
    Cyan           = 0xff76c9c9,
    BrightBlue     = 0xff94b7e6,
    BrightNavy     = 0xff5970a2,
    BrightTeal     = 0xff89cccc,
    BrightCyan     = 0xff8fdcdc,

    // Purples
    Magenta        = 0xffc57fd8,
    Violet         = 0xffb495d9,
    Lavender       = 0xffc8b5e6,
    BrightMagenta  = 0xffd8a0e8,
    BrightViolet   = 0xffc9abeb,
    BrightLavender = 0xffdcd0f2,
};

struct ConsolePoint {
    ConsoleColor backgroundColor;
    ConsoleColor foregroundColor;
    u32 xPos;
    u32 yPos;
};

class Console {
    public:
        Console(Camel2D *camel2d);
        void clear();
        void clear(ConsoleColor color);
        void setConsolePoint(ConsolePoint point);
        void setConsolePoint(ConsoleColor backgroundColor, ConsoleColor foregroundColor, u32 xPos, u32 yPos);
        void setConsoleColor(ConsoleColor backgroundColor, ConsoleColor foregroundColor);
        void setConsolePosition(u32 xPos, u32 yPos);
        void write(char character);
        void write(stringc str);
        void writeLine();
        void writeLine(char character);
        void writeLine(stringc str);

    private:
        Camel2D *camel2d;
        ConsolePoint consolePoint;
};