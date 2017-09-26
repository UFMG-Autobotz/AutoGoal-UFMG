//#include "fila.hpp"
#include "funcoes.hpp"


int **Alocar_matriz_inteira (int m, int n)
{
  int **v;  /* ponteiro para a matriz */
  int   i;    /* variavel auxiliar      */
  if (m < 1 || n < 1) { /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (NULL);
     }
  /* aloca as linhas da matriz */
  v = (int **) calloc (m, sizeof(int *));
  if (v == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
     }
  /* aloca as colunas da matriz */
  for ( i = 0; i < m; i++ ) {
      v[i] = (int*) calloc (n, sizeof(int));
      if (v[i] == NULL) {
         printf ("** Erro: Memoria Insuficiente **");
         return (NULL);
         }
      }
  return (v); /* retorna o ponteiro para a matriz */
}

int **Liberar_matriz_inteira (int m, int n, int **v)
{
  int  i;  /* variavel auxiliar */
  if (v == NULL) return (NULL);
  if (m < 1 || n < 1) {  /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (v);
     }
  for (i=0; i<m; i++) free (v[i]); /* libera as linhas da matriz */
  free (v);      /* libera a matriz */
  return (NULL); /* retorna um ponteiro nulo */
}


void zerarCampo(Campo *campo){

    int i, j;

    for (i=0; i<campo->num_linhas; i++){

        for (j=0; j<campo->num_colunas; j++){

            campo->campo[i][j] = 0;

        }

    }

}

void imprime(Campo *campo){

    int i, j;


    printf ("\n");

    for (i=0; i<campo->num_linhas; i++){

        for (j=0; j<campo->num_colunas; j++){

            printf ("%3d ", campo->campo[i][j]);

        }

        printf ("\n");
    }


}

int achaMenor (int *opcoes, int tam){

    int i;
    int menor;

    menor = 0; // indice q vai ficar o menor elemento do vetor



    for (i=0; i<tam; i++){

        if (opcoes[i] < opcoes[menor])
            menor = i;

    }

    return menor;

}

void achaCaminho(Campo *campo, Jogador *jogadores_nossos, int indice_carro){

    int i, j, k;
    int opcoes[8]; // todas as direcoes: cima, baixo, dir, esq e diagonais
    int l, c; // usado so pra simplificar notacao
    int passo_atual=0;
    int direcao; // 0 esq, 1 dir, 2 baixo, 3 cima


    // inicio da trajetoria
    jogadores_nossos[indice_carro].trajetoria.caminho[0].linha = jogadores_nossos[indice_carro].pos.linha;
    jogadores_nossos[indice_carro].trajetoria.caminho[0].coluna = jogadores_nossos[indice_carro].pos.coluna;
    l = jogadores_nossos[indice_carro].pos.linha;
    c = jogadores_nossos[indice_carro].pos.coluna;

    passo_atual++;

    while (campo->campo[l][c]!=1 && passo_atual<campo->caminho_max-1){


        for (i=0; i<8; i++)
            opcoes[i] = campo->num_colunas * 10;


        if ((l - 1) >= 0 && campo->campo[l-1][c] < campo->num_colunas*10) // cima
            opcoes[2] = campo->campo[l-1][c];
        if ((l + 1) < campo->num_linhas && campo->campo[l+1][c] < campo->num_colunas*10) // baixo
            opcoes[6] = campo->campo[l+1][c];


        if ((c + 1) < campo->num_colunas && campo->campo[l][c+1] < campo->num_colunas*10) // direita
            opcoes[4] = campo->campo[l][c+1];
        if ((c - 1) >= 0 && campo->campo[l][c-1] < campo->num_colunas*10) // esquerda
            opcoes[0] = campo->campo[l][c-1];

        // diagonais
        if ((l - 1) >= 0 && campo->campo[l-1][c-1] < campo->num_colunas*10) // superior esquerda
            opcoes[1] = campo->campo[l-1][c-1];

        if ((l - 1) >= 0 && campo->campo[l-1][c+1] < campo->num_colunas*10) // superior direita
            opcoes[3] = campo->campo[l-1][c+1];

        if ((l + 1) < campo->num_linhas && campo->campo[l+1][c-1] < campo->num_colunas*10) // inferior esquerda
            opcoes[7] = campo->campo[l+1][c-1];

        if ((l + 1) < campo->num_linhas && campo->campo[l+1][c+1] < campo->num_colunas*10) // inferior direita
            opcoes[5] = campo->campo[l+1][c+1];


        direcao = achaMenor(opcoes, 8);


        switch(direcao){

            case 0: // esquerda
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].linha;
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].coluna - 1;
                break;
            case 1: // superior esquerda
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].linha - 1;
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].coluna - 1;
                break;
            case 2: // cima
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].linha - 1;
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].coluna;
                break;
            case 3: // superior direita
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].linha - 1;
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].coluna + 1;
                break;
            case 4: // direita
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].linha;
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].coluna + 1;
                break;
            case 5: // iferior direita
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].linha + 1;
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].coluna + 1;
                break;
            case 6: // baixo
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].linha + 1;
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].coluna;
                break;
            case 7: // inferior esquerda
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].linha + 1;
                jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual-1].coluna - 1;
                break;

        }



        l = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha;
        c = jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna;

        passo_atual++;

    }




    // ultima posicao
    jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].linha = l;
    jogadores_nossos[indice_carro].trajetoria.caminho[passo_atual].coluna = c;

    jogadores_nossos[indice_carro].trajetoria.num_passos = passo_atual+1;
}



