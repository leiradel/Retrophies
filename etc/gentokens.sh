#!/bin/sh
gperf -c -C -l -L ANSI-C -t -m 100 -I -H retrophies_lexer_hash -N retrophies_lexer_lookup --output-file=tokens.c tokens.gperf
