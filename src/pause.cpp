#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <stdlib.h>


int main (int argc, char **argv){

    bool tmp = true;
    int leitura;

    ros::init (argc, argv, "pause_game");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<std_msgs::Bool>("vss/pause", 1000);

    ros::Rate rate(2);

    std_msgs::Bool msg;

    while (ros::ok()){



        if (tmp == false){
            printf ("\n\nDigite 0 para pausar o jogo: ");
            scanf ("%d", &leitura);
            if (leitura == 0)
                msg.data = true;
        }
        else{
            printf ("\n\nDigite 1 para iniciar o jogo: ");
            scanf ("%d", &leitura);
            if (leitura == 1)
                msg.data = false;

        }

        tmp = msg.data;


        pub.publish(msg);

        //ROS_INFO_STREAM ("Sending random velocity command: " << " linear=" << msg.linear.x << " angular=" << msg.angular.z);
        rate.sleep();

    }

}