void setObstaculos(Campo *campo, Jogador *jogadores_nossos,  Jogador *jogadores_deles, int indice_carro){



    int i, j, k, x, y;

    int valor;

    int dimensao_nos, dimensao_eles;

    dimensao_nos = jogadores_nossos[0].dimensao;
    dimensao_eles = jogadores_deles[0].dimensao;

    valor = campo->num_colunas * 10; // garantido que nao havera valor maior que esse no campo depois de gerar as ondas


    // setando obstaculos dos nossos carrinhos
    for (i=0; i<3; i++){

        x = jogadores_nossos[i].pos.linha;
        y = jogadores_nossos[i].pos.coluna;


        for (j=0; j<dimensao_nos; j++)

            for (k=0; k<dimensao_nos; k++)

                if ((x - (dimensao_nos/2) + j) >= 0 && (x - (dimensao_nos/2) + j) < campo->num_linhas && // teste das bordas das linhas
                    (y - (dimensao_nos/2) + k) >= 0 && (y - (dimensao_nos/2) + k) < campo->num_colunas && i != indice_carro){ // teste das bordas das colunas
                        campo->campo[x - (dimensao_nos/2) + j][y - (dimensao_nos/2) + k] = valor; // preenchendo o quadrado do carrinho no campo com um valor alto para eveitar trajetorias por ali




        }


    }

    // setando obstaculos dos carrinhos adversarios
    for (i=0; i<3; i++){

        x = jogadores_deles[i].pos.linha;
        y = jogadores_deles[i].pos.coluna;



        for (j=0; j<dimensao_eles; j++)

            for (k=0; k<dimensao_eles; k++)


                if ((x - (dimensao_eles/2) + j) >= 0 && (x - (dimensao_eles/2) + j) < campo->num_linhas && // teste das bordas das linhas
                    (y - (dimensao_eles/2) + k) >= 0 && (y - (dimensao_eles/2) + k) < campo->num_colunas) // teste das bordas das colunas
                        campo->campo[x - (dimensao_eles/2) + j][y - (dimensao_eles/2) + k] = valor*2; // preenchendo o quadrado do carrinho no campo com um valor alto para eveitar trajetorias por ali



    }



}



