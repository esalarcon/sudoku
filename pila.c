#include <string.h>
#include "pila.h"

int initpila(PILA *pila, void *buf, int lenitem, int nitems)
{
	pila->pos=0;
	pila->buf=buf;
	pila->lenitem=lenitem;
	pila->nitems=nitems;
	pila->encola=0;
	return 0;
}

int push(PILA *pila, void *item)
{
	if((pila->pos+pila->lenitem)>pila->nitems*pila->lenitem) return 1;
	memcpy(pila->buf+pila->pos,item,pila->lenitem);
	pila->pos+=pila->lenitem;
	pila->encola++;
	return 0;
}

int pop(PILA *pila, void *item)
{
	if(pila->pos>0)
	{
		pila->pos-=pila->lenitem;
		memcpy(item,pila->buf+pila->pos,pila->lenitem);
		pila->encola--;
		return 1;
	}
	return 0;
}

int enpila(PILA *pila)
{
	return pila->encola;
}
