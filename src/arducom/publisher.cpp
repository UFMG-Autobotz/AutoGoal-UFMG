#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
  
#define incr 10

using namespace std;

int main(int argc, char **argv){
//configura getc
struct termios old_tio, new_tio;

	/* get the terminal settings for stdin */
	tcgetattr(STDIN_FILENO,&old_tio);

	/* we want to keep the old setting to restore them a the end */
	new_tio=old_tio;

	/* disable canonical mode (buffered i/o) and local echo */
	new_tio.c_lflag &=(~ICANON & ~ECHO);

	
//fim



char aux[20], c;
int pwm1 = 0,pwm2 = 0;
int pwm1a = 0,pwm2a = 0;
int robo = 1, count = 0;
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("serial", 1000);
  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  while (ros::ok()){
	
	/* set the new settings immediately */
	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

    std_msgs::String msg;
    std::stringstream ss;
    fflush(stdin);
    c = getchar();
    fflush(stdin);


    //interpreta teclas
    if(c == 'w'){
	pwm1+=incr;
	pwm2+=incr;
	if(pwm1 > 255) pwm1 = 255;
	if(pwm2 > 255) pwm2 = 255;
    } 
    else if (c == 's'){
	pwm1= pwm1 - incr;
	pwm2= pwm2 - incr;
	if(pwm1 < -255) pwm1 = -255;
	if(pwm2 < -255) pwm2 = -255;
    }
    else if (c == 'a'){
	pwm1-=incr/2;
	pwm2+=incr/2;
	if(pwm1 < -255) pwm1 = -255;
	if(pwm2 > 255) pwm2 = 255;
    }
    else if (c == 'd'){
	pwm1+=incr/2;
	pwm2-=incr/2;
        if(pwm1 > 255) pwm1 = 255;
	if(pwm2 < -255) pwm2 = -255;	
    }
     else if (c == 'q'){
	pwm2+=incr;
	if(pwm2 > 255) pwm2 = 255;
    }
    else if (c == 'e'){
	pwm1+=incr;
        if(pwm1 > 255) pwm1 = 255;
    }
    else if(c == 'r'){
        pwm1 = pwm2 = 0;
    }
    //muda o robo que esta sendo controlado
	else if(c == '1'){ robo = 1; pwm1 = pwm2 = 0;}
	else if(c == '2'){ robo = 2; pwm1 = pwm2 = 0;}
	else if(c == '3'){ robo = 3; pwm1 = pwm2 = 0;}
	else if(c == '4'){ robo = 4; pwm1 = pwm2 = 0;}
	else if(c == '5'){ robo = 5; pwm1 = pwm2 = 0;}
	else if(c == '6'){ robo = 6; pwm1 = pwm2 = 0;}

    //traduz dados para string
	pwm1a = pwm1;
	pwm2a = pwm2;
	if(pwm1a < 0){
		pwm1a = pwm1a*(-1);
		pwm1a = pwm1a + 255;
		if (pwm1a > 510) pwm1a = 510;
	}
	if(pwm2a < 0){
		pwm2a = pwm2a*(-1);
		pwm2a = pwm2a + 255;
		if (pwm2a > 510) pwm2a = 510;
	}
	sprintf(aux,"BA%i%.3i%.3iB",robo, pwm1a, pwm2a);
	if(count == 1){//só mostra a string na saida de 5 em 5 modificações pra nao pesar a execução do codigo
		cout << aux << endl;
		count = 0;
	}
    ss << aux;
    msg.data = ss.str();

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    chatter_pub.publish(msg);

    ros::spinOnce();
	count ++;
    /* restore the former settings */
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio); //se as configurações se manterem, ele vai se aplicar ao terminal todo
  }

	
  return 0;
}
