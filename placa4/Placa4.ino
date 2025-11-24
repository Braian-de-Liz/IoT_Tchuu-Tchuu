#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

// Pinos de direção (não usados ainda, mas declarados)
const byte dir1 = 33;
const byte dir2 = 25;

// Pinos LED de sinalização
const byte vm = 5;
const byte vd = 18;
const byte al = 19;

// Pinos LED RGB 
const byte redPin = 19; 
const byte greenPin = 21; 
const byte bluePin = 18; 

int status = 0; 

void setup() {
  Serial.begin(115200);
  pinMode(vm, OUTPUT);
  pinMode(vd, OUTPUT);
  pinMode(al, OUTPUT);

  // Configura LEDC para PWM no LED RGB
  ledcAttach(redPin, 5000, 8);
  ledcAttach(greenPin, 5000, 8);
  ledcAttach(bluePin, 5000, 8);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado ao Wi-Fi!");

 
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  String clientID = "Tchuu-Tchuu-" + String(random(0xffff), HEX);
  
  while (!mqtt.connect(clientID.c_str())) {
    Serial.print("#");
    delay(200);
  }
  Serial.println("\nConectado ao broker!");

  mqtt.setCallback(callback);
  mqtt.subscribe(Topic_S4_Vel);
}

void loop() {
  // Atualiza LED RGB com base no status
  switch (status) {
    case 0:
      ledcWrite(redPin, 255); ledcWrite(greenPin, 0); ledcWrite(bluePin, 0);
      break;
    case 1:
      ledcWrite(redPin, 0); ledcWrite(greenPin, 255); ledcWrite(bluePin, 0);
      break;
    case 2:
      ledcWrite(redPin, 0); ledcWrite(greenPin, 190); ledcWrite(bluePin, 255);
      break;
    case 10:
      ledcWrite(redPin, 150); ledcWrite(greenPin, 255); ledcWrite(bluePin, 0);
      break;
    case 20:
      ledcWrite(redPin, 150); ledcWrite(greenPin, 0); ledcWrite(bluePin, 255);
      break;
    default:
  
      ledcWrite(redPin, 0); ledcWrite(greenPin, 0); ledcWrite(bluePin, 255);
      delay(500);
      ledcWrite(redPin, 0); ledcWrite(greenPin, 0); ledcWrite(bluePin, 0);
      delay(500);
      return; 
  }

  mqtt.loop();
}

void callback(char *topic, byte *payload, unsigned long length)
{

  String MensagemRecebida = "";
  for (int i = 0; i < length; i++)
  { // Pega cada letra de payload e junta na mensagem
    MensagemRecebida += (char)payload[i];
  }

  if (strcmp(topic, Topic_S4_Vel) == 0){
    Serial.println(Velocidade);
  }

// usar a velocidade para alguma coisa
}