#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  Serial.println("Conectado a rede");

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
  

}
