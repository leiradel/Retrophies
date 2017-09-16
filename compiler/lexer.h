#ifndef RETROPHIES_LEXER_H
#define RETROPHIES_LEXER_H

#include <stddef.h>
#include <stdint.h>

enum
{
  RETROPHIES_TOKEN_EOF = 256,

  /* Literals */
  RETROPHIES_TOKEN_FALSE,
  RETROPHIES_TOKEN_IDENTIFIER,
  RETROPHIES_TOKEN_INTEGER_LITERAL,
  RETROPHIES_TOKEN_FLOAT_LITERAL,
  RETROPHIES_TOKEN_STRING_LITERAL,
  RETROPHIES_TOKEN_TRUE,

  /* Control-flow */
  RETROPHIES_TOKEN_ELSE,
  RETROPHIES_TOKEN_ELSEIF,
  RETROPHIES_TOKEN_END,
  RETROPHIES_TOKEN_FOR,
  RETROPHIES_TOKEN_IF,
  RETROPHIES_TOKEN_NEXT,
  RETROPHIES_TOKEN_REPEAT,
  RETROPHIES_TOKEN_STEP,
  RETROPHIES_TOKEN_THEN,
  RETROPHIES_TOKEN_TO,
  RETROPHIES_TOKEN_UNTIL,
  RETROPHIES_TOKEN_WHILE,

  /* Declarations */
  RETROPHIES_TOKEN_AS,
  RETROPHIES_TOKEN_DIM,
  RETROPHIES_TOKEN_FUNCTION,
  RETROPHIES_TOKEN_HANDLES,
  RETROPHIES_TOKEN_STATIC,
  RETROPHIES_TOKEN_SUBROUTINE,
  RETROPHIES_TOKEN_TROPHY,
  
  /* Commands */
  RETROPHIES_TOKEN_AWARD,
  RETROPHIES_TOKEN_RETURN,

  /* Functions */
  RETROPHIES_TOKEN_ABS,
  RETROPHIES_TOKEN_CEIL,
  RETROPHIES_TOKEN_FLOOR,
  RETROPHIES_TOKEN_RAND,
  RETROPHIES_TOKEN_TRUNC,

  /* Types */
  RETROPHIES_TOKEN_BOOLEAN,
  RETROPHIES_TOKEN_DOUBLE,
  RETROPHIES_TOKEN_INT8,
  RETROPHIES_TOKEN_INT16,
  RETROPHIES_TOKEN_INT32,
  RETROPHIES_TOKEN_INT64,
  RETROPHIES_TOKEN_SINGLE,
  RETROPHIES_TOKEN_STRING,
  RETROPHIES_TOKEN_UINT8,
  RETROPHIES_TOKEN_UINT16,
  RETROPHIES_TOKEN_UINT32,
  RETROPHIES_TOKEN_UINT64,

  /* Operators */
  RETROPHIES_TOKEN_AND,
  RETROPHIES_TOKEN_GREATEREQUAL,
  RETROPHIES_TOKEN_LEFTSHIFT,
  RETROPHIES_TOKEN_LESSEQUAL,
  RETROPHIES_TOKEN_MOD,
  RETROPHIES_TOKEN_NOT,
  RETROPHIES_TOKEN_NOTEQUAL,
  RETROPHIES_TOKEN_OR,
  RETROPHIES_TOKEN_RIGHTSHIFT,
  RETROPHIES_TOKEN_XOR,

  /* Memory */
  RETROPHIES_TOKEN_BIGENDIAN,
  RETROPHIES_TOKEN_BINARYCODEDDECIMAL,
  RETROPHIES_TOKEN_LITTLEENDIAN,

  RETROPHIES_TOKEN_LAST
};

typedef struct
{
  const char* chars;
  size_t      length;
}
retrophies_lexer_string_t;

typedef struct
{
  int                       token;
  retrophies_lexer_string_t lexeme;
  uint32_t                  hash;
  int                       line;
}
retrophies_lexer_lookahead_t;

typedef struct
{
  const char*    source_name;
  int            line;
  const uint8_t* source;
  const uint8_t* end;
  int            last_char;
  char*          error;
  size_t         error_size;
}
retrophies_lexer_t;

void retrophies_lexer_init(retrophies_lexer_t* self, const char* source_name, const void* source, size_t length, char* error, size_t error_size);
int  retrophies_lexer_next(retrophies_lexer_t* self, retrophies_lexer_lookahead_t* la);
void retrophies_lexer_getlexeme(char* buffer, size_t size, int token);

#endif /* RETROPHIES_LEXER_H */
