    #include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const byte Trigger = **;
const byte echo = **;

const byte ledPin = **;

//colocar os servos aqui



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
    mqtt.publish(Topic_S3_Presenca, "S3 - Presença : em rota colisão !!");
  }else{
    mqtt.publish(Topic_S3_Presenca, "S3 - Presença : Caminho Livre")
  }

  //servo 1






  //servo 2 
}
