#!/usr/bin/env python
# license removed for brevity
from bola import Bola
from robo import Robo
from aliado import Aliado
from bola import Bola
from campo import Campo

import numpy
import rospy

from geometry_msgs.msg import Pose

if __name__ == '__main__':
    rospy.init_node('goleiro', anonymous=True)

    try:
        campo = Campo()
        bola = Bola("/ball/pose")
        goleiro = Aliado("/player1/pose", "/player1__player__chassi_left_wheel/joint_vel", "/player1__player__chassi_right_wheel/joint_vel")
        artilheiro = Robo("/player2/pose")

        # rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            goleiro.move_goleiro(campo, bola, artilheiro)
            # rate.sleep()

    except rospy.ROSInterruptException:
        pass
