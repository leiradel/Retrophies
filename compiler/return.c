#include "parser.h"

static void retrophies_parser_parsereturn(retrophies_parser_t* self)
{
  retrophies_parser_matchany(self);

  if (self->sub->sub_type == RETROPHIES_TOKEN_FUNCTION)
  {
    retrophies_parser_parseexpression(self, self->sub->ret_type);
    retrophies_parser_emit(self, RETROPHIES_INSN_RETURN_INTEGER);
  }
  else
  {
    retrophies_parser_emit(self, RETROPHIES_INSN_RETURN);
  }
}
