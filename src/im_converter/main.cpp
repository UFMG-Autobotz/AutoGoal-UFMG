#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <string.h>
#include <iostream>

#define blueP ".blueParam.ini"
#define yellowP ".yellowParam.ini"

using namespace cv;
using namespace std;
using namespace cv_bridge;

/**********************************

- SIMULADOR -

**** AZUL ****
H_min = 66        S_min = 63            V_min = 123
H_max = 146       S_max = 198           V_max = 180


**** AMARELO ****
H_min = 12       S_min = 82           V_min = 68
H_max = 38       S_max = 139          V_max = 255


- REAL -

**** AZUL ****
H_min =        S_min =             V_min =
H_max =        S_max =           V_max =


**** AMARELO ****
H_min =        S_min =            V_min =
H_max =        S_max =           V_max =

/********************************/

int H_MIN;
int H_MAX;
int S_MIN;
int S_MAX;
int V_MIN;
int V_MAX;
int ONOFF = 0; //essa variavel desliga todos os trackbars e a imagem tratada (pra ficar mais leve depois de já ter calibrado)
bool DestroyWind = 0;
bool teamcolor; //0 = amarelo 1 =  azul
cv_bridge::CvImagePtr cv_ptr;

void carregaParam(char *filename){
    if(teamcolor) ROS_INFO("Time Azul\n");
    else ROS_INFO("Time Amarelo\n");
    FILE *entrada;
    if(!(entrada = fopen(filename,"r"))){//Caso o arquivo não exista, ele cria um com valores padrao
        ROS_INFO("Arquivo de paramestros não encontrado\n");
        entrada = fopen(filename,"w");

        fprintf(entrada,"%i %i\n",0,255);
        fprintf(entrada,"%i %i\n",0,255);
        fprintf(entrada,"%i %i\n",0,255);
        fclose(entrada);//parametros recebem valor padrão
        H_MIN = 0; H_MAX = 255;
        S_MIN = 0; S_MAX = 255;
        V_MIN = 0; V_MAX = 255;
        if(!entrada){
            ROS_INFO("Arquivo de parametros não criado, impossivel salvar parametros\n");
            perror("");
        }
        else ROS_INFO("Arquivo de parametros criado\n");
        ROS_INFO("H -> %.3i %.3i\n",H_MIN,H_MAX);
        ROS_INFO("S -> %.3i %.3i\n",S_MIN,S_MAX);
        ROS_INFO("V -> %.3i %.3i\n",V_MIN,V_MAX);
    }
    else{ //carrega parametros lidos no arquivo


        fscanf(entrada,"%i %i\n", &H_MIN, &H_MAX);
        fscanf(entrada,"%i %i\n", &S_MIN, &S_MAX);
        fscanf(entrada,"%i %i\n", &V_MIN, &V_MAX);
        fclose(entrada);
        ROS_INFO("Leitura de parametros completa!\n");
        ROS_INFO("H -> %.3i %.3i\n",H_MIN,H_MAX);
        ROS_INFO("S -> %.3i %.3i\n",S_MIN,S_MAX);
        ROS_INFO("V -> %.3i %.3i\n",V_MIN,V_MAX);
    }
    ROS_INFO("-------------------------------------\n");
}

void salvaParam(char *filename){
    FILE *saida = fopen(filename,"w");
    if(saida){
        fprintf(saida,"%i %i\n",H_MIN,H_MAX);
        fprintf(saida,"%i %i\n",S_MIN,S_MAX);
        fprintf(saida,"%i %i\n",V_MIN,V_MAX);
        ROS_INFO("Parametros salvos:\n");
        ROS_INFO("H -> %.3i %.3i\n",H_MIN,H_MAX);
        ROS_INFO("S -> %.3i %.3i\n",S_MIN,S_MAX);
        ROS_INFO("V -> %.3i %.3i\n",V_MIN,V_MAX);
    }
    else{
        ROS_INFO("Erro: impossivel salvar parametros\n");
        perror("");
    }
}

