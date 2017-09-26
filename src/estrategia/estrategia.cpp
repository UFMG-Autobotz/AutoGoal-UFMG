#include "estrategia.hpp"


void estrategia_goleiro (Campo *campo, Posicao *pos_goleiro, Posicao *predicao_bola){


    int adiantamento = 5;

    pos_goleiro->linha = predicao_bola->linha;

    if (campo->nosso_lado == 1){ // direito

        pos_goleiro->coluna = predicao_bola->coluna - adiantamento; // adianta o goleiro pra nao bater nas paredes internas do gol

        if (predicao_bola->linha <= campo->quinas[3].linha) // trave direita
            pos_goleiro->linha = campo->quinas[3].linha;
        else if (predicao_bola->linha >= campo->quinas[6].linha) // trave esquerda
                    pos_goleiro->linha = campo->quinas[6].linha;
    }
    else{ // lado esquerdo
        pos_goleiro->coluna = predicao_bola->coluna + adiantamento;

        if (predicao_bola->linha <= campo->quinas[14].linha) // trave direita
            pos_goleiro->linha = campo->quinas[14].linha;
        else if (predicao_bola->linha >= campo->quinas[11].linha) // trave esquerda
            pos_goleiro->linha = campo->quinas[11].linha;

    }


}
