# 🌐 IoT com Arduino: Sensores de Distância e Clima

Projeto educacional de Internet das Coisas (IoT) com Arduino, desenvolvido para medir **distância**, **temperatura** e **umidade** em tempo real.  
Ideal para automação residencial, monitoramento ambiental e protótipos de dispositivos inteligentes.

> 🔬 Feito com: Arduino, HC-SR04, DHT11, Serial Monitor

---

## 📦 Funcionalidades

- 📏 Medição de distância com sensor ultrassônico (HC-SR04)
- 🌡️ Leitura de temperatura e umidade com DHT11
- 📊 Saída de dados via Serial Monitor
- 🧠 Classificação de distância: **perto**, **médio**, **longe**

---

## 🛠️ Componentes Utilizados

| Componente | Função |
|----------|--------|
| 🎛️ Arduino (Uno/Mega) | Placa de controle |
| 📏 HC-SR04 | Sensor de distância ultrassônico |
| 🌡️ DHT11 | Sensor de temperatura e umidade |
| 🔌 Jumpers e Protoboard | Conexões |
| 🔋 Fonte de alimentação | Energia para o circuito |

---

## 📄 Código: Sensor de Distância (HC-SR04)

```cpp
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
  else if(distancia > 50 && distancia < 100){
    Serial.println("media");
  }
  else {
    Serial.println("perto");
  }
}
