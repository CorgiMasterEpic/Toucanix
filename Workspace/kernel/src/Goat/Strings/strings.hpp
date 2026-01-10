/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#pragma once

#include <stdtypes.hpp>

class Strings {
public:
  // b = bit:
  static constexpr u8 b64BufferSize = 22; // Max u64/i64 length + null terminator
  static constexpr u8 b32BufferSize = 12; // Max u32/i32 length + null terminator
  static constexpr u8 b16BufferSize = 7;  // Max u16/i16 length + null terminator
  static constexpr u8 b8BufferSize  = 5;  // Max u8/i8   length + null terminator
  static const char *toString(u64 value, char (&buffer)[b64BufferSize]);
  static const char *toString(u32 value, char (&buffer)[b32BufferSize]);
  static const char *toString(u16 value, char (&buffer)[b16BufferSize]);
  static const char *toString(u8  value, char (&buffer)[ b8BufferSize]);
  static const char *toString(i64 value, char (&buffer)[b64BufferSize]);
  static const char *toString(i32 value, char (&buffer)[b32BufferSize]);
  static const char *toString(i16 value, char (&buffer)[b16BufferSize]);
  static const char *toString(i8  value, char (&buffer)[ b8BufferSize]);

private:
  static const char *internalToString(u64 value, char *buffer);
  static const char *internalToString(i64 value, char *buffer);
};