int predicao_linha_bola(Posicao pos[], Campo *campo, int coluna_predicao){


    int predicao_linha;

    float a, b;

    int bate_coluna;

    float aux;

    float angulo;

    if (campo->nosso_lado == 1){ // direito nosso

        if (pos[0].coluna <= pos[1].coluna)
            //return ((campo->quinas[3].linha + campo->quinas[6].linha)/2); // ponto medio entre as traves
            return -1;

        else{

            a = (float)(pos[0].linha - pos[1].linha) / (float)(pos[0].coluna - pos[1].coluna);
            b = pos[0].linha - a * pos[0].coluna;

            predicao_linha = a * coluna_predicao + b;

            if (predicao_linha < campo->quinas[1].linha || predicao_linha >= campo->quinas[8].linha){

                //lei de snell aqui
                angulo = atan ((float)(pos[0].linha - pos[1].linha) / (float)(pos[0].coluna - pos[1].coluna));


                if (predicao_linha < campo->quinas[1].linha){

                    bate_coluna = (campo->quinas[1].linha - b)/a;
                    aux = (bate_coluna - campo->quinas[3].coluna)* tan(angulo) + campo->quinas[1].linha;
                    predicao_linha = aux;


                }
                else if (predicao_linha >= campo->quinas[8].linha){

                    bate_coluna = (campo->quinas[8].linha - b)/a;
                    predicao_linha = (-1)*(bate_coluna - campo->quinas[7].coluna) * tan(angulo) + campo->quinas[8].coluna;


                }

                if (predicao_linha < campo->quinas[1].linha || predicao_linha >= campo->quinas[8].linha)
                    predicao_linha = (campo->quinas[3].linha+campo->quinas[6].linha)/2;


            }



        }
    }


    else { // lado esquerdo nosso

       // int coluna_predicao = quinas[4][1]; // trave do gol do lado direito

        if (pos[0].coluna >= pos[1].coluna)
            //return (campo->quinas[14].linha + campo->quinas[11].linha)/2;
            return -1;

        else{

            a = (float)(pos[0].linha - pos[1].linha) / (float)(pos[0].coluna - pos[1].coluna);
            b = pos[0].linha - a * pos[0].coluna;

            predicao_linha = a * coluna_predicao + b;

            if (predicao_linha < campo->quinas[0].linha || predicao_linha >= campo->quinas[9].linha){

                //lei de snell aqui
                angulo = atan ((float)(pos[0].linha - pos[1].linha) / (float)(pos[0].coluna - pos[1].coluna));


                if (predicao_linha < campo->quinas[0].linha){

                    bate_coluna = (campo->quinas[0].linha - b)/a;
                    aux = (campo->quinas[14].coluna - bate_coluna) * tan(angulo) + campo->quinas[0].linha;
                    predicao_linha = aux;



                }
                else if (predicao_linha >= campo->quinas[9].linha){

                    bate_coluna = (campo->quinas[9].linha - b)/a;
                    predicao_linha = (-1)*(campo->quinas[14].coluna - bate_coluna) * tan(angulo) +  campo->quinas[9].linha;

                }

                if (predicao_linha < campo->quinas[0].linha || predicao_linha >= campo->quinas[9].linha)
                    predicao_linha = (campo->quinas[14].linha + campo->quinas[11].linha)/2;

            }



        }


    }


    return predicao_linha;


}

void setBordas(Campo *campo){


    int i, j;
    int valor;

    valor = campo->num_colunas * 12;

    // retangulo, esquerda, acima do gol
    for (i = 0; i < campo->quinas[13].linha; i++) // linha
        for (j = 0; j <= campo->quinas[15].coluna; j++) // coluna
            campo->campo[i][j] = valor;

    // retangulo, esquerda, atras do gol
    for (i = campo->quinas[13].linha; i <= campo->quinas[12].linha; i++) // linha
        for (j=0; j<=campo->quinas[13].coluna; j++) // coluna
            campo->campo[i][j] = valor;

    // retangulo, esquerda, abaixo do gol
    for (i = campo->quinas[12].linha; i < campo->num_linhas; i++) // linha
        for (j = 0; j <= campo->quinas[11].coluna; j++) // coluna
            campo->campo[i][j] = valor;


    // retangulo, direita, acima do gol
    for (i = 0; i < campo->quinas[3].linha; i++) // linha
        for (j = campo->quinas[2].coluna; j < campo->num_colunas; j++) // coluna
            campo->campo[i][j] = valor;

    // retangulo, direita, atras do gol
    for (i = campo->quinas[3].linha; i < campo->quinas[5].linha; i++) // linha
        for (j = campo->quinas[4].coluna; j < campo->num_colunas; j++) // coluna
            campo->campo[i][j] = valor;

    // retangulo, direita, abaixo do gol
    for (i = campo->quinas[5].linha; i < campo->num_linhas; i++) // linha
        for (j = campo->quinas[7].coluna; j < campo->num_colunas; j++) // coluna
            campo->campo[i][j] = valor;


    // retangulo acima do campo
    for (i = 0; i < campo->quinas[0].linha; i++) // linha
        for (j = campo->quinas[15].coluna; j < campo->quinas[2].coluna; j++) // coluna
            campo->campo[i][j] = valor;

    // retangulo abaixo do campo
    for (i = campo->quinas[9].linha; i < campo->num_linhas; i++) // linha
        for (j = campo->quinas[10].coluna; j < campo->quinas[7].coluna; j++) // coluna
            campo->campo[i][j] = valor;

    // ----------- CANTOS ------------

    // canto superior esquerdo
    for (i = campo->quinas[0].linha; i <= campo->quinas[15].linha; i++) // linha
        for (j = campo->quinas[15].coluna; j < (campo->quinas[0].coluna - i + campo->quinas[0].linha); j++) // coluna
            campo->campo[i][j] = valor;

    // canto superior direito
    for (i = campo->quinas[1].linha; i <= campo->quinas[2].linha; i++) // linha
        for (j = campo->quinas[1].coluna + i - campo->quinas[1].linha; j < campo->quinas[2].coluna; j++) // coluna
            campo->campo[i][j] = valor;

    // canto inferior direito
    for (i = campo->quinas[7].linha; i <= campo->quinas[8].linha; i++) // linha
        for (j = campo->quinas[7].coluna - i + campo->quinas[7].linha; j < campo->quinas[7].coluna; j++) // coluna
            campo->campo[i][j] = valor;

    // canto inferior esquerdo
    for (i = campo->quinas[10].linha; i <= campo->quinas[9].linha; i++) // linha
        for (j = campo->quinas[10].coluna; j < (campo->quinas[10].coluna + i - campo->quinas[10].linha); j++) // coluna
            campo->campo[i][j] = valor;



}


