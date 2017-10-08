/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -c -C -l -L ANSI-C -t -m 100 -I -H retrophies_lexer_hash -N retrophies_lexer_lookup --output-file=tokens.c tokens.gperf  */
/* Computed positions: -k'1,3' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "tokens.gperf"
struct retrophies_lexer_keyword_t { const char* name; int token; };
#include <string.h>

#define TOTAL_KEYWORDS 38
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 18
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 40
/* maximum key range = 38, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
retrophies_lexer_hash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41,  2,  9, 30,
      16,  0,  5, 41, 15, 23, 16, 41, 22, 20,
      19,  5, 10, 19, 10,  6,  4,  6, 26, 16,
      22,  6, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
      41, 41, 41, 41, 41, 41, 41
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[2]+1];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct retrophies_lexer_keyword_t *
retrophies_lexer_lookup (register const char *str, register size_t len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  3,  2,  3,  2,  2,  4,  3,  6,  2,  3,  4,
       3,  4,  5,  5,  8,  4,  6,  6,  6,  3,  2,  2,  7,  7,
       3,  4,  5,  3, 18,  9,  4,  6,  7,  5,  3,  3, 12
    };
  static const struct retrophies_lexer_keyword_t wordlist[] =
    {
      {""}, {""}, {""},
#line 7 "tokens.gperf"
      {"end",                RETROPHIES_TOKEN_END},
#line 17 "tokens.gperf"
      {"as",                 RETROPHIES_TOKEN_AS},
#line 30 "tokens.gperf"
      {"and",                RETROPHIES_TOKEN_AND},
#line 14 "tokens.gperf"
      {"to",                 RETROPHIES_TOKEN_TO},
#line 33 "tokens.gperf"
      {"or",                 RETROPHIES_TOKEN_OR},
#line 5 "tokens.gperf"
      {"else",               RETROPHIES_TOKEN_ELSE},
#line 26 "tokens.gperf"
      {"abs",                RETROPHIES_TOKEN_ABS},
#line 6 "tokens.gperf"
      {"elseif",             RETROPHIES_TOKEN_ELSEIF},
#line 36 "tokens.gperf"
      {"be",                 RETROPHIES_TOKEN_BIGENDIAN},
#line 38 "tokens.gperf"
      {"bcd",                RETROPHIES_TOKEN_BINARYCODEDDECIMAL},
#line 13 "tokens.gperf"
      {"then",               RETROPHIES_TOKEN_THEN},
#line 8 "tokens.gperf"
      {"for",                RETROPHIES_TOKEN_FOR},
#line 12 "tokens.gperf"
      {"step",               RETROPHIES_TOKEN_STEP},
#line 24 "tokens.gperf"
      {"award",              RETROPHIES_TOKEN_AWARD},
#line 15 "tokens.gperf"
      {"until",              RETROPHIES_TOKEN_UNTIL},
#line 19 "tokens.gperf"
      {"function",           RETROPHIES_TOKEN_FUNCTION},
#line 27 "tokens.gperf"
      {"rand",               RETROPHIES_TOKEN_RAND},
#line 23 "tokens.gperf"
      {"trophy",             RETROPHIES_TOKEN_TROPHY},
#line 21 "tokens.gperf"
      {"static",             RETROPHIES_TOKEN_STATIC},
#line 25 "tokens.gperf"
      {"return",             RETROPHIES_TOKEN_RETURN},
#line 31 "tokens.gperf"
      {"mod",                RETROPHIES_TOKEN_MOD},
#line 40 "tokens.gperf"
      {"le",                 RETROPHIES_TOKEN_LITTLEENDIAN},
#line 9 "tokens.gperf"
      {"if",                 RETROPHIES_TOKEN_IF},
#line 28 "tokens.gperf"
      {"boolean",            RETROPHIES_TOKEN_BOOLEAN},
#line 20 "tokens.gperf"
      {"handles",            RETROPHIES_TOKEN_HANDLES},
#line 32 "tokens.gperf"
      {"not",                RETROPHIES_TOKEN_NOT},
#line 10 "tokens.gperf"
      {"next",               RETROPHIES_TOKEN_NEXT},
#line 3 "tokens.gperf"
      {"false",              RETROPHIES_TOKEN_FALSE},
#line 34 "tokens.gperf"
      {"xor",                RETROPHIES_TOKEN_XOR},
#line 37 "tokens.gperf"
      {"binarycodeddecimal", RETROPHIES_TOKEN_BINARYCODEDDECIMAL},
#line 35 "tokens.gperf"
      {"bigendian",          RETROPHIES_TOKEN_BIGENDIAN},
#line 4 "tokens.gperf"
      {"true",               RETROPHIES_TOKEN_TRUE},
#line 11 "tokens.gperf"
      {"repeat",             RETROPHIES_TOKEN_REPEAT},
#line 29 "tokens.gperf"
      {"integer",            RETROPHIES_TOKEN_INTEGER},
#line 16 "tokens.gperf"
      {"while",              RETROPHIES_TOKEN_WHILE},
#line 18 "tokens.gperf"
      {"dim",                RETROPHIES_TOKEN_DIM},
#line 22 "tokens.gperf"
      {"sub",                RETROPHIES_TOKEN_SUBROUTINE},
#line 39 "tokens.gperf"
      {"littleendian",       RETROPHIES_TOKEN_LITTLEENDIAN}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = retrophies_lexer_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
