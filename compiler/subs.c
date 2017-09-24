#include "parser.h"

static void retrophies_parser_parsesubroutine(retrophies_parser_t* self, int type)
{
  retrophies_parser_matchany(self);
  retrophies_lexer_lookahead_t id = self->la;
  retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);
  
  retrophies_parser_subroutine_t* sub = retrophies_parser_findsub(self, id.hash);

  if (sub != NULL)
  {
    retrophies_parser_error(self, id.line, "Duplicated identifier: %.*s", id.lexeme.chars, id.lexeme.length);
    /* never returns */
  }

  sub = RETROPHIES_PARSER_ALLOC(retrophies_parser_subroutine_t);
  sub->previous = self->subroutines;
  self->subroutines = sub;
  sub->locals = NULL;
  sub->num_args = 0;
  sub->num_locals = 0;
  sub->events = NULL;
  sub->code.count = 0;
  self->code = &sub->code;
  sub->sub_type = type;
  sub->name = self->la;
  self->code_size = 0;
  self->sub = sub;


  if (retrophies_parser_matchopt(self, '('))
  {
    retrophies_parser_matchany(self);

    for (;;)
    {
      retrophies_parser_local_t* local = RETROPHIES_PARSER_ALLOC(retrophies_parser_local_t);
      local->previous = sub->locals;
      sub->locals = local;
      local->name = self->la;
      local->index = sub->num_locals;
      retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);
      retrophies_parser_match(self, RETROPHIES_TOKEN_AS);
      local->type = retrophies_parser_parsetype(self);

      sub->num_args++;
      sub->num_locals++;

      if (self->la.token != ',')
      {
        break;
      }

      retrophies_parser_matchany(self);
    }

    retrophies_parser_match(self, ')');
  }

  if (type == RETROPHIES_TOKEN_SUBROUTINE)
  {
    if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_HANDLES))
    {
      for (;;)
      {
        retrophies_parser_event_t* event = RETROPHIES_PARSER_ALLOC(retrophies_parser_event_t);
        event->previous = sub->events;
        sub->events = event;
        event->name = self->la;
        retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);

        if (self->la.token != ',')
        {
          break;
        }
  
        retrophies_parser_matchany(self);
      }
    }
  }
  else if (type == RETROPHIES_TOKEN_FUNCTION)
  {
    retrophies_parser_match(self, RETROPHIES_TOKEN_AS);
    sub->ret_type = retrophies_parser_parsetype(self);
  }

  int was_return = retrophies_parser_parsestatements(self);

  if (self->la.token != RETROPHIES_TOKEN_END)
  {
    retrophies_parser_error(self, self->la.line, "Statement expected");
    /* never returns */
  }

  if (type == RETROPHIES_TOKEN_FUNCTION)
  {
    if (!was_return)
    {
      retrophies_parser_error(self, self->la.line, "No return statement in function");
      /* never returns */
    }
  }
  else
  {
    retrophies_parser_emit(self, RETROPHIES_INSN_RETURN);
  }

  retrophies_parser_matchany(self);
  retrophies_parser_match(self, type);
  sub->code_size = self->code_size;
}
