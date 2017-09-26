#include <ros/ros.h>
#include <sstream>
#include <string>
#include <iostream>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "robot.hpp"
#include <vector>

#include <std_msgs/String.h>

using namespace cv;
using namespace cv_bridge;
using namespace std;

#define blueP ".blueParam.ini"
#define yellowP ".yellowParam.ini"
#define orangeP ".orangeParam.ini"

//struct que contem a mensagem a ser postada no ROS
typedef struct{
	int x, y;
}coordenada;
typedef struct {
	int num;//numero de robos detectados
	coordenada robots[3], ball;
}topic_msg;
//define variavel global da struct acima
topic_msg pontos;

//variaveis auxiliares
bool DestroyWind = 0;
int ONOFF = 0;
bool teamcolor = 0;
char aux[sizeof(pontos)];
//cv bridge
	cv_bridge::CvImagePtr cv_ptr;
//vectors for poses
	vector<Point> enemy,ball;
//hsv values for nonCalibrationMode
int H_MIN_Enemy;
int H_MAX_Enemy;
int S_MIN_Enemy;
int S_MAX_Enemy;
int V_MIN_Enemy;
int V_MAX_Enemy;

int H_MIN_Ball = 0;
int H_MAX_Ball = 19;
int S_MIN_Ball = 149;
int S_MAX_Ball = 237;
int V_MIN_Ball = 32;
int V_MAX_Ball = 255;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=5;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;


void carregaParam(char *filename){
    if(teamcolor) ROS_INFO("Time inimigo amarelo");
    else ROS_INFO("Time inimigo azul");
    FILE *entrada; 
    FILE *entrada2;
    if(!(entrada = fopen(filename,"r"))){//Caso o arquivo não exista, ele cria um com valores padrao
        ROS_INFO("Arquivo de parametros não encontrado");
        entrada = fopen(filename,"w");

        fprintf(entrada,"%i %i\n",0,255);
        fprintf(entrada,"%i %i\n",0,255);
        fprintf(entrada,"%i %i\n",0,255);
        fclose(entrada);//parametros recebem valor padrão
        H_MIN_Enemy = 0; H_MAX_Enemy = 255;
        S_MIN_Enemy = 0; S_MAX_Enemy = 255;
        V_MIN_Enemy = 0; V_MAX_Enemy = 255;
        if(!entrada){
            ROS_INFO("Arquivo de parametros não criado, impossivel salvar parametros\n");
            perror("");
        }
        else ROS_INFO("Arquivo de parametros criado");
        ROS_INFO("H -> %.3i %.3i\n",H_MIN_Enemy,H_MAX_Enemy);
        ROS_INFO("S -> %.3i %.3i\n",S_MIN_Enemy,S_MAX_Enemy);
        ROS_INFO("V -> %.3i %.3i\n",V_MIN_Enemy,V_MAX_Enemy);
    }
    else{ //carrega parametros lidos no arquivo


        fscanf(entrada,"%i %i\n", &H_MIN_Enemy, &H_MAX_Enemy);
        fscanf(entrada,"%i %i\n", &S_MIN_Enemy, &S_MAX_Enemy);
        fscanf(entrada,"%i %i\n", &V_MIN_Enemy, &V_MAX_Enemy);
        fclose(entrada);
        ROS_INFO("Leitura de parametros completa!");
        ROS_INFO("H -> %.3i %.3i",H_MIN_Enemy,H_MAX_Enemy);
        ROS_INFO("S -> %.3i %.3i",S_MIN_Enemy,S_MAX_Enemy);
        ROS_INFO("V -> %.3i %.3i",V_MIN_Enemy,V_MAX_Enemy);
    }
	ROS_INFO("-----------------Laranja--------------------");
	if(!(entrada2 = fopen(orangeP,"r"))){//Caso o arquivo não exista, ele cria um com valores padrao
		ROS_INFO("Arquivo de parametros não encontrado");
		entrada2 = fopen(orangeP,"w");
		//salva valores padrao no arquivo
		fprintf(entrada2,"%i %i\n",H_MAX_Ball,H_MIN_Ball);
		fprintf(entrada2,"%i %i\n",S_MAX_Ball,S_MIN_Ball);
		fprintf(entrada2,"%i %i\n",V_MAX_Ball,V_MIN_Ball);
		fclose(entrada);//parametros recebem valor padrão
		if(!entrada){
		    ROS_INFO("Arquivo de parametros não criado, impossivel salvar parametros\n");
		    perror("");
        }
        else ROS_INFO("Arquivo de parametros criado");
        ROS_INFO("H -> %.3i %.3i\n",H_MIN_Ball,H_MAX_Ball);
        ROS_INFO("S -> %.3i %.3i\n",S_MIN_Ball,S_MAX_Ball);
        ROS_INFO("V -> %.3i %.3i\n",V_MIN_Ball,V_MAX_Ball);
    }
    else{ //carrega parametros lidos no arquivo


        fscanf(entrada2,"%i %i\n", &H_MIN_Ball, &H_MAX_Ball);
        fscanf(entrada2,"%i %i\n", &S_MIN_Ball, &S_MAX_Ball);
        fscanf(entrada2,"%i %i\n", &V_MIN_Ball, &V_MAX_Ball);
        fclose(entrada2);
        ROS_INFO("Leitura de parametros completa!");
        ROS_INFO("H -> %.3i %.3i",H_MIN_Ball,H_MAX_Ball);
        ROS_INFO("S -> %.3i %.3i",S_MIN_Ball,S_MAX_Ball);
        ROS_INFO("V -> %.3i %.3i",V_MIN_Ball,V_MAX_Ball);
    }

}

