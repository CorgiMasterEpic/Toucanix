/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#pragma once

/*
[Warning]:
This header is designed for systems UEFI programming in kernel land.
*This should apply to any x86_64 uefi booting enviorment if your using the following:
Clang/Clang++
Gcc/G++

[Conventions Notice]:
Rust integer / decimal naming conventions.
Extras like byte or word.
*/


// ---------------------- //
// ---- ABI CONTRACT ---- //
// ---------------------- //

// Architecture
static_assert(sizeof(void*) == 8, "ABI error: requires 64-bit pointers");

// Integer sizes
static_assert(sizeof(char)      == 1, "ABI error: char must be 1 byte");
static_assert(sizeof(short)     == 2, "ABI error: short must be 16-bit");
static_assert(sizeof(int)       == 4, "ABI error: int must be 32-bit");
static_assert(sizeof(long long) == 8, "ABI error: long long must be 64-bit");

// Endianness (Clang/GCC)
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__,
              "ABI error: requires little-endian");
#else
#error "ABI error: compiler does not expose byte order macros"
#endif

// Compiler
#if !defined(__clang__) && !defined(__GNUC__)
#error "ABI error: requires Clang or GCC"
#endif

// Language
#if !defined(__cplusplus)
#error "ABI error: requires C++"
#endif

//
// -- Number Types --
//

// 8-bit integers
typedef char            i8;
typedef unsigned char   u8;
typedef u8              byte;

// 16-bit integers
typedef short           i16;
typedef unsigned short  u16;
typedef u16             word;

// 32-bit integers
typedef int             i32;
typedef unsigned int    u32;
typedef u32             dword;

// 64-bit integers
typedef long long           i64;
typedef unsigned long long  u64;
typedef u64                 qword;

// 128-bit integers
typedef __int128           i128;
typedef unsigned __int128  u128;
typedef u128               dqword;

// Native integers
typedef u64   usize;
typedef i64   isize;
typedef usize nword;

// Floating point
typedef float  f32;
typedef double f64;

// Address types
typedef u64    addr; // Ragular addreess
typedef void* raddr; // Raw address

//
// -- Storage Sizes --
//

// Binary (computer-correct, base 1024)
constexpr usize KiB = 1024ULL;
constexpr usize MiB = KiB * 1024;
constexpr usize GiB = MiB * 1024;
constexpr usize TiB = GiB * 1024;
constexpr usize PiB = TiB * 1024;

// Decimal (human-marketing, base 1000)
constexpr usize kB = 1000ULL;
constexpr usize MB = kB * 1000;
constexpr usize GB = MB * 1000;
constexpr usize TB = GB * 1000;
constexpr usize PB = TB * 1000;

//
// -- Attribute helpers --
//

#define PACKED      __attribute__((packed))
#define ALIGNED(x)  __attribute__((aligned(x)))
#define NORETURN    __attribute__((noreturn))
#define UNUSED      __attribute__((unused))
#define UNSAFE     [[deprecated("THIS FUNCTION IS NOT DEPRECATED, This function is just considered unsafe!")]]
