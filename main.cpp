#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string.h>
#include<iostream>
using namespace std;
//TIPOS 
const int DIM=6;
typedef char tTab[DIM-1][DIM];
typedef int tContador[DIM];// contador para ver que segun las columnas hasta que fila hay valores

//PROTOTIPOS
int menu();
void imprime(tTab tab);
void inicializa_tablero(tTab tab);
int genera_valor();
bool pedir_jugada_usuario(tTab tab, tContador cont);
bool mov_MAQ(tTab tab, tContador cont);
int comprobar_4linea(tTab tab);

    int main()
{
    tTab tablero;
    tContador cont;
    bool maq = false, jug = false;
    srand(time(NULL));
    bool non_exit= false;
    string nombre;
    ifstream ficheroC;
    ofstream ficheroG;
    while(non_exit){ // Mientras no quiera salir sigue jugando
        switch (menu()) // mostramos menu y escoge una opcion
        {
        case 1: //nueva partida
            inicializa_tablero(tablero, cont);
            imprime(tablero);
            while(jug || maq){ //Mientras el jugador y la maquina no tengan 4 en linea y el jugador no quiera salir...
                jug= pedir_jugada_usuario(tablero, cont);//Comprueba
                imprime(tablero);
                if(jug){
                    maq = mov_MAQ(tablero, cont);
                    imprime(tablero); // comprobamos
                }             
            }
            break;
        case 2: // carga partida 
                cout <<"Introduzca el nombre de la partida : " ;
                cin >> nombre; 
                cout << endl;
                nombre += ".txt"; // para abrir el fichero de texto donde guardamos la partida
                ficheroC.open(nombre);
                if (ficheroC.is_open()){
                    for(int i=0; i <DIM-1; i++){
                        for(int j=0; j < DIM; j++){
                            ficheroC >> tablero[i][j]; //de esta forma extraemos del fichero nuestro tablero
                        }
                        //leemos lo que queda de linea
                    }
                }
                else{
                    cout << "ERROR de apertura" << endl;
                }
                ficheroC.close();
                break;
        case 3: // guarda partida
                cout << "Introduzca el nombre de la partida : ";
                cin >> nombre;
                cout << endl;
                ficheroG.open(nombre);
                if(ficheroG.is_open()){
                    for(int i=0; i < DIM-1; i++){
                        for(int j=0; j <DIM; j++){
                            ficheroG << tablero[i][j]; // de esta forma cargamos en nuestro fichero los elementos del tablero
                        }
                        ficheroG << endl;
                    }
                }
                else{
                    cout << "ERROR de apertura" << endl;
                }
                ficheroG.close();
            break;
        case 4: // Salir del juego
            non_exit = true;
            break;
        }
    }
}

int menu(){
    int opcion;
    cout << "Juego de conecta 4" << endl;
    cout << "------------------" << endl;
    cout << "1. Nueva partida" << endl;
    cout << "2. Cargar partida" << endl;
    cout << "3. Guardar partida" <<endl;
    cout << "4. Salir" << endl;
    cout << "Seleccione una opcion";
    cin >> opcion ;
    cout << endl;
    while (opcion > 4 || opcion < 1){
        cout << "ERROR, la opcion no es valida" << endl;
        cin >> opcion;
    }
}

void imprime(tTab tab) {
    // funcion que dibujara en cada iteraccion el tablero
    cout << "1 2 3 4 5 6 7 " << endl;
    cout << "--------------" << endl;
    for(int i=0; i < DIM; i++){
        for(int j=0; j <DIM +1; j++){
            cout << tab[i][j];
        }
        cout << "|"  << endl;
        cout << "--------------" << endl;
    }
    cout << "--------------" << endl;
}