void salvaParam(char* filename){
    FILE *saida = fopen(filename,"w");
    FILE *saida2= fopen(orangeP,"w");
    if(saida && saida2){
        fprintf(saida,"%i %i\n",H_MIN_Enemy,H_MAX_Enemy);
        fprintf(saida,"%i %i\n",S_MIN_Enemy,S_MAX_Enemy);
        fprintf(saida,"%i %i\n",V_MIN_Enemy,V_MAX_Enemy);
	fprintf(saida2,"%i %i\n",H_MIN_Ball,H_MAX_Ball);
        fprintf(saida2,"%i %i\n",S_MIN_Ball,S_MAX_Ball);
        fprintf(saida2,"%i %i\n",V_MIN_Ball,V_MAX_Ball);
        ROS_INFO("Parametros salvos:");
        ROS_INFO("H -> %.3i %.3i",H_MIN_Enemy,H_MAX_Enemy);
        ROS_INFO("S -> %.3i %.3i",S_MIN_Enemy,S_MAX_Enemy);
        ROS_INFO("V -> %.3i %.3i",V_MIN_Enemy,V_MAX_Enemy);
	ROS_INFO("laranja");
	ROS_INFO("H -> %.3i %.3i",H_MIN_Ball,H_MAX_Ball);
        ROS_INFO("S -> %.3i %.3i",S_MIN_Ball,S_MAX_Ball);
        ROS_INFO("V -> %.3i %.3i",V_MIN_Ball,V_MAX_Ball);
    }
    else{
        ROS_INFO("Erro: impossivel salvar parametros\n");
        perror("");
    }
}



