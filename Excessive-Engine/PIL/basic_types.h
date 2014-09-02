#ifndef basic_types_h
#define basic_types_h

#include <cstdint>

//basic types
typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef float f32;
typedef double f64;

static_assert(sizeof(u8) == 1, "u8 is not 1 bytes in size!");
static_assert(sizeof(u16) == 2, "u16 is not 2 bytes in size!");
static_assert(sizeof(u32) == 4, "u32 is not 4 bytes in size!");
static_assert(sizeof(u64) == 8, "u64 is not 8 bytes in size!");
static_assert(sizeof(f32) == 4, "f32 is not 4 bytes in size!");
static_assert(sizeof(f64) == 8, "f64 is not 8 bytes in size!");

//handle endianness
inline u8 swap_u8(u8 val)
{
  return val;
}

inline u16 swap_u16(u16 val)
{
  return ((val & 0x00ff) << 8) |
         ((val & 0xff00) >> 8);
}

inline u32 swap_u32(u32 val)
{
  return ((val & 0x000000ff) << 24) |
         ((val & 0x0000ff00) <<  8) |
         ((val & 0x00ff0000) >>  8) |
         ((val & 0xff000000) >> 24);
}

inline u64 swap_u64(u64 val)
{
  return ((val & 0x00000000000000ff) << 56) |
         ((val & 0x000000000000ff00) << 40) |
         ((val & 0x0000000000ff0000) << 24) |
         ((val & 0x00000000ff000000) <<  8) |
         ((val & 0x000000ff00000000) >>  8) |
         ((val & 0x0000ff0000000000) >> 24) |
         ((val & 0x00ff000000000000) >> 40) |
         ((val & 0xff00000000000000) >> 56);
}

union u32_f32
{
    u32 as_u32;
    f32 as_f32;
};

inline f32 swap_f32(f32 val)
{
  u32_f32 conv;
  conv.as_f32 = val;
  conv.as_u32 = swap_u32(conv.as_u32);
  return conv.as_f32;
}

union u64_f64
{
    u64 as_u64;
    f64 as_f64;
};

inline f64 swap_f64(f64 val)
{
  u64_f64 conv;
  conv.as_f64 = val;
  conv.as_u64 = swap_u64(conv.as_u64);
  return conv.as_f64;
}


#endif