void wave_front(Campo *campo, Jogador jogadores_nossos[], Jogador jogadores_deles[], int indice_carro, Posicao destino){

    Posicao posicao, LC;

    int nl, nc, acima, abaixo, dir, esq, l, c, k;

    fila *posicoes;
    initiate(&posicoes);


    zerarCampo(campo);

    setBordas(campo);

    setObstaculos(campo, jogadores_nossos, jogadores_deles, indice_carro);


    l = destino.linha;
    c = destino.coluna;
    k = 1;

    campo->campo[l][c] = 1;


    posicao.linha = l;
    posicao.coluna = c;


    pushBack(posicoes, posicao);

    getFront(posicoes, &LC);


    while (!empty(posicoes)){
        //imprimi(posicoes);
        //imprime(campo, largura_campo, altura_campo);

        getFront(posicoes, &LC);
        popFront(posicoes);

        l=LC.linha;
        c=LC.coluna;
        k=campo->campo[l][c];
        nl=l-1;
        if (nl >= 0){
            acima=campo->campo[nl][c];
            if (acima == 0){
                campo->campo[nl][c] = k+1;
                posicao.linha = nl;
                posicao.coluna = c;
                pushBack(posicoes, posicao);
            }
        }
        nl=l+1;
        if (nl < campo->num_linhas){
            abaixo=campo->campo[nl][c];
            if (abaixo == 0){
                campo->campo[nl][c] = k+1;
                posicao.linha = nl;
                posicao.coluna = c;
                pushBack(posicoes,posicao);
            }
        }
        nc=c-1;
        if (nc >= 0){
            esq=campo->campo[l][nc];
            if (esq == 0){
                campo->campo[l][nc] = k+1;
                posicao.linha = l;
                posicao.coluna = nc;
                pushBack(posicoes, posicao);
            }
        }
        nc=c+1;
        if (nc < campo->num_colunas){
            dir=campo->campo[l][nc];
            if (dir == 0){
                campo->campo[l][nc] = k+1;
                posicao.linha = l;
                posicao.coluna = nc;
                pushBack(posicoes, posicao);
            }
        }

    }
    free(posicoes);


}


void le_quinas(Campo *campo){

    int temp;

        FILE *entrada;

        int cont=0;
        entrada = fopen("pos_campo.txt","r+");
        if (!entrada)
            printf ("\nNao foi possivel abrir o arquivo de entrada!\n");
        while (true){

            fscanf(entrada,"%d", &campo->quinas[cont].coluna); // o x sera o numero de colunas, por isso colocado na posicao 1
            fscanf(entrada,"%d", &campo->quinas[cont].linha); // o y sera o numero de linhas, por isso colocado na posicao 0
            if (feof(entrada))
                break;

            // convertendo quinas passados do campo 640x480 para o campo usado na estrategia 200x150
            campo->quinas[cont].coluna /= (640.0/(float)campo->num_colunas);
            campo->quinas[cont].linha /= (480.0/(float)campo->num_linhas);

            temp = campo->quinas[3].coluna;


            cont++;

        }

        fclose(entrada);


}



