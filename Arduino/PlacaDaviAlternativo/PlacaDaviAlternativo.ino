#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>

// --- Configuração LED RGB ---
#define LED_PIN_R 16
#define LED_PIN_G 17
#define LED_PIN_B 18

#define PWM_CHANNEL_LED_R 0
#define PWM_CHANNEL_LED_G 1
#define PWM_CHANNEL_LED_B 2
#define PWM_RESOLUTION 8  // 0–255

// --- Pino do sensor ---
const int PINO_SENSOR = 4;

// --- Protótipos de funções ---
void statusLED(byte status);
void turnOffLEDs();
void setLEDColor(byte r, byte g, byte b);

WiFiClientSecure client;
PubSubClient mqtt(client);

// ========================
// Funções do LED RGB
// ========================

void setLEDColor(byte r, byte g, byte b) {
    ledcWrite(PWM_CHANNEL_LED_R, r);
    ledcWrite(PWM_CHANNEL_LED_G, g);
    ledcWrite(PWM_CHANNEL_LED_B, b);
}

void turnOffLEDs() {
    setLEDColor(0, 0, 0);
}

void statusLED(byte status) {
    turnOffLEDs();
    switch (status) {
        case 254:  // Erro / conectando MQTT (Vermelho)
            setLEDColor(255, 0, 0);
            break;
        case 1:  // Conectando ao Wi-Fi (Amarelo)
            setLEDColor(150, 255, 0);
            break;
        case 2:  // Conectado ao MQTT (Rosa)
            setLEDColor(150, 0, 255);
            break;
        case 3:  // Movendo para frente (Verde)
            setLEDColor(0, 255, 0);
            break;
        case 4:  // Movendo para trás (Ciano)
            setLEDColor(0, 255, 255);
            break;
        default:  // Estado desconhecido (pisca azul)
            for (byte i = 0; i < 4; i++) {
                setLEDColor(0, 0, 255);
                delay(100);
                turnOffLEDs();
                delay(100);
            }
            break;
    }
}

// ========================
// Callback MQTT
// ========================

void callback(char* topic, byte* payload, unsigned int length) {
    String cmd = "";
    for (int i = 0; i < length; i++) {
        cmd += (char)payload[i];
    }
    cmd.trim();
    Serial.print("Comando recebido: ");
    Serial.println(cmd);

    if (cmd == "parar") {
        turnOffLEDs();
    }
    else if (cmd.startsWith("frente,")) {
        statusLED(3);  // Verde
    }
    else if (cmd.startsWith("re,")) {
        statusLED(4);  // Ciano
    }
}

// ========================
// Setup
// ========================

void setup() {
    Serial.begin(115200);
    pinMode(PINO_SENSOR, INPUT);

    // Inicializa PWM para LED RGB
    ledcSetup(PWM_CHANNEL_LED_R, 5000, PWM_RESOLUTION);
    ledcSetup(PWM_CHANNEL_LED_G, 5000, PWM_RESOLUTION);
    ledcSetup(PWM_CHANNEL_LED_B, 5000, PWM_RESOLUTION);

    ledcAttachPin(LED_PIN_R, PWM_CHANNEL_LED_R);
    ledcAttachPin(LED_PIN_G, PWM_CHANNEL_LED_G);
    ledcAttachPin(LED_PIN_B, PWM_CHANNEL_LED_B);

    statusLED(1); // Amarelo: conectando ao Wi-Fi

    // Conecta ao WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Conectando ao WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(200);
    }
    Serial.println("\nWiFi OK");

    client.setInsecure();
    mqtt.setServer(BROKER_URL, BROKER_PORT);
    mqtt.setCallback(callback);

    String clientID = "DOIT_TREM_" + String(random(0xffff), HEX);
    statusLED(254); // Vermelho: conectando ao MQTT
    while (!mqtt.connect(clientID.c_str(), BROKER_USER, BROKER_PASS)) {
        Serial.print(".");
        delay(200);
    }
    Serial.println("\nMQTT OK");
    statusLED(2); // Rosa: conectado!

    mqtt.subscribe(TOPIC_PRESENCE1);
    mqtt.publish("trem/status", "Trem online");
}

// ========================
// Loop principal
// ========================

void loop() {
    mqtt.loop();

    // Publica status do sensor a cada 2 segundos
    static unsigned long ultimoEnvio = 0;
    if (millis() - ultimoEnvio > 2000) {
        String status = (digitalRead(PINO_SENSOR)) ? "movimento_detectado" : "livre";
        mqtt.publish("trem/status", status.c_str());
        ultimoEnvio = millis();
    }
}