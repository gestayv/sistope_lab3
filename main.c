#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

pthread_mutex_t mutex;
int parseAndCreate(char *nombre);
void* hiloLuchador(void *newFighter);

typedef struct entry
{
    int x, y;
    char    *name;
    int     energy;
    int     universo;
    int     color;
} entry;

int main(int argc, char* argv[])
{
    char    *entries;
    int     iflag = 0;
    int     size;
    int     nflag = 0;
    int     debug;
    int     dflag = 0;

    int c;
    opterr = 0;

    while((c = getopt(argc, argv, "I:N:D:")) != -1)
	{
		switch(c)
		{
			//	En caso de ser I, se toma el nombre del .txt y se guarda en la variable "entries" por medio de sscanf
			case 'I':
				//	Si los argumentos pasados por terminal son "-I -N", se indica que se debe entregar el
				//	número de hijos.
				if(strcmp(optarg, "-N") == 0)
				{
					fprintf(stderr, "Se debe indicar el nombre del archivo de texto junto al parametro -I.\n");
                    fprintf(stderr, "Uso: ./laboratorio3 -I archivo_entrada.txt -N ancho_tablero -D opcion_debug.\n");
                    return 1;
				}
                sscanf(optarg, "%s", entries);
                iflag = 1;
				break;
            case 'N':
                //  Si el número de valores asignados a size es distinto a 1, se informa el error
                if(sscanf(optarg, "%d", &size) != 1)
                {
                    fprintf(stderr, "Las dimensiones del tablero han sido ingresadas de forma erronea.\n");
                    fprintf(stderr, "Uso: ./laboratorio3 -I archivo_entrada.txt -N ancho_tablero -D opcion_debug.\n");
                    return 1;
                }
                nflag = 1;
                break;
            //	En caso de ser D, se cambia el valor de la variable "debug" para indicar que el argumento se ingresó en la terminal.
			case 'D':
                if(sscanf(optarg, "%d", &debug) != 1)
                {
                    fprintf(stderr, "La opcion debug ha sido ingresada de forma erronea.\n");
                    fprintf(stderr, "Uso: ./laboratorio3 -I archivo_entrada.txt -N ancho_tablero -D opcion_debug.\n");
                    return 1;
                }
				dflag = 1;
				break;
			//	Si getopt detecta un error, devuelve el valor '?'. Por lo que es necesario agregar el caso.
			case '?':
				//	Si el error encontrado está en el argumento "-h", se indica que se debe ingresar el número de hijos.
                if(optopt == 'I')
				{
					fprintf(stderr, "Se debe indicar el nombre del archivo de texto junto al parametro -%c.\n", optopt);
                    fprintf(stderr, "Uso: ./laboratorio3 -I archivo_entrada.txt -N ancho_tablero -D opcion_debug.\n");
				}
                else if(optopt == 'N')
                {
                    fprintf(stderr, "Se deben indicar las dimensiones del tablero junto al parametro -%c.\n", optopt);
                    fprintf(stderr, "Uso: ./laboratorio3 -I archivo_entrada.txt -N ancho_tablero -D opcion_debug.\n");
                }
                else if(optopt == 'D')
                {
                    fprintf(stderr, "Se debe indicar la opcion de debug junto al paramtero -%c.\n", optopt);
                    fprintf(stderr, "Uso: ./laboratorio3 -I archivo_entrada.txt -N ancho_tablero -D opcion_debug.\n");
                }
				//	Si el argumento en el que se encontró el error se puede imprimir,
				//	se indica que la opción no existe.
				else if (isprint(optopt))
				{
					fprintf(stderr, "Opcion desconocida -%c.\n", optopt);
                    fprintf(stderr, "Uso: ./laboratorio3 -I archivo_entrada.txt -N ancho_tablero -D opcion_debug.\n");
				}
				//	Si el argumento en el que se encuentra el error no se puede imprimir,
				//	se indica que la opción tiene un caracter desconocido.
				else
				{
					fprintf(stderr, "Opcion con caracter desconocido `\\x%x'.\n", optopt);
                    fprintf(stderr, "Uso: ./laboratorio3 -I archivo_entrada.txt -N ancho_tablero -D opcion_debug.\n");
				}
				return 1;
			default:
				abort();
		}
	}

    if(!iflag)
    {
        fprintf(stderr, "No se ingreso el parametro -I.\n");
        return 1;
    }
    else if(!nflag)
    {
        fprintf(stderr, "No se ingreso el parametro -N.\n");
        return 1;
    }
    else if(!dflag)
    {
        fprintf(stderr, "No se ingreso el parametro D.\n");
        return 1;
    }
    srand(time(NULL));
    parseAndCreate(entries);
    return 0;
}

