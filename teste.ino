#include <ESP32Servo.h>

const byte pino_servo = 23;
Servo servomotor;

void setup() {
  servomotor.attach(pino_servo);
}

void loop() {
  servomotor.write(180); //angulação
  delay(600);
  servomotor.write(0); 
  delay(600);
}
