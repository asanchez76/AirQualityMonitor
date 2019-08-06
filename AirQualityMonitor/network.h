#ifndef NETWORK_H
#define NETWORK_H

//network related headers
#include <WiFiEsp.h>


// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#define ESP_BAUDRATE  19200
#else
//using hardware serial, HWSERIAL1 refers to RX1,TX1 on the board
#define ESP_BAUDRATE  115200
#endif

void SetupESP();
void connectToNetwork();
void setEspBaudRate(unsigned long);
void writeIoTFields();
void setIoTField(int, float);
void setIoTField(int, String);

#endif
