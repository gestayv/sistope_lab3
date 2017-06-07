all: back front

back: main.c fighters.c fighters.h
	gcc main.c fighters.c -o lab3 -pthread -I.

front: graphics.c graphics.h
	gcc graphics.c -o lab3f	 -lncurses -I.
