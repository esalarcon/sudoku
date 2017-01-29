#ifndef __PILA
#define __PILA
	typedef struct
	{
		int pos;
		int lenitem;
		int nitems;
		int enpila;
		void *buf;
	}PILA;

	int initpila(PILA* pila, void *buf, int lenitem, int nitems);
	int push(PILA* pila, void *item);
	int pop(PILA* pila, void *item);
	int enpila(PILA *pila);
#endif

