#include "fighters.h"
#include "graphics.h"

int checkDamage(entry* fighter);
void attackFighter(entry *fighter);

//  En fighters.c se definen todas las funciones que tienen que ver con cada pelea.
//  Esto es, ubicación de peleadores en el tablero, movimiento de peleadores en el tablero
//  y asegurarse de que los peleadores ataquen y reciban daño.

//  Función placeFighter: Se encarga de otorgarle a un peleador su posición inicial
//  en el tablero la que se escoge aleatoriamente.
//  Entrada:
//          - fighter: Estructura que posee todos los datos importantes de cada peleador.
void placeFighter(entry *fighter)
{
    do{
        fighter -> posx = rand()%sizeT;
        fighter -> posy = rand()%sizeT;
    }while(tablero[fighter -> posx][fighter->posy] != 0);
    wrefresh(mainwin);
}

//  Función printBoard: Se encarga de mostrar el tablero por pantalla usando una
//  barrera, de manera que se espera a que pasen N-1 threads y se imprime sólo cuando
//  pasa el último. Esta función se creó para debuggear el programa y saber
//  si el comportamiento otorgado a cada peleador funciona de manera correcta.
void printBoard()
{
    numThreads--;
    if(numThreads == 0)
    {
        numThreads = numThreadsAux;
        wrefresh(mainwin);
    }

}

//  Función moveFighter: Se encarga de mover un peleador dentro del tablero. Este
//  movimiento puede ser en cuatro direcciones (arriba, abajo, izquierda y derecha)
//  y además existe la opción de que se quede en el mismo lugar.
//  Entrada:
//          - fighter:  Estructura que posee todos los datos importantes de cada peleador.
void moveFighter(entry *fighter)
{
    int mov = 0, x_aux = 0, y_aux = 0, same = 0;
    checkDamage(fighter);
    do
    {
        mov = rand()%500;
        if(mov >= 0 && mov < 100)
        {
            x_aux = fighter->posx;
            y_aux = fighter->posy;
        }
        else if(mov >= 100 && mov < 200)
        {
            if(fighter->posx < sizeT - 1)
            {
                x_aux = fighter->posx+1;
                y_aux = fighter->posy;
                //printf("%d %d fighter: %s\n", x_aux, y_aux, fighter->name);
            }
            else
            {
                x_aux = fighter->posx;
                y_aux = fighter->posy;
            }
        }
        else if(mov >= 200 && mov < 300)
        {
            if(fighter->posx > 0)
            {
                x_aux = fighter->posx-1;
                y_aux = fighter->posy;
                //printf("%d %d fighter: %s\n", x_aux, y_aux, fighter->name);
            }
            else
            {
                x_aux = fighter->posx;
                y_aux = fighter->posy;
            }
        }
        else if(mov >= 300 && mov < 400)
        {
            if(fighter->posy < sizeT - 1)
            {
                x_aux = fighter->posx;
                y_aux = fighter->posy+1;
                //printf("%d %d fighter: %s\n", x_aux, y_aux, fighter->name);
            }
            else
            {
                x_aux = fighter->posx;
                y_aux = fighter->posy;
            }
        }
        else if(mov >= 400 && mov < 500)
        {
            if(fighter->posy > 0)
            {
                x_aux = fighter->posx;
                y_aux = fighter->posy-1;
                //printf("%d %d fighter: %s\n", x_aux, y_aux, fighter->name);
            }
            else
            {
                x_aux = fighter->posx;
                y_aux = fighter->posy;
            }
        }
        if(x_aux == fighter->posx && y_aux == fighter->posy)
        {
            same = 1;
        }
    }while(tablero[x_aux][y_aux] != 0 && same != 1);

    tablero[fighter->posx][fighter->posy] = 0;
    tablero[x_aux][y_aux] = 1;
    limpiarbloque(fighter->posx, fighter->posy, mainwin);
    mover(fighter->name[0], fighter->color, x_aux, y_aux, mainwin);
    wrefresh(mainwin);
    fighter->posx = x_aux;
    fighter->posy = y_aux;
    fighter->ki++;
    attackFighter(fighter);
}

