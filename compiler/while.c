static void retrophies_parser_while(retrophies_parser_t* self, retrophies_parser_subroutine_t* sub)
{
  retrophies_parser_matchany(&self);

  int pc_loop = self->code_size;
  retrophies_parser_parseexpression(self, RETROPHIES_TOKEN_BOOLEAN);
  
  void* label_false = (void*)(self->code->code + self->code->count);
  retrophies_parser_emit(self, RETROPHIES_INSN_JNE, 0);

  retrophies_parser_parsestatements(self, sub);
  retrophies_parser_emit(self, RETROPHIES_INSN_JUMP, pc_loop);

  retrophies_emitter_patch(label_false, self->code_size);
}
