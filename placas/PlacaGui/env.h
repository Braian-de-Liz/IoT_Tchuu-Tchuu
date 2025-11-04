#ifndef ENV_H
#define ENV_H

#define WIFI_SSID "FIESC_IOT_EDU"
#define WIFI_PASS "8120gv08"


#define BROKER_URL "hivemq.webclient.1762258808551"
#define BROKER_PORT 8883
// -------Login criado para cada usuario no HiveMQ -------
#define BROKER_USR_ID "Placa_1"
#define BROKER_USR_PASS "Aa_11111"

//    ----A partir daqui depende da placa----

// #define TOPIC_PRESENCE1  "Tchuu-Tchuu/S4/Presence1" // nome do topico

#define Topic_S1_Temp "Tchuu-Tchuu/S1/Temp"
#define Topic_S1_Umid  "Tchuu-Tchuu/S1/Umid"
#define Topic_S1_Presenca "Tchuu-Tchuu/S1/Presenca"
#define Topic_S1_Iluminacao "Tchuu-Tchuu/S1/Ilumina"

#endif 