//  Función attackFighter: Se encarga de verificar si el peleador puede realizar
//  un ataque y realizarlo en caso de ser posible.
//  Entrada:
//          - fighter:  Estructura que posee todos los datos importantes de cada peleador.
void attackFighter(entry *fighter)
{
    int ataque = 0;
    int xpos = fighter->posx;
    int ypos = fighter->posy;
    if(xpos > 0)
    {
        if(tablero[xpos-1][ypos] > 0)
        {
            if(tablero[xpos-1][ypos] == 1)
            {
                tablero[xpos-1][ypos] = fighter->ki*5;
                ataque = 1;
            }
            else if(tablero[xpos-1][ypos] > 1)
            {
                tablero[xpos-1][ypos] += fighter->ki*5;
                ataque = 1;
            }
        }
    }
    if(xpos < sizeT-1)
    {
        if(tablero[xpos+1][ypos] > 0)
        {
            if(tablero[xpos+1][ypos] == 1)
            {
                ataque = 1;
                tablero[xpos+1][ypos] = fighter->ki*5;
            }
            else if(tablero[xpos+1][ypos] > 1)
            {
                ataque = 1;
                tablero[xpos+1][ypos] += fighter->ki*5;
            }
        }
    }
    if(ypos > 0)
    {
        if(tablero[xpos][ypos-1] > 0)
        {
            ataque = 1;
            if(tablero[xpos][ypos-1] == 1)
            {
                ataque = 1;
                tablero[xpos][ypos-1] = fighter->ki*5;
            }
            else if(tablero[xpos][ypos-1] > 1)
            {
                ataque = 1;
                tablero[xpos][ypos-1] += fighter->ki*5;
            }
        }
    }
    if(ypos < sizeT-1)
    {
        if(tablero[xpos][ypos+1] > 0)
        {
            if(tablero[xpos][ypos+1] == 1)
            {
                ataque = 1;
                tablero[xpos][ypos+1] = fighter->ki*5;
            }
            else if(tablero[xpos][ypos+1] > 1)
            {
                ataque = 1;
                tablero[xpos][ypos+1] += fighter->ki*5;
            }
        }
    }
    if(ataque == 1)
    {
        fighter->ki=0;
    }
}

//  Función checkDamage: se encarga de verificar si un peleador ha recibido daño.
//  Entrada:
//          - fighter: Estructura que posee todos los datos importantes de cada peleador.
int checkDamage(entry *fighter)
{
    printBoard(fighter);
    if(tablero[fighter->posx][fighter->posy] > 1)
    {
        int hpaux = fighter->hp;
        fighter->hp = fighter->hp - tablero[fighter->posx][fighter->posy];
        tablero[fighter->posx][fighter->posy] = 1;

        if(fighter->hp <= 0)
        {
            universos[fighter->posArr] = -1;
            tablero[fighter->posx][fighter->posy] = -1;
            return 1;
        }
    }
    return 0;
}

//  Función victoryCondition: Se encarga de verificar si un universo es ganador o no.
int victoryCondition()
{
    int i = 0;
    int aux = 0;
    for(i = 0; i < numThreadsAux; i++)
    {
        if(universos[i] != -1)
        {
            aux = universos[i];
            break;
        }
    }

    for(i = 0; i < numThreadsAux; i++)
    {
        if(universos[i] != -1)
        {
            if(universos[i] != aux)
            {
                return -2;
            }
        }
    }

    return aux;
}

//  updateData: Función que actualiza los datos de cada luchador al comienzo de cada turno.
void updateData(entry* fighter)
{
    escribirStat(fighter->posArr, fighter->hp, fighter->universo, fighter->ki, fighter->color, fighter->name, second);
    wrefresh(second);
}

//  hiloLuchador: Rutina de inicio de cada thread de cada luchador.
//  Esta contiene todas las acciones que puede realizar un luchador tomando
//  en consideración cuáles podrían presentar condiciones de carrera y empleando
//  los mecanismos necesarios para removerlas/evitarlas.
void* hiloLuchador(void *newFighter)
{
    entry *fighter = (struct entry *)newFighter;

    //  Se ubica un mutex para que entre solo una hebra a la sgte sección de código.
    pthread_mutex_lock(&mutex);
    escribirStat(fighter->posArr, fighter->hp, fighter->universo, fighter->ki, fighter->color, fighter->name, second);
    wrefresh(second);
    placeFighter(fighter);
    pthread_mutex_unlock(&mutex);

    pthread_barrier_wait(&barrera);

    pthread_mutex_lock(&mutex);
    printBoard();
    pthread_mutex_unlock(&mutex);
    //  Si HP > 0
    while(1)
    {
        sleep(1);
        pthread_barrier_wait(&barrera);

        pthread_mutex_lock(&mutex);
        updateData(fighter);
        pthread_mutex_unlock(&mutex);

        pthread_barrier_wait(&barrera);

        pthread_mutex_lock(&mutex);
        finish = victoryCondition();
        pthread_mutex_unlock(&mutex);

        if(finish != -2)
        {
            break;
        }
            pthread_barrier_wait(&barrera);

            pthread_mutex_lock(&mutex);
            moveFighter(fighter);
            pthread_mutex_unlock(&mutex);

            pthread_mutex_lock(&mutex);
            printBoard();
            pthread_mutex_unlock(&mutex);

    }
}
