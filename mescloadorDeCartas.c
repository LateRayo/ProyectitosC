#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define tam 48
#define mano 7

struct carta 
{
	char palo[10];
	int num;
	char nombre[10];
};

void asignar(int tam_M, struct carta ** mazo);
void mezclar(int tam_M,struct carta ** mazo);
int repartir(int tam_M,struct carta ** mazo,char * jugador, int tam_mano, int repartido);
void mostrar(int tam_M,struct carta ** mazo, char * jugador);
void ordenar(int tam_M,struct carta ** mazo,char * jugador, int tam_mano);
int suasca(char *cadena);

int main()
{	
	struct carta ** mazo;
    int repartido = 0;

    mazo = (struct carta**)malloc(tam*sizeof(struct carta**));

    if (mazo == NULL)
    {
        printf("Fallo al asignar menoria\n");
        exit(-1);
    }

    for (int i = 0; i < tam; i++)
    {
        *(mazo+i) = (struct carta*)malloc(sizeof(struct carta));

        if (*(mazo+i) == NULL)
        {
            printf("Fallo al asignar menoria\n");
            exit(-1);
        }

    }
    

    srandom(time(NULL));

    //crea el mazo ordenado
    asignar(tam,mazo);
    //muestra el mazo ordenado
    mostrar(tam,mazo,"secret");
    
    printf("\n\n\n-----------------------------------------\n\n\n");

    //mezcla em mazo
    mezclar(tam,mazo);
    //muestra el mazo desordenado
    mostrar(tam,mazo,"secret");

    printf("\n\n\n-----------------------------------------\n\n\n");

    //le reparte cartas a nicolas
    repartido = repartir(tam,mazo,"nicolas",mano, repartido);
    //le reparte cartas a martin
    repartido = repartir(tam,mazo,"martin",mano, repartido);
    mostrar(tam,mazo,"nicolas");
    printf("\n");
    mostrar(tam,mazo,"martin");

    printf("\n\n\n-----------------------------------------\n\n\n");

    //ordena las cartas de nicolas
    ordenar(tam,mazo,"nicolas",mano);
    mostrar(tam,mazo,"nicolas");
    printf("\n");
    //ordena las cartas de nicolas
    ordenar(tam,mazo,"martin",mano);
    mostrar(tam,mazo,"martin");


    //libero memoria
    for (int i = 0; i < tam; i++)
    {
        free(*(mazo+i));
    }
    free(mazo);

    return 0;	
}

//le asinga el palo t el numero a cada carta (unicamente valido para cartas españolas)
void asignar(int tam_M, struct carta ** mazo)
{   
    int cartas = 1;
    int i = 0;
    
    /*
    for (i; i < tam_M/4; i++)
    {
        strcpy((*(*(mazo+i))).palo,"copas");
        (*(*(mazo+i))).num = cartas;
        cartas ++;
    }
    */

    for (i; i < tam_M/4; i++)
    {
        strcpy(mazo[i]->palo,"copas");
        mazo[i]->num = cartas;
        cartas ++;
    }

    cartas = 1;

    for (i ;i < 2*(tam_M/4); i++)
    {
        strcpy(mazo[i]->palo,"oro");
        mazo[i]->num = cartas;
        cartas ++;
    }

    cartas = 1;

    for (i; i < 3*(tam_M/4); i++)
    {
        strcpy(mazo[i]->palo,"espada");
        mazo[i]->num = cartas;
        cartas ++;
    }

    cartas = 1;

    for (i; i < 4*(tam_M/4); i++)
    {
        strcpy(mazo[i]->palo,"basto");
        mazo[i]->num = cartas;
        cartas ++;
    }
    
}

void mezclar(int tam_M,struct carta ** mazo)
{   
    struct carta aux;
    long int barajear = random()%10000 + 500;

    for (int i = 0; i < tam_M + barajear; i++)     
    {
        long int rand1 = random()%48;
        long int rand2 = random()%48;

        aux = *(mazo[rand1]);
        *(mazo[rand1]) = *(mazo[rand2]);
        *(mazo[rand2]) = aux;
    }
}

void mostrar(int tam_M,struct carta ** mazo, char * jugador)
{
    if(strcmp(jugador,"secret"))
    {
        printf("cartas de: %s\n",jugador);

        for (int i = 0; i < tam_M; i++)
        {
            if(!strcmp(jugador,mazo[i]->nombre))
            {
                printf("numero: %d, palo: %s\n",mazo[i]->num,mazo[i]->palo);
            }
        }

    }else
    {
        for (int i = 0; i < tam_M; i++)
        {
            printf("pos:%2d, %2d %6s\n",i, mazo[i]->num,mazo[i]->palo);
            //printf("id carta: %d, numero: %d, palo: %s, jugador: %s\n",i, mazo[i].num,mazo[i].palo, mazo[i].nombre);
        }
    }
}

int repartir(int tam_M,struct carta ** mazo,char * jugador, int tam_mano, int repartido)
{
    int i = repartido;
    int r = repartido;

    if (tam_M<(repartido + tam_mano))
    {
        printf("\nno hay suficientes cartas para repartirle al jugador: %s\n",jugador);
        i = -1;

    }else
    {
        for (i; i < (tam_mano+r); i++)
        {
            strcpy(mazo[i]->nombre,jugador);
        }
    }
    return i;
}

//ordena por palo y por numero
void ordenar(int tam_M,struct carta ** mazo,char * jugador, int tam_mano)
{
    struct carta aux;
    int j = 0;

    for (int i = 0; i < tam_M; i++,j++)
    {
        if (!strcmp(jugador,mazo[i]->nombre))
        {
            i = tam_M;
        } 
    }

    int f = j-1;

    for (j; j < (f+tam_mano) ; j++)
    {
        for (int y = 0; y < (tam_mano); y++)
        {
            if ((mazo[y+f]->num + suasca(mazo[y+f]->palo))  > (mazo[y+f+1]->num + suasca(mazo[y+f+1]->palo)))
            {   
                aux = *(mazo[y+f]);
                *(mazo[y+f]) = *(mazo[y+f+1]);
                *(mazo[y+f+1]) = aux;
            }
        }
    }

}
//SUma el AScii de la los CAracteres de una cadena
int suasca(char *cadena) 
{
    int suma = 0;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        suma += (int)cadena[i]; 
    }

    return suma;
}