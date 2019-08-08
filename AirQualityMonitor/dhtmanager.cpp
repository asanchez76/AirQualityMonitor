#include "Arduino.h"
#include "dhtmanager.h"
#include "hardwarepins.h"
#include "display.h"
#include "network.h"
#include "floathelper.h"

DHT_Unified dht(DHTPIN, DHTTYPE);

//DHT sensor code goes here
int InitDHTSensor()
{
  // Initialize the DHT device.
  Serial.println("[DHT]Initializing DHT Sensor");
  dht.begin();
  Serial.println("[DHT]Sensor detected");
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  // return the delay between sensor readings based on sensor details.
  return sensor.min_delay;
}

void performDHTSensorReading()
{
  //Serial.println("[DHT]DHT Sensor reading triggered");
  // Delay between measurements.
  //delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;

  dht.temperature().getEvent(&event);
  char tempstrBuf[17];
  char humstrBuf[17];
  if (isnan(event.temperature)) {
    Serial.println("[DHT]Temp sensor reading error");
  }
  else {
    char charBuf[4];
    dtostrf(event.temperature, sizeof(charBuf), 1, charBuf);
    setIoTField(1, event.temperature);
    fmtDouble(event.temperature, 1, charBuf);
    snprintf(tempstrBuf, 17, "Room Temp: %sC", charBuf);
    //LCDPrint(tempstrBuf,"",false);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("[DHT]Humidity sensor reading error");
  }
  else {
	char charBuf[4];
    setIoTField(2, (int)event.relative_humidity);
    snprintf(humstrBuf, 17, "Humidity: %i%%",(int)event.relative_humidity);
    //LCDPrint("",humstrBuf,false);
  }
  LCDPrint(tempstrBuf,humstrBuf,0);


}
