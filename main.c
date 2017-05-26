#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int parser(char *nombre);

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
                    fprintf(stderr, "Las dimensiones del tablero ha sido ingresado de forma erronea.\n");
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
    parser(entries);
    return 0;
}

int parser(char *nombre)
{
    FILE *io = fopen(nombre, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    int energy, team, color;
    char* name = malloc(200);

    while ((nread = getline(&line, &len, io)) != -1)
    {
       sscanf(line, "%d %d %d %s", &energy, &team, &color, name);
       printf("%d - %d - %d - %s\n", energy, team, color, name);
    }

    free(line);
    fclose(io);
    return 1;
}
