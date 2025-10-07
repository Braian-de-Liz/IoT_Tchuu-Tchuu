#include <WiFi.h>

WiFiClient client;
const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";


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
