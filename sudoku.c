#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "pila.h"

#define INTENTOS 1000000
#define LENPILA	 100
#define MAX(X,Y) ((X>Y)?X:Y)

typedef struct{
	int fila;
	int columna;
	int nposibles;
	int posibles[9];
}memoriaSudoku;

char bufpila[sizeof(memoriaSudoku)*LENPILA];
PILA pilaSudoku;
int maxstack=0;

void imprimirSudoku(int *s)
{
	int i,j;
	for(i=0;i<9;i++)
	{
		if(!(i%3))
		{
			for(j=0;j<13;j++)
				printf("-");
			printf("\n");
		}	
		for(j=0;j<9;j++)
		{
			if(!(j%3))
				printf("|");
			if(s[i*9+j])
				printf("%d",s[i*9+j]);
			else
				printf(" ");
		}
		printf("|\n");
	}
	for(j=0;j<13;j++)
		printf("-");
	printf("\n");
}

void cargarSudoku(int *s, char *str)
{
	int i,j;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			s[i*9+j]=str[i*9+j]-'0';
}

int esfijo(int *s,int fila, int col)
{
	int ret = 0;
	if(s[fila*9+col]) ret++;
	return ret;
}

void limpiarPosibles(int *posibles)
{
	int i;
	for(i=0;i<9;i++) 
		posibles[i]=1;
}

void buscarFila(int *s, int fila, int *posibles)
{
	int i;
	for(i=0;i<9;i++)
		if(s[fila*9+i] && posibles[s[fila*9+i]-1])
			posibles[s[fila*9+i]-1]--;
}

void buscarCol(int *s, int col, int *posibles)
{
	int i;
	for(i=0;i<9;i++)
		if(s[i*9+col] && posibles[s[i*9+col]-1])
			posibles[s[i*9+col]-1]--;
}

void buscarGrilla(int *s, int fila, int col, int *posibles)
{
	int i;
	int j;
	int i0 = (fila/3)*3;
	int i1 = i0+3;
	int j0 = (col/3)*3;
	int j1 = j0+3;
	for(i=i0;i<i1;i++)
		for(j=j0;j<j1;j++)
			if(s[i*9+j] && posibles[s[i*9+j]-1])
				posibles[s[i*9+j]-1]--;
				
}

int contarPosibles(int *posibles)
{
	int i;
	int j;
	for(i=0,j=0;i<9;i++)
		if(posibles[i]) j++;
	return j;
}

int primerPosible(int *posibles)
{
	int i;
	int j;
	for(i=0,j=0;i<9;i++)
		if(posibles[i])
		{
			j=i+1;
			break;
		}
	return j;
}

int eliminaPrimerPosible(int *posibles)
{
	int i;
	int j;
	for(i=0,j=0;i<9;i++)
		if(posibles[i])
		{
			posibles[i]=0;
			j=i+1;
			break;
		}
	return j;

}

int noestaresuelto(int *s)
{
	int i,j;
	int ret = 0;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			if(!s[i*9+j])
			{
				ret++;
				break;
			}
	return ret;
}

int decisionMinima(int *s, int *fila, int *col, int *p)
{
	int i,j,k;
	int posibles[9];
	k=10;
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			if(esfijo((int*)s,i,j))
				continue;
			limpiarPosibles(posibles);
			buscarFila((int*)s,i,posibles);
			buscarCol((int*)s,j,posibles);
			buscarGrilla((int*)s,i,j,posibles);
			if(contarPosibles(posibles)<k)
			{
				k=contarPosibles(posibles);
				memcpy(p,posibles,sizeof(posibles));
				*fila=i;
				*col=j;	
			}
		}
	}
	return k;
}

int resolver(int *s, int intentos)
{
	memoriaSudoku ms;
	int i,j,k;
	int p[9];
	k=0;
	do {
		switch(decisionMinima(s,&i,&j,&p[0]))
		{
			case 0:
				if(noestaresuelto(s))
				{
					//Elegi mal. Deshago hasta la ultima decisión, cambio el rumbo e intento de nuevo.
					while(pop(&pilaSudoku,&ms))
					{
						if(ms.nposibles==1)
						{
							s[ms.fila*9+ms.columna]=0;
							continue;
						}
						else
						{
							eliminaPrimerPosible(ms.posibles);
							if(contarPosibles(ms.posibles))
							{
								s[ms.fila*9+ms.columna]=primerPosible(ms.posibles);
								ms.nposibles=contarPosibles(ms.posibles);
								push(&pilaSudoku,&ms);
								maxstack=MAX(maxstack,enpila(&pilaSudoku));
								break;
							}
						}
					}
				}
				break;
			case 1:
				//No hay que decidir nada, lleno un casillero.
				ms.fila=i;
				ms.columna=j;
				ms.nposibles=contarPosibles(p);
				memcpy(ms.posibles,p,sizeof(p));
				s[i*9+j]= primerPosible(p);
				push(&pilaSudoku,&ms);
				maxstack=MAX(maxstack,enpila(&pilaSudoku));
				break;
			default:
				//Tengo que probar. Pruebo en la primera decisión mínima. 
				//Recuerdo que decidi.
				ms.fila=i;
				ms.columna=j;
				ms.nposibles=contarPosibles(p);
				memcpy(ms.posibles,p,sizeof(p));
				s[i*9+j]= primerPosible(p);
				push(&pilaSudoku,&ms);
				maxstack=MAX(maxstack,enpila(&pilaSudoku));				
				break;
				
		}
		k++;	
	} while(noestaresuelto(s) && k<intentos);
	return (k<intentos)?k:-1;
}


double get_process_time() {
    struct rusage usage;
    if( 0 == getrusage(RUSAGE_SELF, &usage) ) {
        return (double)(usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) +
               (double)(usage.ru_utime.tv_usec + usage.ru_stime.tv_usec) / 1.0e6;
    }
    return 0;
}

int main(int argc, char *argv[])
{
	int sudoku[9][9];
	int k;
	double start_t, end_t;
	
	if(argc!=2)
	{
		fprintf(stderr,"./sudoku [81 numeros, cero para vacios]\n");
		return EXIT_FAILURE;
	}

	if(strlen(argv[1])!=81)
	{
		fprintf(stderr,"./sudoku [81 numeros, cero para vacios]\n");
		return EXIT_FAILURE;

	}

	initpila(&pilaSudoku, bufpila,sizeof(memoriaSudoku),LENPILA);
	memset(sudoku,0,sizeof(sudoku));			
	cargarSudoku((int*)sudoku,argv[1]);	
	start_t = get_process_time();
	k=resolver((int*)sudoku,INTENTOS);
	end_t = get_process_time();
	imprimirSudoku((int*)sudoku);
	if(k>0)
		printf("Resuelto. %d\tIteraciones\t%d Elementos en pila (max.) - %0.03f s\n",k,maxstack,end_t-start_t);
	else
		printf("No pude resolverlo - %0.03f s\n",end_t-start_t);
	return EXIT_SUCCESS;
}

