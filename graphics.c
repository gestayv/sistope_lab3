#include "graphics.h"
//P corresponde al peliador
/*Funcion limpiarbloque: Limpia el bloque en la posicion (x,y)
	EJ: limpiarbloque(P->posx,P->posY,mainwin)
*/

void limpiarbloque(int x, int y, WINDOW* ventana){
	attron(COLOR_PAIR(0));
	mvaddstr(y+1, x+1, " ");
	attroff(COLOR_PAIR(0));
}

/*Funcion posicionar. Escoge una nueva posicion (x2,y2) al azar desde la posicion actual (x,y)
	EJ: posicionar(P->posx,P->posy.&P->posx,&P->posy,mainwin)
*/
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


/*Funcion mover: mueve al caracter Peleador de "identificador" color a la posicion (x,y)
	EJ: mover(P->name[0],P->color,P->posx,P->posy);
*/
void mover(char nombre, int identificador, int x, int y, WINDOW* ventana){
	attron(COLOR_PAIR(identificador));
	move(y+1,x+1);
	mvwaddch(ventana, y+1, x+1, nombre);
	attroff(COLOR_PAIR(identificador));

}


/*Escribe el dato en la pantalla de informacion
	EJ: escribirStat (linea, P->hp, P->universo, P->ki, P->color, P->name,second)
  */
void escribirStat(int linea, int hp, int universo, int ki, int color, char* nombre, WINDOW* ventana){
	/*limpiar la linea */
	wmove(ventana,2+linea,0);
	clrtoeol();
	/*paso de int a string*/
	char universoS[2];
	sprintf(universoS,"%d",universo);
	char hpS[4];
	sprintf(hpS,"%d",hp);
	char kiS[5];
	sprintf(kiS,"%d",ki);

	/*Impresion*/
	wattron(ventana,COLOR_PAIR(color));
	mvwprintw(ventana,2+linea,1,nombre);
	mvwprintw(ventana,2+linea,12,universoS);
	mvwprintw(ventana,2+linea,21,hpS);
	mvwprintw(ventana,2+linea,26,kiS);
	wattroff(ventana,COLOR_PAIR(color));

	/*refresh*/
}


/*mostrarDmg. Funcion que muestra el fondo rojo
	mostrarDmg(P->nombre[0],P->color,P->posx,P->posy,ventana)*/
void mostrarDmg(char nombre,int color,int x, int y, WINDOW* ventana){
	init_pair(99,color,COLOR_RED);
	attron(COLOR_PAIR(99));
	mvwaddch(ventana,y,x,nombre);
	attroff(COLOR_PAIR(99));

}

WINDOW* mainwin;
WINDOW* second;
WINDOW* finale;

void pantallaPunt(int n, int numero){
	//Se crea la segunda pantalla
    second = newwin(6+numero,32,0,n+2);
    wborder(second,0,0,0,0,ACS_BULLET,ACS_BULLET,ACS_BULLET,ACS_BULLET);
   	//wrefresh(second);
   	mvwprintw(second,1,1,"Nombre");
   	mvwprintw(second,1,12,"Univ");
   	mvwprintw(second,1,21,"HP");
   	mvwprintw(second,1,26,"KI");
   	wrefresh(second);
   	}

int inicializarPantalla(int n, int numero){
	mainwin = initscr();

    /*  Initialize ncurses  */
    if  (mainwin == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }

    /*Colores */
    start_color();
    init_pair(0, COLOR_BLACK,COLOR_BLACK); //Color del fondo
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); //
    init_pair(2, COLOR_RED, COLOR_BLACK);	//Color del borde del tablero.
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_color(8,250,128,114);
    init_pair(8,8,COLOR_BLACK);
    init_color(9,128,0,128);
    init_pair(9,9,COLOR_BLACK);
    init_color(10,0,255,255);
    init_pair(10,10,COLOR_BLACK);


    //Pone la terminal en nxn
    wresize(mainwin,n+2,n+2);
	curs_set(0);
	wmove(mainwin, 1, 1);
    /*Tablero principal */
    attron(COLOR_PAIR(2));
    wborder(mainwin,0,0,0,0,ACS_BULLET,ACS_BULLET,ACS_BULLET,ACS_BULLET);
    attroff(COLOR_PAIR(2));

    pantallaPunt(n,numero);
	wrefresh(mainwin);
}


/*finishScreen. Funcion que entrega el mensaje de victorioso
	EJ: finishScreen(P->universo, n, finale) */
void finishScreen(int universo, int n , WINDOW* ventana){

	finale = newwin(n+2,n+2,0,0);
	wborder(finale,'$','$','$','$','$','$','$','$');
	int x, y;
	getmaxyx(finale,y,x);
	int Xmedio = x/2;
	int Ymedio = y/2;
	mvwprintw(finale,Ymedio-3,Xmedio,"GANADOR");
	mvwprintw(finale,Ymedio,Xmedio,"Universo %d", universo);
}

/*Funcion para ejemplar  */
int test(){
	srand(time(NULL));
    mainwin = initscr();

    /*  Initialize ncurses  */
    if  (mainwin == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }

    /*Colores */
    start_color();
    init_pair(0, COLOR_BLACK,COLOR_BLACK); //Color del fondo
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); //
    init_pair(2, COLOR_RED, COLOR_BLACK);	//Color del borde del tablero.
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_color(8,250,128,114);
    init_pair(8,8,COLOR_BLACK);
    init_color(9,128,0,128);
    init_pair(9,9,COLOR_BLACK);
    init_color(10,0,255,255);
    init_pair(10,10,COLOR_BLACK);

 	int n = 50;

    //Pone la terminal en nxn
    wresize(mainwin,n,n);
	curs_set(0);

    /*Tablero principal */
    attron(COLOR_PAIR(2));
    wborder(mainwin,0,0,0,0,ACS_BULLET,ACS_BULLET,ACS_BULLET,ACS_BULLET);
    attroff(COLOR_PAIR(2));


    /*Datos de prueba*/
    int x = 10;
    int y = 10;
    int xx = 20;
    int yy = 20;
    int xx2;
    int yy2;
    int x2;
    int y2;



    int ki = 10;
    int ki2 = 15;

    while(true){
    	//Limpia el bloque con la posicion anterior
    	limpiarbloque(x,y, mainwin);
    	limpiarbloque(xx,yy,mainwin);
    	//Elije la nueva posicion (Esto lo hace la hebra)
    	posicionar(x,y,&x2,&y2,mainwin);
    	posicionar(xx,yy,&xx2,&yy2,mainwin);
    	//Mueve hacia la posicion nueva
		mover('A',9,x2,y2,mainwin);
		mover('B',8,xx2,yy2,mainwin);
		//Se modifican los nuevos x e y
		x = x2;
		y = y2;
		xx = xx2;
		yy = yy2;

		/*Imprime en la pantalla de puntaje */
		escribirStat(1,10,1,ki,9,"A",second);
		escribirStat(2,10,1,ki2,8,"B",second);
		wrefresh(second);

		ki = ki+5;
		ki2 = ki2+2;

		refresh();
		getch();
    }


    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();
    return 0;
}
