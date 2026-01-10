/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#include <efi.h>
#include <efilib.h>
#include <elf.h>

// ---------------- //
// Macros & Structs //
// ---------------- //
#define SHRIMP_FULL_NAME L"Shrimp Bootloader 25.0.0"
#define SHRIMP_CRLF L"\r\n"
typedef unsigned char ShrimpStatus;
typedef unsigned long long size_t;
#define SHRIMP_SUCCESS 0
#define SHRIMP_FAILURE 1
#define SHRIMP_WARNING 2
#define SHRIMP_INFO 3
#define SHRIMP_LOAD_PATH L"\\Shrimp\\Toucanix.elf"
#define SHRIMP_DEFAULT_FONT_PATH L"\\Shrimp\\default.psf"
#define EFI_FG_MASK 0x0F
#define EFI_BG_MASK 0x70
#define PSF_MAGIC_ZERO 0x36
#define PSF_MAGIC_ONE 0x04

typedef struct {
	void *baseAddress;
	size_t bufferSize;
	unsigned int width;
	unsigned int height;
	unsigned int pitch;
} GOPFrameBuffer;

typedef struct __attribute__((packed)) {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char height;
} PSF1HeaderRaw;

typedef struct {
	unsigned char width;  // always 8
	unsigned char height;
	unsigned char mode;
} PSFHeader;

typedef struct {
	PSFHeader *psfHeader;
	void *glyphBuffer;
}	PSFFont;

typedef struct {
	GOPFrameBuffer *fb;
	PSFFont *ft; // So it lines up and sounds better with fb, ft.
} BootInfo;

// -------------- //
// Implementation //
// -------------- //
void shrimpSetColor(EFI_SYSTEM_TABLE* SystemTable, UINTN attr) {
	UINTN clean = (attr & 0x0F) | (attr & 0x70);
  SystemTable->ConOut->SetAttribute(SystemTable->ConOut, clean);
}

void shrimpChangeColorToErrorLevel(EFI_SYSTEM_TABLE *SystemTable, ShrimpStatus errorLevel) {
	switch (errorLevel) {
		case SHRIMP_SUCCESS: {
			shrimpSetColor(SystemTable, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
			break;
		} case SHRIMP_FAILURE: {
			shrimpSetColor(SystemTable, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);
			break;
		} case SHRIMP_WARNING: {
			shrimpSetColor(SystemTable, EFI_YELLOW | EFI_BACKGROUND_BLACK);
			break;
		} default: {
			shrimpSetColor(SystemTable, EFI_LIGHTCYAN | EFI_BACKGROUND_BLACK);
			break;
		}
	}
}

void shrimpPrintln(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *str, ShrimpStatus errorLevel) {
	shrimpChangeColorToErrorLevel(SystemTable, errorLevel);
	Print(L">");

	shrimpSetColor(SystemTable, EFI_WHITE | EFI_BACKGROUND_BLACK);
	Print(L" [");

	shrimpSetColor(SystemTable, EFI_CYAN | EFI_BACKGROUND_BLACK);
	Print(SHRIMP_FULL_NAME);

	shrimpSetColor(SystemTable, EFI_WHITE | EFI_BACKGROUND_BLACK);
	Print(L"]");

	shrimpSetColor(SystemTable, EFI_MAGENTA | EFI_BACKGROUND_BLACK);
	Print(L" >> ");

	shrimpSetColor(SystemTable, EFI_CYAN | EFI_BACKGROUND_BLACK);
	Print(str);
	Print(SHRIMP_CRLF); // CRLF for backwards compatibility
}

void shrimpPrintlnSimple(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *str, ShrimpStatus errorLevel) {
	shrimpChangeColorToErrorLevel(SystemTable, errorLevel);
	Print(L"> ");

	shrimpSetColor(SystemTable, EFI_CYAN | EFI_BACKGROUND_BLACK);
	Print(str);
	Print(SHRIMP_CRLF); // CRLF for backwards compatibility
}

EFI_FILE *shrimpLoadFile(CHAR16 *path, EFI_HANDLE ImageHandle) {
	EFI_FILE *loadedFile;
	EFI_LOADED_IMAGE_PROTOCOL *loadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fileSystem;
	EFI_FILE *directory = NULL;

	BS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loadedImage);
	BS->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&fileSystem);
	fileSystem->OpenVolume(fileSystem, &directory);
	if (directory->Open(directory, &loadedFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY) != EFI_SUCCESS) {
		return NULL;
	} else {
		return loadedFile;
	}
}

