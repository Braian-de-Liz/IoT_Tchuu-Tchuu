#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>

WiFiClientSecure client;
PubSubClient mqtt(client);




const String topic = "TchuuTchuu";

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200); // configura a placa para monstrar na tela 
  WiFi.begin(WIFI_SSID, WIFI_PASS); //tenta conectar na rede
  Serial.println("Conectado a rede");
  client.setInsecure();
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
     }


Serial.println("Conectado com sucesso!");
mqtt.setServer(brokerURL.c_str(),brokerPort);
String clientID = "S1-" ; // criando um id unico e aleatorio
clientID += String(random(0xffff),HEX);
while(mqtt.connect(clientID.c_str())  == 0){  // tentativa de conetar,  0 = não conectado, 1 = conectado
  Serial.print(".");
  delay(200);
    
 }
   Serial.println("\nConectado ao broker!");
}

void loop() {
  String mensagem = ""; // cria dentro do loop para recriala toda vez que rodar o programa
  if(Serial.available()> 0 ){  // checagem se tem palavras na "fila de espera", se tiver alguma palavra faça
   mensagem = Serial.readStringUntil('/n');    // salva a palavra digitada / lê a mensagem do usuario
    mensagem = "Davi:" + mensagem; 
  // Serial.print("A mensagem foi: ");
  // Serial.println(mensagem);
    mqtt.publish("TchuuTchuu2", mensagem.c_str());
  };  

  mqtt.loop();
}

void callback(char* topic, byte* payload, unsigned long length) {
  String MensagemRecebida = "";
 for(int i = 0; i < length; i++) {
  //Pega cada letra de payload e junta na mensagem
  MensagemRecebida += (char) payload[i];
 }
 Serial.println(MensagemRecebida);
}
