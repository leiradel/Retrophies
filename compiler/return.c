#include "parser.h"

static void retrophies_parser_parsereturn(retrophies_parser_t* self)
{
  retrophies_parser_matchany(self);

  if (self->sub->sub_type == RETROPHIES_TOKEN_FUNCTION)
  {
    retrophies_parser_parseexpression(self, self->sub->ret_type);

    switch (self->sub->ret_type)
    {
    case RETROPHIES_TOKEN_BOOLEAN:
    case RETROPHIES_TOKEN_INT8:
    case RETROPHIES_TOKEN_INT16:
    case RETROPHIES_TOKEN_INT32:
    case RETROPHIES_TOKEN_SINGLE:
    case RETROPHIES_TOKEN_UINT8:
    case RETROPHIES_TOKEN_UINT16:
    case RETROPHIES_TOKEN_UINT32:
      retrophies_parser_emit(self, RETROPHIES_INSN_RETURN_X32);
      break;

    case RETROPHIES_TOKEN_DOUBLE:
    case RETROPHIES_TOKEN_INT64:
    case RETROPHIES_TOKEN_UINT64:
      retrophies_parser_emit(self, RETROPHIES_INSN_RETURN_X64);
      break;
    }
  }
  else
  {
    retrophies_parser_emit(self, RETROPHIES_INSN_RETURN);
  }
}
