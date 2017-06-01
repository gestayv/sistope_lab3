#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>

/*Funcion limpiarbloque: Limpia el bloque en la posicion (x,y) */
void limpiarbloque(int x, int y, WINDOW* ventana){
	attron(COLOR_PAIR(0));
	mvaddstr(y, x, " ");
	attroff(COLOR_PAIR(0));
}

/*Funcion posicionar. Escoge una nueva posicion (x2,y2) al azar desde la posicion actual (x,y) */
void posicionar(int x, int y, int* x2, int* y2, WINDOW* ventana){
	int Xmax;
	int Ymax;
	//printf("%d %d \n", Xmax,Ymax);
	getmaxyx(ventana,Ymax,Xmax);
	int random = rand() % 5;
	switch(random){
		case 0:
			if(x+1 < Xmax){
				*x2 = x + 1;
			}	
			*y2 = y;
			break;
		case 1:
			if(x-1 > 0){
				*x2 = x - 1;
			}
			*y2 = y;
			break;
		case 2:
			*x2 = x;	
			if(y+1 < Ymax){
				*y2 = y + 1;
			}
			break;
		case 3:
			*x2 = x;
			if(y-1 > 0){
				*y2 = y - 1;
			}
			break;
		case 4:
			*x2 = x;
			*y2 = y;
			break;
	}
}

/*Funcion mover: mueve al caracter Peleador de "identificador" color a la posicion (x,y) */
void mover(char peleador, int identificador, int x, int y, WINDOW* ventana){
	attron(COLOR_PAIR(identificador));
	move(x,y);
	mvaddch(y, x, peleador);
	attroff(COLOR_PAIR(identificador));
}



int main(){
	srand(time(NULL));
    WINDOW * mainwin;
    

    
    /*  Initialize ncurses  */

    if ( (mainwin = initscr()) == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }


    initscr();
    start_color();

    init_pair(0, COLOR_BLUE,COLOR_BLUE);
    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_BLUE);
    attron(COLOR_PAIR(2));
    wborder(mainwin,0,0,0,0,ACS_BULLET,ACS_BULLET,ACS_BULLET,ACS_BULLET);
    attroff(COLOR_PAIR(2));
    refresh();
    getch();
    int x = 20;
    int y = 20;
    int x2;
    int y2;


    while(true){
    	//Limpia el bloque anterior
    	limpiarbloque(x,y, mainwin);
    	//Elije la nueva posicion
    	posicionar(x,y,&x2,&y2,mainwin);
    	//Mueve	
		mover('A',1,x2,y2,mainwin);
		//Se modifican los nuevos x e y
		x = x2;
		y = y2;
		getch();    	
    }


    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();

    return 0;	

}
