from regiao import Regiao
from vetor import Vetor

# classe para armazenar as regioes: campo, gol esquerdo e gol direito
# dimensoes em cm
class Campo():

    tamanhoGol = 0.1
    largura = 1.5
    altura = 1.3

    def __init__(self):
        self.campo_completo = Regiao.coordenadas(-0.75, 0.65, 0.75, -0.65)
        self.lado = [Regiao.coordenadas(-0.75, 0.65, 0, -0.65), Regiao.coordenadas(0, 0.65, 0.75, -0.65)]
        self.areaGol = [Regiao.coordenadas(-0.75, 0.35, -0.6, -0.35), Regiao.coordenadas(0.6, 0.35 , 0.75, -0.35)]

        self.minLgol = -0.2
        self.maxLgol = 0.2

    # retorna gol dado o lado do campo (0 ou 1)
    def get_gol(self, lado):
        return self.areaGol[lado]
