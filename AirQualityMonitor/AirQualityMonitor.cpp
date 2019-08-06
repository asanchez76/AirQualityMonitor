#include <Arduino.h>

#include "hardwarepins.h"

#include "network.h"
#include "secrets.h"
#include "aqi.h"
#include "dhtmanager.h"
#include "display.h"

#include <Timer.h>
#include <Event.h>

Timer timer;


void setup() {
  Serial.begin(115200);
  int photoSensorEvent = timer.every(500, readPhotoSensor);
  LCDInit();
  LCDPrint("Initializing...");
  delay(3000);
  //Agustins: TODO disconnected until sensor restablished
  //SetupESP();

  int dhtsensordelay = SetupDHT();


  //initialize timed events, functions are fired every x seconds using the timer/event library
  int DHTreadingEvent = timer.every(5000, performDHTSensorReading);

  //Agustins: TODO disconnected until sensor restablished
  //int writingEvent = timer.every(60000, writeIoTFields);
  //int connectionEvent = timer.every(10000, connectToNetwork);
  //connectToNetwork(); //need to call it once here and then refreshed via the timer call to this same function

  SetupAQISensor();
  int aqiTimerid = timer.every(AQI_REFRESH, performAQISensorReading); //only after sensor is initialized
}

void loop() {
  timer.update();
}

