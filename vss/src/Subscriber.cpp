#include <ros/ros.h>
#include <vss/pos.h>
#include <vss/pos_array.h>
#include <iomanip>

void poseMessageReceived (const vss::pos_array& msg){

	
	for(int i = 0; i<msg.component.size(); i++){
	
        printf ("\ncarro %d na posicao %.2f %.2f\n", msg.component[i].num, msg.component[i].x, msg.component[i].y);
	
	}
    printf ("\n--------------------------------------\n");

}

int main(int argc, char **argv){

	ros::init (argc, argv, "subscribe_to_caminho");
	ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe ("caminho/robo1", 1000, &poseMessageReceived);

     while (ros::ok()){



        ros::spinOnce();

        //sleep a bit
        sleep(1);


     }



}


