#include "emitter.h"

#include <stdint.h>

static void retrophies_emitter_emit_void(uint8_t* code, int insn)
{
  *code = insn;
}

static void retrophies_emitter_emit_i8(uint8_t* code, int insn, int8_t imm)
{
  *code++ = insn;
  *code   = imm;
}

static void retrophies_emitter_emit_u8(uint8_t* code, int insn, uint8_t imm)
{
  *code++ = insn;
  *code   = imm;
}

static void retrophies_emitter_emit_i16(uint8_t* code, int insn, int16_t imm)
{
  *code++ = insn;
  *code++ = imm;
  *code   = imm >> 8;
}

static void retrophies_emitter_emit_u16(uint8_t* code, int insn, uint16_t imm)
{
  *code++ = insn;
  *code++ = imm;
  *code   = imm >> 8;
}

static void retrophies_emitter_emit_i32(uint8_t* code, int insn, int32_t imm)
{
  *code++ = insn;
  *code++ = imm;
  *code++ = imm >> 8;
  *code++ = imm >> 16;
  *code   = imm >> 24;
}

static void retrophies_emitter_emit_u32(uint8_t* code, int insn, uint32_t imm)
{
  *code++ = insn;
  *code++ = imm;
  *code++ = imm >> 8;
  *code++ = imm >> 16;
  *code   = imm >> 24;
}

static void retrophies_emitter_emit_i64(uint8_t* code, int insn, int64_t imm)
{
  *code++ = insn;
  *code++ = imm;
  *code++ = imm >> 8;
  *code++ = imm >> 16;
  *code++ = imm >> 24;
  *code++ = imm >> 32;
  *code++ = imm >> 40;
  *code++ = imm >> 48;
  *code   = imm >> 56;
}

static void retrophies_emitter_emit_u64(uint8_t* code, int insn, uint64_t imm)
{
  *code++ = insn;
  *code++ = imm;
  *code++ = imm >> 8;
  *code++ = imm >> 16;
  *code++ = imm >> 24;
  *code++ = imm >> 32;
  *code++ = imm >> 40;
  *code++ = imm >> 48;
  *code   = imm >> 56;
}

static void retrophies_emitter_emit_f32(uint8_t* code, int insn, float imm)
{
  union { uint32_t u32; float f32; } bits;
  bits.f32 = imm;
  retrophies_emitter_emit_u32(code, insn, bits.u32);
}

static void retrophies_emitter_emit_f64(uint8_t* code, int insn, double imm)
{
  union { uint64_t u64; double f64; } bits;
  bits.f64 = imm;
  retrophies_emitter_emit_u64(code, insn, bits.u64);
}

int retrophies_emitter_size(int insn)
{
  if (insn <= RETROPHIES_INSN_LAST_1BYTE)
  {
    return 1;
  }
  else if (insn <= RETROPHIES_INSN_LAST_2BYTES)
  {
    return 2;
  }
  else if (insn <= RETROPHIES_INSN_LAST_3BYTES)
  {
    return 3;
  }
  else if (insn <= RETROPHIES_INSN_LAST_5BYTES)
  {
    return 5;
  }
  else /*if (insn <= RETROPHIES_INSN_LAST_7BYTE)*/
  {
    return 7;
  }
}

