#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#define DIM 7

//DEFINICIONES
int menu();
void imprime(char tablero[DIM-1][DIM]);
void inicializa_tablero(char tablero[DIM-1][DIM], int contador[DIM]);
int genera_valor();
bool pedir_jugada_usuario(char tablero[DIM-1][DIM], int contador[DIM]);
bool mov_MAQ(char tablero[DIM-1][DIM], int contador[DIM]);
bool comprobar_4linea(char tablero[DIM-1][DIM], int coordX, int coordY);
bool dentro_tablero(int x, int y);
bool completo(int contador[DIM]);

int main() // Revisar a fondo los ficheros del case 2 y case 3 !!!
{
    char tablero[DIM-1][DIM];
    int cont[DIM];
    bool jug = true;
    srand(time(NULL));
    bool non_exit = true;
    char  nombre[30];
    FILE *fichero = NULL;   
    while (non_exit == true) { // Mientras no quiera salir sigue jugando
      switch (menu()) // mostramos menu y escoge una opcion
	{
    case 1: //nueva partida
        jug = true;
        imprime(tablero);
        while (jug)
        {
            jug = pedir_jugada_usuario(tablero, cont); //Añade ficha y comprueba si hay 4 en línea
            if (completo(cont) == true)
            {
                printf("Empate\n");
                jug = false;
            }
            if (jug)
            {
                jug = mov_MAQ(tablero, cont);
                if (completo(cont) == true)
                {
                    printf("Empate\n");
                    jug = false;
                }
                imprime(tablero); // comprobamos
            }
        }
            break;
        case 2: // carga partida
            inicializa_tablero(tablero, cont);
            printf ("Introduzca el nombre de la partida : ");
            scanf ("%s", nombre);
            printf("\n");
            strcat(nombre,".txt");
            fichero = fopen(nombre,"r");//esto me permite abrir un fichero con el nombre de la variable nombre
            if (fichero==NULL) {
                printf ("No se encontró dicha partida\n");
                exit(1);
            }
            while ( !feof (fichero) ){
                for (int i = 0; i < DIM-1; i++) {
                    for (int j = 0; j < DIM; j++) {
                        fscanf (fichero, "%c", &tablero[i][j]);
                        if (tablero[i][j] == 'X' || tablero[i][j] == 'O') {// tenemos que inicializar el contador
                            cont[j]++;
                        }
                    }
                    //fscanf(fichero," \n");
                }
            }
            fclose (fichero);
            jug = true;
            imprime(tablero);
            while (jug)
            {
                jug = pedir_jugada_usuario(tablero, cont); //Añade ficha y comprueba si hay 4 en línea
                if (completo(cont) == true)
                {
                    printf("Empate\n");
                    jug = false;
                }
                if (jug)
                {
                    jug = mov_MAQ(tablero, cont);
                    if (completo(cont) == true)
                    {
                        printf("Empate\n");
                        jug = false;
                    }
                    imprime(tablero); // comprobamos
                }
            }
break;
        case 3: // guarda partida
            printf ( "Introduzca el nombre para guardar la partida : ");
            scanf ("%s", nombre);
            printf ("\n");
            strcat(nombre,".txt");
            fichero = fopen(nombre,"w");//esto me permite guardar el fichero con cualquier nombre mediante la variable nombre
            if (fichero==NULL) {
                 printf ("No es posible guardar dicha partida\n");
                 exit(1);
            }
            while ( !feof (fichero) ){
                for (int i = 0; i < DIM-1; i++) {
                    for (int j = 0; j < DIM; j++) {
                        fprintf (fichero, "%c", tablero[i][j]);
                    }
                    fprintf(fichero," \n");
                }
                //feof(fichero);
            }
            fclose (fichero);
            break;
        case 4: // Salir del juego
            non_exit = false;
            break;
        }
    }
exit(0);
}

int menu()
{ 
    int opcion;
    printf ( "Juego de conecta 4 \n");
    printf ( "------------------ \n");
    printf ( "1. Nueva partida \n");
    printf ( "2. Cargar partida \n");
    printf ( "3. Guardar partida \n");
    printf  ("4. Salir \n");
    printf ("Seleccione una opcion : ");
    scanf  ("%d", &opcion);
    printf ("\n" );
    while (opcion > 4 || opcion < 1) {
        printf ( "ERROR, la opcion no es valida \n");
        scanf ( "%d", &opcion);
    }
    return (opcion) ;
}

void imprime(char tablero[DIM-1][DIM])
{
    // funcion que dibujara en cada iteraccion el tablero
    printf  ( "  1    2    3    4    5    6    7   \n");
    printf ( "------------------------------------\n");
    for (int i = DIM-2; i >= 0; i--) {//Como en el tablero las filas van de 6 a 0, recoremos el for en sentido decreciente
        for (int j = 0; j < DIM; j++) {
 printf ( "|   %c", tablero[i][j] );
        }
        printf ( " | \n");
        printf ( "-------------------------------------\n");
    }
}

