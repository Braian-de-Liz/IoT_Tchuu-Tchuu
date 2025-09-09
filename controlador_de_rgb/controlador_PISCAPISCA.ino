const byte pinos_leds[4] = {7, 6, 5, 4};

const byte pino_botao_inc = 8;
const byte pino_botao_dec = 9;


bool estado_botao_inc = false;
bool estado_botao_dec = false;

byte count = 0;
String binario = "";

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(pinos_leds[i], OUTPUT);
  }
  pinMode(pino_botao_inc, INPUT_PULLUP);
  pinMode(pino_botao_dec, INPUT_PULLUP);
}

void loop() {
  estado_botao_inc = !digitalRead(pino_botao_inc);
  if (estado_botao_inc == true && count < 15) {
    delay(100);
    count++;
  }

  estado_botao_dec = !digitalRead(pino_botao_dec);
  if (estado_botao_dec == true && count < 0) {
    delay(100);
    count--;
  }
  binario = String(count, BIN);
  while (binario.length() < 4) {
    binario = "0"   + binario;
  }
  for (int i = 0; i < 4; i++) {
    if (binario[i] == '1') {

      digitalWrite(pinos_leds[i], HIGH);
    }
    else {
      digitalWrite(pinos_leds[i], LOW);
    }
  }
}