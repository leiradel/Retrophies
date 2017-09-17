#include "emitter.h"

#include <stdint.h>

static const uint8_t retrophies_emitter_types[65] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x66, 0x66, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xa9, 0x53, 0x17, 0x64, 0x18, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
  0x00,
};

int retrophies_emitter_size(int insn)
{
  static const uint8_t sizes[16] =
  {
    1, 2, 2, 3, 3, 5, 5, 7, 7, 5, 7, 0, 0, 0, 0, 0
  };

  int type  = retrophies_emitter_types[insn / 2];
  int shift = (insn & 1) * 4;

  return sizes[(type >> shift) & 0x0f];
}

void retrophies_emitter_emit(void* buffer, int insn, va_list args)
{
  uint8_t* code = (uint8_t*)code;
  *code++ = insn;

  int type  = retrophies_emitter_types[insn / 2];
  int shift = (insn & 1) * 4;

  uint16_t u16;
  uint32_t u32;
  uint64_t u64;

  union { float f32; uint32_t u32; }  f32;
  union { double f64; uint64_t u64; } f64;

  switch ((type >> shift) & 0x0f)
  {
  case 0:  /* void */
  case 11: /* invalid */
  case 12: /* invalid */
  case 13: /* invalid */
  case 14: /* invalid */
  case 15: /* invalid */
    break;
  
  case 1: /* int8_t */
  case 2: /* uint8_t */
    *code = (uint8_t)va_arg(args, int);
    break;
  
  case 3: /* int16_t */
  case 4: /* uint16_t */
    u16 = (uint16_t)va_arg(args, int);
    *code++ = u16;
    *code = u16 >> 8;
    break;
  
  case 5: /* int32_t */
  case 6: /* uint32_t */
    u32 = va_arg(args, uint32_t);
    *code++ = u32;
    *code++ = u32 >> 8;
    *code++ = u32 >> 16;
    *code = u32 >> 24;
    break;
  
  case 7: /* int64_t */
  case 8: /* uint64_t */
    u64 = va_arg(args, uint64_t);
    *code++ = u64;
    *code++ = u64 >> 8;
    *code++ = u64 >> 16;
    *code++ = u64 >> 24;
    *code++ = u64 >> 32;
    *code++ = u64 >> 40;
    *code++ = u64 >> 48;
    *code = u64 >> 56;
    break;
  
  case 9: /* float */
    f32.f32 = (float)va_arg(args, double);
    u32 = f32.u32;
    *code++ = u32;
    *code++ = u32 >> 8;
    *code++ = u32 >> 16;
    *code = u32 >> 24;
    break;
  
  case 10: /* double */
    f64.f64 = va_arg(args, double);
    u64 = f64.u64;
    *code++ = u64;
    *code++ = u64 >> 8;
    *code++ = u64 >> 16;
    *code++ = u64 >> 24;
    *code++ = u64 >> 32;
    *code++ = u64 >> 40;
    *code++ = u64 >> 48;
    *code = u64 >> 56;
    break;
  }
}

void retrophies_emitter_patch(void* jump, int label)
{
  uint8_t* code = (uint8_t*)jump + 1;
  *code++ = label;
  *code++ = label >> 8;
  *code++ = label >> 16;
  *code   = label >> 24;
}