void inicializa_tablero(char tablero[DIM-1][DIM], int contador[DIM]) {
    for (int i = 0; i < DIM-1; i++) { // funcion que inicializa el tablero a cero
        for (int j = 0; j < DIM; j++) {
            tablero[i][j] = ' ';
        }
    }
    for (int x = 0; x < DIM ; x++) {
        contador[x] = 0;
    }

}
int genera_valor()
{ 
    //genera un numero aleatorio
    int aleat;
    aleat = (rand()%7); //Límite inferior es 0 y limite superior es 6
    return aleat;
}
bool pedir_jugada_usuario(char tablero[DIM-1][DIM], int contador[DIM])
{
    // funcion que pide jugada al usuario jugada en caso de que este quiera salir retorna true o en caso de que tenga 4 en linea
    int columna_usuario;
    bool jugar = true;
    printf ("En que columna quiere introducir su ficha?(0 para volver al menú) : ");
    scanf ("%d", &columna_usuario);
    printf ("\n");
    if (columna_usuario == 0)
    { // si mete un cero se corta la ejecucion del programa
       jugar = false;
    }
    else if (columna_usuario > 7 || contador[columna_usuario-1] > 5)
    {
      printf ("ERROR, acaba de perder su turno \n"); //Implementamos una penalización al jugador si no escoge una columna del 1 al 7 o si escoge una columna llena, perdiendo este su turno.
    }
    else{
        tablero[contador[columna_usuario - 1]][columna_usuario - 1] = 'X';
        if (comprobar_4linea(tablero, contador[columna_usuario - 1], (columna_usuario - 1))==false) { // si al colocar la ficha el jugador tiene 4 en línea
            printf ("Winner, bien jugado\n"); // gana la partida
            jugar = false;
        }
        else {
                contador[columna_usuario - 1]++; // Si no, actualizamos la columna pues tiene una ficha más!
        }
    }
    return jugar;
}

bool mov_MAQ(char tablero[DIM-1][DIM], int contador[DIM])
{// se genera un movimiento de la maquina de forma aleatoria
    int columna_MAQ;
    bool jugar = true;
    columna_MAQ = genera_valor();
    while (contador[columna_MAQ] > 5){
      columna_MAQ = genera_valor();
    }
    tablero[contador[columna_MAQ]][columna_MAQ] = 'O'; // de froma aleatoria la maquina mueve
    if (comprobar_4linea(tablero, contador[columna_MAQ], columna_MAQ)==false) { // si al colocar ficha la maquina tiene 4 en linea
        printf ( "LOOSER \n" ) ; // el jugador pierde
        jugar = false;
    }
    else {
        contador[columna_MAQ]++; // si no, actualizamos la columna pues tiene una ficha más!
    }
    return jugar;
}

bool comprobar_4linea(char tablero[DIM-1][DIM], int coordX, int coordY)
{
    int contLinea = 1;
    int x;
    int y;
    bool non_encontrado = true;
    //COLUMNA, unicamente valoramos abajo
    x = coordX;// de esta forma no perdemos el valor de las coordenadas de origen
    y = coordY;
    while (tablero[x][y] == tablero[x - 1][y] && dentro_tablero(x - 1, y) && non_encontrado) { // mientras no encontremos 4 y sean del tipo que corresponde
        x -= 1; // bajamos una posicion en la columna
        contLinea++; // incrementamos el contador
        if (contLinea == 4) {
            non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
        }
    }
    //FILA, valoramos tanto por la izq como por la drcha
    x = coordX;
    y = coordY;
    contLinea = 1;
    while (tablero[x][y] == tablero[x][y + 1] && dentro_tablero(x, y + 1) && non_encontrado) {
        y += 1;
        contLinea++;
        if (contLinea == 4) {
            non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
        }
    }
    x = coordX;
    y = coordY;
    while (tablero[x][y] == tablero[x][y - 1] && dentro_tablero(x, y - 1) && non_encontrado) {
        y -= 1;
        contLinea++;
        if (contLinea == 4) {
            non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
        }
    }
    //DIAGONALES 1, valoramos por arriba y por abajo
    x = coordX;
    y = coordY;
    contLinea = 1;
    while (tablero[x][y] == tablero[x + 1][y + 1] && dentro_tablero(x + 1, y + 1) && non_encontrado) {
        x += 1;
        y += 1;
        contLinea++;
        if (contLinea == 4) {
            non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
        }
    }
    x = coordX;
    y = coordY;
    while (tablero[x][y] == tablero[x - 1][y - 1] && dentro_tablero(x - 1, y - 1) && non_encontrado) {
        x -= 1;
        y -= 1;
        contLinea++;
        if (contLinea == 4) {
            non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
        }
    }
    //DIAGONALES 2, valoramos por arriba y por abajo
    x = coordX;
    y = coordY;
    contLinea = 1;
    while (tablero[x][y] == tablero[x - 1][y + 1] && dentro_tablero(x - 1, y + 1) && non_encontrado)
    {
        x -= 1;
        y += 1;
        contLinea++;
        if (contLinea == 4)
        {
            non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
        }
    }
    x = coordX;
    y = coordY;
    while (tablero[x][y] == tablero[x + 1][y - 1] && dentro_tablero(x + 1, y - 1) && non_encontrado)
    {
        x += 1;
        y -= 1;
        contLinea++;
        if (contLinea == 4)
        {
            non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
        }
    }
    return non_encontrado;
}

bool dentro_tablero(int x, int y)
{ // funcion que comprueba que no nos salimos del tablero
    bool dentro = true;
    if (x < 0 || y < 0 || x > 5 || y > 6) {
        dentro = false;
    }
    return dentro;
}

bool completo(char tablero[DIM - 1][Dim], int contador[DIM]){
    bool completo= true;
    for(int i=0; i < 7; i++){
        if(contador[i] < 5){
            completo= false;
        }
    }
    return completo;
}
