#include "parser.h"

static void retrophies_parser_parseglobal(retrophies_parser_t* self)
{
  for (;;)
  {
    retrophies_parser_var_t* previous = self->globals.vars;
    retrophies_parser_var_t* var;

    for (;;)
    {
      int scope_index;
      var = retrophies_parser_findvar(self, self->la.hash, &scope_index);

      if (var != NULL && scope_index == 0)
      {
        retrophies_parser_error(self, self->la.line, "Duplicated identifier: %.*s", self->la.lexeme.chars, self->la.lexeme.length);
        /* never returns */
      }

      var = retrophies_parser_allocvar(self);
      var->previous = self->globals.vars;
      self->globals.vars = var;
      var->name = self->la;
      var->hash = self->la.hash;
      var->flags = RETROPHIES_VAR_GLOBAL;
      retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);

      if (self->la.token != ',')
      {
        break;
      }

      retrophies_parser_matchany(self);
    }

    retrophies_parser_match(self, RETROPHIES_TOKEN_AS);
    int type = retrophies_parser_parsetype(self);

    var = self->globals.vars;

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
    retrophies_parser_var_t* previous = self->sub->locals->vars;
    retrophies_parser_var_t* var;
    int count = 0;

    for (;;)
    {
      int scope_index;
      var = retrophies_parser_findvar(self, self->la.hash, &scope_index);

      if (var != NULL && scope_index == 0)
      {
        retrophies_parser_error(self, self->la.line, "Duplicated identifier: %.*s", self->la.lexeme.chars, self->la.lexeme.length);
        /* never returns */
      }

      var = retrophies_parser_allocvar(self);
      var->previous = self->sub->locals->vars;
      self->sub->locals->vars = var;
      var->name = self->la;
      var->index = self->sub->num_locals++;
      var->flags = 0;

      if (self->sub->num_locals > self->sub->max_locals)
      {
        self->sub->max_locals = self->sub->num_locals;
      }

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
      retrophies_parser_parseexpression(self, type);
      retrophies_parser_emit(self, RETROPHIES_INSN_SETLOCAL, var->index);
    }

    var = self->sub->locals->vars;

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

static void retrophies_parser_parsestatic(retrophies_parser_t* self)
{
  for (;;)
  {
    retrophies_parser_var_t* previous = self->sub->locals->vars;
    retrophies_parser_var_t* var;
    int count = 0;

    for (;;)
    {
      int scope_index;
      var = retrophies_parser_findvar(self, self->la.hash, &scope_index);

      if (var != NULL && scope_index == 0)
      {
        retrophies_parser_error(self, self->la.line, "Duplicated identifier: %.*s", self->la.lexeme.chars, self->la.lexeme.length);
        /* never returns */
      }

      var = retrophies_parser_allocvar(self);
      var->previous = self->sub->locals->vars;
      self->sub->locals->vars = var;
      var->name = self->la;
      var->hash = self->la.hash ^ self->sub->name.hash;
      var->flags = RETROPHIES_VAR_STATIC;

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
      retrophies_parser_parseexpression(self, type);
      retrophies_parser_emit(self, RETROPHIES_INSN_SETLOCAL, var->index);
    }

    var = self->sub->locals->vars;

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
  if (is_global)
  {
    retrophies_parser_match(self, RETROPHIES_TOKEN_DIM);
    retrophies_parser_parseglobal(self);
  }
  else
  {
    int is_static = retrophies_parser_matchopt(self, RETROPHIES_TOKEN_STATIC);
    retrophies_parser_match(self, RETROPHIES_TOKEN_DIM);
    
    if (is_static)
    {
      retrophies_parser_parsestatic(self);
    }
    else
    {
      retrophies_parser_parselocal(self);
    }
  }
}
