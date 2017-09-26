#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//#include "funcoes.hpp"

typedef struct nodo{

    Posicao dado;
    int posicao;
    nodo *proximo;

}nodo;

typedef struct fila{

    int tamanho;
    nodo *ultimo;
    nodo *primeiro;

}fila;

//void push_front(int n);
void pushBack(fila *f, Posicao dado);
//void pop_front();
void popFront(fila *f);

void initiate(fila **f);

int size(fila *f);
bool empty(fila *f);

//int &getFront();
void getFront(fila *f, Posicao *posicao);

void imprimi(fila *f);

