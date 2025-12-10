#pragma once
#include <efi.h>
#include <efilib.h>
#include "printImproved.h"

// Structs & Data
typedef unsigned long size_t;

typedef struct {
    void *baseAddress;
    size_t bufferSize;
    unsigned int BBP; // Bytes Per Pixel
    unsigned int BPP; // Bits Per Pixel
    unsigned int version;
} GopInfo;

typedef struct {
    GopInfo *info;
    unsigned int width;
    unsigned int height;
    unsigned int fullWidth;
} GopMode;

#define PSFMagicZero 0x36
#define PSFMagicOne 0x04

typedef struct {
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charSize;
} PSFHeader;

typedef struct {
    PSFHeader *header;
    void *glyphBuffer;
} PSFFont;

typedef struct {
    GopMode *mode;
    PSFFont *font;
} Canvas;

typedef struct {
    Canvas *canvas;
} BootInfo;

//
// Utility: Load a file from disk
//
EFI_FILE *LoadFile(EFI_FILE *Directory, CHAR16 *Path,
EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_FILE *LoadedFile;

    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    SystemTable->BootServices->HandleProtocol(
        ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage
    );

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    SystemTable->BootServices->HandleProtocol(
        LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid,
        (void**)&FileSystem
    );

    if (Directory == NULL) {
        FileSystem->OpenVolume(FileSystem, &Directory);
    }

    EFI_STATUS Status = Directory->Open(
        Directory, &LoadedFile, Path,
        EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY
    );

    if (Status != EFI_SUCCESS)
        return NULL;

    return LoadedFile;
}

//
// Tiny memcmp implementation
//
int memcmp(const void *aptr, const void *bptr, size_t n) {
    const unsigned char *a = aptr;
    const unsigned char *b = bptr;

    for (size_t i = 0; i < n; i++) {
        if (a[i] < b[i]) return -1;
        else if (a[i] > b[i]) return 1;
    }
    return 0;
}

GopMode gopMode;
GopInfo gopInfo;
GopMode *InitializeGraphicsOutputProtocol(EFI_SYSTEM_TABLE *SystemTable) {
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    
    if (EFI_ERROR(uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid, NULL, (void**)&gop))) {
        PrintError(SystemTable, L"Unable to locate graphics output protocol.");
        return NULL;
    } else {
        PrintOkay(SystemTable, L"Graphics output protocol located.");
    }

    gopInfo.baseAddress = (void*)gop->Mode->FrameBufferBase;
    gopInfo.bufferSize = gop->Mode->FrameBufferSize;
    gopInfo.version = gop->Mode->Info->Version;
    gopInfo.BBP = 4;
    gopInfo.BPP = 32;
    gopMode.info = &gopInfo;
    gopMode.fullWidth = gop->Mode->Info->PixelsPerScanLine;
    gopMode.height = gop->Mode->Info->VerticalResolution;
    gopMode.width = gop->Mode->Info->HorizontalResolution;

    return &gopMode;
}

PSFFont *LoadPSFFont(EFI_FILE *Directory, CHAR16 *Path,
EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_FILE *font = LoadFile(Directory, Path, ImageHandle, SystemTable);

    if (font == NULL) {
        return NULL;
    }

    PSFHeader *header;
    SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSFHeader), (void**)&header);
    UINTN size = sizeof(PSFHeader);
    font->Read(font, &size, header);

    if (header->magic[0] != PSFMagicZero || header->magic[1] != PSFMagicOne) {
        PrintInfo(SystemTable, L"Unexpected font header.");
    }

    UINTN glyphBufferSize = header->charSize * 512;
    if (header->mode == 1) { // 512 glyph
        glyphBufferSize = header->charSize * 512;
    }

    void *glyphBuffer;
    {
        font->SetPosition(font, sizeof(PSFHeader));
        SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
        font->Read(font, &glyphBufferSize, glyphBuffer);
    }

    PSFFont *finishedFont;
    SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSFFont), (void**)&finishedFont);
    finishedFont->header = header;
    finishedFont->glyphBuffer = glyphBuffer;
    return finishedFont;
}
