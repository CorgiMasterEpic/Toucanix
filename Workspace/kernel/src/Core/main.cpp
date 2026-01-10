/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#include "main.hpp"

Kernel::Kernel(BootInfo* bootInfo) : bootInfo(bootInfo), citrus(bootInfo->fb, bootInfo->ft) {
  if constexpr (KConfig::Debug::displayBootLog) {
    Console console(&citrus);
    console.log("Booting -> ", LogType::Info);
    console.write(this->CorgiInfo);
    console.writeLine("...");

    // Example: Fake neo fetch
    // ASCII art (left side)
    console.setConsoleColor(ConsoleColor::Mint, ConsoleColor::Black);
    console.writeLine("        /\\_/\\");
    console.writeLine("       ( o.o )");
    console.writeLine("        > ^ <");

    // Reset color
    console.setConsoleColor(ConsoleColor::White, ConsoleColor::Black);

    console.writeLine();
    console.writeLine("Toucanix");
    console.writeLine("--------");

    console.setConsoleColor(ConsoleColor::Cyan, ConsoleColor::Black);
    console.write("Codename: ");
    console.setConsoleColor(ConsoleColor::White, ConsoleColor::Black);
    console.writeLine("Corgi");

    console.setConsoleColor(ConsoleColor::Cyan, ConsoleColor::Black);
    console.write("Version: ");
    console.setConsoleColor(ConsoleColor::White, ConsoleColor::Black);
    console.writeLine("0.1.0");

    console.setConsoleColor(ConsoleColor::Cyan, ConsoleColor::Black);
    console.write("Arch: ");
    console.setConsoleColor(ConsoleColor::White, ConsoleColor::Black);
    console.writeLine("x86_64");

    console.setConsoleColor(ConsoleColor::Cyan, ConsoleColor::Black);
    console.write("Renderer: ");
    console.setConsoleColor(ConsoleColor::White, ConsoleColor::Black);
    console.writeLine("Citrus2D");

    console.setConsoleColor(ConsoleColor::Cyan, ConsoleColor::Black);
    console.write("Build: ");
    console.setConsoleColor(ConsoleColor::White, ConsoleColor::Black);
    console.writeLine("Devloper Preview");
    console.writeLine();

    // Neofetch-style color bars
    ConsoleColor bg = ConsoleColor::Black;

    console.setConsoleColor(ConsoleColor::Black, ConsoleColor::Red);
    console.write("  ");
    console.setConsoleColor(ConsoleColor::Black, ConsoleColor::Mint);
    console.write("  ");
    console.setConsoleColor(ConsoleColor::Black, ConsoleColor::Yellow);
    console.write("  ");
    console.setConsoleColor(ConsoleColor::Black, ConsoleColor::Cyan);
    console.write("  ");
    console.setConsoleColor(ConsoleColor::Black, ConsoleColor::Blue);
    console.write("  ");
    console.setConsoleColor(ConsoleColor::Black, ConsoleColor::Purple);
    console.write("  ");
    console.setConsoleColor(ConsoleColor::Black, ConsoleColor::White);
    console.write("  ");

    // Reset colors
    console.setConsoleColor(ConsoleColor::White, ConsoleColor::Black);
    console.writeLine();
  }
}

void Kernel::kMain() {
  // Empty for now
}

NORETURN extern "C" void _start(BootInfo *bootInfo) {
  auto kernel = Kernel(bootInfo);
  while (true) {
    kernel.kMain();
  }
}
