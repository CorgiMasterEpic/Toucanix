#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include "printImproved.h"
#include "util.h"

//
// Entry point
//
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    //
    // Bootloader title
    //
    SetColor(SystemTable, EFI_WHITE, EFI_BLACK);
    Print(L"[");
    SetColor(SystemTable, EFI_LIGHTCYAN, EFI_BLACK);
    Print(L"Toucanix Bootloader");
    SetColor(SystemTable, EFI_WHITE, EFI_BLACK);
    Print(L"]\r\n");

    Print(L"-------------------------------------\r\n");

    PrintInfo(SystemTable, L"Initializing...");

    //
    // Load Kernel
    //
    PrintInfo(SystemTable, L"Loading kernel from: \\Nexus\\Toucanix\\Toucanix.elf");

    EFI_FILE *Kernel = LoadFile(
        NULL,
        L"\\Nexus\\Toucanix\\Toucanix.elf",
        ImageHandle,
        SystemTable
    );

    if (Kernel == NULL) {
        PrintError(SystemTable, L"Could not load kernel!");
        return EFI_LOAD_ERROR;
    }

    PrintOkay(SystemTable, L"Kernel file opened.");

    //
    // Read ELF Header
    //
    Elf64_Ehdr Header;
    {
        UINTN InfoSize;
        EFI_FILE_INFO *FileInfo;

        Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &InfoSize, NULL);
        SystemTable->BootServices->AllocatePool(
            EfiLoaderData, InfoSize, (void**)&FileInfo
        );
        Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &InfoSize, (void**)&FileInfo);

        UINTN Size = sizeof(Header);
        Kernel->Read(Kernel, &Size, &Header);
    }

    //
    // Validate ELF Header
    //
    PrintInfo(SystemTable, L"Verifying ELF header...");

    if (
        memcmp(&Header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
        Header.e_ident[EI_CLASS] != ELFCLASS64 ||
        Header.e_ident[EI_DATA] != ELFDATA2LSB ||
        Header.e_type != ET_EXEC ||
        Header.e_machine != EM_X86_64 ||
        Header.e_version != EV_CURRENT
    ) {
        PrintError(SystemTable, L"Invalid ELF format!");
        return EFI_LOAD_ERROR;
    }

    PrintOkay(SystemTable, L"ELF header is valid.");

    //
    // Read Program Headers
    //
    Elf64_Phdr *Phdrs;
    {
        Kernel->SetPosition(Kernel, Header.e_phoff);
        UINTN Size = Header.e_phnum * Header.e_phentsize;

        SystemTable->BootServices->AllocatePool(
            EfiLoaderData, Size, (void**)&Phdrs
        );
        Kernel->Read(Kernel, &Size, Phdrs);
    }

    //
    // Load Segments
    //
    PrintInfo(SystemTable, L"Loading segments...");

    for (
        Elf64_Phdr *ph = Phdrs;
        (char*)ph < (char*)Phdrs + Header.e_phnum * Header.e_phentsize;
        ph = (Elf64_Phdr*)((char*)ph + Header.e_phentsize)
    ) {
        if (ph->p_type == PT_LOAD) {
            int Pages = (ph->p_memsz + 0x1000 - 1) / 0x1000;
            Elf64_Addr Segment = ph->p_paddr;

            SystemTable->BootServices->AllocatePages(
                AllocateAddress, EfiLoaderData, Pages, &Segment
            );

            Kernel->SetPosition(Kernel, ph->p_offset);

            UINTN Size = ph->p_filesz;
            Kernel->Read(Kernel, &Size, (void*)Segment);

            PrintSegment(SystemTable, Segment, Pages);
        }
    }

    PrintInfo(SystemTable, L"Kernel fully loaded.");

	//
	// Initialize GOP
	//
	GopMode *newMode = InitializeGraphicsOutputProtocol(SystemTable);
	Print(L"[Base]: 0x%x\n\r[Size]: 0x%x\n\r[Resolution]: %dx%d %d PPS\n\r", 
	newMode->info->baseAddress, 
	newMode->info->bufferSize, 
	newMode->width, 
	newMode->height, 
	newMode->fullWidth);

    PSFFont *newFont = LoadPSFFont(
        NULL,
        L"\\Nexus\\Fonts\\Default.psf",
        ImageHandle,
        SystemTable
    );

    Canvas newCanvas;
	newCanvas.mode = newMode;
    newCanvas.font = newFont;

    BootInfo bootInfo;
    bootInfo.canvas = &newCanvas;

    //
    // Jump to kernel
    //
    SetColor(SystemTable, EFI_LIGHTGREEN, EFI_BLACK);
    Print(L"> ");
    SetColor(SystemTable, EFI_CYAN, EFI_BLACK);
    Print(L"Jumping to entry point: 0x%lx\r\n\r\n", Header.e_entry);
    SetColor(SystemTable, EFI_LIGHTGRAY, EFI_BLACK);

    void (*KernelStart)(BootInfo*) = ((__attribute__((sysv_abi)) void (*)(BootInfo*))Header.e_entry);

    KernelStart(&bootInfo);

    return EFI_SUCCESS;
}