string intToString(int number){

    std::stringstream ss;
    ss << number;
    return ss.str();
}
void createTrackbars(char *janela, int *H_MIN, int *H_MAX, int *S_MIN,int *S_MAX,int *V_MIN,int *V_MAX){
	//Create trackbars in "Control" window
    namedWindow(janela, CV_WINDOW_AUTOSIZE); //cria uma nova jane
    cvCreateTrackbar("Config On/Off",janela, &ONOFF, 1);
    if(ONOFF){
        cvCreateTrackbar("Hmin", janela, H_MIN, 254); //Hue (0 - 255)
        cvCreateTrackbar("Hmax", janela, H_MAX, 255); //Hue (0 - 255)
        cvCreateTrackbar("Smin", janela, S_MIN, 254); //Saturation (0 - 255
        cvCreateTrackbar("Smax", janela, S_MAX, 255); //Saturation (0 - 255)
        cvCreateTrackbar("Vmin", janela, V_MIN, 254); //Value (0 - 255)
        cvCreateTrackbar("Vmax", janela, V_MAX, 255); //Value (0 - 255
        cv::imshow(janela, cv_ptr->image);
        DestroyWind = 1;
        //nunca deixa o max ser menor que o min
        if (*H_MAX < *H_MIN) *H_MAX = *H_MIN + 1;
        if (*S_MAX < *S_MIN) *S_MAX = *S_MIN + 1;
        if (*V_MAX < *V_MIN) *V_MAX = *V_MIN + 1;
    }
    else{
        if(DestroyWind){
            destroyAllWindows();//fecha a janela
            //salva parametros no arquivo do time         	
			if(teamcolor) salvaParam(yellowP);
		    	else salvaParam(blueP);
		DestroyWind = 0;//só distroi a janela 1 vez, se não n da pra ligar denovo as configurações
        }
    }
}


void drawObject(vector<robot> therobots,Mat &frame, bool robo){
	pontos.num = therobots.size();
    for(int i=0;i<therobots.size() && i < 3;i++){
        cv::circle(frame,cv::Point(therobots.at(i).getxpos(),therobots.at(i).getypos()),10,cv::Scalar(0,0,255));
        cv::putText(frame,intToString(therobots.at(i).getxpos())+ " , " + intToString(therobots.at(i).getypos()),cv::Point(therobots.at(i).getxpos(),therobots.at(i).getypos()+20),1,1,Scalar(0,255,0));
	if (robo){
		pontos.robots[i].x = therobots.at(i).getxpos();
		pontos.robots[i].y = therobots.at(i).getypos();
	}
	else{
		pontos.ball.x = therobots.at(i).getxpos();
		pontos.ball.y = therobots.at(i).getypos();	
	}
    }
}
void morphOps(Mat &thresh){

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

    erode(thresh,thresh,erodeElement);
    erode(thresh,thresh,erodeElement);


    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);



}
vector<Point> trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed, bool robo){

    int x,y;

    vector<robot> robots;

    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Point> poses;
    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA){
                    //x = moment.m10/area;
                    //y = moment.m01/area;

                    robot rob;

                    rob.setxpos(moment.m10/area);
                    rob.setypos(moment.m01/area);

                    robots.push_back(rob);

                    objectFound = true;

                }else objectFound = false;


            }
            //let user know you found an object
            if(objectFound ==true){
                //draw object location on screen
                drawObject(robots,cameraFeed, robo);}

        }else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
    }
    for(int i=0;i<robots.size();i++){
        poses.push_back(Point(robots.at(i).getxpos(),robots.at(i).getypos()));
    }
    return poses;
}

vector<Point> enemy_track(Mat threshold,Mat HSV, Mat &cameraFeed){
    int i;
    vector<Point> enemyPose;
    enemyPose = trackFilteredObject(threshold,HSV,cameraFeed, 1);

    return enemyPose;
}
vector<Point> ball_track(Mat threshold,Mat HSV, Mat &cameraFeed){

    vector<Point> ballPose;
    ballPose = trackFilteredObject(threshold,HSV,cameraFeed, 0);
	//salva posições dos robos que tem, se houverem menos do que 3, coloca o resto em 0
    return ballPose;
}

