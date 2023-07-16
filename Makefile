all: ht.o main.o
	gcc ht.o main.o -o out

ht.o: ht.c
	gcc -o ht.o -c ht.c

main.o: main.c
	gcc -o main.o -c main.c

clean:
	rm -rf *.o out