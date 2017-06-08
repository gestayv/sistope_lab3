all: back

back: main.c fighters.c fighters.h graphics.h
	gcc main.c fighters.c graphics.c -o lab3 -lncurses -lpthread -I.