int shrimpMemcmp(const void *aPointer, const void *bPointer, size_t count) {
	const unsigned char *a = aPointer, *b = bPointer;

	for (size_t i = 0; i < count; i++) {
		if (a[i] > b[i]) {
			return 1;
		} else if (a[i] < b[i]) {
			return -1;
		}
	}

	return 0;
}

GOPFrameBuffer gopFb;
GOPFrameBuffer *shrimpInitializeGOP(EFI_SYSTEM_TABLE *SystemTable) {
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	if (EFI_ERROR(uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid, NULL, (void**)&gop))) {
		shrimpPrintlnSimple(SystemTable, L"Unable to locate UEFI Graphics Output Protocol (GOP).", SHRIMP_WARNING);
		return NULL;
	} else {
		shrimpPrintlnSimple(SystemTable, L"Located UEFI Graphics Output Protocol (GOP).", SHRIMP_SUCCESS);
	}

	gopFb.baseAddress = (void*)gop->Mode->FrameBufferBase;
	gopFb.bufferSize  =        gop->Mode->FrameBufferSize;
	gopFb.width       =        gop->Mode->Info->HorizontalResolution;
	gopFb.height      =        gop->Mode->Info->VerticalResolution;
	gopFb.pitch       =        gop->Mode->Info->PixelsPerScanLine;

	return &gopFb;
}

