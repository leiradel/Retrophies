#include "parser.h"
#include "lexer.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define RETROPHIES_PARSER_MAXLOCALS  32
#define RETROPHIES_PARSER_MAXEVENTS  8
#define RETROPHIES_PARSER_MAXIFCHAIN 64

typedef struct
{
  retrophies_lexer_lookahead_t names[RETROPHIES_PARSER_MAXLOCALS];
  int                          types[RETROPHIES_PARSER_MAXLOCALS];
  unsigned                     count;
}
retrophies_parser_locals_t;

typedef struct
{
  retrophies_lexer_lookahead_t names[RETROPHIES_PARSER_MAXEVENTS];
  unsigned                     count;
}
retrophies_parser_events_t;

typedef struct
{
  int                          type;
  retrophies_lexer_lookahead_t name;
  retrophies_parser_locals_t   locals;
  retrophies_parser_events_t   events;
  int                          ret;
}
retrophies_parser_subroutine_t;

static int retrophies_parser_findlocal(const retrophies_parser_subroutine_t* sub, uint32_t hash)
{
  unsigned i, count = sub->locals.count;

  for (i = 0; i < count; i++)
  {
    if (sub->locals.names[i].hash == hash)
    {
      return i;
    }
  }

  return -1;
}

static void retrophies_parser_error(retrophies_parser_t* self, unsigned line, const char* format, ...)
{
  int written = snprintf(self->lexer.error, self->lexer.error_size, "%s:%u: ", self->lexer.source_name, line);

  va_list args;

  va_start(args, format);
  vsnprintf(self->lexer.error + written, self->lexer.error_size - written, format, args);
  va_end(args);

  longjmp(self->rollback, 1);
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
  case RETROPHIES_TOKEN_STRING:
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

static void retrophies_parser_parsestatements(retrophies_parser_t* self, retrophies_parser_subroutine_t* sub);

static void retrophies_parser_parseif(retrophies_parser_t* self, retrophies_parser_subroutine_t* sub)
{
  retrophies_emitter_patch_t finish[RETROPHIES_PARSER_MAXIFCHAIN];
  unsigned chain = 0;

  retrophies_parser_matchany(self);
  retrophies_parser_parseexpression(self, RETROPHIES_TOKEN_BOOLEAN);
  retrophies_parser_matchopt(self, RETROPHIES_TOKEN_THEN);

  retrophies_emitter_patch_t when_false = retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_JNE, 0);
  retrophies_parser_parsestatements(self, sub);
  finish[chain++] = retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_JUMP, 0);
  
  retrophies_emitter_patch(&self->emitter, when_false);

  while (self->la.token == RETROPHIES_TOKEN_ELSEIF)
  {
    if (chain == RETROPHIES_PARSER_MAXIFCHAIN)
    {
      retrophies_parser_error(self, self->la.line, "If chain too long, the maximum is %d", RETROPHIES_PARSER_MAXIFCHAIN);
      /* never returns */
    }

    retrophies_parser_matchany(self);
    retrophies_parser_parseexpression(self, RETROPHIES_TOKEN_BOOLEAN);
    retrophies_parser_matchopt(self, RETROPHIES_TOKEN_THEN);

    when_false = retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_JNE, 0);
    retrophies_parser_parsestatements(self, sub);
    finish[chain++] = retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_JUMP, 0);

    retrophies_emitter_patch(&self->emitter, when_false);
  }

  if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_ELSE))
  {
    retrophies_parser_parsestatements(self, sub);
  }

  retrophies_parser_match(self, RETROPHIES_TOKEN_END);
  retrophies_parser_match(self, RETROPHIES_TOKEN_IF);

  unsigned i;

  for (i = 0; i < chain; i++)
  {
    retrophies_emitter_patch(&self->emitter, finish[i]);
  }
}

static void retrophies_parser_parsefor(retrophies_parser_t* self, retrophies_parser_subroutine_t* sub)
{
  retrophies_parser_matchany(self);
  retrophies_lexer_lookahead_t name = self->la;
  int index = retrophies_parser_findlocal(sub, name.hash);
  retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);

  if (index == -1)
  {
    retrophies_parser_error(self, name.line, "Unknown identifier: %.*s", name.lexeme.chars, name.lexeme.length);
    /* never returns */
  }

  retrophies_parser_match(self, '=');
  retrophies_parser_parseexpression(self, sub->locals.types[index]);
  retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_SETLOCAL, index);

  retrophies_parser_match(self, RETROPHIES_TOKEN_TO);
  retrophies_parser_parseexpression(self, sub->locals.types[index]);

  if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_STEP))
  {
    retrophies_parser_parseexpression(self, sub->locals.types[index]);
  }
  else
  {
    switch (sub->locals.types[index])
    {
    case RETROPHIES_TOKEN_INT32:  /* fall through */
    case RETROPHIES_TOKEN_INT64:  /* fall through */
    case RETROPHIES_TOKEN_UINT32: /* fall through */
    case RETROPHIES_TOKEN_UINT64: retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_PUSH_I8, 1); break;
    case RETROPHIES_TOKEN_SINGLE: retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_PUSH_F32, 1.0f); break;
    case RETROPHIES_TOKEN_DOUBLE: retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_PUSH_F64, 1.0); break;
    }
  }

  uint32_t loop = retrophies_emitter_getpc(&self->emitter);

  retrophies_parser_parsestatements(self, sub);
  retrophies_parser_match(self, RETROPHIES_TOKEN_NEXT);

  name = self->la;

  if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_IDENTIFIER))
  {
    retrophies_lexer_string_t var = sub->locals.names[index].lexeme;
    
    if (var.length != name.lexeme.length || memcmp(var.chars, name.lexeme.chars, var.length) != 0)
    {
      retrophies_parser_error(self, name.line, "Wrong control variable: %.*s", name.lexeme.chars, name.lexeme.length);
      /* never returns */
    }
  }

  retrophies_emitter_emit(&self->emitter, RETROPHIES_INSN_NEXT, loop);
}

