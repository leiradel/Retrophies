#include "parser.h"

static int retrophies_parser_parsestatements(retrophies_parser_t* self)
{
  retrophies_parser_local_t* previous = self->sub->locals;
  int returned = 0;

  for (;;)
  {
    switch (self->la.token)
    {
    case RETROPHIES_TOKEN_IF:         retrophies_parser_parseif(self); goto out;
    case RETROPHIES_TOKEN_FOR:        retrophies_parser_parsefor(self); goto out;
    case RETROPHIES_TOKEN_WHILE:      retrophies_parser_parsewhile(self); goto out;
    case RETROPHIES_TOKEN_REPEAT:     retrophies_parser_parserepeat(self); goto out;
    case RETROPHIES_TOKEN_DIM:        /* fall through */
    case RETROPHIES_TOKEN_STATIC:     retrophies_parser_parsedim(self, 0); goto out;
    //case RETROPHIES_TOKEN_IDENTIFIER: retrophies_parser_parseassign(self); goto out;
    case RETROPHIES_TOKEN_RETURN:     retrophies_parser_parsereturn(self); returned = 1; goto out;
    
    case RETROPHIES_TOKEN_AWARD:
      if (self->sub->sub_type != RETROPHIES_TOKEN_TROPHY)
      {
        goto out;
      }

      //retrophies_parser_parseaward(self);
      break;
    
    default:
      goto out;
    }
  }

out:
  retrophies_parser_freelocals(self, previous);
  return returned;
}