void color_track (cv_bridge::CvImagePtr image){

	//Matrix to store each frame of the webcam feed

//resolução 640 x 480

	cvtColor(image->image,image->image,COLOR_BGR2HSV);

  int n = image->image.cols;
  int m = image->image.rows;
    for (int j = 0; j<m; j++){
      for (int i=0; i<n; i++) {
        if ( image->image.at<cv::Vec3b>(j,i)[0] >= H_MIN && image->image.at<cv::Vec3b>(j,i)[0]<= H_MAX && image->image.at<cv::Vec3b>(j,i)[1] >= S_MIN && image->image.at<cv::Vec3b>(j,i)[1] <= S_MAX && image->image.at<cv::Vec3b>(j,i)[2] >= V_MIN && image->image.at<cv::Vec3b>(j,i)[2] <= V_MAX){
                image->image.at<cv::Vec3b>(j,i)[0]= 0;
                image->image.at<cv::Vec3b>(j,i)[1]= 0;
                image->image.at<cv::Vec3b>(j,i)[2]= 0;
        }
        else{
          image->image.at<cv::Vec3b>(j,i)[0]= 255;
          image->image.at<cv::Vec3b>(j,i)[1]= 255;
          image->image.at<cv::Vec3b>(j,i)[2]= 255;
        }
    }
  }

}

static const std::string OPENCV_WINDOW = "Image window";
class ImageConverter{

  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("camera/image_raw", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("camera/team", 1);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    //------------------------------------------------------------------------//
    //imshow("image", cv_ptr->image);
    color_track(cv_ptr);

   //Create trackbars in "Control" window
    namedWindow("Alies", CV_WINDOW_AUTOSIZE); //cria uma nova jane
    cvCreateTrackbar("Config On/Off","Alies", &ONOFF, 1);
    if(ONOFF){
        cvCreateTrackbar("Hmin", "Alies", &H_MIN, 254); //Hue (0 - 255)
        cvCreateTrackbar("Hmax", "Alies", &H_MAX, 255); //Hue (0 - 255)
        cvCreateTrackbar("Smin", "Alies", &S_MIN, 254); //Saturation (0 - 255
        cvCreateTrackbar("Smax", "Alies", &S_MAX, 255); //Saturation (0 - 255)
        cvCreateTrackbar("Vmin", "Alies", &V_MIN, 254); //Value (0 - 255)
        cvCreateTrackbar("Vmax", "Alies", &V_MAX, 255); //Value (0 - 255
        cv::imshow("Alies", cv_ptr->image);
        DestroyWind = 1;
        //nunca deixa o max ser menor que o min
        if (H_MAX < H_MIN) H_MAX = H_MIN + 1;
        if (S_MAX < S_MIN) S_MAX = S_MIN + 1;
        if (V_MAX < V_MIN) V_MAX = V_MIN + 1;
    }
    else{
        if(DestroyWind){
            destroyAllWindows();//fecha a janela
            //salva parametros no arquivo do time
            if(teamcolor) salvaParam(blueP);
            else salvaParam(yellowP);
        }
        DestroyWind = 0;//só distroi a janela 1 vez, se não n da pra ligar denovo as configurações
    }

    //------------------------------------------------------------------------//

    // Update GUI Window
    cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv){

    //aqui o codigo ve se existe um parametro pra cor do time atual no jogo
    //se nao existir parametro ou o parametro for diferente de 'amarelo' ou 'azul
    //o programa fecha
    if(!argv[1]){
        ROS_INFO("Erro: Parametro faltante <cor do time>\n");
        return 0;
    }
    else if (!strcmp("amarelo", argv[1])) teamcolor = 0;
    else if (!strcmp("azul", argv[1])) teamcolor = 1;
    else {
        ROS_INFO("Erro: Parametro incorreto azul/amarelo\n");
        return 0;
    }

    //carrega parametros salvos pro time em questao
          if(teamcolor) carregaParam(blueP);
          else carregaParam(yellowP);


          ros::init(argc, argv, "image_converter");
          ImageConverter ic;
            ros::spin();
          return 0;
}
