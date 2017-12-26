#!/usr/bin/env python
# license removed for brevity
from vetor import Vetor
import rospy
from geometry_msgs.msg import Pose
import math

class Robo():
    L = 0.063 # tamanho do eixo das rodas em cm
    r = 0.016 # raio das rodas em cm
    distCR = 0.0155 # distancia do centro de rotacao ao centro do robo em cm
    w = 10 # velocidade angular das rodas - determinar valor adequado


    # inicializa o objeto dadas as coordenadas da posicao, a orientacao e o numero do jogador
    def __init__(self, topic):
        self.orientacao = Vetor(0, 0)
        self.pos = Vetor(0, 0)

        self.vel = 0 #velocidade linear (inicializada como zero)
        self.omega = 0 #velocidade angular (inicializada como zero)

        self.lado = 0

        rospy.Subscriber(topic, Pose, self.recebe_dados)


    # retorna a posicao do robo
    def get_pos(self):
        return self.pos


     # retorna a orientacao do robo
    def get_theta(self):
        return self.orientacao.angulo()


     # recebe posicao e orientacao da visao
    def recebe_dados(self, msg):
    	self.pos.x = msg.position.x
    	self.pos.y = msg.position.y

    	q = msg.orientation
    	yaw = math.atan2((2 * q.w * q.z ),(1 - 2 * q.z * q.z))

    	self.orientacao.x = math.cos(yaw)
    	self.orientacao.y = math.sin(yaw)
