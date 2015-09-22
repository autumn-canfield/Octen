#ifndef _TYPES_H
#define _TYPES_H

typedef signed char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
//typedef unsigned long u32;
typedef long long i64;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef enum{ false, true } bool;

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

#endif // _TYPES_H