void setQuadrante (Jogador *jogadores, Campo *campo){

    int i;


    for (i=0; i<3; i++){ // Numero de jogadores = 3

        if (jogadores[i].pos.linha < (campo->quinas[9].linha - campo->quinas[0].linha)/2 + campo->quinas[0].linha){ // Quadrante impar

            if (jogadores[i].pos.coluna < (campo->quinas[2].coluna - campo->quinas[15].coluna)/2 + campo->quinas[15].coluna){ // quadrantes 1 e 3


                if (jogadores[i].pos.coluna < (campo->quinas[2].coluna - campo->quinas[15].coluna)/4 + campo->quinas[15].coluna) // quadrante 1
                    jogadores[i].quadrante = 1;


                else // quadrante 3
                    jogadores[i].quadrante = 3;


            }

            else{ // quadrantes 5 e 7


                if (jogadores[i].pos.coluna < (campo->quinas[2].coluna - campo->quinas[15].coluna)*3/4 + campo->quinas[15].coluna) // quadrante 5
                    jogadores[i].quadrante = 5;


                else // quadrante 7
                    jogadores[i].quadrante = 7;




            }


        }



        else{ // Quadrante par


             if (jogadores[i].pos.coluna < (campo->quinas[2].coluna - campo->quinas[15].coluna)/2 + campo->quinas[15].coluna){ // quadrantes 2 e 4



                 if (jogadores[i].pos.coluna < (campo->quinas[2].coluna - campo->quinas[15].coluna)/4 + campo->quinas[15].coluna) // quadrante 2
                    jogadores[i].quadrante = 2;


                 else // quadrante 4
                    jogadores[i].quadrante = 4;


             }

             else{ // quadrantes 6 e 8


                 if (jogadores[i].pos.coluna < (campo->quinas[2].coluna - campo->quinas[15].coluna)*3/4 + campo->quinas[15].coluna) // quadrante 6
                    jogadores[i].quadrante = 6;


                 else // quadrante 8
                    jogadores[i].quadrante = 8;



             }



        }

    }

}



// ################################## ESTRATEGIA ########################################

void estrategia_goleiro (Campo *campo, Posicao *pos_goleiro, Posicao *predicao_bola, Posicao pos_bola[]){


    int adiantamento = 5;

    pos_goleiro->linha = predicao_bola->linha;

    if (predicao_bola->linha == -1){ // caso tenha sido identificado a bolinha se movendo na direcao contratia do gol
        pos_goleiro->linha = pos_bola[0].linha;
        predicao_bola->linha = pos_bola[0].linha;
        printf ("\n#### PASSOU! pos bola = [%d, %d] pos goleiro: %d %d#####\n", pos_bola[0].linha, pos_bola[0].coluna, pos_goleiro->linha, pos_goleiro->coluna);

    }

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




//########################################################################################


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FILA %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


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

        printf ("%d %d\n", aux->dado.linha, aux->dado.coluna);
        aux = aux->proximo;

    }
    printf ("\n---------- FILA -------------\n");

}



