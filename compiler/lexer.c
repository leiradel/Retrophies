#include "lexer.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define RETROPHIES_LEXER_ISSPACE(k)  (isspace(k))
#define RETROPHIES_LEXER_ISALPHA(k)  (k == '_' || isalpha(k))
#define RETROPHIES_LEXER_ISALNUM(k)  (k == '_' || isalnum(k))
#define RETROPHIES_LEXER_ISDIGIT(k)  (isdigit(k))
#define RETROPHIES_LEXER_ISXDIGIT(k) (isxdigit(k))
#define RETROPHIES_LEXER_ISBDIGIT(k) ((k & 0xfe) == '0' /* ascii only */)

#define RETROPHIES_LEXER_GET(s) ((s)->source < (s)->end ? *(s)->source : -1)

#include "tokens.c"

void retrophies_lexer_init(retrophies_lexer_t* self, const char* source_name, const void* source, size_t length, char* error, size_t error_size)
{
  self->source_name = source_name;
  self->line        = 0;
  self->source      = (uint8_t*)source;
  self->end         = self->source + length;
  self->last_char   = RETROPHIES_LEXER_GET(self);
  self->error       = error;
  self->error_size  = error_size;
}

static inline int retrophies_lexer_skip(retrophies_lexer_t* self)
{
  self->line += *self->source == '\n';

  ptrdiff_t left = self->end - self->source;

  if (left >= 2)
  {
    return *++self->source;
  }
  else if (left == 1)
  {
    self->source++;
  }

  return -1;
}

static void retrophies_lexer_formatchar(char* buffer, size_t size, int k)
{
  if (isprint(k))
  {
    snprintf(buffer, size, "'%c'", k);
  }
  else if (k != -1)
  {
    snprintf(buffer, size, "'\\%c%c%c", (k >> 6) + '0', ((k >> 3) & 7) + '0', (k & 7) + '0');
  }
  else
  {
    strncpy(buffer, "<eof>", size);
  }
}

static int retrophies_lexer_error(retrophies_lexer_t* self, const char* format, ...)
{
  int written = snprintf(self->error, self->error_size, "%s:%d: ", self->source_name, self->line);

  va_list args;

  va_start(args, format);
  vsnprintf(self->error + written, self->error_size - written, format, args);
  va_end(args);

  return -1;
}

