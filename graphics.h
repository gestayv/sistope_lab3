#ifndef GNU_H
#define GNU_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>

void limpiarbloque(int x, int y, WINDOW* ventana);

void posicionar(int x, int y, int* x2, int* y2, WINDOW* ventana);

void mover(char peleador, int identificador, int x, int y, WINDOW* ventana);



#endif