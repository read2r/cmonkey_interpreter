# Makefile

CC=gcc
FLAGS=-g
OBJS=main.o token.o lexer.o repl.o ast.o parser.o
LIBS=
all : intp.out

intp.out : $(OBJS)
	$(CC) $(FLAGS) -o intp.out $(OBJS) $(LIBS)

main.o : main.c
	$(CC) $(FLAGS) -c main.c
token.o : token.c
	$(CC) $(FLAGS) -c token.c
lexer.o : lexer.c
	$(CC) $(FLAGS) -c lexer.c
repl.o : repl.c
	$(CC) $(FLAGS) -c repl.c
ast.o : ast.c
	$(CC) $(FLAGS) -c ast.c
parser.o : parser.c
	$(CC) $(FLAGS) -c parser.c

clean:
	rm -f $(OBJS) *.out *.o core
