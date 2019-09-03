#ifndef NETWORK_H
#define NETWORK_H

//network related headers
#include <WiFiEsp.h>


//using hardware serial, HWSERIAL1 refers to RX1,TX1 on the board
#define ESP_BAUDRATE  115200

void InitNetwork();
void connectToNetwork();
void setEspBaudRate(unsigned long);
void writeIoTFields();
void setIoTField(int, float);
void setIoTStatus(String);

#endif
