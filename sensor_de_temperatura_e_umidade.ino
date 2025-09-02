#include "DHT.h"


#define DHTPIN 18
#define DHTYPE DHT11

DHT sensorTempumi(DHTPIN, DHTYPE);

float umidade = 0;
float temperatura = 0;

// ELIAS

void setup() {
  Serial.begin(9600);
  sensorTempumi.begin();
}

void loop() {
  umidade = sensorTempumi.readHumidity();
  temperatura = sensorTempumi.readTemperature();

  Serial.printl(temperatura);
  Serial.printl("  !  ");
  Serial.printl(umidade);
  delay(2000);
}
