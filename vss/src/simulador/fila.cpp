#include "fila.hpp"
//#include "funcoes.hpp"

using namespace std;

void initiate(fila **f){

    (*f) = (fila*) malloc (sizeof(fila));
    (*f)->tamanho = 0;
    //(*f)->ultimo = NULL;
    //(*f)->primeiro = NULL;

}

void pushBack(fila *f, Posicao dado){

    nodo *aux;
    aux = (nodo*) malloc (sizeof(nodo));

    aux->dado.linha = dado.linha;
    aux->dado.coluna = dado.coluna;


    if (f->tamanho == 0)
        f->primeiro = aux;


    else
        f->ultimo->proximo = aux;

    aux->posicao = f->tamanho + 1;

    f->ultimo = aux;

    f->tamanho += 1;


}

void popFront(fila *f){


    nodo *aux;

    aux = f->primeiro;
    f->primeiro = f->primeiro->proximo;

    f->tamanho -= 1;

    free(aux);


}

int size(fila *f){

    return f->tamanho;


}

bool empty(fila *f){

    if (f->tamanho == 0)
        return true;

    return false;

}

void getFront(fila *f, Posicao *posicao){

    if (f->tamanho > 0){

        posicao->linha = f->primeiro->dado.linha;
        posicao->coluna = f->primeiro->dado.coluna;

    }


}

void imprimi(fila *f){

    int i;
    nodo *aux;
    aux = f->primeiro;
    printf ("\n---------- FILA -------------\n\n");
    for (i=0; i<f->tamanho; i++){

        printf ("%d %d\n", aux->dado[0], aux->dado[1]);
        aux = aux->proximo;

    }
    printf ("\n---------- FILA -------------\n");

}

