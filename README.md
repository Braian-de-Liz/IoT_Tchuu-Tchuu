# 🌐 Sistemas de Sensores IoT para Ferroramas

Projeto de Internet das Coisas (IoT) desenvolvido para integrar **sensores inteligentes** em ambientes de ferroramas, com o objetivo de **monitorar e automatizar** aspectos físicos como distância, temperatura e umidade.

Este projeto faz parte do ecossistema do **[TCHUU-TCHUU](https://github.com/Braian-de-Liz/Tchuu-Tchuu)** — sistema de gerenciamento de ferroramas desenvolvido por alunos do SESI de Referência.

> 🛠️ **Plataforma principal: ESP32** (não Arduino)  
> 📡 Comunicação via Wi-Fi, ideal para integração com backends e dashboards em tempo real

---

## 🚆 Aplicação no TCHUU-TCHUU

Esses sensores serão usados para:
- 📏 Detectar obstáculos nos trilhos (evitando colisões)
- 🌡️ Monitorar temperatura dentro do cenário (segurança e conforto)
- 💧 Controlar umidade em efeitos climáticos (chuva, névoa)
- 🔄 Enviar dados em tempo real para o dashboard do sistema

👉 Tudo integrado ao **backend Node.js + WebSocket** do projeto.

---

## 📦 Funcionalidades

- 📏 Medição de distância com sensor ultrassônico (HC-SR04)
- 🌡️ Leitura de temperatura e umidade com DHT11
- 📶 Transmissão de dados via Wi-Fi (ESP32)
- 📊 Envio automático para servidores web ou dashboard
- 🧠 Classificação de distância: **perto**, **médio**, **longe**

---

## 🛠️ Componentes Utilizados

| Componente | Função |
|----------|--------|
| 📡 **ESP32** | Placa principal com Wi-Fi e Bluetooth (substitui o Arduino) |
| 📏 HC-SR04 | Sensor de distância ultrassônico |
| 🌡️ DHT11 | Sensor de temperatura e umidade |
| 🔌 Jumpers e Protoboard | Conexões |
| 🔋 Fonte de alimentação | Energia estável para o circuito |

> ✅ O **ESP32** é o coração do sistema: permite comunicação direta com o backend, sem depender de Serial ou computador.

---

## 📄 Código: Sensor de Distância (HC-SR04) – ESP32

```CPP
const byte echo_pin = 22;
const byte trigg_pin = 23;
int distancia = 0;
unsigned long tempo = 0;

void setup() {
  pinMode(echo_pin, INPUT);
  pinMode(trigg_pin, OUTPUT);
  Serial.begin(115200); // Velocidade maior para Wi-Fi
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
