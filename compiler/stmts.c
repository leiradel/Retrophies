static int retrophies_parser_parsestatements(retrophies_parser_t* self, retrophies_parser_subroutine_t* sub)
{
  for (;;)
  {
    switch (self->la.token)
    {
    case RETROPHIES_TOKEN_IF:         retrophies_parser_parseif(self, sub); break;
    case RETROPHIES_TOKEN_FOR:        retrophies_parser_parsefor(self, sub); break;
    case RETROPHIES_TOKEN_WHILE:      retrophies_parser_parsewhile(self, sub); break;
    //case RETROPHIES_TOKEN_REPEAT:     retrophies_parser_parserepeat(self, sub); break;
    //case RETROPHIES_TOKEN_DIM:        /* fall through */
    //case RETROPHIES_TOKEN_STATIC:     retrophies_parser_parselocal(self, sub); break;
    //case RETROPHIES_TOKEN_IDENTIFIER: retrophies_parser_parseassign(self, sub); break;
    //case RETROPHIES_TOKEN_RETURN:     retrophies_parser_parsereturn(self, sub); return 1;
    
    case RETROPHIES_TOKEN_AWARD:
      if (sub->type != RETROPHIES_TOKEN_TROPHY)
      {
        return 0;
      }

      //retrophies_parser_parseaward(self, sub);
      break;
    
    default: return 0;
    }
  }
}
