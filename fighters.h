#ifndef FIGHTERS_H
#define FIGHTERS_H

#include "graphics.h"

typedef struct entry
{
    int     posx;       //  Posición X en el tablero.
    int     posy;       //  Posición Y en el tablero.
    char    *name;      //  Nombre del peleador.
    int     hp;         //  Vida del peleador.
    int     ki;         //  Ki del peleador.
    int     universo;   //  Universo del peleador.
    int     color;      //  Color del peleador.
    int     posArr;     //  Posición del peleador en el arreglo de universos.
} entry;

//  Variables que almacenan el número de threads.
int numThreads, numThreadsAux;
//  Tablero de 1 y 0 en el cual se ubican los luchadores.
int **tablero;
//  Arreglo de tamaño N, cada columna corresponde a un universo y posee el num
//  de peleadores vivos restantes.
int *universos;
//  Variable que almacena el tamaño del tablero.
int sizeT;

int finish;
//  Mutex a ser utilizado para proteger secciones críticas.
pthread_mutex_t mutex;
//  Variable condición para crear una barrera
pthread_barrier_t barrera;
//  Ventana principal del tablero, donde se ven las batallas
WINDOW* mainwin;
//  Ventana de informacion, donde se ven los datos
WINDOW* second;
//	Ventalla de finalizacion, donde se ven el universo ganador
WINDOW* finale;

void* hiloLuchador(void *newFighter);

#endif