void retrophies_emitter_emit(void* buffer, int insn, va_list args)
{
  uint8_t* code = (uint8_t*)code;

  switch (insn)
  {
  case RETROPHIES_INSN_ABS_I32:          /* fall through */
  case RETROPHIES_INSN_ABS_I64:          /* fall through */
  case RETROPHIES_INSN_ABS_F32:          /* fall through */
  case RETROPHIES_INSN_ABS_F64:          /* fall through */
  case RETROPHIES_INSN_ADD_X32:          /* fall through */
  case RETROPHIES_INSN_ADD_X64:          /* fall through */
  case RETROPHIES_INSN_ADD_F32:          /* fall through */
  case RETROPHIES_INSN_ADD_F64:          /* fall through */
  case RETROPHIES_INSN_AND_BOOL:         /* fall through */
  case RETROPHIES_INSN_AND_X32:          /* fall through */
  case RETROPHIES_INSN_AND_X64:          /* fall through */
  case RETROPHIES_INSN_CEIL_F32:         /* fall through */
  case RETROPHIES_INSN_CEIL_F64:         /* fall through */
  case RETROPHIES_INSN_DIV_I32:          /* fall through */
  case RETROPHIES_INSN_DIV_I64:          /* fall through */
  case RETROPHIES_INSN_DIV_U32:          /* fall through */
  case RETROPHIES_INSN_DIV_U64:          /* fall through */
  case RETROPHIES_INSN_DIV_F32:          /* fall through */
  case RETROPHIES_INSN_DIV_F64:          /* fall through */
  case RETROPHIES_INSN_EQUAL_X32:        /* fall through */
  case RETROPHIES_INSN_EQUAL_X64:        /* fall through */
  case RETROPHIES_INSN_EQUAL_F32:        /* fall through */
  case RETROPHIES_INSN_EQUAL_F64:        /* fall through */
  case RETROPHIES_INSN_FLOOR_F32:        /* fall through */
  case RETROPHIES_INSN_FLOOR_F64:        /* fall through */
  case RETROPHIES_INSN_GREATEREQUAL_I32: /* fall through */
  case RETROPHIES_INSN_GREATEREQUAL_I64: /* fall through */
  case RETROPHIES_INSN_GREATEREQUAL_U32: /* fall through */
  case RETROPHIES_INSN_GREATEREQUAL_U64: /* fall through */
  case RETROPHIES_INSN_GREATEREQUAL_F32: /* fall through */
  case RETROPHIES_INSN_GREATEREQUAL_F64: /* fall through */
  case RETROPHIES_INSN_GREATERTHAN_I32:  /* fall through */
  case RETROPHIES_INSN_GREATERTHAN_I64:  /* fall through */
  case RETROPHIES_INSN_GREATERTHAN_U32:  /* fall through */
  case RETROPHIES_INSN_GREATERTHAN_U64:  /* fall through */
  case RETROPHIES_INSN_GREATERTHAN_F32:  /* fall through */
  case RETROPHIES_INSN_GREATERTHAN_F64:  /* fall through */
  case RETROPHIES_INSN_IDIV_X32:         /* fall through */
  case RETROPHIES_INSN_IDIV_X64:         /* fall through */
  case RETROPHIES_INSN_IDIV_U32:         /* fall through */
  case RETROPHIES_INSN_IDIV_U64:         /* fall through */
  case RETROPHIES_INSN_LESSEQUAL_I32:    /* fall through */
  case RETROPHIES_INSN_LESSEQUAL_I64:    /* fall through */
  case RETROPHIES_INSN_LESSEQUAL_U32:    /* fall through */
  case RETROPHIES_INSN_LESSEQUAL_U64:    /* fall through */
  case RETROPHIES_INSN_LESSEQUAL_F32:    /* fall through */
  case RETROPHIES_INSN_LESSEQUAL_F64:    /* fall through */
  case RETROPHIES_INSN_LESSTHAN_I32:     /* fall through */
  case RETROPHIES_INSN_LESSTHAN_I64:     /* fall through */
  case RETROPHIES_INSN_LESSTHAN_U32:     /* fall through */
  case RETROPHIES_INSN_LESSTHAN_U64:     /* fall through */
  case RETROPHIES_INSN_LESSTHAN_F32:     /* fall through */
  case RETROPHIES_INSN_LESSTHAN_F64:     /* fall through */
  case RETROPHIES_INSN_MOD_I32:          /* fall through */
  case RETROPHIES_INSN_MOD_I64:          /* fall through */
  case RETROPHIES_INSN_MOD_U32:          /* fall through */
  case RETROPHIES_INSN_MOD_U64:          /* fall through */
  case RETROPHIES_INSN_MOD_F32:          /* fall through */
  case RETROPHIES_INSN_MOD_F64:          /* fall through */
  case RETROPHIES_INSN_MUL_X32:          /* fall through */
  case RETROPHIES_INSN_MUL_X64:          /* fall through */
  case RETROPHIES_INSN_MUL_F32:          /* fall through */
  case RETROPHIES_INSN_MUL_F64:          /* fall through */
  case RETROPHIES_INSN_NEG_I32:          /* fall through */
  case RETROPHIES_INSN_NEG_I64:          /* fall through */
  case RETROPHIES_INSN_NEG_F32:          /* fall through */
  case RETROPHIES_INSN_NEG_F64:          /* fall through */
  case RETROPHIES_INSN_NOT:              /* fall through */
  case RETROPHIES_INSN_NOTEQUAL_X32:     /* fall through */
  case RETROPHIES_INSN_NOTEQUAL_X64:     /* fall through */
  case RETROPHIES_INSN_NOTEQUAL_F32:     /* fall through */
  case RETROPHIES_INSN_NOTEQUAL_F64:     /* fall through */
  case RETROPHIES_INSN_OR_BOOL:          /* fall through */
  case RETROPHIES_INSN_OR_X32:           /* fall through */
  case RETROPHIES_INSN_OR_X64:           /* fall through */
  case RETROPHIES_INSN_PUSH_0:           /* fall through */
  case RETROPHIES_INSN_PUSH_1:           /* fall through */
  case RETROPHIES_INSN_PUSH_2:           /* fall through */
  case RETROPHIES_INSN_PUSH_10:          /* fall through */
  case RETROPHIES_INSN_PUSH_16:          /* fall through */
  case RETROPHIES_INSN_RAND:             /* fall through */
  case RETROPHIES_INSN_RETURN_X32:       /* fall through */
  case RETROPHIES_INSN_RETURN_X64:       /* fall through */
  case RETROPHIES_INSN_SHIFTLEFT_X32:    /* fall through */
  case RETROPHIES_INSN_SHIFTLEFT_X64:    /* fall through */
  case RETROPHIES_INSN_SHIFTRIGHT_I32:   /* fall through */
  case RETROPHIES_INSN_SHIFTRIGHT_I64:   /* fall through */
  case RETROPHIES_INSN_SHIFTRIGHT_U32:   /* fall through */
  case RETROPHIES_INSN_SHIFTRIGHT_U64:   /* fall through */
  case RETROPHIES_INSN_SUB_X32:          /* fall through */
  case RETROPHIES_INSN_SUB_X64:          /* fall through */
  case RETROPHIES_INSN_SUB_F32:          /* fall through */
  case RETROPHIES_INSN_SUB_F64:          /* fall through */
  case RETROPHIES_INSN_TRUNC_F32:        /* fall through */
  case RETROPHIES_INSN_TRUNC_F64:        /* fall through */
  case RETROPHIES_INSN_XOR_BOOL:         /* fall through */
  case RETROPHIES_INSN_XOR_X32:          /* fall through */
  case RETROPHIES_INSN_XOR_X64:          retrophies_emitter_emit_void(code, insn); break;

  case RETROPHIES_INSN_PUSH_I8:          retrophies_emitter_emit_i8(code, insn, (int8_t)va_arg(args, int)); break;
  case RETROPHIES_INSN_PUSH_U8:          retrophies_emitter_emit_u8(code, insn, (uint8_t)va_arg(args, unsigned)); break;

  case RETROPHIES_INSN_GETGLOBAL:        /* fall through */
  case RETROPHIES_INSN_GETLOCAL:         /* fall through */
  case RETROPHIES_INSN_PUSH_U16:         /* fall through */
  case RETROPHIES_INSN_SETGLOBAL:        /* fall through */
  case RETROPHIES_INSN_SETLOCAL:         /* fall through */
  case RETROPHIES_INSN_SYSTEM:           retrophies_emitter_emit_u16(code, insn, (uint16_t)va_arg(args, unsigned)); break;
  case RETROPHIES_INSN_PUSH_I16:         retrophies_emitter_emit_i16(code, insn, (int16_t)va_arg(args, int)); break;

  case RETROPHIES_INSN_CALL:             /* fall through */
  case RETROPHIES_INSN_CHECKFOR_I32:     /* fall through */
  case RETROPHIES_INSN_CHECKFOR_U32:     /* fall through */
  case RETROPHIES_INSN_CHECKFOR_I64:     /* fall through */
  case RETROPHIES_INSN_CHECKFOR_U64:     /* fall through */
  case RETROPHIES_INSN_CHECKFOR_F32:     /* fall through */
  case RETROPHIES_INSN_CHECKFOR_F64:     /* fall through */
  case RETROPHIES_INSN_JE:               /* fall through */
  case RETROPHIES_INSN_JNE:              /* fall through */
  case RETROPHIES_INSN_JUMP:             /* fall through */
  case RETROPHIES_INSN_NEXT_I32:         /* fall through */
  case RETROPHIES_INSN_NEXT_U32:         /* fall through */
  case RETROPHIES_INSN_NEXT_I64:         /* fall through */
  case RETROPHIES_INSN_NEXT_U64:         /* fall through */
  case RETROPHIES_INSN_NEXT_F32:         /* fall through */
  case RETROPHIES_INSN_NEXT_F64:         /* fall through */
  case RETROPHIES_INSN_PUSH_U32:         retrophies_emitter_emit_u32(code, insn, va_arg(args, uint32_t)); break;
  case RETROPHIES_INSN_PUSH_I32:         retrophies_emitter_emit_i32(code, insn, va_arg(args, int32_t)); break;
  case RETROPHIES_INSN_PUSH_F32:         retrophies_emitter_emit_f32(code, insn, (float)va_arg(args, double)); break;
  
  case RETROPHIES_INSN_PUSH_I64:         retrophies_emitter_emit_i64(code, insn, va_arg(args, int64_t)); break;
  case RETROPHIES_INSN_PUSH_U64:         retrophies_emitter_emit_u64(code, insn, va_arg(args, uint64_t)); break;
  case RETROPHIES_INSN_PUSH_F64:         retrophies_emitter_emit_f64(code, insn, va_arg(args, double)); break;
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
