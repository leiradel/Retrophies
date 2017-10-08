#include "parser.h"

static void retrophies_parser_parsefor(retrophies_parser_t* self)
{
  retrophies_parser_matchany(self);
  retrophies_lexer_lookahead_t name = self->la;
  const retrophies_parser_var_t* var = retrophies_parser_findvar(self, name.hash, NULL);

  if (var == NULL)
  {
    retrophies_parser_error(self, name.line, "Unknown identifier: %.*s", name.lexeme.chars, name.lexeme.length);
    /* never returns */
  }

  if (var->type != RETROPHIES_TOKEN_INTEGER)
  {
    retrophies_parser_error(self, name.line, "Type mismatch: %.*s must be an integer", name.lexeme.chars, name.lexeme.length);
    /* never returns */
  }

  retrophies_parser_match(self, RETROPHIES_TOKEN_IDENTIFIER);

  retrophies_parser_match(self, '=');
  retrophies_parser_parseexpression(self, var->type);
  retrophies_parser_emitsetvar(self, var);

  retrophies_parser_match(self, RETROPHIES_TOKEN_TO);
  retrophies_parser_parseexpression(self, var->type);

  if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_STEP))
  {
    retrophies_parser_parseexpression(self, var->type);
  }
  else
  {
    retrophies_parser_emit(self, RETROPHIES_INSN_PUSH_1);
  }

  void* label_check = (void*)(self->code->code + self->code->count);
  retrophies_parser_emit(self, RETROPHIES_INSN_CHECKFOR, 0);
  int pc_loop = self->code_size;

  retrophies_parser_parsestatements(self);
  retrophies_parser_match(self, RETROPHIES_TOKEN_NEXT);

  name = self->la;

  if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_IDENTIFIER))
  {
    if (var->name.hash != name.hash)
    {
      retrophies_parser_error(self, name.line, "Wrong control variable: %.*s, should be %.*s", name.lexeme.chars, name.lexeme.length, var->name.lexeme.chars, var->name.lexeme.length);
      /* never returns */
    }
  }

  retrophies_parser_emit(self, RETROPHIES_INSN_NEXT, pc_loop);
  retrophies_emitter_patch(label_check, self->code_size);
}
