
#include <iomanip>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Float32.h>


// *******************************************************

#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include "funcoes.hpp"
//#include "fila.hpp"
//#include "estrategia.hpp"

#include <GL/glut.h>

#define NUM_JOGADORES 3


 Trajetoria *trajetoria;

 Campo *campo;

 // posicoes dos carros no campo
 Jogador jogadores_nossos[NUM_JOGADORES];
 Jogador jogadores_deles[NUM_JOGADORES];

 // posicao da bola
 Posicao pos_bola[2]; // ultima e penultima posicao
 int quadrante_bola;
 Posicao *pos_goleiro;

 // pro goleiro
 Posicao *predicao_bola;


 /*---------------------- Funcoes callback ----------------------*/

 /*

 // funcao callback para ler as posicoes dos robos deles
 void their_robots(const vss::pos_array& msg)
 {

     for(int i = 0; i<msg.component.size(); i++){
         const vss::pos &data = msg.component[i];

         // caso o numero passado seja o identificador igual estou usando, que eh o indice do vetor, se nao tem que tratar
         //jogadores_deles[data.num].pos.linha = data.y;
         //jogadores_deles[data.num].pos.coluna = data.x;

         //ROS_INFO_STREAM("num: " << data.num << " x: " << data.x << " y: " << data.y << " theta: " << data.theta);
     }
 }


 // funcao callback para ler as posicoes dos nosso robos
 void robots(const vss::pos_array& msg)
 {

     for(int i = 0; i<msg.component.size(); i++){
         const vss::pos &data = msg.component[i];

         // caso o numero passado seja o identificador igual estou usando, que eh o indice do vetor, se nao tem que tratar
         //jogadores_nossos[data.num].pos.linha = data.y;
         //jogadores_nossos[data.num].pos.coluna = data.x;

         //ROS_INFO_STREAM("num: " << data.num << " x: " << data.x << " y: " << data.y << " theta: " << data.theta);
     }
 }

 // funcao callback para ler a posicao da bola
 void bola(const vss::pos& msg)
 {
     pos_bola[1] = pos_bola[0];
     pos_bola[0].linha = msg.y;
     pos_bola[0].coluna = msg.x;
    //ROS_INFO_STREAM(" x: " << msg.x << " y: " << msg.y);

 }*/


 // funcao copiada do tutorial haha
void Inicializa (void)

{
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-100,100,-100,100);
        glMatrixMode(GL_MODELVIEW);

}


void DesenhaNaTela(void)

