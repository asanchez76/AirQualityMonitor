#include <Arduino.h>

#include "hardwarepins.h"

#include "network.h"
#include "secrets.h"
#include "aqi.h"
#include "dhtmanager.h"
#include "display.h"
#include "buttons.h"


#include "Timer.h"
//#include <Event.h>

Timer timer;

void button1_pressed()
{
#ifdef DEBUG
	Serial.print("Button 1 pressed");
#endif
	//shift display mode LEFT
	if (GetDisplayMode() == AIR_QUALITY || GetDisplayMode() == TEMP_PRESSURE)
	{
		SwitchDisplayRegularData();
	}
}

void button2_pressed()
{
#ifdef DEBUG
	Serial.print("Button 2 pressed");
#endif
}

void button3_pressed()
{
#ifdef DEBUG
	Serial.print("Button 3 pressed");
#endif
	//shift display mode RIGHT
	if (GetDisplayMode() == AIR_QUALITY || GetDisplayMode() == TEMP_PRESSURE)
	{
		SwitchDisplayRegularData();
	}
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  timer.every(500, readPhotoSensor);
  initButtons(button1_pressed, button2_pressed, button3_pressed);
  timer.every(100, readButtonsState);

  LCDInit();
  LCDPrint("Initializing...");
  delay(3000);

  InitNetwork();

  InitDHTSensor();


  //initialize timed events, functions are fired every x seconds using the timer/event library
  timer.every(5000, performDHTSensorReading);

  timer.every(120000, writeIoTFields);
  timer.every(10000, connectToNetwork);
  connectToNetwork(); //need to call it once here and then refreshed via the timer call to this same function

  InitAQISensor();
  refreshAQISensor();
  performAQISensorReading();
  timer.every(AQI_REFRESH, performAQISensorReading); //only after sensor is initialized

}

void loop() {
	refreshAQISensor();
	timer.update();
}