PSFFont *shrimpLoadPSFFont(CHAR16 *path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	EFI_FILE *font = shrimpLoadFile(path, ImageHandle);

	if (font == NULL) {
		return NULL;
	}

	PSF1HeaderRaw *psfHeader;
	UINTN size = sizeof(PSF1HeaderRaw);
	BS->AllocatePool(EfiLoaderData, size, (void**)&psfHeader);
	font->Read(font, &size, psfHeader);

	if (psfHeader->magic[0] != PSF_MAGIC_ZERO || psfHeader->magic[1] != PSF_MAGIC_ONE) {
		shrimpPrintlnSimple(SystemTable, L"Invalid psf font magic values (Could cause problems).", SHRIMP_WARNING);
	}

	UINTN glyphBufferSize = psfHeader->height * 256;
	if (psfHeader->mode == 1) { // 512 glyph mode
		glyphBufferSize = psfHeader->height * 512;
	}

	void *glyphBuffer; {
		font->SetPosition(font, size);
		BS->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSFHeader *psfHeaderCooked; // (Didnt konw what to name so its cooked instead of raw lol)
	BS->AllocatePool(
		EfiLoaderData,
		sizeof(PSFHeader),
		(void**)&psfHeaderCooked
	);

	psfHeaderCooked->width = 8;
	psfHeaderCooked->height = psfHeader->height;
	psfHeaderCooked->mode = psfHeader->mode;

	PSFFont *psfFont;
	BS->AllocatePool(EfiLoaderData, sizeof(PSFFont), (void**)&psfFont);
	psfFont->psfHeader = psfHeaderCooked;
	psfFont->glyphBuffer = glyphBuffer;

	return psfFont;
}

void shrimpPrintDragonLogo(EFI_SYSTEM_TABLE *SystemTable) {
	shrimpSetColor(SystemTable, EFI_LIGHTCYAN | EFI_BACKGROUND_BLACK);

	Print(
		L"                / \\  //\\\r\n"
		L"      |\\___/|      /   \\//  \\\\\r\n"
		L"      /0  0  \\__  /    //  | \\ \\\r\n"
		L"     /     /  \\/_/    //   |  \\  \\\r\n"
		L"     \\_^_\\'/   \\/_   //    |   \\   \\\r\n"
		L"     //_^_/     \\/_ //     |    \\    \\\r\n"
		L"  ( //) |        \\///      |     \\     \\\r\n"
		L"(( /// ))        //        |      \\     \\\r\n"
		L"(( /// ))       //         |       \\     \\\r\n"
		L" (( //))       //          |        \\     \\\r\n"
		L"   (( ))      //           |         \\     \\\r\n"
		L"              ^^            \\__________\\_____/\r\n"
		L"\r\n"
	);
}

BOOLEAN shrimpStallOrKey(EFI_SYSTEM_TABLE *SystemTable, UINTN seconds) {
	EFI_EVENT events[2];
	UINTN index;

	// Timer
	BS->CreateEvent(
		EVT_TIMER,
		0,
		NULL,
		NULL,
		&events[0]
	);

	BS->SetTimer(
		events[0],
		TimerRelative,
		seconds * 10 * 1000 * 1000
	);

	// Keyboard
	events[1] = SystemTable->ConIn->WaitForKey;

	BS->WaitForEvent(2, events, &index);

	BS->CloseEvent(events[0]);

	// TRUE if key pressed, FALSE if timer expired
	return (index == 1);
}

void shrimpClearScreen(EFI_SYSTEM_TABLE *SystemTable) {
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	shrimpPrintDragonLogo(SystemTable);
	shrimpStallOrKey(SystemTable, 2);
	shrimpClearScreen(SystemTable);
	shrimpPrintln(SystemTable, L"Starting...", SHRIMP_INFO);

	shrimpPrintlnSimple(SystemTable, L"Attempting to load UEFI Graphics Output Protocol (GOP)!", SHRIMP_INFO);

	GOPFrameBuffer *newFb = shrimpInitializeGOP(SystemTable);

	shrimpSetColor(SystemTable, EFI_MAGENTA | EFI_BACKGROUND_BLACK);
	Print(L"[UEFI Graphics Output Protocol (GOP) Info] >> {\r\n  [Base] -> 0x%x\r\n  [Size] -> 0x%x\r\n  [Resolution] -> %dx%d\r\n  [Pitch] -> %d\r\n}\r\n",
	newFb->baseAddress, newFb->bufferSize, newFb->width, newFb->height, newFb->pitch);

	shrimpPrintlnSimple(SystemTable, L"Attempting to load the default psf font!", SHRIMP_INFO);
	PSFFont *newFt = shrimpLoadPSFFont(SHRIMP_DEFAULT_FONT_PATH, ImageHandle, SystemTable);
	if (newFt == NULL) {
		shrimpPrintlnSimple(SystemTable, L"Unsupported or not found psf font.", SHRIMP_WARNING);
	} else {
		shrimpPrintlnSimple(SystemTable, L"Default font loaded!", SHRIMP_SUCCESS);
		shrimpSetColor(SystemTable, EFI_MAGENTA | EFI_BACKGROUND_BLACK);
		Print(L"[Default Font Info] >> {\r\n  [Mode] -> %d\r\n  [Size] -> %dx%d\r\n  [Version] -> 1\r\n}\r\n",
		newFt->psfHeader->mode, newFt->psfHeader->width, newFt->psfHeader->height);
	}

	EFI_FILE *kernel = shrimpLoadFile(SHRIMP_LOAD_PATH, ImageHandle);
	if (kernel == NULL) {
		shrimpPrintlnSimple(SystemTable, L"Failure to load kernel file.", SHRIMP_FAILURE);
		return EFI_LOAD_ERROR;
	} else {
		shrimpPrintlnSimple(SystemTable, L"Successfully loaded kernel file!", SHRIMP_SUCCESS);
	}

	Elf64_Ehdr header; {
		UINTN fileInfoSize;
		EFI_FILE_INFO *fileInfo;
		kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, NULL);
		BS->AllocatePool(EfiLoaderData, fileInfoSize, (void**)&fileInfo);
		kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, (void**)&fileInfo);

		UINTN size = sizeof(header);
		kernel->Read(kernel, &size, &header);
	}

	if (
		shrimpMemcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	) {
		shrimpPrintlnSimple(SystemTable, L"Unsupported kernel format.", SHRIMP_WARNING);
	} else {
		shrimpPrintlnSimple(SystemTable, L"Successfully verified kernel format!", SHRIMP_SUCCESS);
	}

	Elf64_Phdr *phdrs; {
		kernel->SetPosition(kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		BS->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		kernel->Read(kernel, &size, phdrs);
	}

	for (
		Elf64_Phdr *phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	) {
		switch (phdr->p_type) {
			case PT_LOAD: {
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				BS->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				kernel->SetPosition(kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				kernel->Read(kernel, &size, (void*)segment);
				break;
			}
		}
	}

	BootInfo bootInfo;
	bootInfo.fb = newFb;
	bootInfo.ft = newFt;

	shrimpPrintlnSimple(SystemTable, L"Kernel loaded succsessfully, Jumping to entry point!", SHRIMP_SUCCESS);

	void (*KernelStart)(BootInfo *) = ((__attribute__((sysv_abi)) void (*)(BootInfo *)) header.e_entry);
	KernelStart(&bootInfo);

	return EFI_SUCCESS;
}
