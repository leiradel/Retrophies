#include "parser.h"

static void retrophies_parser_parseprogram(retrophies_parser_t* self)
{
  for (;;)
  {
    switch (self->la.token)
    {
    case RETROPHIES_TOKEN_SUBROUTINE: retrophies_parser_parsesubroutine(self, RETROPHIES_TOKEN_SUBROUTINE); break;
    case RETROPHIES_TOKEN_FUNCTION:   retrophies_parser_parsesubroutine(self, RETROPHIES_TOKEN_FUNCTION); break;
    case RETROPHIES_TOKEN_TROPHY:     retrophies_parser_parsesubroutine(self, RETROPHIES_TOKEN_TROPHY); break;
    case RETROPHIES_TOKEN_DIM:        retrophies_parser_parsedim(self, 1); break;

    default:
      retrophies_parser_error(self, self->la.line, "Expected sub, function, trophy, or dim");
      /* never returns */
    }
  }
}
