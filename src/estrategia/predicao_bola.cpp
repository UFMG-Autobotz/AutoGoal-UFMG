#include <iostream>
#include <math.h>

int ultima_posicao_x = 5;
int ultima_posicao_y = 5;
int penultima_posicao_x = 4;
int penultima_posicao_y = 4;

int largura_campo = 40;
int altura_campo = largura_campo * (13/15);

int predicao_y;

int lado = 0;

int a, b;

int bate_x;

double angulo;

int main(){

    if (ultima_posicao_x >= penultima_posicao_x);
    else{

        a = (ultima_posicao_y - penultima_posicao_y) / (ultima_posicao_x - penultima_posicao_x);
        b = ultima_posicao_y - a * ultima_posicao_x;

        predicao_y = b;

        if (b < 0 || b >= alutra_campo){

            //lei de snell aqui
            angulo = atan ((ultima_posicao_y - penultima_posicao_y) / (ultima_posicao_x - penultima_posicao_x));


            if (b<0){

                bate_x = (-1 * b)/a;
                predicao_y = bate_x * tan(angulo);


            }
            else if (b >= alutra_campo){

                bate_x = (altura_campo - b)/a;
                predicao_y = bate_x * tan(angulo);

            }

            if (predicao_y < 0 || predicao_y >= altura_campo)
                predicao_y = altura_campo/2;

        }



    }




}
