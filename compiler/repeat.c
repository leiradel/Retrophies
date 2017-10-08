#include "parser.h"

static void retrophies_parser_parserepeat(retrophies_parser_t* self)
{
  retrophies_parser_matchany(self);

  int pc_loop = self->code_size;

  retrophies_parser_parsestatements(self);
  retrophies_parser_parseexpression(self, RETROPHIES_TOKEN_BOOLEAN);
  
  retrophies_parser_emit(self, RETROPHIES_INSN_JFALSE, pc_loop);
}
