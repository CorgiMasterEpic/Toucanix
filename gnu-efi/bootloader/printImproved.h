#pragma once
#include <efi.h>
#include <efilib.h>
#include <elf.h>

void SetColor(EFI_SYSTEM_TABLE *SystemTable, UINTN fg, UINTN bg) {
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(fg, bg));
}

void PrintArrow(EFI_SYSTEM_TABLE *SystemTable) {
    SetColor(SystemTable, EFI_LIGHTGREEN, EFI_BLACK);
    Print(L"> ");
}

void PrintInfo(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *Msg) {
    PrintArrow(SystemTable);
    SetColor(SystemTable, EFI_CYAN, EFI_BLACK);
    Print(Msg);
    SetColor(SystemTable, EFI_LIGHTGRAY, EFI_BLACK);
    Print(L"\r\n");
}

void PrintOkay(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *Msg) {
    PrintArrow(SystemTable);

    // White [
    SetColor(SystemTable, EFI_WHITE, EFI_BLACK);
    Print(L"[");

    // Green "Okay"
    SetColor(SystemTable, EFI_LIGHTGREEN, EFI_BLACK);
    Print(L"Okay");

    // White ]
    SetColor(SystemTable, EFI_WHITE, EFI_BLACK);
    Print(L"] ");

    // -> indicator in light green
    SetColor(SystemTable, EFI_LIGHTGREEN, EFI_BLACK);
    Print(L"-> ");

    // Okay message
    SetColor(SystemTable, EFI_LIGHTGREEN, EFI_BLACK);
    Print(Msg);

    // Reset
    SetColor(SystemTable, EFI_LIGHTGRAY, EFI_BLACK);
    Print(L"\r\n");
}

void PrintError(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *Msg) {
    PrintArrow(SystemTable);

    // White [
    SetColor(SystemTable, EFI_WHITE, EFI_BLACK);
    Print(L"[");

    // Red "Error"
    SetColor(SystemTable, EFI_LIGHTRED, EFI_BLACK);
    Print(L"Error");

    // White ]
    SetColor(SystemTable, EFI_WHITE, EFI_BLACK);
    Print(L"] ");

    // -> indicator in light green
    SetColor(SystemTable, EFI_LIGHTGREEN, EFI_BLACK);
    Print(L"-> ");

    // Error message
    SetColor(SystemTable, EFI_LIGHTRED, EFI_BLACK);
    Print(Msg);

    // Reset
    SetColor(SystemTable, EFI_LIGHTGRAY, EFI_BLACK);
    Print(L"\r\n");
}

void PrintSegment(EFI_SYSTEM_TABLE *SystemTable, Elf64_Addr addr, int pages) {
    SetColor(SystemTable, EFI_LIGHTGREEN, EFI_BLACK);
    Print(L"   [Segment] ");

    SetColor(SystemTable, EFI_CYAN, EFI_BLACK);
    Print(L"Loaded at 0x%lx (%d pages)\r\n", addr, pages);

    SetColor(SystemTable, EFI_LIGHTGRAY, EFI_BLACK);
}