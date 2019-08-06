/*
 * hardwarepins.h
 *
 *  Created on: Aug 3, 2019
 *      Author: agustin
 */

#ifndef HARDWAREPINS_H_
#define HARDWAREPINS_H_

#include "Arduino.h"

/* All PIN and hardware definitions go in this section for ease of reading/changing
 *  avoid including PIN definitions in other files
 */

#define DHTPIN 8     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11

/* Connect the DSM501 sensor as follow
 * https://www.elektronik.ropla.eu/pdf/stock/smy/dsm501.pdf
 * 1 green vert - Not used
 * 2 yellow jaune - Vout2 - 1 microns (PM1.0)
 * 3 white blanc - Vcc
 * 4 red rouge - Vout1 - 2.5 microns (PM2.5)
 * 5 black noir - GND
*/
#define DUST_SENSOR_DIGITAL_PIN_PM10  6        // DSM501 Pin 2 of DSM501 (Yellow)
#define DUST_SENSOR_DIGITAL_PIN_PM25  7        // DSM501 Pin 4 (red)

//LCD Pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

const int AQI_REFRESH = 300000L;

const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int ledPin = 13;       // Led pin at Arduino pin 9


#endif /* HARDWAREPINS_H_ */
