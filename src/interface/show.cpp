#include "ros/ros.h"
#include "ARMarkers.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "std_msgs/String.h"
//include das mensagens de posição
#include "vss/pos.h"
#include "vss/pos_array.h"
ros::Publisher pub; //variavel que publica vetor de posição dos robôs aliados
ros::Publisher pub2;//variável que publica posição da bola

ros::Publisher pubIni; //variavel que publica vetor de posição dos robôs inimigos

//struct que contem a mensagem a ser postada no ROS
typedef struct {
	int x, y;
}pontos;
typedef struct {
	int num;//numero de robos detectados
	pontos robots[3], ball;
}topic_msg;
//define variavel global da struct acima
topic_msg objetos;

using namespace std;
using namespace cv;

const float pi = 3.141592;

int B, G, R;
int iB, iG, iR;
char caminhoIm[100];
pontos pnt[17];

//Le arquivos de pontos feito pelo setLine
void leLinhas(){
	FILE *campo = fopen(".pInteresse.ini","r");
	if(campo){
		for (int i = 0; i < 17; i ++){
			fscanf(campo,"%i %i\n",&pnt[i].x, &pnt[i].y);
		}
		fclose(campo);
		
	}
}

//remapeia/normaliza
float rMap(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//calcula o segundo ponto da linha
void CalcP2 (float x,float y,float *xp, float *yp,float ang, float l){
	ang = ang -3.14/2; //ajuste de 90º pra que a rotação na tela seja igual o visto no campo 
	*xp = x + l*cos(ang);
	*yp = y + l*sin(ang);
}
void objectPlacer(const std_msgs::String::ConstPtr& msg){
	//converte string pra struct
	char aux[sizeof(objetos)];
	char *num;
	sprintf(aux,"%s", msg->data.c_str());	
	//pega numero por numero da string e assicia ao lugar certo da struct
	//bola
	num = strtok(aux, " ");
	objetos.ball.x = atoi(num);
	num = strtok(NULL, " ");
	objetos.ball.y = atoi(num);
	//robo 1
	num = strtok(NULL, " ");
	objetos.robots[0].x = atoi(num);
	num = strtok(NULL, " ");
	objetos.robots[0].y = atoi(num);
	//robo 2
	num = strtok(NULL, " ");
	objetos.robots[1].x = atoi(num);
	num = strtok(NULL, " ");
	objetos.robots[1].y = atoi(num);
	//robo 3
	num = strtok(NULL, " ");
	objetos.robots[2].x = atoi(num);
	num = strtok(NULL, " ");
	objetos.robots[2].y = atoi(num);
}

/******************************************************************************************************************

Está função está lendo as coordenadas do ar_pose e postando na tela, além de inserir os dados em uma mensagem do ROS

*******************************************************************************************************************/
Mat campo = cvCreateImage(cvSize(600, 480), 8, 3);
	void processing (const ar_pose::ARMarkers::ConstPtr& msg){
		
		//Mat campo = imread(caminhoIm, CV_LOAD_IMAGE_COLOR);
		campo = cv::Scalar(0,0,0);
		int robNum = msg->markers.size();
		float x, y, z, w, ang;
		ar_pose::ARMarker ar_pose_marker;
		float X, Y, rot, xp, yp;
		char idRob[2];
		//desenha as linhas do campo
		for(int i = 0; i < 15; i++){
		line(campo, Point(pnt[i].x, pnt[i].y), Point(pnt[i+1].x, pnt[i+1].y), Scalar(160,160,160), 2, 8, 0);
		}

		line(campo, Point(pnt[15].x, pnt[15].y), Point(pnt[0].x, pnt[0].y), Scalar(160,160,160), 2, 8, 0);
		//desenha meio de campo
		circle(campo, Point(pnt[16].x, pnt[16].y), 4,  Scalar(160,160,160), 4, 8, 0 );
		circle(campo, Point(pnt[16].x, pnt[16].y), 50,  Scalar(160,160,160), 2, 8, 0 );

        //Declarção da variável que vair armazenar as posições na mensagem
        vss::pos robot, robotEnemy;
        vss::pos ball;
        //Declaração do array de mensagens de posição
        vss::pos_array array, arrayEnemy;

		ROS_INFO_STREAM("Número de aliados: " << robNum);
		
		//passa por todas as tags encontradas
		for(int i=0;i < robNum;i++){

			ar_pose_marker = msg->markers.at(i);

			x = ar_pose_marker.pose.pose.orientation.x;
			y = ar_pose_marker.pose.pose.orientation.y;
			z = ar_pose_marker.pose.pose.orientation.z;
			w = ar_pose_marker.pose.pose.orientation.w;	

			//calcula a rotação em radiandos no eixo z
			//o - 3.14/2 é só uma juste pro robo da imagem ficar orientado igual 
			rot = atan2((w*z + x*y),(1-2*(pow(y,2)+pow(z,2))));
			//salva ID na string pra escrever na tela
			sprintf(idRob,"%i",ar_pose_marker.id+1);
			//faz a escala de metros (ar_pose) pra pixels (imagem)
			X = rMap(ar_pose_marker.pose.pose.position.x, -1.0, 1.0, 0, 600);
			Y = rMap(ar_pose_marker.pose.pose.position.y, -0.8, 0.8, 0, 480);

			//armazena as posiçoes do robô na mensagem pos declarada como robot antes do início do for
			robot.num = (ar_pose_marker.id +1);
			robot.x = X + 30;
			robot.y = Y;
			//convertendo de radianos para graus
			rot = rot*180/pi;

			//conversão dos quadrantes do atan2
			if((rot <= -90) && (rot >= -180))
				robot.theta = rot*(-1) - 270;
			else
				robot.theta = 90 - rot;

			robot.theta*=(-1);
 
			//armazena no vetor de mensagem de posições todos os dados do robô (x,y,theta)
			array.component.push_back(robot);


            // pegando posicao dos inimigos
            robotEnemy.x = objetos.robots[i].x;
            robotEnemy.y = objetos.robots[i].y;
            // armazenando no vetor as posicoes inimigas
            arrayEnemy.component.push_back(robotEnemy);


				
			//desenha circulo com centro na posição do robo
			circle(campo, Point(X,Y), 12,  Scalar(B,G,R), 3, 0);
			//calcula segundo ponto (pra onde a o robo esta apontando)
			cout << rot << endl;			
			CalcP2 (X, Y, &xp, &yp, rot, 17);
			//desenha linha apontando pra orientação do robo
			line(campo, Point(X,Y), Point(xp,yp), Scalar(0,255,0), 3);		
			//escreve id do robo na tela
			putText(campo, idRob, Point(X-6,Y+5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 2);

		}

		//publica vetor de posição dos robôs aliados
        if(robNum > 0){
            pub.publish(array);
            pubIni.publish(arrayEnemy);

        }

		for (int i = 0; i < 3; i++){//desenha time inimigo

			circle(campo, Point(objetos.robots[i].x,objetos.robots[i].y), 8,  Scalar(iB,iG,iR), 20, 0);
		}

		//desenha bola
		circle(campo, Point(objetos.ball.x,objetos.ball.y), 3,  Scalar(0,69,255), 4, 0);

		//armazena posições da bola numa variavel mensagem
		ball.x = objetos.ball.x;
		ball.y = objetos.ball.y;

		pub2.publish(ball);
				
		//mostra imagem criada
		imshow ("Campo",campo);
		waitKey(3);
	}

/******************************************************************************************************************
fim processing()
*******************************************************************************************************************/	
	
int main (int argc, char** argv){
	
	// Initialize ROS
	//leLinhas();
	ros::init (argc, argv, "robot_show");
	ros::NodeHandle nh;
	//Le argumento
	if(!argv[1]){
		printf("Argumento faltante azul/amarelo\n");
		return 0;
	}
	else if (!strcmp("azul",argv[1])){
		R = 0;
		G = 0;
		B = 255;
		iR = 255;
		iG = 255;
		iB = 0;
	}
	else if (!strcmp("amarelo",argv[1])){
		R = 255;
		G = 255;
		B = 0;
		iR = 0;
		iG = 0;
		iB = 255;
	}
	else {
		printf("Argumento invalido azul/amarelo\n");
		return 0;
	}
	
	//escreve caminho da imagem de fundo
	//sprintf(caminhoIm,"%s/campo.jpg", argv[0]);
	//printf("%s\n",caminhoIm);
	namedWindow("Campo");
	// Create a ROS subscriber
	ros::Subscriber sub = nh.subscribe ("ar_pose_marker", 20, processing);
	ros::Subscriber sub2 = nh.subscribe("detectedObjects", 1000, objectPlacer);

	pub = nh.advertise<vss::pos_array>("robots_position", 1000);
	pub2 = nh.advertise<vss::pos>("ball_position", 1000);

    pubIni = nh.advertise<vss::pos_array>("enemies_position", 1000);

	ros::spin ();
}
