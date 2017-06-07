#include "fighters.h"
#include "graphics.h"

//  Cabeceras de las funciones definidas luego del main.
int parseAndCreate(char *nombre, int debug);

int main(int argc, char* argv[])
{
    char    *entries = malloc(200);
    int     iflag = 0;
    int     nflag = 0;
    int     debug;
    int     dflag = 0;

    int c;
    opterr = 0;

    //  Se emplea getopt para obtener datos por terminal.
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
                if(sscanf(optarg, "%d", &sizeT) != 1)
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

    //  Se verifica si se ingresaron los parámetros o no.
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

    //  Se entrega una semilla a srand para luego calcular numeros pseudo random.
    srand(time(NULL));

    //  Se reserva memoria para el tablero.
    int i = 0, j = 0;
    tablero = malloc(sizeT * sizeof(int*));
    for (i = 0; i < sizeT; i++)
    {
        tablero[i] = malloc(sizeT * sizeof(int));
    }
    //  Se inicializa el tablero.
    for(i = 0; i < sizeT; i++)
    {
        for(j = 0; j < sizeT; j++)
        {
            tablero[i][j] = 0;
        }
    }

    finish = 1;

    parseAndCreate(entries, debug);
    return 0;
}

//  Función parseAndCreate: Se encarga de leer el archivo de texto y crear
//  los N threads correspondiente al número de peleadores.
int parseAndCreate(char *nombre, int debug)
{
    //  Se abre el archivo con los datos de cada participante.
    FILE *io = fopen(nombre, "r");
    //  Variable utilizada para leer el archivo línea por línea.
    char *line = NULL;
    //  Variable que indica la cantidad de caracteres a ser leídos por línea.
    size_t len = 0;
    //  Variable que se usará para indicar cuantos caracteres se leyeron por línea.
    ssize_t bytes_leidos;

    //  Variable que contiene el número de threads.
    int nThreads = 0;

    //  Arreglo de threads, uno para cada participante.
    pthread_t *fighters;

    //  Se recorre el archivo caracter por caracter para saber el número de luchadores.
    char c = 0;
    while((c = fgetc(io)) != EOF)
    {
        if (c == '\n')
        {
            nThreads++;
        }
    }

    numThreads = nThreads;
    numThreadsAux = nThreads;

    //  Se reserva memoria para los N threads y se ubica el cursor que lee el
    //  archivo con los luchadores al principio.
    fighters =  malloc(sizeof(pthread_t)*nThreads);
    universos = malloc(sizeof(int)*nThreads);
    inicializarPantalla(sizeT, nThreads);


    //  Se devuelve el cursor al principio del archivo.
    rewind(io);

    //  Se inicializa el mutex.
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("No se pudo inicializar el mutex\n");
        return -1;
    }

    //  Se inicializa la barrera.
    if (pthread_barrier_init(&barrera, NULL, numThreads) != 0)
    {
        printf("No se pudo inicializar la variable de condicion\n");
        return -1;
    }

    int i = 0, j = 0, nUni = 0;
    while ((bytes_leidos = getline(&line, &len, io)) != -1)
    {
        entry *newFighter = malloc(sizeof(entry));
        newFighter -> name = malloc(sizeof(char)*200);
        newFighter -> ki = 0;
        sscanf(line, "%d %d %d %s", &newFighter->hp, &newFighter->color, &newFighter->universo, newFighter->name);
        if(debug == 1)
        {
            //printf("Hp: %d Color: %d Universo: %d  Luchador: %s\n", newFighter->hp, newFighter->color, newFighter->universo, newFighter->name);
        }

        universos[i] = newFighter->universo;
        newFighter->posArr = i;

        pthread_create(&fighters[i], NULL, hiloLuchador, (void *)newFighter);
        i++;
    }

    for(i = 0; i < nThreads; i++)
    {
        pthread_join(fighters[i], NULL);
    }

    finishScreen(finish, sizeT, finale);
    wrefresh(finale);

    getchar();
    delwin(mainwin);
    delwin(second);
    delwin(finale);
    endwin();
    refresh();

    free(line);
    fclose(io);
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrera);
    return 1;
}