static void retrophies_parser_parsestatements(retrophies_parser_t* self, retrophies_parser_subroutine_t* sub)
{
  for (;;)
  {
    switch (self->la.token)
    {
    case RETROPHIES_TOKEN_IF:         retrophies_parser_parseif(self, sub); break;
    case RETROPHIES_TOKEN_FOR:        retrophies_parser_parsefor(self, sub); break;
    //case RETROPHIES_TOKEN_WHILE:      retrophies_parser_parsewhile(self, sub); break;
    //case RETROPHIES_TOKEN_REPEAT:     retrophies_parser_parserepeat(self, sub); break;
    //case RETROPHIES_TOKEN_DIM:        /* fall through */
    //case RETROPHIES_TOKEN_STATIC:     retrophies_parser_parselocal(self, sub); break;
    //case RETROPHIES_TOKEN_IDENTIFIER: retrophies_parser_parseassign(self, sub); break;

    case RETROPHIES_TOKEN_RETURN:
      if (sub->type == RETROPHIES_TOKEN_FUNCTION)
      {
        //retrophies_parser_parsereturn(self, sub);
      }
    }
  }
}

static void retrophies_parser_parsesubroutine(retrophies_parser_t* self, int type)
{
  retrophies_parser_subroutine_t sub;
  sub.type = type;

  retrophies_parser_matchany(self);
  sub.name = self->la;
  retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);

  sub.locals.count = 0;
  sub.events.count = 0;

  if (retrophies_parser_matchopt(self, '('))
  {
    retrophies_parser_matchany(self);

    for (;;)
    {
      sub.locals.names[sub.locals.count] = self->la;
      retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);
      retrophies_parser_match(self, RETROPHIES_TOKEN_AS);
      sub.locals.types[sub.locals.count] = retrophies_parser_parsetype(self);
      sub.locals.count++;

      if (self->la.token != ',')
      {
        break;
      }

      retrophies_parser_matchany(self);

      if (sub.locals.count == RETROPHIES_PARSER_MAXLOCALS)
      {
        retrophies_parser_error(self, self->la.line, "Too many parameters + locals, the maximum is %d", RETROPHIES_PARSER_MAXLOCALS);
        /* never returns */
      }
    }

    retrophies_parser_match(self, ')');
  }

  if (type == RETROPHIES_TOKEN_SUBROUTINE)
  {
    if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_HANDLES))
    {
      for (;;)
      {
        sub.events.names[sub.events.count] = self->la;
        retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);
        sub.events.count++;

        if (self->la.token != ',')
        {
          break;
        }
  
        retrophies_parser_matchany(self);
  
        if (sub.events.count == RETROPHIES_PARSER_MAXEVENTS)
        {
          retrophies_parser_error(self, self->la.line, "Too many events, the maximum is %d", RETROPHIES_PARSER_MAXEVENTS);
          /* never returns */
        }
      }
    }
  }
  else if (type == RETROPHIES_TOKEN_FUNCTION)
  {
    retrophies_parser_match(self, RETROPHIES_TOKEN_AS);
    sub.ret = retrophies_parser_parsetype(self);
  }

  retrophies_parser_parsestatements(self, &sub);

  if (self->la.token != RETROPHIES_TOKEN_END)
  {
    retrophies_parser_error(self, self->la.line, "Statement expected");
    /* never returns */
  }

  retrophies_parser_matchany(self);
  retrophies_parser_match(self, sub.type);
}

static void retrophies_parser_parseprogram(retrophies_parser_t* self)
{
  for (;;)
  {
    switch (self->la.token)
    {
    case RETROPHIES_TOKEN_SUBROUTINE: retrophies_parser_parsesubroutine(self, RETROPHIES_TOKEN_SUBROUTINE); break;
    case RETROPHIES_TOKEN_FUNCTION:   retrophies_parser_parsesubroutine(self, RETROPHIES_TOKEN_FUNCTION); break;
    case RETROPHIES_TOKEN_TROPHY:     retrophies_parser_parsesubroutine(self, RETROPHIES_TOKEN_TROPHY); break;
    //case RETROPHIES_TOKEN_DIM:        retrophies_parser_parseglobal(self); break;

    default:
      retrophies_parser_error(self, self->la.line, "Expected sub, function, trophy, or dim");
      /* never returns */
    }
  }
}

void retrophies_parser_init(retrophies_parser_t* self, const char* source_name, const void* source, size_t length, char* error, size_t error_size)
{
  retrophies_lexer_init(&self->lexer, source_name, source, length, error, error_size);
}

int retrophies_parser_parse(retrophies_parser_t* self, retrophies_emitter_t* emitter)
{
  if (setjmp(self->rollback) != 0)
  {
    return -1;
  }

  retrophies_parser_matchany(self);
  retrophies_parser_parseprogram(self);
  return 0;
}
