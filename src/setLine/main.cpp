#include <stdio.h>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

typedef struct {
	int x, y;
}ponto;

typedef struct{
	int num;
	ponto pos[17];
}pontos;

typedef struct {
	int H_MIN;
	int H_MAX;
	int S_MIN;
	int S_MAX;
	int V_MIN;
	int V_MAX;
} ThresholdLimits; //variavel pra guardar limites de threshold pras cores

using namespace cv;
using namespace std;
using namespace cv_bridge;
//----------variaveis----------//
ros::Subscriber color_sub;
cv_bridge::CvImagePtr cv_ptr;
pontos pInteresse;

//----------funções----------//
void escreveArquivo(){
	FILE *saida = fopen (".pInteresse.ini", "w");
	for (int i = 0; i < 17; i++){
		fprintf(saida,"%i %i\n", pInteresse.pos[i].x, pInteresse.pos[i].y);
	}
	cout << "Pontos salvos" << endl;
	exit (0);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
	if  ( event == EVENT_LBUTTONDOWN ){//quando ha evento de mouse, salva a posição num vetor
		if(pInteresse.num < 17){
			pInteresse.pos[pInteresse.num].x = x;
			pInteresse.pos[pInteresse.num].y = y;
			pInteresse.num++;
		}
		cout << "Evento de mouse detectado: "<< x << " " << y << endl;
	}
	else if  ( event == EVENT_RBUTTONDOWN ){
          if(pInteresse.num  < 16) cout << "Termine de marcar antes de salvar o arquivo"<< endl;
	  else escreveArquivo();
     }
}

void colorCallback(const sensor_msgs::ImageConstPtr& image){
	try{
		cv_ptr = cv_bridge::toCvCopy(image, "bgr8");
	}
	catch (cv_bridge::Exception& e){
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}	
    /*__________________undistort funcion: retifica a imagem da camera para compensar a distorção da lente__________________*/
	Mat output;
    Mat cameraMatrix = (Mat_<double>(3,3) << 695.720371404826, 0, 333.610999947472, 0, 688.534666038923, 251.819584533216, 0, 0, 1);
    Mat distCoeff = (Mat_<double>(1,5) << -0.437435103257912, 0.345063363726242, -0.00353207577450334, -0.00310674166181843, 0);

    undistort(cv_ptr->image,output, cameraMatrix,distCoeff,cameraMatrix);
    /*______________________________________________undistorted______________________________________________________________*/

	//desenha circulos na imagem
		for(int i = 0; i < pInteresse.num; i++) 
		circle(output, Point( pInteresse.pos[i].x, pInteresse.pos[i].y ), 5,  Scalar(255,100,255), 2, 8, 0 );
		if (pInteresse.pos[16].x){
			circle(output, Point( pInteresse.pos[16].x, pInteresse.pos[16].y ), 2,  Scalar(255,100,255), 2, 8, 0 );
			circle(output, Point( pInteresse.pos[16].x, pInteresse.pos[16].y ), 50,  Scalar(255,100,255), 2, 8, 0 );
		}
	//Escreve na imagem

	//Mostra a imagem com os circulos
		imshow("Linhas de campo",output);
	cv::waitKey(3);
}


int main(int argc, char **argv) {
	pInteresse.num = 0;
	//cria janela
		namedWindow("Linhas de campo", CV_WINDOW_AUTOSIZE);
	//set the callback function for any mouse event
		setMouseCallback("Linhas de campo", CallBackFunc, NULL);
	ros::init(argc, argv, "Linhas_de_campo");
	ros::NodeHandle node;
    	image_transport::ImageTransport it(node);
	
	color_sub = node.subscribe<sensor_msgs::Image>("camera/image_raw", 1 , colorCallback);

	ros::spin();

}
