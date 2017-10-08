#include "parser.h"

static int retrophies_parser_parsestatements(retrophies_parser_t* self)
{
  int returned = 0;

  retrophies_parser_pushscope(self);

  for (;;)
  {
    switch (self->la.token)
    {
    case RETROPHIES_TOKEN_IF:         retrophies_parser_parseif(self); break;
    case RETROPHIES_TOKEN_FOR:        retrophies_parser_parsefor(self); break;
    case RETROPHIES_TOKEN_WHILE:      retrophies_parser_parsewhile(self); break;
    case RETROPHIES_TOKEN_REPEAT:     retrophies_parser_parserepeat(self); break;
    case RETROPHIES_TOKEN_DIM:        /* fall through */
    case RETROPHIES_TOKEN_STATIC:     retrophies_parser_parsedim(self, 0); break;
    //case RETROPHIES_TOKEN_IDENTIFIER: retrophies_parser_parseassign(self); break;
    //case RETROPHIES_TOKEN_AWARD:      retrophies_parser_parseaward(self); break;

    case RETROPHIES_TOKEN_RETURN:     retrophies_parser_parsereturn(self); returned = 1; goto out;
    
    default: goto out;
    }
  }

out:
  retrophies_parser_popscope(self);
  return returned;
}
