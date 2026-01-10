/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#pragma once

#include <stdtypes.hpp>

struct GOPFrameBuffer {
	void *const baseAddress;
	const usize bufferSize;
	const u32 width;
	const u32 height;
	const u32 pitch; // pitch == pixels per scanline (Just a cooler name)
};

struct PSFHeader {
	const byte width;  // always 8
	const byte height;
	const byte mode;
};

struct PSFFont {
	const PSFHeader *psfHeader;
	const void *glyphBuffer;
};
