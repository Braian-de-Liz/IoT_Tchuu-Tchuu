const int LDR_PIN = 34;        
const int POT_COR_PIN = 35;    
const int POT_INT_PIN = 32;    
const int LED_R = 25; 
const int LED_G = 26;  
const int LED_B = 27;  

const int LIMIAR_ESCURO = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  analogWrite(LED_R, 0);
  analogWrite(LED_G, 0);
  analogWrite(LED_B, 0);
}

void loop() {
  int luminosidade = analogRead(LDR_PIN);
  int potCor = analogRead(POT_COR_PIN);
  int potInt = analogRead(POT_INT_PIN);

  Serial.print("Luminosidade: ");
  Serial.print(luminosidade);
  Serial.print(" | PotCor: ");
  Serial.print(potCor);
  Serial.print(" | PotInt: ");
  Serial.println(potInt);

  if (luminosidade < LIMIAR_ESCURO) {

    int intensidade = map(potInt, 0, 4095, 0, 255);
    int nivelVermelho = map(potCor, 0, 4095, 0, intensidade);
    int nivelAzul = intensidade - nivelVermelho;

    analogWrite(LED_R, nivelVermelho);
    analogWrite(LED_G, 0);  // não usamos verde
    analogWrite(LED_B, nivelAzul);

  } else {
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);
  }

  delay(100);
}
