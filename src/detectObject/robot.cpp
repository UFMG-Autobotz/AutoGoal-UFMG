#include "robot.hpp"

robot::robot(void){

}
robot::~robot(void){

}

int robot::getxpos(){
    return robot::xpos;
}
void robot::setxpos(int x){
    robot::xpos=x;
}
int robot::getypos(){
    return robot::ypos;
}
void robot::setypos(int y){
    robot::ypos=y;
}