Posicao onde_chutar (Campo *campo){


    int i;
    Posicao pos_chute;

    int linha_inicial, linha_final, coluna_inicial, coluna_final, linha_atual;
    int amplitude_teste = 10; // amplitude de teste

    int distancia_superior, distancia_inferior;

    int distancia_trave = 3;



    switch (campo->nosso_lado){

        case 1: // o noss eh o lado direito
            linha_inicial = campo->quinas[14].linha;
            linha_final = campo->quinas[11].linha;
            coluna_inicial = campo->quinas[14].coluna;
            coluna_final = coluna_inicial + amplitude_teste;

            break;

        case 0: // o nosso eh o lado esquerdo
            linha_inicial = campo->quinas[3].linha;
            linha_final = campo->quinas[6].linha;
            coluna_inicial = campo->quinas[3].coluna - amplitude_teste;
            coluna_final = campo->quinas[3].coluna;

            break;

    }

    // caso nao encontre goleiro, chuta no meio
    distancia_superior = abs(linha_inicial - linha_final)/2;
    distancia_inferior = abs(linha_inicial - linha_final)/2;

    // partindo da trave de cima, procura o goleiro e calcula distancia_superior

    linha_atual = linha_inicial;


    while (linha_atual < linha_final){

        for (i=coluna_inicial; i<coluna_final; i++){
            if (campo->campo[linha_atual][i] >= campo->num_colunas*10)
                distancia_superior = linha_atual - linha_inicial;



        }

        linha_atual++;

    }


    // partindo da trave de baixo, procura o goleiro e calcula distancia_inferior

    linha_atual = linha_final;

    while (linha_atual > linha_inicial){

        for (i=coluna_inicial; i<coluna_final; i++){

            if (campo->campo[linha_atual][i] >= campo->num_colunas*10)
                distancia_inferior = linha_final - linha_atual;

        }

        linha_atual--;

    }



    pos_chute.coluna = coluna_inicial;

    if (distancia_superior > distancia_inferior)
        pos_chute.linha = linha_inicial+distancia_trave;

    else
        pos_chute.linha = linha_final-distancia_trave;


    return pos_chute;
}


void caminho_chute (Campo *campo, Posicao pos_bola[], int raio_chute, Posicao caminho[]){



    float a, b, theta;
    float dl, dc;

    Posicao pos_chute;
    Posicao pto_atras;


    pos_chute = onde_chutar(campo);

    a = (float)(pos_bola[0].linha - pos_chute.linha)/(float)(pos_bola[0].coluna - pos_chute.coluna);
    b = pos_chute.linha - a * pos_chute.coluna;

    theta = atan2(pos_bola[0].linha - pos_chute.linha, pos_bola[0].coluna - pos_chute.coluna);

    dc = raio_chute * cos(theta);
    dl = raio_chute * sin(theta);


    // TESTAR !!! nao sei se esses valores iguais ta certo
    switch (campo->nosso_lado){

        case 0:
            pto_atras.linha = pos_bola[0].linha + dl;
            pto_atras.coluna = pos_bola[0].coluna + dc;


            break;


        // nesse case deu certo!!! TESTAR O OUTRO!!!!!
        case 1:

            pto_atras.linha = pos_bola[0].linha + dl;
            pto_atras.coluna = pos_bola[0].coluna + dc;

            break;

    }


    caminho[0] = pto_atras;
    caminho[1] = pos_bola[0];
    caminho[2] = pos_chute;

}


int getQuadranteBola(Campo *campo, Posicao pos){

    int metade_linha, quarto_coluna;
    int quadrante;

    int k, b; // par 2k e impar 2k-1, 2k + b, b=0 no par e b=1 no impar

    metade_linha = (campo->quinas[9].linha - campo->quinas[0].linha)/2;
    quarto_coluna = (campo->quinas[2].coluna - campo->quinas[15].coluna)/4;

    k = (float)(pos.coluna - campo->quinas[15].coluna + quarto_coluna)/(float)quarto_coluna;
    b = (float)(pos.linha - campo->quinas[0].linha)/(float)metade_linha; // vai dar 0 pra posicoes impares e 1 pra posicoes pares

    b -= 1; // ajuste pra equacao abaixo funcionar, b=0 pros pares e b=-1 pros impares

    quadrante = 2 * k + b;

    return quadrante;

}

float distancia (Posicao pos1, Posicao pos2){

    float dist;

    dist = sqrt(pow(pos1.coluna - pos2.coluna,2) + pow(pos1.linha - pos2.linha, 2));

    return dist;


}

int mais_perto(Jogador jogadores_nossos[], Posicao pos_bola[]){

    float dist1, dist2;

    dist1 = distancia(jogadores_nossos[1].pos, pos_bola[0]);
    dist2 = distancia(jogadores_nossos[2].pos, pos_bola[0]);

    if (dist1 < dist2)
        return 1;

    return 2;



}

