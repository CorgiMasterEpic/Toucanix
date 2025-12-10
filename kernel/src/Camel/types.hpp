#pragma once

// Standard number types
typedef unsigned char u8;
typedef char i8;

typedef unsigned short u16;
typedef short i16;

typedef unsigned int u32;
typedef int i32;

typedef unsigned long u64;
typedef long i64;

typedef u64 usize;
typedef i64 isize;

typedef float f32;
typedef double f64;

// Unstandard number types
typedef u8 *u8p;
typedef i8 *i8p;

typedef u16 *u16p;
typedef i16 *i16p;

typedef u32 *u32p;
typedef i32 *i32p;

typedef u64 *u64p;
typedef i64 *i64p;

typedef f32 *f32p;
typedef f64 *f64p;

typedef u64p usizep;
typedef i64p isizep;

// Missing types
typedef i8p string;
typedef const i8 *stringc;
typedef void *any;
typedef any *anyp;
typedef any addr;