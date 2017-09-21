#include "parser.h"
#include "lexer.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define RETROPHIES_PARSER_ALLOC(s) ((s*)retrophies_parser_alloc(self, sizeof(s), RETROPHIES_PARSER_ALIGNOF(s)))

static void retrophies_parser_error(retrophies_parser_t* self, int line, const char* format, ...)
{
  int written = snprintf(self->lexer.error, self->lexer.error_size, "%s:%d: ", self->lexer.source_name, line);

  va_list args;

  va_start(args, format);
  vsnprintf(self->lexer.error + written, self->lexer.error_size - written, format, args);
  va_end(args);

  longjmp(self->rollback, 1);
}

static void* retrophies_parser_alloc(retrophies_parser_t* self, size_t size, size_t alignment)
{
  alignment--;
  self->temp_ptr = (self->temp_ptr + alignment) & ~alignment;

  if (self->temp_ptr + size > self->temp_size)
  {
    retrophies_parser_error(self, self->la.line, "Out of memory");
    /* never returns */
  }

  void* ptr = (void*)(self->temp_buffer + self->temp_ptr);
  self->temp_ptr += size;
  return ptr;
}

static retrophies_parser_local_t* retrophies_parser_alloclocal(retrophies_parser_t* self)
{
  if (self->free_locals != NULL)
  {
    retrophies_parser_local_t* local = self->free_locals;
    self->free_locals = local->previous;
    return local;
  }

  return RETROPHIES_PARSER_ALLOC(retrophies_parser_local_t);
}

static void retrophies_parser_freelocals(retrophies_parser_t* self, retrophies_parser_local_t* previous)
{
  retrophies_parser_local_t* local = self->sub->locals;

  if (local != NULL)
  {
    int num_locals = self->sub->num_locals;

    while (local->previous != previous)
    {
      local = local->previous;
      num_locals--;
    }

    local->previous = self->free_locals;
    self->free_locals = local;
    self->sub->num_locals = num_locals;
  }

  self->sub->locals = previous;
}

static int retrophies_parser_emit(retrophies_parser_t* self, int insn, ...)
{
  retrophies_parser_codeblock_t* code = self->code;
  int size = retrophies_emitter_size(insn);
  int pc = self->code_size;

  if (size > code->count)
  {
    code = RETROPHIES_PARSER_ALLOC(retrophies_parser_codeblock_t);
    code->count = 0;
    self->code->next = code;
    self->code = code;
  }

  va_list args;
  va_start(args, insn);
  retrophies_emitter_emit((void*)(code->code + code->count), insn, args);
  code->count += size;
  self->code_size += size;
  va_end(args);

  return pc;
}

static retrophies_parser_local_t* retrophies_parser_findlocal(retrophies_parser_subroutine_t* sub, uint32_t hash)
{
  retrophies_parser_local_t* local = sub->locals;

  if (local != NULL)
  {
    do
    {
      if (local->name.hash == hash)
      {
        return local;
      }

      local = local->previous;
    }
    while (local != NULL);
  }

  return NULL;
}

static retrophies_parser_global_t* retrophies_parser_findglobal(retrophies_parser_t* self, uint32_t hash)
{
  retrophies_parser_global_t* global = self->globals;

  if (global != NULL)
  {
    do
    {
      if (global->name.hash == hash)
      {
        return global;
      }

      global = global->previous;
    }
    while (global != NULL);
  }

  return NULL;
}

static void retrophies_parser_matchany(retrophies_parser_t* self)
{
  if (retrophies_lexer_next(&self->lexer, &self->la) != 0)
  {
    longjmp(self->rollback, 1);
  }
}

static int retrophies_parser_matchopt(retrophies_parser_t* self, int token)
{
  if (self->la.token == token)
  {
    retrophies_parser_matchany(self);
    return 1;
  }

  return 0;
}

static void retrophies_parser_match(retrophies_parser_t* self, int token)
{
  if (self->la.token != token)
  {
    char lexeme[256];
    retrophies_lexer_getlexeme(lexeme, sizeof(lexeme), token);
    retrophies_parser_error(self, self->la.line, "%s expected", lexeme);
    /* never returns */
  }

  retrophies_parser_matchany(self);
}

static int retrophies_parser_parsetype(retrophies_parser_t* self)
{
  int type = self->la.token;

  switch (type)
  {
  case RETROPHIES_TOKEN_BOOLEAN:
  case RETROPHIES_TOKEN_DOUBLE:
  case RETROPHIES_TOKEN_INT8:
  case RETROPHIES_TOKEN_INT16:
  case RETROPHIES_TOKEN_INT32:
  case RETROPHIES_TOKEN_INT64:
  case RETROPHIES_TOKEN_SINGLE:
  case RETROPHIES_TOKEN_UINT8:
  case RETROPHIES_TOKEN_UINT16:
  case RETROPHIES_TOKEN_UINT32:
  case RETROPHIES_TOKEN_UINT64:
    retrophies_parser_matchany(self);
    return type;
  }

  retrophies_parser_error(self, self->la.line, "Type expected");
  /* never returns */

  return 0; /* silence the compiler */
}

static void retrophies_parser_parseexpression(retrophies_parser_t* self, int type)
{
  (void)self;
  (void)type;
}

static int retrophies_parser_parsestatements(retrophies_parser_t* self);

#include "dim.c"
#include "for.c"
#include "if.c"
#include "repeat.c"
#include "return.c"
#include "while.c"
#include "stmts.c"
#include "subs.c"
#include "program.c"

void retrophies_parser_init(retrophies_parser_t* self, const char* source_name, const void* source, size_t length, void* temp_buffer, size_t temp_size, char* error, size_t error_size)
{
  retrophies_lexer_init(&self->lexer, source_name, source, length, error, error_size);

  self->temp_ptr = 0;
  self->temp_size = temp_size;
  self->temp_buffer = (uint8_t*)temp_buffer;
}

int retrophies_parser_parse(retrophies_parser_t* self)
{
  if (setjmp(self->rollback) != 0)
  {
    return -1;
  }

  retrophies_parser_matchany(self);
  retrophies_parser_parseprogram(self);
  return 0;
}