bool posicao_possivel(Posicao pos, Campo *campo){

    /*if (pos.linha < 0 || pos.linha >= campo->num_linhas)
        return false;
    if (pos.coluna < 0 || pos.coluna >= campo->num_colunas)
        return false;

    return true;*/


    // nao estou tratando as quininhas e nem engordando os objetos
    if (pos.linha <= campo->quinas[0].linha || pos.linha >= campo->quinas[9].linha ||
        pos.coluna <= campo->quinas[15].coluna || pos.coluna >= campo->quinas[2].coluna)
            return false;

    return true;



}


void chuta(Campo *campo, Jogador jogadores_nossos[], int indice, Jogador jogadores_deles[], Posicao pos_bola[]){


    int Raio = campo->num_linhas / 6;
    int l, c;
    int num_passos;
    int salto_raio = 2;
    Posicao ultima, penultima;


    // ta executando de novo qnd chama wavefront, mas foi a solucao por enquanto
    zerarCampo(campo);
    setBordas(campo);
    setObstaculos(campo, jogadores_nossos, jogadores_deles, indice);

    printf ("\nDISTANCIA = %f Raio = %d\n", distancia(pos_bola[0], jogadores_nossos[indice].pos), Raio);

    if (distancia(pos_bola[0], jogadores_nossos[indice].pos) > Raio){ // quando jogador esta distante da bola
        do {

            caminho_chute (campo, pos_bola, Raio, jogadores_nossos[indice].trajetoria.caminho);

            l = jogadores_nossos[indice].trajetoria.caminho[0].linha;
            c = jogadores_nossos[indice].trajetoria.caminho[0].coluna;

            penultima = jogadores_nossos[indice].trajetoria.caminho[1];
            ultima = jogadores_nossos[indice].trajetoria.caminho[2];
            Raio -= salto_raio;

            if (posicao_possivel(jogadores_nossos[indice].trajetoria.caminho[0], campo))
                if (campo->campo[l][c] < (campo->num_colunas)*10)

                    break;


        }while(Raio > 0);

        jogadores_nossos[indice].trajetoria.num_passos = 3; // pto atras [0], pto bola [1], pto no gol [2]
    }

    else{ // para quando o jogador esta mto proximo da bola

        jogadores_nossos[indice].trajetoria.num_passos = 0;
        jogadores_nossos[indice].trajetoria.caminho[0] = pos_bola[0];

        ultima = onde_chutar(campo);
        penultima = ultima;
        jogadores_nossos[indice].trajetoria.caminho[1] = ultima;
        jogadores_nossos[indice].trajetoria.num_passos = 2;


    }


    wave_front(campo, jogadores_nossos, jogadores_deles, indice, jogadores_nossos[indice].trajetoria.caminho[0]);

    achaCaminho(campo, jogadores_nossos, indice);

    num_passos = jogadores_nossos[indice].trajetoria.num_passos;


    jogadores_nossos[indice].trajetoria.caminho[num_passos] = penultima;
    jogadores_nossos[indice].trajetoria.caminho[num_passos+1] = ultima;


    jogadores_nossos[indice].trajetoria.num_passos += 2;


}

void volta(Campo *campo, Jogador *jogadores_nossos, int indice, Jogador *jogadores_deles, Posicao volta_pos){


    zerarCampo(campo);
    setBordas(campo);
    setObstaculos(campo, jogadores_nossos, jogadores_deles, indice);

    wave_front(campo, jogadores_nossos, jogadores_deles, indice, volta_pos);

    achaCaminho(campo, jogadores_nossos, indice);

}


