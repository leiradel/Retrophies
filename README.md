# Retrophies

Retrophies is:

* A language to write code that tests for awards target at emulators;
* A compiler that takes code written in that language and generates bytecode;
* A virtual machine that runs that bytecode, and signals to the host emulator if a trophy should be awarded; and
* A specification of how to correctly implement trophies in an emulator.

## Grammar

I've started this as a BASIC dialect, but I ended up borrowing more and more from Visual Basic, so I decided to make the syntax as close as possible to VB.


```
program = ( subroutine | function | trophy | dim )* .

subroutine = 'sub' ID ( '(' ID 'as' type ( ',' ID 'as' type )* ')' )? ( 'handles' ID ( ',' ID )* )?
             statement*
             'end' 'sub' .

function = 'function' ID ( '(' ID 'as' type ( ',' ID 'as' type )* ')' )? 'as' type
           statement*
           'end' 'function' .

trophy = 'trophy' ID ( '(' ID 'as' type ( ',' ID 'as' type )* ')' )?
         statement*
         'end' 'trophy' .

type = 'boolean' | 'string' | 'int8' | 'int16' | 'int32' | 'int64' | 'uint8' | 'uint16' | 'uint32' | 'uint64' | 'single' | 'double' .

statement = control-flow | commands | assignment-or-call .

control-flow = if | for | while | repeat .

commands = dim | return | award .

if = 'if' expression ( 'then' )? statement* ( 'elseif' expression ( 'then' )? statement* )* ( 'else' statement* )? 'end' 'if' .

for = 'for' ID '=' expression 'to' expression ( 'step' expression )? statement* 'next' ( ID )? .

while = 'while' expression statement* 'end' 'while' .

repeat = 'repeat' statement* 'until' expression .

dim = ( 'dim' | 'static' ( 'dim' )? ) variable-list ( ',' variable-list )* .

variable-list = ID ( ',' ID )* 'as' type ( '=' expression )? .

return  = 'return' ( expression )? .

award = 'award' STRING ',' STRING ',' STRING ',' NUMBER .

assignment-or-call = ID '=' expression
                   | ID ( '.' mem-type )? ( '.' mem-mode )? '[' expression ']' '=' expression
                   | ID '(' ( expression ( ',' expression )* )? ')'
                   .

expression = xor .

xor = or ( 'xor' or )* .

or = and ( 'or' and )* .

and = not ( 'and' not )* .

not = ( 'not' )? relational .

relational = shift ( ( '=' | '<' | '>' | '<>' | '<=' | '>=' ) shift )* .

shift = concat ( ( '<<' | '>>' ) concat )* .

concat = term ( '&' term )* .

term = mod ( ( '+' | '-' ) mod )* .

mod = idiv ( 'mod' idiv )* .

idiv = factor ( '\' factor )* .

factor = unary ( ( '*' | '/' ) unary )* .

unary = ( '-' | '+' )? terminal .

terminal = NUMBER | STRING | 'true' | 'false' | '(' expression ')' | ID-or-mem-or-call | math-function .

ID-or-mem-or-call = ID
                  | ID ( '.' mem-type )? ( '.' mem-mode )? '[' expression ']'
                  | ID '(' ( expression ( ',' expression )* )? ')'
                  .

mem-type = ( int8 | int16 | int32 | int64 | uint8 | uint16 | uint32 | uint64 | single | double ) .

mem-mode = ( 'be' | 'le' ) ( 'bcd' )? .

math-function = 'floor' '(' expression ')'
              | 'ceil' '(' expression ')'
              | 'trunc' '(' expression ')'
              | 'rand' '(' ( expression ',' expression )? ')'
              | 'abs' '(' expression ')'
              .
```
