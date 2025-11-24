#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const byte Trigger_1 = 5;
const byte echo_1 = 18;

const byte Trigger_2 = 4;
const byte echo_2 = 19;

const byte ledPin = 21;

void setup()
{
  Serial.begin(115200);             // configura a placa para monstrar na tela
  WiFi.begin(WIFI_SSID, WIFI_PASS); // tenta conectar na rede
  Serial.println("Conectado a rede");
  client.setInsecure();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }

  Serial.println("Conectado com sucesso!");

  mqtt.setServer(BROKER_URL, BROKER_PORT);
  String clientID = "S1-"; // criando um id unico e aleatorio
  clientID += String(random(0xffff), HEX);

  while (mqtt.connect(clientID.c_str()) == 0)
  { // tentativa de conetar,  0 = não conectado, 1 = conectado
    Serial.print(".");
    delay(200);
  }

  // topicos

  mqtt.subscribe(Topic_S2_Presenca1);
  mqtt.subscribe(Topic_S2_Presenca2);
  mqtt.subscribe(Topic_S1_Iluminacao);

  Serial.println("\nConectado ao broker!");
  mqtt.setCallback(callback);

  pinMode(Trigger_1, OUTPUT);
  pinMode(Trigger_2, OUTPUT);
  pinMode(echo_1, INPUT);
  pinMode(echo_2, INPUT);
}

long lerDistancia1()
{
  digitalWrite(Trigger_1, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger_1, LOW);
  long duracao = pulseIn(echo_1, HIGH);
  long distancia1 = duracao * 349.24 / 2 / 10000;
  return distancia1;
}

long lerDistancia2()
{
  digitalWrite(Trigger_2, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger_2, LOW);
  long duracao = pulseIn(echo_2, HIGH);
  long distancia2 = duracao * 349.24 / 2 / 10000;
  return distancia2;
}

void loop()
{
  long distancia1 = lerDistancia1();
  long distancia2 = lerDistancia2();

  if (distancia1 < 50)
  {
    mqtt.publish(Topic_S2_Presenca1, "S2 - Presença 1:  Em rota de Colisão !!!");
  }
  else
  {
    mqtt.publish(Topic_S2_Presenca1, "S2 - Presença 1: Caminho Livre");
  }

  if (distancia2 < 50)
  {
    mqtt.publish(Topic_S2_Presenca2, "S2 - Presença 2: Em rota de Colisão !!!");
  }
  else
  {
    mqtt.publish(Topic_S2_Presenca2, "S2 - Presença 2: Caminho Livre");
  }
}

void callback(char *topic, byte *payload, unsigned long length)
{

  String MensagemRecebida = "";
  for (int i = 0; i < length; i++)
  { // Pega cada letra de payload e junta na mensagem
    MensagemRecebida += (char)payload[i];
  }
  Serial.printf("Msg:%s / Topic%s\n", MensagemRecebida, topic);

  if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Escuro")
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("luz ");
  }
  else if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Claro")
  {
    digitalWrite(ledPin, LOW);
    Serial.println("chegou aqui");
  }
}
