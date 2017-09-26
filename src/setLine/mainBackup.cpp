#include "color_track.hpp"

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
Mat image_rgb;
ros::Subscriber color_sub;
cv_bridge::CvImagePtr cv_ptr;
int calibraBranco = 0; //fica em 1 quando esta calibrando as cores
int DestroyWind = 0;
ThresholdLimits orangeBall, enemyTeam, lines;
int thresh = 100;
//----------funções----------//
void criaTrackbar(char *windowName, ThresholdLimits *l, Mat imagem,int *ONOFF){
   //Cria janela
    namedWindow(windowName, CV_WINDOW_AUTOSIZE);
   //Cria trackbar de configuração
    cvCreateTrackbar("Config On/Off",windowName, ONOFF, 1);
    if(*ONOFF){
        cvCreateTrackbar("Hmin", windowName, &l->H_MIN, 254); //Hue (0 - 255)
        cvCreateTrackbar("Hmax", windowName, &l->H_MAX, 255); 
        cvCreateTrackbar("Smin", windowName, &l->S_MIN, 254); //Saturation (0 - 255
        cvCreateTrackbar("Smax", windowName, &l->S_MAX, 255);
        cvCreateTrackbar("Vmin", windowName, &l->V_MIN, 254); //Value (0 - 255)
        cvCreateTrackbar("Vmax", windowName, &l->V_MAX, 255); 
        //imshow(windowName,imagem);
        DestroyWind = 1;
        //nunca deixa o max ser menor que o min
        if (l->H_MAX < l->H_MIN) l->H_MAX = l->H_MIN + 1;
        if (l->S_MAX < l->S_MIN) l->S_MAX = l->S_MIN + 1;
        if (l->V_MAX < l->V_MIN) l->V_MAX = l->V_MIN + 1;
    }
    else{
        if(DestroyWind){
            destroyAllWindows();//fecha a janela
        }
        DestroyWind = 0;//só distroi a janela 1 vez, se não n da pra ligar denovo as configurações
    }

}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if ( event == EVENT_MOUSEMOVE )
     {
          cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

     }
}

void detectaLinhas(Mat campo){
	//cria janela
		namedWindow("linha do campo", CV_WINDOW_AUTOSIZE);
	//set the callback function for any mouse event
		setMouseCallback("linha do campo", CallBackFunc, NULL);
	//mostra imagem na janela
		imshow("linha do campo", campo);
               //circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
}

void colorCallback(const sensor_msgs::ImageConstPtr& image){
	sensor_msgs::Image ros_image;
	Mat HSV;	


	try{
		cv_ptr = cv_bridge::toCvCopy(image, "bgr8");
	}
	catch (cv_bridge::Exception& e){
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}

    image_rgb = cv_ptr->image;
    //processa imagem	
	cvtColor(cv_ptr->image,HSV,COLOR_BGR2HSV);//converte pra HSV
	detectaLinhas(HSV); //detecta linhas de campo


	cv::waitKey(3);
}


int main(int argc, char **argv) {

	ros::init(argc, argv, "visao");
	ros::NodeHandle node;
    	image_transport::ImageTransport it(node);

	color_sub = node.subscribe<sensor_msgs::Image>("camera/image_raw", 1 , colorCallback);

	ros::spin();

}
