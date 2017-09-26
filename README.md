# README #

### Rodando o código ###

Rodar o código dentro do diretório /vss/launch com o comando:

=> roslaunch vss vss.launch team:=<cor> view:=<true/false> usb:=<numero tabelado>

team:=
	Parametro Obrigatorio, A cor do nosso time, azul ou amarelo

view:= 
	Parametro opcional, se vc digitar view:=1 ou view:=true, alem de tudo uma janela vai abrir com a imagem pura da camera
	default: false

usb:=
	Parametro opcional, O numero da porta USB tabelado pelos criadores da RS232, normalmenta pra arduino o numero é o 24 ou 25, se não for nenhum deles, tem que procurar no site da rs232 (o link é mostrado quando o numero errado é inserido)
	default: 23

exemplo:
 roslaunch vss vss.launch team:=azul usb:=25

### Descrição dos nós ###

---------Arducom---------
main.cpp: Comunica com o arduino via serial, le uma mensagem do tipo string no topico 'serial' e envia sem alteração para o serial do arduino

	=> Chamada: rosrun vss arducom
	=> precisa o rs232.c e rs232.h (inclusos na pasta)
	
publisher.cpp: Controla o robo usando as teclas WASDQE (anda e vira) e R(reseta a velocidade pra 0), pra escolhes o numero do robo a ser controlado use os numeros (de 1 a 6)... Esse programa publica as strings no formato correto pra que o arducom consiga ler e enviar pro robo

	=> chamada: rosrun vss stringPub

------detectObject------
multipleObjetctTracking.cpp: Detecta os robos inimigos e as bolinhas e retorna uma string com a posição dos mesmos (ainda vou fazer publicar num topico melhor do que string)

	=> Chamada: rosrun vss detectEnemy <cor>

cor sendo a cor do SEU time 'azul' ou 'amarelo'
		
------im_converter-------
main.cpp:  Le a camera de um topico chamado camera/image_raw e publica a imagem com a cor separada num topico chamado camera/team

	=> Chamada: rosrun vss ar_converter <cor>

cor sendo 'azul' ou 'amarelo', depois do primeiro uso, os parametros da separação de cor são salvos no computador pra configurar os parametros basta arrastar a trackbar 'config' pra um, pra salvar é só arrastar o 'config' denovo pra 0


--------interface--------
show.cpp: Daqui que voce vai saber como retirar qualquer tipo de dado dos outros codigos da visão. Le do topico ar_pose_markers (do ar_multi) e mostra em uma janela as posições e orientação do robo

	=> Chamada: rosrun vss robot_view <cor>
		
cor sendo 'azul' ou 'amarelo'
		
---------setLine----------
main.cpp: Setline abre uma janela com a imagem da camera, voce deve clicar em todos os cantos que contornam o campo e por ultimo no centro do campo depois disso, aperte o botao direito do mouse pra salvar os valores no arquivo

	=> Chamada: rorsun vss setLine
	
