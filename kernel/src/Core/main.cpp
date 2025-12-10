#include "main.hpp"

extern "C" void _start(BootInfo *bootInfo) {
    /* u32 y = 50;

	for (u32 x = 0; x < bootInfo->canvas->mode->width / 2 * bootInfo->canvas->mode->info->BBP; x += bootInfo->canvas->mode->info->BBP) {
		*(u32p)(x + (y * bootInfo->canvas->mode->width * bootInfo->canvas->mode->info->BBP) + (u8p)bootInfo->canvas->mode->info->baseAddress) = 0xff00ffff;
	} */

	Camel2D camel2d(bootInfo->canvas);
	Console console(&camel2d);
	
	console.writeLine("Hello, World!");
}