#include "gnu.h"

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
			if(x+1 < Xmax-1){
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
			if(y+1 < Ymax-1){
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
	initscr();
    WINDOW * mainwin = initscr();

    /*  Initialize ncurses  */
    if  (mainwin == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }

    /*Colores */
    start_color();
    init_pair(0, COLOR_BLACK,COLOR_BLACK);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);

    //Pone la terminal en nxn
    wresize(mainwin,20,25);
	curs_set(0);
    /*Tablero principal */
    attron(COLOR_PAIR(2));
    wborder(mainwin,0,0,0,0,ACS_BULLET,ACS_BULLET,ACS_BULLET,ACS_BULLET);
    attroff(COLOR_PAIR(2));
    refresh();
    getch();
    int x = 10;
    int y = 10;
    int xx = 1;
    int yy = 1;
    int xx2;
    int yy2;
    int x2;
    int y2;


    while(true){
    	//Limpia el bloque anterior
    	limpiarbloque(x,y, mainwin);
    	limpiarbloque(xx,yy,mainwin);
    	//Elije la nueva posicion
    	posicionar(x,y,&x2,&y2,mainwin);
    	posicionar(xx,yy,&xx2,&yy2,mainwin);
    	//Mueve
		mover('A',1,x2,y2,mainwin);
		mover('B',3,xx2,yy2,mainwin);
		//Se modifican los nuevos x e y
		x = x2;
		y = y2;
		xx = xx2;
		yy = yy2;
		getch();
    }


    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();

    return 0;

}
