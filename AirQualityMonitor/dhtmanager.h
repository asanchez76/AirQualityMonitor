#ifndef DHTMANAGER_H
#define DHTMANAGER_H

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

int InitDHTSensor();
void performDHTSensorReading();

#endif