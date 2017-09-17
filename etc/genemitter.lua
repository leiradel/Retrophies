-- Only append
local insns = {
  {'RETROPHIES_INSN_ABS_F32', 'void'},
  {'RETROPHIES_INSN_ABS_F64', 'void'},
  {'RETROPHIES_INSN_ABS_I32', 'void'},
  {'RETROPHIES_INSN_ABS_I64', 'void'},
  {'RETROPHIES_INSN_ADD_F32', 'void'},
  {'RETROPHIES_INSN_ADD_F64', 'void'},
  {'RETROPHIES_INSN_ADD_X32', 'void'},
  {'RETROPHIES_INSN_ADD_X64', 'void'},
  {'RETROPHIES_INSN_AND_BOOL', 'void'},
  {'RETROPHIES_INSN_AND_X32', 'void'},
  {'RETROPHIES_INSN_AND_X64', 'void'},
  {'RETROPHIES_INSN_CALL', 'uint32_t'},
  {'RETROPHIES_INSN_CEIL_F32', 'void'},
  {'RETROPHIES_INSN_CEIL_F64', 'void'},
  {'RETROPHIES_INSN_CHECKFOR_F32', 'uint32_t'},
  {'RETROPHIES_INSN_CHECKFOR_F64', 'uint32_t'},
  {'RETROPHIES_INSN_CHECKFOR_I32', 'uint32_t'},
  {'RETROPHIES_INSN_CHECKFOR_I64', 'uint32_t'},
  {'RETROPHIES_INSN_CHECKFOR_U32', 'uint32_t'},
  {'RETROPHIES_INSN_CHECKFOR_U64', 'uint32_t'},
  {'RETROPHIES_INSN_DIV_F32', 'void'},
  {'RETROPHIES_INSN_DIV_F64', 'void'},
  {'RETROPHIES_INSN_DIV_I32', 'void'},
  {'RETROPHIES_INSN_DIV_I64', 'void'},
  {'RETROPHIES_INSN_DIV_U32', 'void'},
  {'RETROPHIES_INSN_DIV_U64', 'void'},
  {'RETROPHIES_INSN_EQUAL_F32', 'void'},
  {'RETROPHIES_INSN_EQUAL_F64', 'void'},
  {'RETROPHIES_INSN_EQUAL_X32', 'void'},
  {'RETROPHIES_INSN_EQUAL_X64', 'void'},
  {'RETROPHIES_INSN_FLOOR_F32', 'void'},
  {'RETROPHIES_INSN_FLOOR_F64', 'void'},
  {'RETROPHIES_INSN_GETGLOBAL', 'uint16_t'},
  {'RETROPHIES_INSN_GETLOCAL', 'uint16_t'},
  {'RETROPHIES_INSN_GREATEREQUAL_F32', 'void'},
  {'RETROPHIES_INSN_GREATEREQUAL_F64', 'void'},
  {'RETROPHIES_INSN_GREATEREQUAL_I32', 'void'},
  {'RETROPHIES_INSN_GREATEREQUAL_I64', 'void'},
  {'RETROPHIES_INSN_GREATEREQUAL_U32', 'void'},
  {'RETROPHIES_INSN_GREATEREQUAL_U64', 'void'},
  {'RETROPHIES_INSN_GREATERTHAN_F32', 'void'},
  {'RETROPHIES_INSN_GREATERTHAN_F64', 'void'},
  {'RETROPHIES_INSN_GREATERTHAN_I32', 'void'},
  {'RETROPHIES_INSN_GREATERTHAN_I64', 'void'},
  {'RETROPHIES_INSN_GREATERTHAN_U32', 'void'},
  {'RETROPHIES_INSN_GREATERTHAN_U64', 'void'},
  {'RETROPHIES_INSN_IDIV_U32', 'void'},
  {'RETROPHIES_INSN_IDIV_U64', 'void'},
  {'RETROPHIES_INSN_IDIV_X32', 'void'},
  {'RETROPHIES_INSN_IDIV_X64', 'void'},
  {'RETROPHIES_INSN_JE', 'uint32_t'},
  {'RETROPHIES_INSN_JNE', 'uint32_t'},
  {'RETROPHIES_INSN_JUMP', 'uint32_t'},
  {'RETROPHIES_INSN_LESSEQUAL_F32', 'void'},
  {'RETROPHIES_INSN_LESSEQUAL_F64', 'void'},
  {'RETROPHIES_INSN_LESSEQUAL_I32', 'void'},
  {'RETROPHIES_INSN_LESSEQUAL_I64', 'void'},
  {'RETROPHIES_INSN_LESSEQUAL_U32', 'void'},
  {'RETROPHIES_INSN_LESSEQUAL_U64', 'void'},
  {'RETROPHIES_INSN_LESSTHAN_F32', 'void'},
  {'RETROPHIES_INSN_LESSTHAN_F64', 'void'},
  {'RETROPHIES_INSN_LESSTHAN_I32', 'void'},
  {'RETROPHIES_INSN_LESSTHAN_I64', 'void'},
  {'RETROPHIES_INSN_LESSTHAN_U32', 'void'},
  {'RETROPHIES_INSN_LESSTHAN_U64', 'void'},
  {'RETROPHIES_INSN_MOD_F32', 'void'},
  {'RETROPHIES_INSN_MOD_F64', 'void'},
  {'RETROPHIES_INSN_MOD_I32', 'void'},
  {'RETROPHIES_INSN_MOD_I64', 'void'},
  {'RETROPHIES_INSN_MOD_U32', 'void'},
  {'RETROPHIES_INSN_MOD_U64', 'void'},
  {'RETROPHIES_INSN_MUL_F32', 'void'},
  {'RETROPHIES_INSN_MUL_F64', 'void'},
  {'RETROPHIES_INSN_MUL_X32', 'void'},
  {'RETROPHIES_INSN_MUL_X64', 'void'},
  {'RETROPHIES_INSN_NEG_F32', 'void'},
  {'RETROPHIES_INSN_NEG_F64', 'void'},
  {'RETROPHIES_INSN_NEG_I32', 'void'},
  {'RETROPHIES_INSN_NEG_I64', 'void'},
  {'RETROPHIES_INSN_NEXT_F32', 'uint32_t'},
  {'RETROPHIES_INSN_NEXT_F64', 'uint32_t'},
  {'RETROPHIES_INSN_NEXT_I32', 'uint32_t'},
  {'RETROPHIES_INSN_NEXT_I64', 'uint32_t'},
  {'RETROPHIES_INSN_NEXT_U32', 'uint32_t'},
  {'RETROPHIES_INSN_NEXT_U64', 'uint32_t'},
  {'RETROPHIES_INSN_NOT', 'void'},
  {'RETROPHIES_INSN_NOTEQUAL_F32', 'void'},
  {'RETROPHIES_INSN_NOTEQUAL_F64', 'void'},
  {'RETROPHIES_INSN_NOTEQUAL_X32', 'void'},
  {'RETROPHIES_INSN_NOTEQUAL_X64', 'void'},
  {'RETROPHIES_INSN_OR_BOOL', 'void'},
  {'RETROPHIES_INSN_OR_X32', 'void'},
  {'RETROPHIES_INSN_OR_X64', 'void'},
  {'RETROPHIES_INSN_PUSH_0', 'void'},
  {'RETROPHIES_INSN_PUSH_1', 'void'},
  {'RETROPHIES_INSN_PUSH_10', 'void'},
  {'RETROPHIES_INSN_PUSH_16', 'void'},
  {'RETROPHIES_INSN_PUSH_2', 'void'},
  {'RETROPHIES_INSN_PUSH_F32', 'float'},
  {'RETROPHIES_INSN_PUSH_F64', 'double'},
  {'RETROPHIES_INSN_PUSH_I16', 'int16_t'},
  {'RETROPHIES_INSN_PUSH_I32', 'int32_t'},
  {'RETROPHIES_INSN_PUSH_I64', 'int64_t'},
  {'RETROPHIES_INSN_PUSH_I8', 'int8_t'},
  {'RETROPHIES_INSN_PUSH_U16', 'uint16_t'},
  {'RETROPHIES_INSN_PUSH_U32', 'uint32_t'},
  {'RETROPHIES_INSN_PUSH_U64', 'uint64_t'},
  {'RETROPHIES_INSN_PUSH_U8', 'int8_t'},
  {'RETROPHIES_INSN_RAND', 'void'},
  {'RETROPHIES_INSN_RETURN', 'void'},
  {'RETROPHIES_INSN_RETURN_X32', 'void'},
  {'RETROPHIES_INSN_RETURN_X64', 'void'},
  {'RETROPHIES_INSN_SETGLOBAL', 'uint16_t'},
  {'RETROPHIES_INSN_SETLOCAL', 'uint16_t'},
  {'RETROPHIES_INSN_SHIFTLEFT_X32', 'void'},
  {'RETROPHIES_INSN_SHIFTLEFT_X64', 'void'},
  {'RETROPHIES_INSN_SHIFTRIGHT_I32', 'void'},
  {'RETROPHIES_INSN_SHIFTRIGHT_I64', 'void'},
  {'RETROPHIES_INSN_SHIFTRIGHT_U32', 'void'},
  {'RETROPHIES_INSN_SHIFTRIGHT_U64', 'void'},
  {'RETROPHIES_INSN_SUB_F32', 'void'},
  {'RETROPHIES_INSN_SUB_F64', 'void'},
  {'RETROPHIES_INSN_SUB_X32', 'void'},
  {'RETROPHIES_INSN_SUB_X64', 'void'},
  {'RETROPHIES_INSN_SYSTEM', 'uint16_t'},
  {'RETROPHIES_INSN_TRUNC_F32', 'void'},
  {'RETROPHIES_INSN_TRUNC_F64', 'void'},
  {'RETROPHIES_INSN_XOR_BOOL', 'void'},
  {'RETROPHIES_INSN_XOR_X32', 'void'},
  {'RETROPHIES_INSN_XOR_X64', 'void'},
}

