#include <string.h>
#include "pila.h"

int initpila(PILA *pila, void *buf, int lenitem, int nitems)
{
	pila->pos=0;
	pila->buf=buf;
	pila->lenitem=lenitem;
	pila->nitems=nitems;
	pila->enpila=0;
	return 0;
}

int push(PILA *pila, void *item)
{
	if((pila->pos+pila->lenitem)>pila->nitems*pila->lenitem) return 1;
	memcpy(pila->buf+pila->pos,item,pila->lenitem);
	pila->pos+=pila->lenitem;
	pila->enpila++;
	return 0;
}

int pop(PILA *pila, void *item)
{
	if(pila->pos>0)
	{
		pila->pos-=pila->lenitem;
		memcpy(item,pila->buf+pila->pos,pila->lenitem);
		pila->enpila--;
		return 1;
	}
	return 0;
}

int enpila(PILA *pila)
{
	return pila->enpila;
}
