const byte pinos_led[4] = {4,5,6,7};
const byte pino_botao_1 = 8;
const byte pino_botao_2 = 9;
const byte pino_botao_3 = 10;
const byte pino_botao_4 = 11;
const byte pino_pot = A5; // Potenciômetro

bool estado_botao_1 = 0;
bool estado_botao_2 = 0;
bool estado_botao_3 = 0;
bool estado_botao_4 = 0;
int valor_pot = 0; //Potenciômetro

int tempo = 0; //Tempo entre os leds

void setup() {
  for(int i = 0; i < 4; i++){
    pinMode(pinos_led[i], OUTPUT);
  }
  pinMode(pino_botao_1, INPUT_PULLUP);
  pinMode(pino_botao_2, INPUT_PULLUP);
  pinMode(pino_botao_3, INPUT_PULLUP);
  pinMode(pino_botao_4, INPUT_PULLUP);
  pinMode(pino_pot, INPUT); // Potenciômetro
  Serial.begin(9600);
}

void loop() {
  valor_pot = analogRead(pino_pot);
  tempo = map(valor_pot,0,1023,200,1000);
  
  estado_botao_1 = !digitalRead(pino_botao_1);
  if(estado_botao_1 == true){
    delay(100);
    for(int i= 0; i< 4;i++){
      digitalWrite(pinos_led[i], HIGH);
      delay(tempo);
    }
  }
  
}
