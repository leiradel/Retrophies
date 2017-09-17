typedef struct retrophies_parser_jumpchain_t retrophies_parser_jumpchain_t;

struct retrophies_parser_jumpchain_t
{
  void*                          label;
  retrophies_parser_jumpchain_t* previous;
};

static void retrophies_parser_parseif(retrophies_parser_t* self)
{
  retrophies_parser_matchany(self);
  retrophies_parser_parseexpression(self, RETROPHIES_TOKEN_BOOLEAN);
  retrophies_parser_matchopt(self, RETROPHIES_TOKEN_THEN);

  void* label_false = (void*)(self->code->code + self->code->count);
  retrophies_parser_emit(self, RETROPHIES_INSN_JNE, 0);

  retrophies_parser_parsestatements(self);

  retrophies_parser_jumpchain_t* chain = RETROPHIES_PARSER_ALLOC(retrophies_parser_jumpchain_t);
  chain->label = (void*)(self->code->code + self->code->count);
  chain->previous = NULL;
  retrophies_parser_emit(self, RETROPHIES_INSN_JUMP, 0);
  
  retrophies_emitter_patch(label_false, self->code_size);

  while (self->la.token == RETROPHIES_TOKEN_ELSEIF)
  {
    retrophies_parser_matchany(self);
    retrophies_parser_parseexpression(self, RETROPHIES_TOKEN_BOOLEAN);
    retrophies_parser_matchopt(self, RETROPHIES_TOKEN_THEN);

    label_false = (void*)(self->code->code + self->code->count);
    retrophies_parser_emit(self, RETROPHIES_INSN_JNE, 0);

    retrophies_parser_parsestatements(self);

    retrophies_parser_jumpchain_t* next = RETROPHIES_PARSER_ALLOC(retrophies_parser_jumpchain_t);
    next->label = (void*)(self->code->code + self->code->count);
    next->previous = chain;
    chain = next;
    retrophies_parser_emit(self, RETROPHIES_INSN_JUMP, 0);
  
    retrophies_emitter_patch(label_false, self->code_size);
  }

  if (retrophies_parser_matchopt(self, RETROPHIES_TOKEN_ELSE))
  {
    retrophies_parser_parsestatements(self);
  }

  retrophies_parser_match(self, RETROPHIES_TOKEN_END);
  retrophies_parser_match(self, RETROPHIES_TOKEN_IF);

  do
  {
    retrophies_emitter_patch(chain->label, self->code_size);
    chain = chain->previous;
  }
  while (chain != NULL);
}