int retrophies_lexer_next(retrophies_lexer_t* self, retrophies_lexer_lookahead_t* la)
{
  int k = self->last_char;

  for (;;)
  {
    if (k == -1)
    {
      la->token = RETROPHIES_TOKEN_EOF;
      la->lexeme.chars = "<eof>";
      la->lexeme.length = 5;
      la->line = self->line;
    }
    else if (!RETROPHIES_LEXER_ISSPACE(k))
    {
      break;
    }

    k = retrophies_lexer_skip(self);
  }

  const uint8_t* start = self->source;
  la->lexeme.chars     = (const char*)start;
  la->line             = self->line;

  if (RETROPHIES_LEXER_ISALPHA(k))
  {
    uint32_t hash = 5381;

    do
    {
      hash = hash * 33 + tolower(k);
      k = retrophies_lexer_skip(self);
    }
    while (RETROPHIES_LEXER_ISALPHA(k));

    size_t length = self->source - start;
    const struct retrophies_lexer_keyword_t* keyword = retrophies_lexer_lookup(la->lexeme.chars, length);

    la->token         = (keyword != NULL) ? keyword->token : RETROPHIES_TOKEN_IDENTIFIER;
    la->lexeme.length = length;
    la->hash          = hash;
    self->last_char   = k;
    return 0;
  }

  if (RETROPHIES_LEXER_ISDIGIT(k))
  {
    int isint = 1;

    do
    {
      k = retrophies_lexer_skip(self);
    }
    while (RETROPHIES_LEXER_ISDIGIT(k));

    if (k == '.')
    {
      isint = 0;
      k = retrophies_lexer_skip(self);

      if (RETROPHIES_LEXER_ISDIGIT(k))
      {
        do
        {
          k = retrophies_lexer_skip(self);
        }
        while (RETROPHIES_LEXER_ISDIGIT(k));
      }
      else
      {
        char c[8];

        retrophies_lexer_formatchar(c, sizeof(c), k);
        return retrophies_lexer_error(self, "Invalid digit after decimal separator: %c", c);
      }
    }

    if ((k & 0xdf) == 'E') /* ascii only */
    {
      isint = 0;
      k = retrophies_lexer_skip(self);

      if (k == '+' || k == '-')
      {
        k = retrophies_lexer_skip(self);
      }

      if (RETROPHIES_LEXER_ISDIGIT(k))
      {
        do
        {
          k = retrophies_lexer_skip(self);
        }
        while (RETROPHIES_LEXER_ISDIGIT(k));
      }
      else
      {
        char c[8];

        retrophies_lexer_formatchar(c, sizeof(c), k);
        return retrophies_lexer_error(self, "Invalid digit after decimal separator: %c", c);
      }
    }

    const uint8_t* suffix_start = self->source - 1;
    uint32_t suffix = 0;
    int suffix_length;
    
    if (RETROPHIES_LEXER_ISALPHA(k))
    {
      do
      {
        suffix = suffix << 8 | tolower(k);
        k = retrophies_lexer_skip(self);
      }
      while (RETROPHIES_LEXER_ISALPHA(k));

      if (isint)
      {
        switch (suffix)
        {
        case 0:
          suffix_length = 0;
          break;
        
        case 's': /* short */
        case 'i': /* integer */
        case 'l': /* long */
          suffix_length = 1;
          break;

        case 'u' << 8 | 's': /* unsigned short */
        case 'u' << 8 | 'i': /* unsigned integer */
        case 'u' << 8 | 'l': /* unsigned long */
          suffix_length = 2;
          break;

        default:
          return retrophies_lexer_error(self, "Invalid integer suffix: %.*s", suffix_start, self->source - suffix_start);
        }
      }
      else
      {
        switch (suffix)
        {
        case 0:
          suffix_length = 0;
          break;

        case 'f': /* single */
        case 'r': /* double */
          suffix_length = 1;
          break;
        
        default:
          return retrophies_lexer_error(self, "Invalid float suffix: %.*s", suffix_start, self->source - suffix_start);
        }
      }
    }

    la->token         = isint ? RETROPHIES_TOKEN_INTEGER_LITERAL : RETROPHIES_TOKEN_FLOAT_LITERAL;
    la->lexeme.length = self->source - start - suffix_length;
    self->last_char   = k;
    return 0;
  }

  if (k == '"')
  {
    for (;;)
    {
      k = retrophies_lexer_skip(self);

      if (k == '"')
      {
        if (RETROPHIES_LEXER_GET(self) == '"')
        {
          k = retrophies_lexer_skip(self);
        }
        else
        {
          break;
        }
      }
      else if (k == '"' || k == -1)
      {
        return retrophies_lexer_error(self, "Unterminated string");
      }
    }

    la->token         = RETROPHIES_TOKEN_STRING_LITERAL;
    la->lexeme.length = self->source - start;
    self->last_char   = k;
    return 0;
  }

  int l;

  switch (k)
  {
  case '(':
  case ')':
  case ',':
  case '=':
  case '.':
  case '[':
  case ']':
  case '&':
  case '+':
  case '-':
  case '*':
  case '/':
  case '\\':
    la->token         = k;
    la->lexeme.length = 1;
    self->last_char   = retrophies_lexer_skip(self);
    return 0;
  
  case '<':
    l = k;
    k = retrophies_lexer_skip(self);

    if (k == '>')
    {
      la->token         = RETROPHIES_TOKEN_NOTEQUAL;
      la->lexeme.length = 2;
      self->last_char   = retrophies_lexer_skip(self);
    }
    else if (k == '=')
    {
      la->token         = RETROPHIES_TOKEN_LESSEQUAL;
      la->lexeme.length = 2;
      self->last_char   = retrophies_lexer_skip(self);
    }
    else if (k == '<')
    {
      la->token         = RETROPHIES_TOKEN_LEFTSHIFT;
      la->lexeme.length = 2;
      self->last_char   = retrophies_lexer_skip(self);
    }
    else
    {
      la->token         = l;
      la->lexeme.length = 1;
      self->last_char   = k;
    }

    return 0;

  case '>':
    l = k;
    k = retrophies_lexer_skip(self);

    if (k == '=')
    {
      la->token         = RETROPHIES_TOKEN_GREATEREQUAL;
      la->lexeme.length = 2;
      self->last_char   = retrophies_lexer_skip(self);
    }
    else if (k == '>')
    {
      la->token         = RETROPHIES_TOKEN_RIGHTSHIFT;
      la->lexeme.length = 2;
      self->last_char   = retrophies_lexer_skip(self);
    }
    else
    {
      la->token         = l;
      la->lexeme.length = 1;
      self->last_char   = k;
    }

    return 0;
  }

  char c[8];
  
  retrophies_lexer_formatchar(c, sizeof(c), k);
  return retrophies_lexer_error(self, "Invalid character in input: %c", c);
}

void retrophies_lexer_getlexeme(char* buffer, size_t size, int token)
{
  static const char* lexemes[] =
  {
    "<eof>", "false", "<id>", "<integer>", "<float>", "<string>", "true", "else", "elseif",
    "end", "for", "if", "next", "repeat", "step", "then", "to", "until", "while", "as",
    "dim", "function", "static", "sub", "randomize", "return", "abs", "ceil", "floor", "rand",
    "trunc", "boolean", "double", "int8", "int16", "int32", "int64", "single", "string",
    "uint8", "uint16", "uint32", "uint64", "and", ">=", "<<", "<=", "mod", "not", "<>", "or",
    ">>", "xor", "bigendian", "binarycodeddecimal", "littleendian"
  };

  if (token >= RETROPHIES_TOKEN_EOF && token < RETROPHIES_TOKEN_LAST)
  {
    strncpy(buffer, lexemes[token - RETROPHIES_TOKEN_EOF], size);
  }
  else
  {
    retrophies_lexer_formatchar(buffer, size, token);
  }
}
