#ifndef RETROPHIES_PARSER_H
#define RETROPHIES_PARSER_H

#include "lexer.h"
#include "emitter.h"

#include <setjmp.h>

typedef struct
{
  retrophies_lexer_t           lexer;
  retrophies_lexer_lookahead_t la;
  retrophies_emitter_t         emitter;
  jmp_buf                      rollback;
}
retrophies_parser_t;

void retrophies_parser_init(retrophies_parser_t* self, const char* source_name, const void* source, size_t length, char* error, size_t error_size);
int  retrophies_parser_parse(retrophies_parser_t* self, retrophies_emitter_t* emitter);

#endif /* RETROPHIES_PARSER_H */
