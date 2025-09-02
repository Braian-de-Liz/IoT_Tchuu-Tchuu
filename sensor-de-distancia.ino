const byte echo_pin = 22;
const byte trigg_pin = 23;
int distancia = 0;
unsigned long tempo = 0;

void setup() {
  pinMode(echo_pin, INPUT);
  pinMode(trigg_pin, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  digitalWrite(trigg_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg_pin, LOW);
  tempo = pulseIn(echo_pin, HIGH);
  distancia = ((tempo * 340.29)/2)/10000;
  Serial.println(distancia);
  delay(500);

  if (distancia > 100) {
    Serial.println("longe");

  }
  else if(distancia >50 & distancia < 100){
    Serial.println("media");
  }
  else {
    Serial.println("perto");
  }
}