{
    int i, j;
    int correcao_coluna = campo->num_colunas - 100;
    int correcao_linha = campo->num_linhas - 100;


    glClear(GL_COLOR_BUFFER_BIT);

    //desenhaGrade();

    /*
    glColor3f(0.0f, 0.0f, 0.0f); // preto

    glBegin(GL_LINES);

    // desenhando quinas e conectando algumas
    for (i=0; i<16; i++)
        glVertex2f(quinas[i][1]-correcao_coluna,quinas[i][0]-correcao_linha);

    // conectando quinas restantes
    for (i=1; i<15; i=i+2){

        glVertex2f(quinas[i][1]-correcao_coluna,quinas[i][0]-correcao_linha);
        glVertex2f(quinas[i+1][1]-correcao_coluna,quinas[i+1][0]-correcao_linha);

    }
    // conectando ultima quina
    glVertex2f(quinas[0][1]-correcao_coluna,quinas[0][0]-correcao_linha);
    glVertex2f(quinas[15][1]-correcao_coluna,quinas[15][0]-correcao_linha);

    glEnd();

    glPointSize(10.0f); // aumenta o tamanho dos pontos
    glBegin(GL_POINTS);

    // setando nosos carrinhos
    glColor3f(1.0f, 0.0f, 0.0f); // vermelho

    for (i=0; i<3; i++)
        glVertex2f(obstaculos_nos[i][1]-correcao_coluna,obstaculos_nos[i][0]-correcao_linha);

    // setando carrinhos adversarios
    glColor3f(0.0f, 0.0f, 1.0f); // azul

    for (i=0; i<3; i++)
        glVertex2f(obstaculos_eles[i][1]-correcao_coluna,obstaculos_eles[i][0]-correcao_linha);

    //setando posicao da bola
    glColor3f(0.0f, 0.3f, 0.0f); // verde menos intenso
    glVertex2f(pos_bola[1][1]-correcao_coluna,pos_bola[1][0]-correcao_linha); // penultima posicao
    glColor3f(0.0f, 0.7f, 0.0f); // verde mais intenso
    glVertex2f(pos_bola[0][1]-correcao_coluna,pos_bola[0][0]-correcao_linha); // ultima posicao

    //setando predicao da bola
    glColor3f(0.0f, 1.0f, 0.0f); // verde mais mais intenso
    glVertex2f(*predicao_coluna-correcao_coluna, *predicao_linha -correcao_linha);

    printf ("\nDENTRO DO DESENHA: PREDICAO_LINHA = %d\n", *predicao_linha);

    // setando posicao central
    glColor3f(0.0f, 0.0f, 0.0f); // preto
    glVertex2f(quinas[16][1]-correcao_coluna,quinas[16][0]-correcao_linha);

    glEnd();*/

    glPointSize(5.0f);
    glBegin(GL_POINTS);


    // desenha o campo
    for (i=0; i< campo->num_colunas; i+=1){

        for (j=0; j <campo->num_linhas; j+=1){

            if (campo->campo[j][i] >= campo->num_colunas * 10){
                glColor3f(0.0f, 0.0f, 0.0f); // preto
                glVertex2f(i-100,j-50);
            }
            else{
                glColor3f(0.8f, 0.8f, 0.8f); // azul
                glVertex2f(i-100,j-50);

            }


        }

     }
    for (i=-1; i<=1; i++){

        for (j=-1; j<=1; j++){

            glColor3f(0.0f, 0.2f, 0.0f); // verde
            glVertex2f(pos_bola[1].coluna + j -correcao_coluna, pos_bola[1].linha + i - correcao_linha);

            glColor3f(0.0f, 0.6f, 0.0f); // verde
            glVertex2f(pos_bola[0].coluna + j - correcao_coluna, pos_bola[0].linha + i - correcao_linha);

            glColor3f(0.0f, 1.0f, 0.0f); // verde
            glVertex2f(predicao_bola->coluna + j - correcao_coluna, predicao_bola->linha + i - correcao_linha);

        }

    }

    glEnd();


    glPointSize(2.0f);
    glBegin(GL_POINTS);


    // jogador 1
    for (i=0; i<jogadores_nossos[1].trajetoria.num_passos; i++){

        glColor3f(1.0f, 0.0f, 0.0f); // vermelho
        glVertex2f(jogadores_nossos[1].trajetoria.caminho[i].coluna - correcao_coluna, jogadores_nossos[1].trajetoria.caminho[i].linha - correcao_linha);

    }

    // goleiro
    for (i=0; i<jogadores_nossos[0].trajetoria.num_passos; i++){

        glColor3f(0.0f, 0.5f, 0.0f); // verde
        glVertex2f(jogadores_nossos[0].trajetoria.caminho[i].coluna - correcao_coluna, jogadores_nossos[0].trajetoria.caminho[i].linha - correcao_linha);

    }

    // jogador 2
    for (i=0; i<jogadores_nossos[2].trajetoria.num_passos; i++){

        glColor3f(0.0f, 0.0f, 0.5f); // azul
        glVertex2f(jogadores_nossos[2].trajetoria.caminho[i].coluna - correcao_coluna, jogadores_nossos[2].trajetoria.caminho[i].linha - correcao_linha);

    }

    glEnd();


    glutSwapBuffers();

}