void estrategia1(Campo *campo, Jogador jogadores_nossos[], Jogador jogadores_deles[], Posicao pos_bola[], int quadrante_bola){


    // ************************************ GOLEIRO *************************************

    Posicao *predicao_bola;
    Posicao *pos_goleiro;
    predicao_bola = (Posicao*) malloc (sizeof(Posicao));
    pos_goleiro = (Posicao*) malloc (sizeof(Posicao));

    if (campo->nosso_lado == 1)
        predicao_bola->coluna = campo->quinas[3].coluna; // direito
    else
        predicao_bola->coluna = campo->quinas[14].coluna; // esquerdo

    predicao_bola->linha = predicao_linha_bola(pos_bola, campo, predicao_bola->coluna);

    estrategia_goleiro (campo, pos_goleiro, predicao_bola, pos_bola);

    jogadores_nossos[0].trajetoria.num_passos = 1;
    jogadores_nossos[0].trajetoria.caminho[0] = *pos_goleiro;

    printf ("\nquina[3]=%d quina[6]=%d linha_bola=%d linha_goleiro=%d\n", campo->quinas[3].linha, campo->quinas[6].linha, pos_bola[0].linha, jogadores_nossos[0].trajetoria.caminho[0].linha);


    // **********************************************************************************

    Posicao volta1, volta2;

    int indice, sinal;

    float kc = (float)(1.0/4.0); // constante de recuo da coluna dos defensores
    float kl = (float)(1.0/6.0); // constante de espalhamento ao recuar

    int linha_inicial, linha_final, coluna_inicial, coluna_final;

    linha_inicial = campo->quinas[1].linha;
    linha_final = campo->quinas[8].linha;
    coluna_inicial = campo->quinas[15].coluna;
    coluna_final = campo->quinas[2].coluna;

    if (campo->nosso_lado == 1)
        sinal = 1;
    else
        sinal = -1;

    volta1.linha = ((linha_inicial + linha_final) / 2) - (float)(campo->num_linhas) * kl;
    volta2.linha = ((linha_inicial + linha_final) / 2) + (float)(campo->num_linhas) * kl;

    volta1.coluna = ((coluna_inicial + coluna_final) / 2) + sinal * campo->num_colunas * kc;
    volta2.coluna = ((coluna_inicial + coluna_final) / 2) + sinal * campo->num_colunas * kc;

     //   if (jogadores_nossos[1].pos.coluna < pos_bola[0].coluna && campo->nosso_lado == 1 ||
     //       jogadores_nossos[1].pos.coluna > pos_bola[0].coluna && campo->nosso_lado == 0 ){



        if (jogadores_nossos[1].quadrante < quadrante_bola && campo->nosso_lado == 1 ||
              jogadores_nossos[1].quadrante > quadrante_bola && campo->nosso_lado == 0 ){

            // os dois voltam
            if (jogadores_nossos[2].quadrante < quadrante_bola && campo->nosso_lado == 1 ||
                jogadores_nossos[2].quadrante > quadrante_bola && campo->nosso_lado == 0 ){


                jogadores_nossos[1].trajetoria.num_passos = 1;
                jogadores_nossos[1].trajetoria.caminho[0] = volta1;
                volta(campo, jogadores_nossos, 1, jogadores_deles, volta1);

                jogadores_nossos[2].trajetoria.num_passos = 1;
                jogadores_nossos[2].trajetoria.caminho[0] = volta2;
                volta(campo, jogadores_nossos, 2, jogadores_deles, volta2);

            }

            else { // 1 volta e 2 chuta


                jogadores_nossos[1].trajetoria.num_passos = 1;
                jogadores_nossos[1].trajetoria.caminho[0] = volta1;
                volta(campo, jogadores_nossos, 1, jogadores_deles, volta1);

                chuta(campo, jogadores_nossos, 2, jogadores_deles, pos_bola);

            }

        }
        else{ // os dois atras, o mais perto chuta

            // os dois estao atras da bola, escolhe o mais perto pra chutar
            if (jogadores_nossos[2].quadrante > quadrante_bola && campo->nosso_lado == 1 ||
                jogadores_nossos[2].quadrante < quadrante_bola && campo->nosso_lado == 0 ){


                indice = mais_perto(jogadores_nossos, pos_bola);

                chuta(campo, jogadores_nossos, indice, jogadores_deles, pos_bola);

            }

            else { // 2 volta e 1 chuta



                jogadores_nossos[2].trajetoria.num_passos = 1;
                jogadores_nossos[2].trajetoria.caminho[0] = volta2;
                volta(campo, jogadores_nossos, 2, jogadores_deles, volta2);

                chuta(campo, jogadores_nossos, 1, jogadores_deles, pos_bola);


            }



        }




}


void estrategia(Campo *campo, Jogador jogadores_nossos[], Jogador jogadores_deles[], Posicao pos_bola[], int quadrante_bola, int estrategia){

    switch(estrategia){

        case 1:
            estrategia1(campo, jogadores_nossos, jogadores_deles, pos_bola, quadrante_bola);
            break;
        case 2:
            break;
        default:
            break;

    }


}


void setDimensao (Jogador jogadores[], int dimensao){

    int i;

    for (i=0; i<3; i++)
        jogadores[i].dimensao = dimensao;



}
