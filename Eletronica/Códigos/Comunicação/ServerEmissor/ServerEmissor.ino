//Instalação: http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup

//Rodar o nó
//rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0

/*--- PINAGEM Comunicador---
 * (8) MISO -> 12
 * (5) MOSI -> 11
 * (6) SCK  -> 13
 * (4) CE   -> 8
 * (3) CSN  -> 7
 * (1) VCC  -> 3.3v
 * (2) GND  -> GND
 */

#define Envio 1  // 1 - Para enviar mesma string somente uma vez
                 // (tenta varias até ter sucesso)
                 // 0 - Para enviar a mesma string indefinidamente
                 // (quando uma nova string for inserida, passa a envia-la)
#define CANAL 95 //canal de comunicação
#define TamPayload 11 //tamanho da string em bytes
//#define TimeOut 30 //timeout da tentativa em ms

#include <SPI.h>
#include "Mirf.h"
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;
std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);

//uma string por robo
 char mensagem[TamPayload];
 char ATB1[TamPayload] = "BA1000000B";
 char ATB2[TamPayload] = "BA2000000B";
 char ATB3[TamPayload] = "BA3000000B";
 int tentativas = 0;
 unsigned long time = 0;

int LEDstat = 0;


void messageCb(const std_msgs::String& msg)
{
  char temp[TamPayload];
  digitalWrite(A2, HIGH);
  strcpy(temp, msg.data);
  
  //copia string pra memoria do robo que é pra receber
  if(temp[2] == '1'){
    strcpy(ATB1, msg.data);
    EnviaString(ATB1, "ATBR1");
  }
  else if(temp[2] == '2'){
    strcpy(ATB2, msg.data);
    EnviaString(ATB2, "ATBR2");
  }
  else if(temp[2] == '3'){
    strcpy(ATB3, msg.data);
    EnviaString(ATB3, "ATBR3");
  }
  digitalWrite(A2, LOW);
    
 //  if(mensagem[2] == '1'){
//    strcpy(ATB1, mensagem);
//  }
//  else if(mensagem[2] == '2'){
//    strcpy(ATB2, mensagem);
//  }
//  else if(mensagem[2] == '3'){
//    strcpy(ATB3, mensagem);
//  }
  //retoma envios com as frase nova
//str_msg.data = msg.data;
//chatter.publish( &str_msg );
 // Serial.println(msg.data);
}

ros::Subscriber<std_msgs::String> sub("serial", &messageCb);
unsigned int tempo;
void setup(){
  //Serial.begin(57600);
  //Serial.flush();
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"ATBSV");
  Mirf.payload = TamPayload*sizeof(byte);
  Mirf.config();
  Mirf.channel = CANAL;
  nh.initNode();
  nh.subscribe(sub);
  pinMode(A0, OUTPUT);//LED verde
  pinMode(A2, OUTPUT);//LED vermelho
  pinMode(A4, OUTPUT);//LED azul
//  nh.advertise(chatter);
  digitalWrite(A0, HIGH);
  Serial.println("Sending..."); 
  tempo = millis();
}
void escreveData(char *string, byte *data){
  int i;
  int tam = TamPayload;
  if(strlen(string) < tam) tam = strlen(string);
  for(i = 0; i < tam; i++){
    data[i] = string[i];
  }
  //preenche o resto da string com espaço vazio
  while(i < TamPayload){
    data[i] = '\0';
    i++;
  }
}

int EnviaString(char *string, char Robo[5]){
  digitalWrite(A4, HIGH);//acende o LED azul enquanto envia mensagem
  byte data[Mirf.payload];
  //escreve string em data
  escreveData(string, data);
  //configura receptor e envia
  Mirf.setTADDR((byte *)Robo);
  Mirf.send((byte *)data);
  //enquanto tiver enviando, espera
  while(Mirf.isSending()){
  }
  delay(1);
  digitalWrite(A4, LOW);
  return 0; 
}

void loop()
{
  
  
  if((millis()-tempo) >= 1000){
      tempo = millis();
      if(LEDstat) {digitalWrite(A0, LOW); LEDstat = 0;}
      else        {digitalWrite(A0, HIGH); LEDstat = 1;}
  }
  
//  EnviaString(ATB1, "ATBR1");
//  EnviaString(ATB2, "ATBR2");
//  EnviaString(ATB3, "ATBR3");
  
  nh.spinOnce();
  delayMicroseconds(200);

} 
  
  
  
