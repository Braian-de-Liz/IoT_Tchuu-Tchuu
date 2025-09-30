#include <WiFi.h>

WiFiClient client;
const String SSID = "Nome da Rede";
const String PASS = "senha da Rede";


void setup() {
  Serial.begin(115200);
  
  CLient.begin(SSID, PASS);
  Serial.println("Conectado a rede");

  while(client.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);

  }

}

void loop() {
  

}
