#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <vss/pos.h>
#include <vss/pos_array.h>
#include "fut.h"

robot tsubaza;
target brazuca;

void robots(const vss::pos_array& msg)
{
  	
  	const vss::pos &data = msg.component[0];
  	//ROS_INFO_STREAM("num: " << data.num << " x: " << data.x << " y: " << data.y << " theta: " << data.theta);

  	tsubaza.x = data.x;
  	tsubaza.y = data.y;
  	tsubaza.theta = data.theta;

	// for(int i = 0; i<msg.component.size(); i++){
	//     const vss::pos &data = msg.component[i];
	//     ROS_INFO_STREAM("num: " << data.num << " x: " << data.x << " y: " << data.y << " theta: " << data.theta);
	// }
}

void bola(const vss::pos& msg)
{

	brazuca.x = msg.x;
	brazuca.y = msg.y;
	//ROS_INFO_STREAM("brazuca x: " << brazuca.x << " y: " << brazuca.y);
	
}

float mod(float ang)
{
	if(ang < 0)
		return ang*(-1);
	else
		return ang;
}
////////////////////////////////////////////

int main(int argc, char **argv)
{

	ros::init(argc, argv, "controle7");

	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("robots_position", 1000, robots);
	ros::Subscriber sub1 = n.subscribe("ball_position", 1000, bola);
	ros::Publisher pub = n.advertise<std_msgs::String>("serial", 1000);  //Publica para o serial

	/********************************************************************************************
		Declaração das variáveis

	********************************************************************************************/
	float distancia;
	
	int vel_left = 0;
	int vel_righ = 0;

	char aux[20]; //variável que armazena temporariamente pacote da mensagem a ser enviada via rádio
	bool flag = 0;

	/********************************************************************************************/

	ros::Rate loop_rate(10);

	while (ros::ok())
	{
		std_msgs::String msg;
		std::stringstream ss;
		
		if(mod(errotheta) > 10)
		{
			tsubaza.controlador_angulo(brazuca, vel_righ, vel_left);
			sprintf(aux,"BA%i%.3i%.3iB",1, vel_left, vel_righ);
			ss << aux;
			msg.data = ss.str();
			ROS_INFO("msg : %s", msg.data.c_str());
			pub.publish(msg);

		}
		else if(mod(dist) > 30)
		{
			//tsubaza.controlador_angulo(brazuca, vel_righ, vel_left);
			tsubaza.controlador_dist(brazuca, vel_righ, vel_left);

			sprintf(aux,"BA%i%.3i%.3iB",1, vel_left, vel_righ);
			ss << aux;
			msg.data = ss.str();
			ROS_INFO("msg : %s", msg.data.c_str());
			pub.publish(msg);

		}
		else
		{
			vel_left = 180;
			vel_righ = 180;
		
			sprintf(aux,"BA%i%.3i%.3iB",1, vel_left, vel_righ);
			ss << aux;
			msg.data = ss.str();
			ROS_INFO("msg :%s", msg.data.c_str());
			pub.publish(msg);
			
		}

		ROS_INFO_STREAM(errotheta << " " << dist << " " << msg.data.c_str());
		
	
		ros::spinOnce();
		loop_rate.sleep(); //processo dorme até o próximo loop definido em ros::Rate loop_rate()
	}


	return 0;
}
