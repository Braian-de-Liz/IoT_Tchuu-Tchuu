#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>

#include <ESP32Servo.h>  

WiFiClientSecure client;
PubSubClient mqtt(client);

const byte Trigger = **;
const byte echo = **;

const byte ledPin = **;

//colocar os servos aqui
Servo Servo1_S3;
Servo Servo2_S3;

const byte Servo1Pin = **;
const byte Servo2Pin = **;



void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectado a rede");
  client.setInsecure();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }

  Serial.println("Conectado com sucesso!");

  mqtt.setServer(BROKER_URL, BROKER_PORT);
  String clientID = "S1-";
  clientID += String(random(0xffff), HEX);

  while (mqtt.connect(clientID.c_str()) == 0)
  {
    Serial.print(".");
    delay(200);
  }

  // topicos

  mqtt.subscribe(Topic_S2_Presenca1);
  mqtt.subscribe(Topic_S2_Presenca2);
  mqtt.subscribe(Topic_S1_Iluminacao);
  mqtt.subscribe(Topic_S3_Presenca);

  Serial.println("\nConectado ao broker!");
  mqtt.setCallback(callback);

  Servo1_S3.setPeriodHertz(50);
  Servo1_S3.attach(Servo1Pin, 500, 2400);


  Servo2_S3.setPeriodHertz(50);
  Servo2_S3.attach(Servo2Pin, 500, 2400);


  pinMode(Trigger, OUTPUT);
  pinMode(echo, INPUT);


}

  long lerDistancia(){
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);
  long duracao = pulseIn(echo, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  return distancia;
    

  }



void loop(){

  // distancia 
  long distancia = lerDistancia();

  if (distancia < 20){
    mqtt.publish(Topic_S3_Presenca, "S3 - Presença : em rota colisão !!!");
  }else{
    mqtt.publish(Topic_S3_Presenca, "S3 - Presença : Caminho Livre");
  }

delay(200);
mqtt.loop(500);
}


void callback(char *topic, byte *payload, unsigned long length)
{

  String MensagemRecebida = "";
  for (int i = 0; i < length; i++)
  { // Pega cada letra de payload e junta na mensagem
    MensagemRecebida += (char)payload[i];
  }
  Serial.printf("Msg:%s / Topic%s\n", MensagemRecebida, topic);


// config servos
  if (strcmp(topic, Topic_S2_Presenca1) == 0 && MensagemRecebida == "S2 - Presença 1:  Em rota de Colisão !!!"){
    Servo1_S3.write(90);
    
  } else if (strcmp(topic, Topic_S3_Presenca) == 0 && MensagemRecebida == "S3 - Presença : em rota colisão !!!"){
    Servo1_S3.write(120);
    Servo2_S3.write(90);

  }else if (strcmp(topic, Topic_S2_Presenca2) == 0 && MensagemRecebida == "S2 - Presença 2: Em rota de Colisão !!!"){
    Servo2_S3.write(120);
  }

// luz
    if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Escuro")
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("luz ");
  }
  else if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Claro")
  {
    digitalWrite(ledPin, LOW);
  }
}