//-------------funcao principal--------------/
void detectColor (Mat cameraFeedIn){
    int i;
	
    //Matrix to store each frame of the webcam feed
    Mat threshold1,threshold2;
    Mat HSV1,HSV2, cameraFeed;

    /*__________________undistort funcion: retifica a imagem da camera para compensar a distorção da lente__________________*/

    Mat cameraMatrix = (Mat_<double>(3,3) << 695.720371404826, 0, 333.610999947472, 0, 688.534666038923, 251.819584533216, 0, 0, 1);
    Mat distCoeff = (Mat_<double>(1,5) << -0.437435103257912, 0.345063363726242, -0.00353207577450334, -0.00310674166181843, 0);

    undistort(cameraFeedIn, cameraFeed, cameraMatrix,distCoeff,cameraMatrix);
    /*______________________________________________undistorted______________________________________________________________*/


            //Cria trackbars
		createTrackbars("Enemy", &H_MIN_Enemy, &H_MAX_Enemy, &S_MIN_Enemy, &S_MAX_Enemy, &V_MIN_Enemy, &V_MAX_Enemy);
		createTrackbars("Ball" , &H_MIN_Ball, &H_MAX_Ball, &S_MIN_Ball, &S_MAX_Ball, &V_MIN_Ball, &V_MAX_Ball);
		
            //convert frame from BGR to HSV colorspace
            cvtColor(cameraFeed,HSV1,COLOR_BGR2HSV);
            inRange(HSV1,Scalar(H_MIN_Enemy,S_MIN_Enemy,V_MIN_Enemy),Scalar(H_MAX_Enemy,S_MAX_Enemy,V_MAX_Enemy),threshold1);
	    if (ONOFF) imshow("Enemy",threshold1);
            morphOps(threshold1);
            

            //balltrack 
            cvtColor(cameraFeed,HSV2,COLOR_BGR2HSV);
            inRange(HSV2,Scalar(H_MIN_Ball,S_MIN_Ball,V_MIN_Ball),Scalar(H_MAX_Ball,S_MAX_Ball,V_MAX_Ball),threshold2);
	    if(ONOFF)imshow("Ball",threshold2);
            morphOps(threshold2);
            

            //trackFilteredObject
            enemy = enemy_track(threshold1,HSV1,cameraFeed);
            ball = ball_track(threshold2,HSV2,cameraFeed);
            //cout<<"enemy: ( "<<enemy<<" )"<<endl;
            //cout<<"ball: ( "<<ball<<" )"<<endl;
			

        //show frames

        //imshow("final",cameraFeed);


        //delay 30ms so that screen can refresh.
        //image will not appear without this waitKey() command
        waitKey(30);
    
}

class ImageConverter{
public:	
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  ros::Publisher detectObjects_pub;


  ImageConverter()
    : it_(nh_){
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("camera/image_raw", 1, &ImageConverter::imageCb, this);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg){
    std_msgs::String vetor;
    std::stringstream ss;
    try{
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e){
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    //------------------------------------------------------------------------//
	detectColor (cv_ptr->image);
	//converte struct pra string
	
	sprintf(aux,"%.3i %.3i %.3i %.3i %.3i %.3i %.3i %.3i",
		pontos.ball.x,
		pontos.ball.y,
		pontos.robots[0].x,
		pontos.robots[0].y,
		pontos.robots[1].x,
		pontos.robots[1].y,
		pontos.robots[2].x,
		pontos.robots[2].y
	);
	ss << aux;
	vetor.data=ss.str();
	//publica string
	detectObjects_pub.publish(vetor);
    //------------------------------------------------------------------------//

    // Update GUI Window
    cv::waitKey(3);
  }
};


int main(int argc, char* argv[]){
	if(!argv[1]) printf("Erro: parametro faltante: cor do time\n");
	else if (!strcmp("azul",argv[1])) teamcolor++;
		
		if(teamcolor)carregaParam(yellowP);
		else carregaParam(blueP);

	ros::init(argc, argv, "detectObjects");	
	ImageConverter ic;
	ic.detectObjects_pub = ic.nh_.advertise<std_msgs::String>("detectedObjects", 30);
	ros::spin();

return 0;
}
