#pragma once
#include "../Camel/types.hpp"

struct GopInfo {
    addr baseAddress;
    usize bufferSize;
    u32 BBP; // Bytes Per Pixel
    u32 BPP; // Bits Per Pixel
    u32 version;
};

struct GopMode {
    GopInfo *info;
    u32 width;
    u32 height;
    u32 fullWidth;
};
