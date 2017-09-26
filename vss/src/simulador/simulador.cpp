/*
Simulador do futebol de robôs VSS
Equipe de Robótica Autobotz
Universidade Federal de Minas Gerais
Desenvolvido por Gustavo Domingues e Ramon Duraes
*/


#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <ros/ros.h>
#include "../include/v_repConst.h"
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Float32.h>

// Used data structures:
#include "vrep_common/VrepInfo.h"
#include "vrep_common/JointSetStateData.h"


// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"
#include "vrep_common/simRosGetObjectPose.h"

// Classe
#include "carro.h"


#define MAX_SPEED 52

// Global variables
bool simulationRunning=true;
float simulationTime=0.0f;

// objeto
Carro carro, caster;


// controle



double thetad = 1;
double thetad_caster;
double xd =0;
double yd =0;

int estado=0;	

double distancia;
double erroy;
double errox;
double errotheta;

int caminho_index = 0;

double rot;
int contador=0;

float desiredLeftMotorSpeed;
float desiredRightMotorSpeed;	

double errox_virtual, erroy_virtual, errox_real, erroy_real, b_reta, x_virtual;

// controle


/*
// callback da posicao do caster
void poseMessageReceived2 (const geometry_msgs::PoseStamped& msg){

    caster.pX=msg.pose.position.x;
    caster.pY=msg.pose.position.y;
    caster.pZ=msg.pose.position.z;
    
    caster.oX=msg.pose.orientation.x;
    caster.oY=msg.pose.orientation.y;
    caster.oZ=msg.pose.orientation.z;
    caster.oW=msg.pose.orientation.w;  
    
    //thetad_caster 
    //rot = atan2(carro.pY-caster.pY,carro.pX-caster.pX);
    
     ROS_INFO_STREAM(std::setprecision(5) << std::fixed << "\n### CASTER " << "position=(" << caster.pX << "," << caster.pY << "," << caster.pZ << ")" << "\norientation=(" << caster.oX << "," << caster.oY << "," << caster.oZ << "," << caster.oW << ")" << " Angulo: " << thetad_caster << "###\n");

}*/


// callback da traducao da orientacao
void poseMessageReceived2 (const std_msgs::Float32& msg){

    rot = msg.data;

    //thetad_caster
    //rot = atan2(carro.pY-caster.pY,carro.pX-caster.pX);

     //ROS_INFO_STREAM(std::setprecision(5) << std::fixed << "\n### CASTER " << "position=(" << caster.pX << "," << caster.pY << "," << caster.pZ << ")" << "\norientation=(" << caster.oX << "," << caster.oY << "," << caster.oZ << "," << caster.oW << ")" << " Angulo: " << thetad_caster << "###\n");

}


// callback da posicao do carrinho
void poseMessageReceived (const geometry_msgs::PoseStamped& msg){
     

    // testando as mensagens com posicao dos carros    
    carro.pX=msg.pose.position.x;
    carro.pY=msg.pose.position.y;
    carro.pZ=msg.pose.position.z;
    
    /*carro.oX=msg.pose.orientation.x;
    carro.oY=msg.pose.orientation.y;
    carro.oZ=msg.pose.orientation.z;
    carro.oW=msg.pose.orientation.w;    */
        
    //rot = atan2((carro.oW*carro.oZ + carro.oX*carro.oY),(1-2*(pow(carro.oY,2)+pow(carro.oZ,2))));   
         
        
    // controle    
    //thetad = atan2(carro.pY-yd,carro.pX-xd);
	
    //errox = xd - carro.pX;
    //erroy = yd - carro.pY;
	
    //errotheta = thetad - rot;
    
    //distancia = sqrt(pow(errox,2) + pow(erroy,2));
    
    contador++;
    // controle  
        
     
    
    ROS_INFO_STREAM(std::setprecision(5) << std::fixed << "position=(" << carro.pX << "," << carro.pY << "," << carro.pZ << ")" << "\norientation=(" 
    	<< carro.oX << "," << carro.oY << "," << carro.oZ << "," << carro.oW << ")\n"  << " Angulos: " << rot << "," << thetad << " || erros: " 
    << errox << "," << erroy << "," << errotheta <<  " , " << caminho_index << "\n" << desiredLeftMotorSpeed << ", " << desiredRightMotorSpeed 
    << "\n" << errox_virtual << ", " << erroy_virtual << '\n' );
 	

}


