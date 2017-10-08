#ifndef RETROPHIES_EMITTER_H
#define RETROPHIES_EMITTER_H

#include <stdarg.h>

enum
{
  RETROPHIES_INSN_ABS,
  RETROPHIES_INSN_ADD,
  RETROPHIES_INSN_AND,
  RETROPHIES_INSN_CALL,
  RETROPHIES_INSN_CHECKFOR,
  RETROPHIES_INSN_DIV,
  RETROPHIES_INSN_EQUAL,
  RETROPHIES_INSN_GETGLOBAL,
  RETROPHIES_INSN_GETLOCAL,
  RETROPHIES_INSN_GREATEREQUAL,
  RETROPHIES_INSN_GREATERTHAN,
  RETROPHIES_INSN_JFALSE,
  RETROPHIES_INSN_JTRUE,
  RETROPHIES_INSN_JUMP,
  RETROPHIES_INSN_LESSEQUAL,
  RETROPHIES_INSN_LESSTHAN,
  RETROPHIES_INSN_MOD,
  RETROPHIES_INSN_MUL,
  RETROPHIES_INSN_NEG,
  RETROPHIES_INSN_NEXT,
  RETROPHIES_INSN_NOT,
  RETROPHIES_INSN_NOTEQUAL,
  RETROPHIES_INSN_OR,
  RETROPHIES_INSN_PUSH,
  RETROPHIES_INSN_PUSH_0,
  RETROPHIES_INSN_PUSH_1,
  RETROPHIES_INSN_PUSH_2,
  RETROPHIES_INSN_PUSH_10,
  RETROPHIES_INSN_PUSH_16,
  RETROPHIES_INSN_RAND,
  RETROPHIES_INSN_RETURN,
  RETROPHIES_INSN_RETURN_INTEGER,
  RETROPHIES_INSN_SETGLOBAL,
  RETROPHIES_INSN_SETLOCAL,
  RETROPHIES_INSN_SHIFTLEFT,
  RETROPHIES_INSN_SHIFTRIGHT,
  RETROPHIES_INSN_SUB,
  RETROPHIES_INSN_SYSTEM,
  RETROPHIES_INSN_XOR,
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