int parseAndCreate(char *nombre)
{
    //  Se abre el archivo con los datos de cada participante.
    FILE *io = fopen(nombre, "r");
    //  Variable utilizada para leer el archivo línea por línea.
    char *line = NULL;
    int bytes_leidos;
    size_t len;

    //  Variable que contiene el número de threads.
    int nThreads = 0;
    //  Arreglo de threads, uno para cada participante.
    pthread_t *fighters;

    //  Variables en las que se almacenan los datos de cada participante.
    int energy, universo, color;
    char* name = malloc(200);


    //  Variable utilizada para recorrer el archivo caracter por caracter,
    //  así se obtiene el número de participantes a partir del número de \n's.
    char c;
    do
    {
        c = fgetc(io);
        if(c == '\n')
        {
            nThreads++;
        }
        if(feof(io))
        {
            break;
        }
    }while(1);

    //  Se reserva memoria para los N threads y se ubica el cursor que lee el
    //  archivo con los luchadores al principio.
    fighters =  malloc(sizeof(pthread_t)*nThreads);
    rewind(io);

    //  Se inicializa el mutex.
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("No se pudo inicializar el mutex\n");
        return -1;
    }

    int i = 0;
    while ((bytes_leidos = getline(&line, &len, io)) != -1)
    {
       sscanf(line, "%d %d %d %s", &energy, &universo, &color, name);

       entry *newFighter = malloc(sizeof(entry));
       newFighter -> name = malloc(sizeof(char)*200);
       strcpy(newFighter -> name, name);
       newFighter -> energy = energy;
       newFighter -> color = color;
       newFighter -> universo = universo;

       pthread_create(&fighters[i], NULL, hiloLuchador, (void *)newFighter);
       i++;
    }

    for(i = 0; i < nThreads; i++)
    {
        pthread_join(fighters[i], NULL);
    }

    free(line);
    fclose(io);
    pthread_mutex_destroy(&mutex);
    return 1;
}

void* hiloLuchador(void *newFighter)
{
    entry *fighter = (struct entry *)newFighter;
    //  Aquí ocurre la magia.
    //  Primero ubico en el "tablero", posición random.
    //  Luego ocurre este ciclo.

    printf("hello, i am %s and i am entering the cs\n", fighter->name);
    pthread_mutex_lock(&mutex);
    printf("hello, i'am %s, and i am at the cs\n", fighter->name);
    while(1);
    pthread_mutex_unlock(&mutex);
    /*ACA HAY UN MUTEX LOCK*/
    //SC, genero posiciones random.
    /*ACA HAY UN MUTEX UNLOCK*/

    //  Si HP > 0


    //  1- Mover a una posición random. (thread safe)
    //      1.1- Aumentar ki (+1).
    //  2- Verificar si puede atacar. (thread safe)
    //      2.1- Si es posible:
    //          2.1.1- Obtengo los luchadores de las posiciones contiguas.
    //          2.1.2- Realiza el ataque (ki*5) y seteo ki en 0.
    //  3- Verificar si ha sido atacado. (thread safe)
    //      3.1- Restar el ataque del HP si fue atacado.

    free(fighter->name);
    free(fighter);
}
