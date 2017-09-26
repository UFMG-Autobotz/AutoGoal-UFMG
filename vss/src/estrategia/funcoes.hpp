#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

typedef struct Posicao{

    int linha;
    int coluna;

}Posicao;


typedef struct Campo{

    int nosso_lado;
    int num_linhas;
    int num_colunas;
    Posicao quinas[17];
    int **campo;
    int caminho_max;


}Campo;

typedef struct Trajetoria{

    int num_passos;
    Posicao *caminho;

}Trajetoria;


typedef struct Jogador{

    Posicao pos;
    int time;
    int quadrante;
    int id;
    int dimensao;
    Trajetoria trajetoria;

}Jogador;

// ******** Structs de Fila *********

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




int **Alocar_matriz_inteira (int m, int n);

int **Liberar_matriz_inteira (int m, int n, int **v);

void zerarCampo(Campo *campo);

void imprime(Campo *campo);

int achaMenor (int *opcoes, int tam);

void achaCaminho(Campo *campo, Jogador *jogadores_nossos, int indice_carro);

void setObstaculos(Campo *campo, Jogador *jogadores_nossos, Jogador *jogadores_deles, int indice_carro);

int predicao_linha_bola(Posicao pos[], Campo *campo, int coluna_predicao);

void wave_front(Campo *campo, Jogador jogadores_nossos[], Jogador jogadores_deles[], int indice_carro, Posicao destino);

void setBordas(Campo *campo);

void le_quinas(Campo *campo);

void setQuadrante (Jogador *jogadores, Campo *campo);

void setDimensao (Jogador jogadores[], int dimensao);

void estrategia(Campo *campo, Jogador jogadores_nossos[], Jogador jogadores_deles[], Posicao pos_bola[], int quadrante_bola, int estrategia);

void estrategia1(Campo *campo, Jogador jogadores_nossos[], Jogador jogadores_deles[], Posicao pos_bola[], int quadrante_bola);

void volta(Campo *campo, Jogador *jogadores_nossos, int indice, Jogador *jogadores_deles, Posicao volta_pos);

void chuta(Campo *campo, Jogador jogadores_nossos[], int indice, Jogador jogadores_deles[], Posicao pos_bola[]);

bool posicao_possivel(Posicao pos, Campo *campo);

int mais_perto(Jogador jogadores_nossos[], Posicao pos_bola[]);

float distancia (Posicao pos1, Posicao pos2);

int getQuadranteBola(Campo *campo, Posicao pos);

void caminho_chute (Campo *campo, Posicao pos_bola[], int raio_chute, Posicao caminho[]);

Posicao onde_chutar (Campo *campo);




// ********** Funcoes da Estrategia **********

void estrategia_goleiro(Campo *campo, Posicao *pos_goleiro, Posicao *predicao_bola, Posicao *pos_bola);


// ********** Funcoes de Fila **********


void pushBack(fila *f, Posicao dado);

void popFront(fila *f);

void initiate(fila **f);

int size(fila *f);

bool empty(fila *f);

void getFront(fila *f, Posicao *posicao);

void imprimi(fila *f);
