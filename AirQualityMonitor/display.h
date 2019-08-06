/*
 * display.h
 *
 *  Created on: Aug 3, 2019
 *      Author: agustin
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "hardwarepins.h"
#include <LiquidCrystal.h>


void LCDPrint(const char[], const char[]);
void LCDPrint(const char[], const char[], bool);
void LCDPrint(const char[]);
void LCDInit();
void readPhotoSensor();

#endif /* DISPLAY_H_ */
