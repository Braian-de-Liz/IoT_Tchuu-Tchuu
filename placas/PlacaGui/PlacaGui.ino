#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>
#include <DHT.h>

#define pinLDR 34  // o LRD, luz

#define pinDHT 4       // pino do DHT , sensor de temp
#define DHTTYPE DHT11  // DHT , sensor tem temp e umidade
DHT dht(pinDHT, DHTTYPE);

const byte ledPin = 19;

WiFiClientSecure client;
PubSubClient mqtt(client);


//Configurar o LDR
const byte LDR_PIN = 34;

// Sensor de distancia
const byte TRIGGER_PIN = 22;
const byte ECHO_PIN = 23;


//Pino RGB

const byte PWM_CHANNEL_LED_R = 14 ;
const byte PWM_CHANNEL_LED_G = 26 ;
const byte PWM_CHANNEL_LED_B = 25 ;


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
  mqtt.subscribe(Topic_S1_Temp);
  mqtt.subscribe(Topic_S1_Umid);
  mqtt.subscribe(Topic_S1_Presenca);
  mqtt.subscribe(Topic_S1_Iluminacao);

  Serial.println("\nConectado ao broker!");
  mqtt.setCallback(callback);


  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ledPin, OUTPUT);
}

long lerDistancia() {
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


  String mensagem = "";                       // cria dentro do loop para recriala toda vez que rodar o programa
  if (Serial.available() > 0) {               // checagem se tem palavras na "fila de espera", se tiver alguma palavra faça
    mensagem = Serial.readStringUntil('\n');  // salva a palavra digitada / lê a mensagem do usuario
    mensagem.trim();
    // Serial.print("A mensagem foi: ");
    // Serial.println(mensagem);

    mqtt.publish(Topic_S1_Iluminacao, mensagem.c_str());
  };
  mqtt.loop();
  delay(500);

  // Publicação da Iluminação

  int luz = map(analogRead(pinLDR), 0, 4095, 0, 100);
  if (luz > 50) {
    mqtt.publish(Topic_S1_Iluminacao, "Claro");
  } else {
    mqtt.publish(Topic_S1_Iluminacao, "Escuro");
  }

  mqtt.loop();
  delay(500);

  // Publicação da Presença
  long distancia = lerDistancia();

  if (distancia < 50) {
    mqtt.publish(Topic_S1_Presenca, "Em rota de Colisão !!!");
  } else {
    mqtt.publish(Topic_S1_Presenca, "Caminho Livre");
  }

  mqtt.loop();
  delay(500);



  //  Publicação da temperatura

  float t = dht.readTemperature();
  if (t > 28) {
    mqtt.publish(Topic_S1_Temp, "A temperatura está elevada");  //quente
  } else {
    mqtt.publish(Topic_S1_Temp, "Regular");  // normal
  }

  mqtt.loop();
  delay(500);


  // Publicação da Umidade
  float u = dht.readHumidity();
  mqtt.publish(Topic_S1_Umid, String(u).c_str());
}

// configuração do LED RGB

void statusLED(byte status) {
    turnOffLEDs();
    switch (status) {
    case 254:  // Erro (Vermelho)
        setLEDColor(255, 0, 0);
        break;
    case 1:  // Conectando ao Wi-Fi (Amarelo)
        setLEDColor(150, 255, 0);
        break;
    case 2:  // Conectando ao MQTT (Rosa)
        setLEDColor(150, 0, 255);
        break;
    case 3:  // Movendo para frente (Verde)
        setLEDColor(0, 255, 0);
        break;
    case 4:  // Movendo para trás (Ciano)
        setLEDColor(0, 255, 255);
        break;
    default:
        for (byte i = 0; i < 4; i++) {
            setLEDColor(0, 0, 255);  // erro no status (pisca azul)
            delay(100);
            turnOffLEDs();
            delay(100);
        }
        break;
    }
}
void turnOffLEDs() { setLEDColor(0, 0, 0); }
void setLEDColor(byte r, byte g, byte b) {
    ledcWrite(PWM_CHANNEL_LED_R, r);
    ledcWrite(PWM_CHANNEL_LED_G, g);
    ledcWrite(PWM_CHANNEL_LED_B, b);
}



// Receber topico da iluminação
void callback(char* topic, byte* payload, unsigned long length) {

  String MensagemRecebida = "";
  for (int i = 0; i < length; i++) {  //Pega cada letra de payload e junta na mensagem
    MensagemRecebida += (char)payload[i];
  }
  Serial.println(MensagemRecebida);

  if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Claro") {
    digitalWrite(ledPin, HIGH);  // acender led
  } else if (strcmp(topic, Topic_S1_Iluminacao) == 0 && MensagemRecebida == "Escuro") {
    digitalWrite(ledPin, LOW);
  }
}
