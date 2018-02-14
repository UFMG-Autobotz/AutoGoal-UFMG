from vetor import Vetor
from robo import Robo
from reta import Reta
from bola import Bola
from campo import Campo

import math
import numpy

import rospy
from std_msgs.msg import Float64

#classe usada para guardar informacoes e funcionalidades dos robos aliados
class Aliado(Robo):

    # inicializa o objeto dadas as coordenadas da posicao e da orientacao atuais, desejadas e previstas, o lado do campo de seu time, sua velocidade linear e angular, as velocidades angulares de cada roda, alem dos topicos do ROS em que esta publicando e recebendo
    def __init__(self, subscriber, publisher_esquerda, publisher_direita):
        Robo.__init__(self, subscriber)

        self.pubL = rospy.Publisher(publisher_esquerda, Float64, queue_size = 100)
        self.pubR = rospy.Publisher(publisher_direita, Float64, queue_size = 100)

        self.vel = 0 #velocidade linear (inicializada como zero)
        self.omega = 0 #velocidade angular (inicializada como zero)

        self.orientacaoDesejada = Vetor(0, 0)
        self.posDesejada = Vetor(0, 0)

        self.orientacaoPrevista = Vetor(0, 0)
        self.posPrevista = Vetor(0, 0)

        self.omegaD = 0 # velocidade angular da roda direita
        self.omegaE = 0 # velocidade angular da roda esquerda

    #envia a velocidade angular das rodas para o robo
    def envia_dados(self):
        omegaE = self.omegaE
        omegaD = self.omegaD

        self.pubR.publish(omegaD)
        self.pubL.publish(omegaE)


    #movimenta o goleiro para que ele defenda o gol 
    def move_goleiro(self, campo, bola):
        self.encontra_posicao_goleiro_novo(campo, bola)
        self.move_linha_reta()
        self.envia_dados()

    #move para a posicao desejada em linha reta, sem alterar sua orientacao
    def move_linha_reta(self):
        margem = 0.05
        if (self.posDesejada.y > (self.pos.y + margem)):
            self.omegaD =  Robo.w
            self.omegaE =  Robo.w

        if(self.posDesejada.y <= (self.pos.y + margem) and self.posDesejada.y >= (self.pos.y - margem)):
            self.omegaD =  0
            self.omegaE =  0

        if (self.posDesejada.y < (self.pos.y - margem)):
            self.omegaD = -1 * Robo.w
            self.omegaE = -1 * Robo.w


    #encontra posicao e orientacao desejadas para goleiro
    #baseado apenas na posicao da bola
    def encontra_posicao_goleiro_novo(self, campo, bola):
        self.orientacaoDesejada = Vetor.unitario(math.pi/2)
        self.posDesejada.x = campo.get_gol(self.lado).centro().x
        self.posDesejada.y = bola.get_pos().y
        if (self.posDesejada.y < campo.minLgol):
            self.posDesejada.y = campo.minLgol
        if (self.posDesejada.y > campo.maxLgol):
            self.posDesejada.y = campo.maxLgol



   #encontra posicao e orientacao desejadas para goleiro
   #baseado na reta que passa pelo artilheiro e pela bola
    def encontra_posicao_goleiro(self, campo, bola, adversario):
        self.orientacaoDesejada = Vetor.unitario(math.pi/2) # orientacao paralela a linha do gol
        self.posDesejada.x = campo.get_gol(self.lado).centro().x # no eixo x, centro do gol

        reta = Reta(adversario.get_pos(), bola.get_pos()) #reta que liga o adversario a bola
        if (reta.e_funcao_de_x()): # se a reta for funcao de x
            self.posDesejada.y = reta.encontra_y(self.posDesejada.x) # encontra o prolongamento da reta
        else: # se a reta for paralela a linha do gol
            self.posDesejada.y = self.pos.y # nao se move no eixo y

       #garante que o goleiro fique na frente do gol
        if (self.posDesejada.y < campo.minLgol):
            self.posDesejada.y = campo.minLgol
        if (self.posDesejada.y > campo.maxLgol):
            self.posDesejada.y = campo.maxLgol

        print "posicao desejada - x: %s, y: %s / orientacao desejada: %s" % (self.posDesejada.x, self.posDesejada.y, self.orientacaoDesejada.angulo())


    # calcula o raio e o sentido da trajetoria
    def encontra_tragetoria(self):
        direcaoDesejada = Vetor.subtrai(self.posDesejada, self.pos) #vetor que liga a posicao atual a posicao desejada

        sentido = self.encontra_sentido(direcaoDesejada);
        raio = self.encontra_raio(direcaoDesejada);

        return (sentido, raio)


 	# encontra sentido do movimento (usado para descobrir o sentido de rotacao dos motores)
    def encontra_sentido(self, direcaoDesejada):
        if  (direcaoDesejada.tamanho()!= 0): # se o robo nao esta na posicao desejada
            ro = self.orientacao.angulo_com(direcaoDesejada) # angulo entre a orientacao do robo e o vetor direcaoDesejada

            if (ro <= math.pi/2):
                sentido = 1 # robo move para frente
            else:
                sentido = -1 # robo move para tras

        else: # se o robo ja esta na posicao desejada
            deltaTheta = self.orientacao.delta_angulo_com(self.orientacaoDesejada)

            if (deltaTheta > 0):
                sentido = 1 # robo vira para a direita sem sair do lugar (sentido horario)
            elif (deltaTheta < 0):
                sentido = -1 # robo vira para a esquerda sem sair do lugar (sentido anti-horario)
            else:
                sentido = 0 # robo nao se move

        # print "sentido: %s" % sentido

        return sentido

    def encontra_raio(self, direcaoDesejada):
        # descobre se o robo vira para esquerda ou direita
        y = Reta.ponto_e_angulo(self.pos, self.orientacao.angulo()).encontra_y(self.posDesejada.x)
        theta = self.orientacao.delta_angulo_com(direcaoDesejada)

        if (theta < 0 and theta != None):
            normal = Vetor.gira(self.orientacao, -math.pi/2) # vira no sentido anti-horario / esquerda
        elif (theta > 0):
            normal = Vetor.gira(self.orientacao, math.pi/2) # vira no sentido horario / direita
        else:
            normal = None # linha reta ou vira sem sair do lugar

        # encontra raio de curvatura
        if (normal == None):
            if (direcaoDesejada.tamanho() == 0):
                raio = 0 # vira sem sair do lugar
            else:
                raio = -1 # linha reta
        else:
            phi = normal.angulo_com(direcaoDesejada)
            raio = direcaoDesejada.quadrado()/(2*direcaoDesejada.tamanho() * math.cos(phi))

            # encontra centro
            centro = Vetor.multiplica(normal, raio)
            centro = Vetor.soma(self.pos, centro)

            # print "centro.x: %s, centro.y: %s, raio: %s" % (centro.x, centro.y, raio)

            # corrige raio comparando com orientacao desejada
            # if (raio > 0):
            #    raio = self.corrige_raio(raio, centro)

        return raio


    def corrige_raio(self, raio, centro):
        # encontra variacao do angulo ate a posicao desejada
        centroPos = Vetor.subtrai(self.pos, centro)
        centroPosDesejada = Vetor.subtrai(self.posDesejada, centro)
        deltaTheta = Vetor.delta_angulo_com(centroPos, centroPosDesejada)

        # encontra diferenca entre orientacao final prevista e desejada
        orientacaoPrevista = Vetor.gira(self.orientacao, deltaTheta)
        erroTheta = orientacaoPrevista.delta_angulo_com(self.orientacaoDesejada)

        if (deltaTheta < 0): # sentido anti-horario
            raio -= 0.5*raio * ( erroTheta / math.pi ) # fracao do raio * fracao do erro do angulo
        else: # sentido horario
            raio += 0.5*raio * ( erroTheta / math.pi )

        return raio


    # encontra velocidades linear e angular e velocidades das rodas
    def encontra_velocidades(self):
        sentido, raio = self.encontra_tragetoria();

        if (raio > 0): # curva
            razaoDE = (2*raio + Robo.L)/(2*raio - Robo.L) # omegaD / omegaE
            if (razaoDE > 1): # curva para a esquerda
                self.omegaD = sentido * Robo.w # roda direita a velocidade maxima
                self.omegaE = sentido * self.omegaD/razaoDE # roda esquerda a uma velocidade menor
            else: # curva para a direita
                self.omegaE = sentido * Robo.w # roda esquerda a velocidade maxima
                self.omegaD = sentido * self.omegaE*razaoDE # roda direita a uma velocidade menor

        elif (raio < 0): # reta
            # as duas rodas a velocidade maxima
            self.omegaD = sentido * Robo.w
            self.omegaE = sentido * Robo.w

        else: # roda sem sair do lugar
            self.omegaD = -1 * sentido * Robo.w
            self.omegaE = sentido * Robo.w

        self.vel = (self.omegaE + self.omegaD)*Robo.r/2
        self.omega = (self.omegaE - self.omegaD)*Robo.r/Robo.L