int main(int argc, char *argv[]){


    /* -------------------- Declaracoes --------------------- */

    int i, j;
    int estrategia_id;       




    /* --------------------- Alocacoes ---------------------- */

    // dimensoes do campo
    campo = (Campo*) malloc (sizeof(Campo));


    //  variaveis globais sendo alocadas
    trajetoria = (Trajetoria*) malloc (sizeof(Trajetoria));
    pos_goleiro = (Posicao*) malloc (sizeof(Posicao));
    predicao_bola = (Posicao*) malloc (sizeof(Posicao));



    /* ------------------- Inicializacoes ------------------- */

    estrategia_id = 1;

    campo->num_colunas = 200;
    campo->num_linhas = campo->num_colunas * 0.75; // proporcao entre largura do campo 150cm e altura 130cm
    // aloca matriz campo de inteiros
    campo->campo = Alocar_matriz_inteira (campo->num_linhas, campo->num_colunas);


    campo->nosso_lado = 0; // 1 para direita e 0 para esquerda

    campo->caminho_max = (campo->num_colunas * campo->num_linhas)/5; // otimizar esse valor aqui! ainda nao pensei
    // alocando trajetoria dos jogadores
    jogadores_nossos[0].trajetoria.caminho = (Posicao*) malloc (campo->caminho_max * sizeof(Posicao));
    jogadores_nossos[1].trajetoria.caminho = (Posicao*) malloc (campo->caminho_max * sizeof(Posicao));
    jogadores_nossos[2].trajetoria.caminho = (Posicao*) malloc (campo->caminho_max * sizeof(Posicao));



    int dimensao_nos = campo->num_colunas/20 + 2; // testar valores
    int dimensao_eles = campo->num_colunas/20 + 4; // pensar sobre o "engordamento", necessarios testes


    // inicializando jogadores

    jogadores_nossos[0].pos.linha = 240;
    jogadores_nossos[0].pos.coluna = 412;
    jogadores_nossos[1].pos.linha = 128;
    jogadores_nossos[1].pos.coluna = 448;
    jogadores_nossos[2].pos.linha = 352;
    jogadores_nossos[2].pos.coluna = 480;

    jogadores_deles[0].pos.linha = 224;
    jogadores_deles[0].pos.coluna = 96;
    jogadores_deles[1].pos.linha = 96;
    jogadores_deles[1].pos.coluna = 192;
    jogadores_deles[2].pos.linha = 400;
    jogadores_deles[2].pos.coluna = 224;


    // inicializando bola

    pos_bola[0].linha = 340;
    pos_bola[0].coluna = 400;

    pos_bola[1].linha = 420;
    pos_bola[1].coluna = 490;



    /* ------------------------ Conversoes -------------------------- */


    // convertendo posicoes da resolucao da camera 640x480 para a resolucao utilizada
    for (i=0; i<3; i++){
            jogadores_nossos[i].pos.linha /= (640.0/(float)campo->num_colunas);
            jogadores_nossos[i].pos.coluna /= (480.0/(float)campo->num_linhas);
            jogadores_deles[i].pos.linha /= (640.0/(float)campo->num_colunas);
            jogadores_deles[i].pos.coluna /= (480.0/(float)campo->num_linhas);
        }    


    // convertendo posicao da bola na resolucao da camera 640x480 para a resolucao utilizada
    for (i=0; i<2; i++){

            pos_bola[i].linha /= (640.0/(float)campo->num_colunas); // a proporcao das colunas eh igual a das linhas
            pos_bola[i].coluna /= (480.0/(float)campo->num_linhas); // a proporcao das colunas eh igual a das linhas
    }



     /* ------------------------ ROS -------------------------- */

/*

    ros::init (argc, argv, "estrategia");
    ros::NodeHandle nh;

    if(!ros::master::check()){
        printf ("\nROS master nao encontrado!\n");
        return(0);
    }

    //
    ros::Subscriber sub1 = nh.subscribe("robots_position", 1000, &robots); // pega as posicoes dos nosso robos
    ros::Subscriber sub2 = nh.subscribe("AINDA NAO SEI", 1000, &bola); // pega as posicoes dos robos deles
    ros::Subscriber sub3 = nh.subscribe("ball_position", 1000, &bola);

    ros::Publisher pub[3]; //variavel que publica vetor de posição dos robôs aliados


    pub[0] = nh.advertise<vss::pos_array>("caminho/robo1", 1000);
    pub[1] = nh.advertise<vss::pos_array>("caminho/robo2", 1000);
    pub[2] = nh.advertise<vss::pos_array>("caminho/robo3", 1000);

    //Declarção da variável que vair armazenar as posições na mensagem
    vss::pos posicao;

    //Declaração do array de mensagens de posição
    vss::pos_array array[3];


    while (ros::ok())  {


        for (i=0; i<3; i++){


            for (j=0; j<jogadores_nossos[i].trajetoria.num_passos; j++){


                //armazena as posiçoes do robô na mensagem pos declarada como robot antes do início do for
                posicao.num = i; // ainda tem que decidir como serao as ids
                posicao.x = jogadores_nossos[i].trajetoria.caminho[j].coluna;
                posicao.y = jogadores_nossos[i].trajetoria.caminho[j].linha;
                //robot.theta = rot; // theta nao usado

                //armazena no vetor de mensagem de posições todos os dados do robô (x,y,theta)
                array[i].component.push_back(posicao);




            }

            pub[i].publish(array[i]);

        }


        ros::spinOnce();

        // sleep a bit:
        usleep(500);'
    }

    */

    /* ------------------------------------------------------ */


    // le as quinas do arquivo passado pela visao e seta dentro do objeto campo ja convertendo
    le_quinas(campo);

    // setando dimensao dos jogadores
    setDimensao (jogadores_nossos, dimensao_nos);
    setDimensao (jogadores_deles, dimensao_eles);


    /*---- Quadrantes ainda nao sao utilizados, mas deixarei aqui ----*/

    // setando os quadrantes dos jogadores
    setQuadrante (jogadores_nossos, campo);
    setQuadrante (jogadores_deles, campo);

    // pegando quadrante da bola
    quadrante_bola = getQuadranteBola(campo, pos_bola[0]);

    /* ---------------------------------------------------------------*/

    // chama funcao que decide a estrategia
    estrategia(campo, jogadores_nossos, jogadores_deles, pos_bola, quadrante_bola, estrategia_id);


    // chamada das funcoes de desenho

      glutInit(&argc,argv);

      glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

      glutInitWindowSize(640,480);

      glutInitWindowPosition(100,100);

      glutCreateWindow("T O C A 3");

      glutDisplayFunc(DesenhaNaTela);

      Inicializa();

      glutMainLoop();


    // libera memorias alocadas
    Liberar_matriz_inteira (campo->num_linhas, campo->num_colunas, campo->campo);
    free(trajetoria->caminho);
    free(campo);
    free(trajetoria);

}
