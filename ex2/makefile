waredb: waredb.o
	gcc waredb.o -o waredb

waredb.o: waredb.c
	gcc -Wall -Wextra -c waredb.c

CheckParenthesis: CheckParenthesis.o
	gcc CheckParenthesis.o -o CheckParenthesis

CheckParenthesis.o: CheckParenthesis.c
	gcc -Wall -Wextra -c CheckParenthesis.c

all: waredb CheckParenthesis

clean:
	rm -f *.o waredb CheckParenthesis
	
