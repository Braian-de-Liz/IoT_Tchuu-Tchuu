#include <ESP32Servo.h>

// Definição de pinos
#define TRIG_PIN   5   // Pino TRIG do ultrassônico
#define ECHO_PIN   18  // Pino ECHO do ultrassônico
#define BUTTON_PIN  4  // Botão de acionamento
#define LED_PIN    22  // LED verde com resistor no D22
#define SERVO_PIN  23  // Servo motor no D23

Servo servoMotor;

// Variáveis de controle
bool portaoAberto = false;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Botão ligado ao GND

  servoMotor.attach(SERVO_PIN);
  servoMotor.write(0); // Inicia com portão fechado
  digitalWrite(LED_PIN, LOW);
}

long medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms (~5m)
  long distancia = duracao * 0.034 / 2; // cm
  return distancia;
}

void loop() {
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) { // Detecção de clique
    long distancia = medirDistancia();
    Serial.print("Distancia: ");
    Serial.println(distancia);

    if (!portaoAberto && distancia > 20) {
      // Abrir portão
      servoMotor.write(90);
      digitalWrite(LED_PIN, HIGH);
      portaoAberto = true;
      Serial.println("Portao ABERTO");
    } else if (portaoAberto) {
      // Fechar portão
      servoMotor.write(0);
      digitalWrite(LED_PIN, LOW);
      portaoAberto = false;
      Serial.println("Portao FECHADO");
    } else {
      Serial.println("Objeto detectado! Nao pode abrir.");
    }
    delay(300); // debounce
  }

  lastButtonState = buttonState;
}