#include "parser.h"

static void retrophies_parser_parseglobal(retrophies_parser_t* self, uint32_t hash)
{
  for (;;)
  {
    retrophies_parser_global_t* previous = self->globals;
    retrophies_parser_global_t* var;

    for (;;)
    {
      var = retrophies_parser_findglobal(self, self->la.hash);

      if (var != NULL)
      {
        retrophies_parser_error(self, self->la.line, "Duplicated identifier: %.*s", self->la.lexeme.chars, self->la.lexeme.length);
        /* never returns */
      }

      var = RETROPHIES_PARSER_ALLOC(retrophies_parser_global_t);
      var->previous = self->globals;
      self->globals = var;
      var->name = self->la;
      var->hash = self->la.hash ^ hash;
      retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);

      if (self->la.token != ',')
      {
        break;
      }

      retrophies_parser_matchany(self);
    }

    retrophies_parser_match(self, RETROPHIES_TOKEN_AS);
    int type = retrophies_parser_parsetype(self);

    var = self->globals;

    while (var != previous)
    {
      var->type = type;
      var = var->previous;
    }

    if (self->la.token != ',')
    {
      break;
    }

    retrophies_parser_matchany(self);
  }
}

static void retrophies_parser_parselocal(retrophies_parser_t* self)
{
  for (;;)
  {
    retrophies_parser_local_t* previous = self->sub->locals;
    retrophies_parser_local_t* var;
    int count = 0;

    for (;;)
    {
      var = retrophies_parser_findlocal(self->sub, self->la.hash);

      if (var != NULL)
      {
        retrophies_parser_error(self, self->la.line, "Duplicated identifier: %.*s", self->la.lexeme.chars, self->la.lexeme.length);
        /* never returns */
      }

      var = retrophies_parser_alloclocal(self);
      var->previous = self->sub->locals;
      self->sub->locals = var;
      var->name = self->la;
      var->index = self->sub->num_locals++;
      count++;
      retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);

      if (self->la.token != ',')
      {
        break;
      }

      retrophies_parser_matchany(self);
    }

    retrophies_parser_match(self, RETROPHIES_TOKEN_AS);
    int type = retrophies_parser_parsetype(self);

    if (count == 1 && retrophies_parser_matchopt(self, '='))
    {
      retrophies_parser_emit(self, RETROPHIES_INSN_SETLOCAL, var->index);
    }

    var = self->sub->locals;

    while (var != previous)
    {
      var->type = type;
      var = var->previous;
    }

    if (self->la.token != ',')
    {
      break;
    }

    retrophies_parser_matchany(self);
  }
}

static void retrophies_parser_parsedim(retrophies_parser_t* self, int is_global)
{
  int is_static = retrophies_parser_matchopt(self, RETROPHIES_TOKEN_STATIC);
  retrophies_parser_match(self, RETROPHIES_TOKEN_DIM);

  if (is_global)
  {
    retrophies_parser_parseglobal(self, 0);
  }
  else if (is_static)
  {
    retrophies_parser_parseglobal(self, self->sub->name.hash);
  }
  else
  {
    retrophies_parser_parselocal(self);
  }
}
