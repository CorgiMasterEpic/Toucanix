#pragma once
#include <efi.h>
#include <efilib.h>

typedef unsigned long size_t;

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