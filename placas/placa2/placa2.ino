#include <Wifi.h>
#include < PubSubClient.h>
#include "env.h"
#include <WifiClientSecure.h>

WifiClientSecure client;
PubSubClient mqtt(client);

NEW SKETCH


const byte Trigger_1 = 5;
const byte echo_1 = 18;

const byte Trigger_2 = 4;
const byte echo_2 = 17; 



void setup() {
  Serial.begin(115200);              // configura a placa para monstrar na tela
  WiFi.begin(WIFI_SSID, WIFI_PASS);  //tenta conectar na rede
  Serial.println("Conectado a rede");
  client.setInsecure();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("Conectado com sucesso!");

  mqtt.setServer(BROKER_URL, BROKER_PORT);
  String clientID = "S1-";  // criando um id unico e aleatorio
  clientID += String(random(0xffff), HEX);

  while (mqtt.connect(clientID.c_str()) == 0) {  // tentativa de conetar,  0 = não conectado, 1 = conectado
    Serial.print(".");
    delay(200);
  }

//topicos 

  mqtt.subscribe(Topic_S2_Presenca1);
  mqtt.subscribe(Topic_S2_Presenca2);

  Serial.println("\nConectado ao broker!");
  mqtt.setCallback(callback);

  pinMode(Trigger_1, OUTPUT);
  pinMode(Trigger_2, OUTPUT);
  pinMode(echo_1, INPUT);
  pinMode(echo_2, INPUT);

}

long lerDistancia1(){
  digitalWrite(trigger_1,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_1,HIGH);
}


long lerDistancia1() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  return distancia;
}


long lerDistancia2() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  return distancia;
}


void loop() {
  long distancia1 = lerDistancia1();
   long distancia2 = lerDistancia2();


  if (distancia1 < 50) {
    mqtt.publish(Topic_S2_Presenca1, "S2 - Presença 1:  Em rota de Colisão !!!");
  } else {
    mqtt.publish(Topic_S2_Presenca1, "S2 - Presença 1: Caminho Livre");
  }


  if (distancia2 < 50) {
    mqtt.publish(Topic_S2_Presenca2, "S2 - Presença 2: Em rota de Colisão !!!");
  } else {
    mqtt.publish(Topic_S2_Presenca2, "S2 - Presença 2: Caminho Livre");
  }

}


void callback(char* topic, byte* payload, unsigned long length) {
  String MensagemRecebida = "";

  for (int = 0; i < length; i++) {
    MensagemRecebida += (char)payload[i];
  }
  Serial;
  println(MensagemRecebida);

//ligar luz
  if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Claro") {
    digitalWrite(ledPin, HIGH);  // acender led
  } else if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Escuro") {
    digitalWrite(ledPin, LOW);
  }
}
