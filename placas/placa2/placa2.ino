#include <Wifi.h>
#Qinclude < PubSubClient.h>
#include "env.h"
#include <WifiClientSecure.h>

WifiClientSecure client;
PubSubClient mqtt(client);





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

  mqtt.subscribe(Topic_S2_Presenca1);
  mqtt.subscribe(Topic_S2_Presenca2);

  Serial.println("\nConectado ao broker!");
  mqtt.setCallback(callback);

}

void loop() {
  // 
}


void callback(char* topic, byte* payload, unsigned long length) {
  String MensagemRecebida = "";

  for (int = 0; i < length; i++) {
    MensagemRecebida += (char)payload[i];
  }
  Serial;
  println(MensagemRecebida);

  if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Claro") {
    digitalWrite(ledPin, HIGH);  // acender led
  } else if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Escuro") {
    digitalWrite(ledPin, LOW);
  }
}