if #insns % 2 == 1 then
  insns[#insns + 1] = {nil, 'void'}
end

local sizes = {
  void = 1,
  int8_t = 2,
  uint8_t = 2,
  int16_t = 3,
  uint16_t = 3,
  int32_t = 5,
  uint32_t = 5,
  int64_t = 7,
  uint64_t = 7,
  float = 5,
  double = 7
}

local code = {
  void = 0,
  int8_t = 1,
  uint8_t = 2,
  int16_t = 3,
  uint16_t = 4,
  int32_t = 5,
  uint32_t = 6,
  int64_t = 7,
  uint64_t = 8,
  float = 9,
  double = 10
}

-- Header
io.write[[
#ifndef RETROPHIES_EMITTER_H
#define RETROPHIES_EMITTER_H

#include <stdarg.h>

enum
{
]]

for i = 1, #insns do
  local name = insns[i][1]

  if name then
    io.write('  ', name, ',\n')
  end
end

io.write[[
};

enum
{
  RETROPHIES_SYSTEM_AWARD,
  RETROPHIES_SYSTEM_MEMSCAN
};

int  retrophies_emitter_size(int insn);
void retrophies_emitter_emit(void* buffer, int insn, va_list args);
void retrophies_emitter_patch(void* jump, int label);

#endif /* RETROPHIES_EMITTER_H */
]]

-- C
io.write[[
#include "emitter.h"

#include <stdint.h>

]]

io.write('static const uint8_t retrophies_emitter_types[', (#insns + 1) // 2, '] =\n{')

for i = 1, #insns, 2 do
  if (i - 1) % 32 == 0 then
    io.write('\n  ')
  else
    io.write(' ')
  end

  local s1 = code[insns[i][2]]
  local s2 = code[insns[i + 1][2]]
  local val = s2 * 16 + s1

  io.write(string.format('0x%02x', val), ',')
end

if #insns % 32 ~= 0 then
  io.write('\n')
end

io.write[[
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
]]