void inicializa_tablero(tTab tab, tContador cont){// funcion que inicializa el tablero a vacio
    for(int i=0; i <DIM-1; i++){
        for (int j=0; j <DIM; j++){
            tab[i][j] = '| x ';
        }
    }
}
int genera_valor() {
    //genera un numero aleatorio
    int aleat;
    aleat = 0 + rand() % (6 + 1 - 0);
    return aleat;
}
bool pedir_jugada_usuario(tTab tab, tContador cont){
    // funcion que pide jugada al usuario en caso de que este quiera salir retorna true
    int columna_usuario;
    bool salir = false;
    cout << "En que columna quiere introducir su ficha? (0 para volver al menu)";
    cin >> columna_usuario;
    cout << endl;
    if (columna_usuario == 0)
    { // si mete un cero se corta la ejecucion del programa
        salir = true;
    }
    else if (columna_usuario > 7)
    {
        cout << "ERROR, debe introducir una columna entroe 1 y 7" << endl;
    }
    tab[cont[columna_usuario]][columna_usuario - 1] = '| x ';
    if(comprobar_4linea(tab, cont[columna_usuario], columna_usuario - 1)){ // si al colocar la ficha el jugador tiene 4 en linea
        cout << "Winner" << endl; // gana la partida
        salir = true;
    }else{
        cont[columna_usuario]++; // Si no actualizamos la columna pues tiene una ficha más!
    }
    return salir;
}

bool mov_MAQ(tTab tab, tContador cont){
    int columna_MAQ;
    bool salir = false;
    columna_MAQ = genera_valor();
    tab[cont[columna_MAQ]][columna_MAQ] = '| O '; // de froma aleatoria la maquina mueve
    if (comprobar_4linea(tab, cont[columna_MAQ], columna_MAQ)){ // si a, colocar ficha la maquina tiene 4 en linea
        cout << "LOOSER" << endl; // el jugador pierde
        salir = true;
    }else{
        cont[columna_MAQ]++; // si no actualizamos la columna pues tiene una ficha más!
    }
    return salir;
}

bool comprobar_4linea(tTab tab, int coordX, int coordY){
    int contLinea=1, x, y;
    bool non_encontrado= true;
    //COLUMNA unicamente valoramos abajo
    x = coordX;// de esta forma no perdemos el valor de las coordenadas de origen
    y = coordY;
   while(tab[x][y] == tab[x - 1][y] && dentro_tablero(x - 1, y) && non_encontrado){ // mientras no encontremos 4 y sean del tipo que corresponde
       x -= 1; // bajamos una posicion en la columna
       contLinea++; // incrementamos el contador
       if(contLinea == 4){
           non_encontrado= false; // cuando tenemos las 4 rompemos el bucle
       }
   }
   //FILA valoramos tanto por la izq como por la drcha
   x = coordX;
   y = coordY;
   contLinea=1;
   while (tab[x][y] == tab[x][y + 1] && dentro_tablero(x, y + 1) && non_encontrado){
       y += 1;
       contLinea++;
       if (contLinea == 4){
           non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
       }
   }
   x = coordX;
   y = coordY;
   while (tab[x][y] == tab[x][y - 1] && dentro_tablero(x, y - 1) && non_encontrado){
       y -= 1;
       contLinea++;
       if (contLinea == 4){
           non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
       }
   }
   //DIAGONALES valoramos por arriba y por abajo
   x = coordX;
   y = coordY;
   contLinea = 1;
   while (tab[x][y] == tab[x + 1][y + 1] && dentro_tablero(x+1, y+1) && non_encontrado){
       x += 1;
       y += 1;
       contLinea++;
       if (contLinea == 4){
           non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
       }
   }
   x = coordX;
   y = coordY;
   while (tab[x][y] == tab[x - 1][y - 1] && dentro_tablero(x-1, y-1) && non_encontrado){
       x -= 1;
       y -= 1;
       contLinea++;
       if (contLinea == 4){
           non_encontrado = false; // cuando tenemos las 4 rompemos el bucle
       }
   }
   //DIAGONALES valoramos por arriba y por abajo
   x = coordX;
   y = coordY;
   contLinea = 1;
   while (tab[x][y] == tab[x - 1][y + 1] && dentro_tablero(x - 1, y + 1) && non_encontrado)
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
   while (tab[x][y] == tab[x + 1][y - 1] && dentro_tablero(x + 1, y - 1) && non_encontrado)
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

bool dentro_tablero(int x, int y){ // funcion que comprueba que no nos salimos del tablero
    bool dentro = true;
    if(x < 0 || y < 0 || x > 5 || y > 6){
        dentro = false;
    }
    return dentro;
}