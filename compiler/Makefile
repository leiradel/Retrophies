ifeq ($(shell uname -a),)
	EXE=.exe
else ifneq ($(findstring MINGW,$(shell uname -a)),)
	EXE=.exe
else
	EXE=
endif

CFLAGS=-O0 -g -Wall -std=c99
OBJS=emitter.o lexer.o parser.o

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

all: retrophiesc$(EXE)

retrophiesc$(EXE): $(OBJS)
	gcc -o $@ $+

emitter.o: emitter.c emitter.h

lexer.o: lexer.c lexer.h tokens.c

parser.o: parser.c parser.h lexer.h emitter.h

tokens.c: tokens.gperf
	gperf -c -C -l -L ANSI-C -t -m 100 -I -H retrophies_lexer_hash -N retrophies_lexer_lookup --output-file=$@ $<

clean:
	rm -f retrophiesc$(EXE) $(OBJS) tokens.c
