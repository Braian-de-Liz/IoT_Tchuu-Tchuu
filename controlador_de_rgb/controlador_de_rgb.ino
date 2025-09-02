const byte redPin = 27;
const byte greenPin = 26;
const byte bluePin = 25;


void setup() {
  // saidas analógicas configuradas para o ESP32
  ledcAttch(redPin, 5000, 8);
  ledcAttch(greenPin, 5000, 8);
  ledcAttch(bluePin, 5000, 8);
}

void loop() {
  ledcWrie(redPin, 255);
  ledcWrie(greenPin, 0);
  ledcWrie(bluePin, 0);
  delay(100);

  ledcWrie(redPin, 0);
  ledcWrie(greenPin, 255);
  ledcWrie(bluePin, 0);
  delay(100);

  ledcWrie(redPin, 0);
  ledcWrie(greenPin, 0);
  ledcWrie(bluePin, 255);
  delay(100);
}