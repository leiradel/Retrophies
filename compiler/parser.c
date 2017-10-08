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

static void retrophies_parser_pushscope(retrophies_parser_t* self)
{
  retrophies_parser_scope_t* scope;

  if (self->free_scopes != NULL)
  {
    scope = self->free_scopes;
    self->free_scopes = scope->previous;
  }
  else
  {
    scope = RETROPHIES_PARSER_ALLOC(retrophies_parser_scope_t);
  }

  scope->previous = self->sub->locals;
  self->sub->locals = scope;
}

static void retrophies_parser_popscope(retrophies_parser_t* self)
{
  retrophies_parser_var_t* var = self->sub->locals->vars;
  int num_locals = self->sub->num_locals;

  while (var != NULL)
  {
    retrophies_parser_var_t* save = var->previous;

    if ((var->flags & RETROPHIES_VAR_STATIC) != 0)
    {
      var->previous = self->globals.vars;
      self->globals.vars = var;
    }
    else
    {
      var->previous = self->free_vars;
      self->free_vars = var;
      num_locals--;
    }

    var = save;
  }
  
  self->sub->num_locals = num_locals;

  self->free_scopes = self->sub->locals;
  self->sub->locals = self->sub->locals->previous;
}

static retrophies_parser_var_t* retrophies_parser_allocvar(retrophies_parser_t* self)
{
  if (self->free_vars != NULL)
  {
    retrophies_parser_var_t* var = self->free_vars;
    self->free_vars = var->previous;
    return var;
  }

  return RETROPHIES_PARSER_ALLOC(retrophies_parser_var_t);
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

static int retrophies_parser_emitsetvar(retrophies_parser_t* self, const retrophies_parser_var_t* var)
{
  if ((var->flags & RETROPHIES_VAR_GLOBAL) != 0)
  {
    return retrophies_parser_emit(self, RETROPHIES_INSN_SETGLOBAL, var->hash);
  }
  else
  {
    return retrophies_parser_emit(self, RETROPHIES_INSN_SETLOCAL, var->hash);
  }
}

static retrophies_parser_var_t* retrophies_parser_findvar(retrophies_parser_t* self, uint32_t hash, int* scope_index)
{
  retrophies_parser_scope_t* scope = self->sub->locals;
  int scope_count = 0;

  while (scope != NULL)
  {
    retrophies_parser_var_t* var = scope->vars;
    
    while (var != NULL)
    {
      if (var->name.hash == hash)
      {
        if (scope_index != NULL)
        {
          *scope_index = scope_count;
        }

        return var;
      }
  
      var = var->previous;
    }

    scope = scope->previous;
    scope_count++;
  }

  return NULL;
}

static retrophies_parser_subroutine_t* retrophies_parser_findsub(retrophies_parser_t* self, uint32_t hash)
{
  retrophies_parser_subroutine_t* sub = self->subroutines;

  while (sub != NULL)
  {
    if (sub->name.hash == hash)
    {
      return sub;
    }

    sub = sub->previous;
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
  case RETROPHIES_TOKEN_INTEGER:
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

  self->globals.vars = NULL;
  self->globals.previous = NULL;
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
