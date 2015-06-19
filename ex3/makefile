all: sudukusolver

sudukusolver: sudukusolver.o sudukutree.o genericdfs.o
	gcc -Wall -Wextra -Wvla -g sudukusolver.o sudukutree.o genericdfs.o -o sudukusolver -lm

genericdfs.a: genericdfs.o
	ar rcs genericdfs.a genericdfs.o

sudukusolver.o: sudukusolver.c
	gcc -c -Wall -Wextra -Wvla sudukusolver.c -o sudukusolver.o

sudukutree.o: sudukutree.c
	gcc -c -Wall -Wextra -Wvla sudukutree.c -o sudukutree.o

genericdfs.o: genericdfs.c
	gcc -c -Wall -Wextra -Wvla genericdfs.c -o genericdfs.o

clean:
	rm -rf *.o *.a