all: commands.o shell.c
	gcc commands.o shell.c -o shell

commands.o: commands.h commands.c
	gcc -c commands.c

clean:
	rm -rf *.o