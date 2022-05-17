build: main.c functions.c
	gcc -Wall -g -o tema3 main.c functions.c

clean: tema3
	rm tema3