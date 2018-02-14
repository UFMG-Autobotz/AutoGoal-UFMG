#!/usr/bin/env python
# license removed for brevity
from vetor import Vetor
import rospy
from geometry_msgs.msg import Pose

#classe para guardar informacoes da bola
class Bola():
    diametro = 4.27 # diametro da bola em cm

    #inicializa o objeto dada sua posicao e o topico dos ROS onde ele esta subscrito
    def __init__(self, topic):
        self.pos = Vetor(0, 0)
        rospy.Subscriber(topic, Pose, self.recebe_dados)

    # recebe posicao da bola
    def recebe_dados(self, msg):
        self.pos.x = msg.position.x
        self.pos.y = msg.position.y

     # retorna posicao da bola
    def get_pos(self):
        return self.pos

     # imprime propriedades da bola
    def get_info(self):
        print 'Posicao: (' + repr(self.pos.x) + ', ' + repr(self.pos.y) + ')'