// Topic subscriber callbacks:
void infoCallback(const vrep_common::VrepInfo::ConstPtr& info)
{
	simulationTime=info->simulationTime.data;
	simulationRunning=(info->simulatorState.data&1)!=0;
}

//calcula modulo
double mod(double &x)
{
	if(x > 0)
		return x;
	else
		return x*(-1);
}


// Main code:
int main(int argc,char* argv[])
{


	// The joint handles are given in the argument list
	// (when V-REP launches this executable, V-REP will also provide the argument list)
	int leftMotorHandle;
	int rightMotorHandle;
    
    // controle    
    estado=0;
    // controle
	
	if (argc>=3)
	{
		leftMotorHandle=atoi(argv[1]);
		rightMotorHandle=atoi(argv[2]);
		
	}
	else
	{
		printf("Indicate following arguments: 'leftMotorHandle rightMotorHandle'!\n");
		sleep(5000);
		return 0;
	}

	// Create a ROS node. The name has a random component: 
	int _argc = 0;
	char** _argv = NULL;
	struct timeval tv;
	unsigned int timeVal=0;
	if (gettimeofday(&tv,NULL)==0)
		timeVal=(tv.tv_sec*1000+tv.tv_usec/1000)&0x00ffffff;
	std::string nodeName("vss");
	std::string randId(boost::lexical_cast<std::string>(timeVal+int(999999.0f*(rand()/(float)RAND_MAX))));
	nodeName+=randId;		
	ros::init(_argc,_argv,nodeName.c_str());


	if(!ros::master::check())
		return(0);
	
	ros::NodeHandle node("~");	
	printf("VSS cabuloso simulator just started with node name %s\n",nodeName.c_str());

	// 1. Let's subscribe to V-REP's info stream (that stream is the only one enabled by default,
	// and the only one that can run while no simulation is running):
	ros::Subscriber subInfo=node.subscribe("/vrep/info",1,infoCallback);
	

		// 4. Let's tell V-REP to subscribe to the motor speed topic (publisher to that topic will be created further down):
		ros::ServiceClient client_enableSubscriber=node.serviceClient<vrep_common::simRosEnableSubscriber>("/vrep/simRosEnableSubscriber");
		vrep_common::simRosEnableSubscriber srv_enableSubscriber;

		srv_enableSubscriber.request.topicName="/"+nodeName+"/wheels"; // the topic name
		srv_enableSubscriber.request.queueSize=1; // the subscriber queue size (on V-REP side)
		srv_enableSubscriber.request.streamCmd=simros_strmcmd_set_joint_state; // the subscriber type
		

		
		if ( client_enableSubscriber.call(srv_enableSubscriber)&&(srv_enableSubscriber.response.subscriberID!=-1) )
		{	// ok, the service call was ok, and the subscriber was succesfully started on V-REP side
			// V-REP is now listening to the desired motor joint states

			// 5. Let's prepare a publisher of those motor speeds:
			ros::Publisher motorSpeedPub=node.advertise<vrep_common::JointSetStateData>("wheels",1);

			// 6. Finally we have the control loop:
	
			//comentei para colocas como globais e imprimir
			//float desiredLeftMotorSpeed;
			//float desiredRightMotorSpeed;	
			
	       
            ros::NodeHandle nh; // subscribe para posicao do carrinho
            //ros::NodeHandle nh2; // subscribe para posicao do caster

            ros::Subscriber sub = nh.subscribe ("path_planning/orientation_translate", 1000, &poseMessageReceived2);
            //ros::Subscriber sub = nh.subscribe ("vrep/goleiro1Position", 1000, &poseMessageReceived);
            //ros::Subscriber sub2 = nh2.subscribe ("vrep/goleiro1CasterPosition", 1000, &poseMessageReceived2);

			vrep_common::JointSetStateData motorSpeeds;	
	
	        double a, b, raio=0.2;
	        int pronto=0;
	        double caminho_x[10], caminho_y[10];
	         
	        int caminho_tam = 3; 
	        
			caminho_x[0]= 0.25;
	        caminho_y[0]= 0.45;
	        caminho_x[1]= -0.1;
	        caminho_y[1]= 0.3;
	        caminho_x[2]= 0;
	        caminho_y[2]= 0;
//	        caminho_x[3]= 0;
//	        caminho_y[3]= 0;
        	        
	         
            yd=0;
            xd=raio;

            ///////////////////////////////tes
            //variaveis auxiliares do codigo de controle, armazenam o erro vezes o ganho
            double aux_x;
            double aux_y;
            //variavel do valor de deslocamento do centro do robo
            double e = 0.05;
            //largura do robo dividida por dois
            double b1 = 0.0375;

            double K = 10;
            //variável que armazena a posição descolado do centro
            double x_dec, y_dec;
           	//flag utilizada para controlar acesso a condição de fornecer proximos pontos ao robo 
            int flag = 1;
            //Variável que armazena a distância do ponto virtual ao ponto de de destino
            double virtualcaminho = 1;
            //comentado para testar como variavel global
            //double errox_virtual, erroy_virtual, errox_real, erroy_real, b_reta, x_virtual;

            ////////////////////////////////////

			while (ros::ok()&&simulationRunning)
			{ // this is the control loop (very simple, just as an example)
			
			    //printf ("\nerrotheta= %lf\n", errotheta);

	            thetad = atan2(carro.pY-caminho_y[caminho_index],carro.pX-caminho_x[caminho_index]);
	
                //rot = atan2(carro.pY-caster.pY,carro.pX-caster.pX);
	
	            errotheta = thetad - rot;
    
                distancia = sqrt(pow(errox,2) + pow(erroy,2));

                //variaveis do centro deslocados de e, isso melhora o controle e diminui oscilações nos movimentos
                x_dec = carro.pX + e*cos(rot);
			   	y_dec = carro.pY + e*sin(rot);
			   	//erro da posicao de destino e do robo
			   	errox_real = caminho_x[caminho_index] - x_dec;
	            erroy_real = caminho_y[caminho_index] - y_dec;

	            //testa se o robo encontra-se na reta que contem seu centro e o ponto de destino
			   	if(mod(errotheta) < 0.01)
			   	{
			   		//se meu erro é maior que zero estou indo no sentido positivo do eixo, devo somar uma ponto positivo
			   		if(errox_real > 0)
			   		{
			   			if(virtualcaminho < 0)
			   				virtualcaminho = virtualcaminho *(-1);
			   		}
			   		else
			   		{
			   			//Deixa virtual caminho positivo
			   			if(virtualcaminho > 0)
			   				virtualcaminho = virtualcaminho *(-1);
			   		}
			   		//cria um ponto virtual além do ponto de destino
			   		x_virtual = caminho_x[caminho_index] + virtualcaminho;
			   		//Erro entre o robo e o ponto virtual
			   		errox_virtual = x_virtual - x_dec;
			   		//calculo do ponto de interseção da reta do ponto de destino deslocado
			   		b_reta = caminho_y[caminho_index] - tan(rot)*caminho_x[caminho_index];
			   		erroy_virtual = (tan(rot)*x_virtual + b_reta) - y_dec;
			   		//se o robo estiver na reta do ponto de destino, considera-se o erro virtual
			   		errox = errox_virtual;
			   		erroy = erroy_virtual;
			   	}
			   	else
			   	{
			   		//se o robo ainda nao estiver na reta do ponto de destino, o erro é calculado com base nos pontos reais
			   		errox = errox_real;
			   		erroy = erroy_real;
			   	}

                /////////////////////////////////////Código de Controle ////////////////////////////////

                aux_x = K*errox;
                aux_y = K*erroy;

                desiredLeftMotorSpeed = (((1/e)*(e*cos(rot) + b1*sin(rot)))*aux_x + (1/e)*(e*sin(rot) - b1*cos(rot))*aux_y)*(-1);
                desiredRightMotorSpeed = (((1/e)*(e*cos(rot) - b1*sin(rot)))*aux_x + (1/e)*(e*sin(rot) + b1*cos(rot))*aux_y)*(-1);

                if(desiredRightMotorSpeed > MAX_SPEED)
                	desiredRightMotorSpeed = MAX_SPEED;

                if(desiredLeftMotorSpeed > MAX_SPEED)
                	desiredLeftMotorSpeed = MAX_SPEED;

                //se o robô está num raio de 3 cm do destino passa pra ele o proximo ponto
                if((mod(errox_real) <= 0.03 || mod(erroy_real) <= 0.03) && flag)
                {
                	caminho_index++;

                	if(caminho_index >= caminho_tam-1)
                		flag = 0; //flag impede acesso a condição se não existirem mais pontos a serem passados 
                }

                ////////////////////////////////////////////////////////////////////////////////////////////
	/*		    
			   switch(estado){
			    
			    
			    case 0: //gira

			        
			        if(abs(errotheta) <= 0.01 && contador > 0){ 
			              if (pronto)
			                estado=2;
			              else
			                estado=1;
			        }
	      	
		            
		            desiredRightMotorSpeed = (K)*(errotheta);
		            c = (-1)*(K)*(errotheta); 
		            
		            break;
		            
		        case 1: //chega no ponto
		        
		            if (desiredRightMotorSpeed < MAX_SPEED)	 
		           	    //desiredRightMotorSpeed = (K/2)*(distancia);
		           	    //desiredRightMotorSpeed = (MAX_SPEED)*(1.0 - exp(-distancia*K));
		           	    
		           	
		           	if (desiredLeftMotorSpeed < MAX_SPEED)		            
		                //desiredLeftMotorSpeed = (MAX_SPEED)*(1.0 - exp(-distancia*K));
		                
		                
		            if(distancia <= 0.001 && contador > 0){
		                 if (caminho_index == caminho_tam-2)
			                pronto=1;
			             xd=0;
			             yd=0;
			             estado=0;	
			             caminho_index++;		             
			        }     
			        break;

		        case 2: //chuta
		        
		            if (desiredRightMotorSpeed < MAX_SPEED)	 
		           	    //desiredRightMotorSpeed = (K/12)*(1/distancia);
		           	
		           	if (desiredLeftMotorSpeed < MAX_SPEED)		            
		                //desiredLeftMotorSpeed = (K/12)*(1/distancia); 
		                
		            if(distancia <= 0.01 && contador > 0){
			             estado=3;
			        }     
		        
		            break;  
		            
		        case 3: // para
		            
		            desiredRightMotorSpeed = 0;	            
		            desiredLeftMotorSpeed = 0; 
		        
		            break;
		                          
			    
			    }
			    */
			    
			    /*if(estado == 0)
	            {
	              
	                if(abs(errotheta) <= 0.001 && contador > 0)
		            {
			             //errotheta = 0;
			             estado = 1;
	                  
		            }	
	                
                    desiredRightMotorSpeed = (K)*(errotheta);
		            desiredLeftMotorSpeed = (-1)*(K)*(errotheta); 
		            
		            
	
		     
		            
	            }
	            else
	            {
		           	
		           	if (desiredRightMotorSpeed < MAX_SPEED)	 
		           	    desiredRightMotorSpeed = (K/4)*(distancia);
		           	
		           	if (desiredLeftMotorSpeed < MAX_SPEED)		            
		                desiredLeftMotorSpeed = (K/4)*(distancia); 
		            
	                
	                    
		        
	             }*/
                
                //desiredLeftMotorSpeed = (K/10)*distancia; 
		        //desiredRightMotorSpeed = (-1)*(K/10)*distancia;	
	       
				// publish the motor speeds:
				motorSpeeds.handles.data.push_back(rightMotorHandle);
				motorSpeeds.handles.data.push_back(leftMotorHandle);				
				motorSpeeds.setModes.data.push_back(2); // 2 is the speed mode [Verificar os outros modos]
				motorSpeeds.setModes.data.push_back(2);
				motorSpeeds.values.data.push_back(desiredRightMotorSpeed);
				motorSpeeds.values.data.push_back(desiredLeftMotorSpeed);
				
				motorSpeedPub.publish(motorSpeeds);

				// handle ROS messages:
				ros::spinOnce();
                
				// sleep a bit:
				usleep(500);
			}
		}
	//}
	
	ros::shutdown();
	printf("VSS simulator just ended!\n");
	return(0);
}

