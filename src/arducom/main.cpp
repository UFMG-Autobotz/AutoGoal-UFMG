#include <ros/ros.h>
#include <std_msgs/String.h>
#include <stdlib.h>
#include "rs232.h"
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>

using namespace std;

//variaveis
int bdrate = 115200, debug = 1;
int serialPort; //porta serial de comunicação

void sendString(const std_msgs::String::ConstPtr& msg){
char aux[100];
	sprintf(aux,"%s\0",msg->data.c_str());

 	        if (debug) ROS_INFO("I heard: [%s]", aux);
		for(int i = 0; i < 10; i++){
		RS232_SendByte(serialPort, aux[i]); 
	}
}

int main(int argc, char **argv){
/******************Configuração Serial**********************/
	//testa parametros
		if(!argv[1]){
			ROS_INFO("Use: rosrun vss arducom [port number]\nVide port number in: http://www.teuniz.net/RS-232/\n");
			return -1;
		}
	 //tenta abrir a comunicação serial
		  serialPort = atoi(argv[1]);
		  if(RS232_OpenComport(serialPort, bdrate))
		  {
		   ROS_INFO("Can not open comport\n");
		    return(0);
		  }
		  else ROS_INFO("Arduino conected!\n");
		if(argv[2]) debug = 1;
/******************        Fim        **********************/

	  ros::init(argc, argv, "sendSerial");

	  ros::NodeHandle n;

	  ros::Subscriber sub = n.subscribe("serial", 1000, sendString);

	  ros::spin();

	  return 0;
}
