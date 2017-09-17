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

parser.o: parser.c parser.h lexer.h emitter.h if.c for.c while.c stmts.c subs.c program.c

clean:
	rm -f retrophiesc$(EXE) $(OBJS)
