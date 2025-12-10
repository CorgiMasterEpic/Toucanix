#pragma once
#include "../Camel/types.hpp"

typedef struct {
    u8 magic[2];
    u8 mode;
    u8 charSize;
} PSFHeader;

typedef struct {
    PSFHeader *header;
    any glyphBuffer;
} PSFFont;