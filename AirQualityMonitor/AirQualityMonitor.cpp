#include <Arduino.h>

#include "hardwarepins.h"

#include "network.h"
#include "secrets.h"
#include "aqi.h"
#include "dhtmanager.h"
#include "display.h"
#include "buttons.h"

#include <Timer.h>
#include <Event.h>

Timer timer;

void button1_pressed()
{
	Serial.print("Button 1 pressed");
}
void button2_pressed()
{
	Serial.print("Button 2 pressed");
}
void button3_pressed()
{
	Serial.print("Button 3 pressed");
}

void setup() {
  Serial.begin(115200);
  int photoSensorEvent = timer.every(500, readPhotoSensor);
  initButtons(button1_pressed, button2_pressed, button3_pressed);
  int buttonTimerEvent = timer.every(100, readButtonsState);

  LCDInit();
  LCDPrint("Initializing...");
  delay(3000);

  InitNetwork();

  int dhtsensordelay = InitDHTSensor();


  //initialize timed events, functions are fired every x seconds using the timer/event library
  int DHTreadingEvent = timer.every(5000, performDHTSensorReading);

  int writingEvent = timer.every(120000, writeIoTFields);
  int connectionEvent = timer.every(10000, connectToNetwork);
  connectToNetwork(); //need to call it once here and then refreshed via the timer call to this same function

  InitAQISensor();
  int aqiTimerid = timer.every(AQI_REFRESH, performAQISensorReading); //only after sensor is initialized

}

void loop() {
	refreshAQISensor();
	timer.update();
}



