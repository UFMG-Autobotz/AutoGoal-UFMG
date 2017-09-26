#include "fut.h"
#include "math.h"
#include <ros/ros.h>
#include <std_msgs/String.h>

const float pi = 3.141592;
float errotheta;
float dist;

void robot::controlador_angulo(target obj, int& vel_righ, int& vel_left)
{
	float thetad = 0;
	float Kp = 0.61; //proporcional de rotação
	float Ki = 0.05;
	float Iterm = 0;

	thetad = (180/pi)*atan2(obj.y - y, obj.x - x);
	errotheta = thetad - theta;

	//normaliza o erro do angulo entre -180 e 180
	while (errotheta > 180) errotheta -= 360;
	while (errotheta < -180)errotheta += 360;

	Iterm+= Ki*errotheta;
	vel_left = Kp*(errotheta) + Iterm;

	ROS_INFO_STREAM("Iterm: " << Iterm);

	if(vel_left > 255)
		{
			vel_left = 255;
			vel_righ = 510;
		}
		else if(vel_left < 0)
		{
			vel_left = vel_left*(-1) + 255;
			vel_righ = vel_left - 255;

			if(vel_left < 295)
			{
				vel_left = 295;
				vel_righ = 40;
			}

			if(vel_left > 510)
			{
				vel_left = 510;
				vel_righ = 255;
			}
		}
		else
		{
			if(vel_left != 0)
			{
				vel_righ = vel_left + 255;

				if(vel_left < 40)
				{
					vel_left = 40;
					vel_righ = 295;
				}

			}
			else
				vel_righ = 0;
		}

}

void robot::controlador_dist(target obj, int& vel_righ, int& vel_left)
{
	float erro;
	float Kpd = 0.21;

	dist = Kpd*sqrt((x-obj.x)*(x-obj.x) + (y-obj.y)*(y-obj.y));
    erro = x - obj.x;

    // vel_left = Kpd*(1.0/(float)dist);
    // vel_righ = Kpd*(1.0/(float)dist);

    vel_left = Kpd*dist;
    vel_righ = vel_left;

    ROS_INFO_STREAM("vel-dist: " << vel_left);

    if(vel_left > 255)
			vel_left = 255;
		else if(vel_left < 0)
		{
			vel_left = vel_left*(-1) + 255;

			// if(pwm_left < 295)
			// 	pwm_left = 305;

			if(vel_left > 510)
				vel_left = 510;
		}

		if(vel_righ > 255)
			vel_righ = 255;
		else if(vel_righ < 0)
		{
			vel_righ = vel_righ*(-1) + 255;

			// if(pwm_right < 295)
			// 	pwm_right = 305;

			if(vel_righ > 510)
				vel_righ = 510;
		}
}