#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topic = "TchuuTchuu";

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200); // configura a placa para monstrar na tela 
  WiFi.begin(SSID, PASS); //tenta conectar na rede
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
  String mensagem = ""; // cria dentro do loop para recriala toda vez que rodar o programa
  if(Serial.available()> 0 ){  // checagem se tem palavras na "fila de espera", se tiver alguma palavra faça
   mensagem = Serial.readStringUntil('\n');    // salva a palavra digitada / lê a mensagem do usuario
    mensagem = "Guilherme:" + mensagem; 
  // Serial.print("A mensagem foi: ");
  // Serial.println(mensagem);
    mqtt.publish("TchuuTchuu", mensagem.c_str());
  };  

  mqtt.loop();
}

void callback(char* topic, byte* payload, unsigned long length) {   // perguntar ao professor pq unsigned long ou inves de unsigned int
  String MensagemRecebida = "";

  for(int i = 0; i < length; i++) { //Pega cada letra de payload e junta na mensagem
  MensagemRecebida += (char) payload[i];
 }
  Serial.println(MensagemRecebida);

  if (MensagemRecebida == "1"){ // "" Pois está lendo a string
    digitalWrite(2,HIGH);
    Serial.prinlt("Sensor Ligado")
  }else{
    digitalWrite(2,LOW); 
    Serial.println("Sensor Desligado")
  };

 
}
