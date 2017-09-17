static void retrophies_parser_parsefor(retrophies_parser_t* self)
{
  retrophies_parser_matchany(self);
  retrophies_lexer_lookahead_t name = self->la;
  const retrophies_parser_local_t* var = retrophies_parser_findlocal(self->sub, name.hash);

  if (var == NULL)
  {
    retrophies_parser_error(self, name.line, "Unknown identifier: %.*s", name.lexeme.chars, name.lexeme.length);
    /* never returns */
  }

  switch (var->type)
  {
  case RETROPHIES_TOKEN_INT32:  /* fall through */
  case RETROPHIES_TOKEN_INT64:  /* fall through */
  case RETROPHIES_TOKEN_UINT32: /* fall through */
  case RETROPHIES_TOKEN_UINT64: /* fall through */
  case RETROPHIES_TOKEN_SINGLE: /* fall through */
  case RETROPHIES_TOKEN_DOUBLE: break;
  
  default:
    retrophies_parser_error(self, name.line, "Type mismatch: %.*s must be an integer of a float", name.lexeme.chars, name.lexeme.length);
    /* never returns */
  }

  retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);

  retrophies_parser_match(self, '=');
  retrophies_parser_parseexpression(self, var->type);
  retrophies_parser_emit(self, RETROPHIES_INSN_SETLOCAL, var->index);

  retrophies_parser_match(self, RETROPHIES_TOKEN_TO);
  retrophies_parser_parseexpression(self, var->type);

  if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_STEP))
  {
    retrophies_parser_parseexpression(self, var->type);
  }
  else
  {
    switch (var->type)
    {
    case RETROPHIES_TOKEN_INT32:  /* fall through */
    case RETROPHIES_TOKEN_INT64:  /* fall through */
    case RETROPHIES_TOKEN_UINT32: /* fall through */
    case RETROPHIES_TOKEN_UINT64: retrophies_parser_emit(self, RETROPHIES_INSN_PUSH_1); break;
    case RETROPHIES_TOKEN_SINGLE: retrophies_parser_emit(self, RETROPHIES_INSN_PUSH_F32, 1.0f); break;
    case RETROPHIES_TOKEN_DOUBLE: retrophies_parser_emit(self, RETROPHIES_INSN_PUSH_F64, 1.0); break;
    }
  }

  void* label_check = (void*)(self->code->code + self->code->count);

  switch (var->type)
  {
  case RETROPHIES_TOKEN_INT32:  retrophies_parser_emit(self, RETROPHIES_INSN_CHECKFOR_I32, 0); break;
  case RETROPHIES_TOKEN_INT64:  retrophies_parser_emit(self, RETROPHIES_INSN_CHECKFOR_I64, 0); break;
  case RETROPHIES_TOKEN_UINT32: retrophies_parser_emit(self, RETROPHIES_INSN_CHECKFOR_U32, 0); break;
  case RETROPHIES_TOKEN_UINT64: retrophies_parser_emit(self, RETROPHIES_INSN_CHECKFOR_U64, 0); break;
  case RETROPHIES_TOKEN_SINGLE: retrophies_parser_emit(self, RETROPHIES_INSN_CHECKFOR_F32, 0); break;
  case RETROPHIES_TOKEN_DOUBLE: retrophies_parser_emit(self, RETROPHIES_INSN_CHECKFOR_F64, 0); break;
  }

  int pc_loop = self->code_size;

  retrophies_parser_parsestatements(self);
  retrophies_parser_match(self, RETROPHIES_TOKEN_NEXT);

  name = self->la;

  if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_IDENTIFIER))
  {
    retrophies_lexer_string_t var_name = var->name.lexeme;

    if (var_name.length != name.lexeme.length || memcmp(var_name.chars, name.lexeme.chars, var_name.length) != 0)
    {
      retrophies_parser_error(self, name.line, "Wrong control variable: %.*s, should be %.*s", name.lexeme.chars, name.lexeme.length, var_name.chars, var_name.length);
      /* never returns */
    }
  }

  switch (var->type)
  {
  case RETROPHIES_TOKEN_INT32:  retrophies_parser_emit(self, RETROPHIES_INSN_NEXT_I32, pc_loop); break;
  case RETROPHIES_TOKEN_INT64:  retrophies_parser_emit(self, RETROPHIES_INSN_NEXT_I64, pc_loop); break;
  case RETROPHIES_TOKEN_UINT32: retrophies_parser_emit(self, RETROPHIES_INSN_NEXT_U32, pc_loop); break;
  case RETROPHIES_TOKEN_UINT64: retrophies_parser_emit(self, RETROPHIES_INSN_NEXT_U64, pc_loop); break;
  case RETROPHIES_TOKEN_SINGLE: retrophies_parser_emit(self, RETROPHIES_INSN_NEXT_F32, pc_loop); break;
  case RETROPHIES_TOKEN_DOUBLE: retrophies_parser_emit(self, RETROPHIES_INSN_NEXT_F64, pc_loop); break;
  }

  retrophies_emitter_patch(label_check, self->code_size);
}
