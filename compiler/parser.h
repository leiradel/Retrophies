#ifndef RETROPHIES_PARSER_H
#define RETROPHIES_PARSER_H

#include "lexer.h"
#include "emitter.h"

#include <setjmp.h>

#define RETROPHIES_PARSER_OFFSETOF(s, f) ((size_t)(&((s*)0)->f))
#define RETROPHIES_PARSER_ALIGNOF(t)     RETROPHIES_PARSER_OFFSETOF(struct { char c; t d; }, d)

typedef struct retrophies_parser_local_t retrophies_parser_local_t;

struct retrophies_parser_local_t
{
  retrophies_lexer_lookahead_t name;
  int                          index;
  int                          type;
  retrophies_parser_local_t*   previous;
};

typedef struct retrophies_parser_event_t retrophies_parser_event_t;

struct retrophies_parser_event_t
{
  retrophies_lexer_lookahead_t names;
  retrophies_parser_event_t*  previous;
};

typedef struct retrophies_parser_codeblock_t retrophies_parser_codeblock_t;

struct retrophies_parser_codeblock_t
{
  uint8_t                        code[64];
  int                            count;
  retrophies_parser_codeblock_t* next;
};

typedef struct retrophies_parser_subroutine_t retrophies_parser_subroutine_t;

struct retrophies_parser_subroutine_t
{
  int                             sub_type;
  retrophies_lexer_lookahead_t    name;
  retrophies_parser_local_t*      locals;
  retrophies_parser_event_t*      events;
  int                             num_args;
  int                             num_locals;
  int                             ret_type;
  retrophies_parser_codeblock_t   code;
  int                             code_size;
  retrophies_parser_subroutine_t* previous;
};

typedef struct retrophies_parser_t retrophies_parser_t;

struct retrophies_parser_t
{
  size_t                          temp_ptr;
  size_t                          temp_size;
  uint8_t*                        temp_buffer;

  retrophies_lexer_t              lexer;
  retrophies_lexer_lookahead_t    la;
  retrophies_parser_subroutine_t* subroutines;
  retrophies_parser_codeblock_t*  code;
  int                             code_size;
  retrophies_emitter_t            emitter;
  jmp_buf                         rollback;
};

void retrophies_parser_init(retrophies_parser_t* self, const char* source_name, const void* source, size_t length, void* temp_buffer, size_t temp_size, char* error, size_t error_size);
int  retrophies_parser_parse(retrophies_parser_t* self, retrophies_emitter_t* emitter);

#endif /* RETROPHIES_PARSER_H */
