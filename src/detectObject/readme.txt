O arquivo multipleObjectTrack.cpp é o que contém o main.

Execute o programa com os seguintes argumentos:

./executável calibrationMode enemyColor

Neste caso executável é "vss";
se quiser habilitar o modo de calibração, calibrationMode é passado como 1, se não, 0;
enemyColor é por padrão azul, para ativar a cor amarela basta escrever "amarelo" como argumento (qualquer outra palavra habilita azul, mas é importante não deixar de colocar esse argumento);

Exemplos de chamada:

//habilita trackbars
$ ./vss 1 a

//não habilita trackbars, inimigos azuis:
$ ./vss 0 s

//não habilita trackbars, inimigos amarelos:
$ ./vss 0 amarelo



