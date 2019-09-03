/*
 * display.h
 *
 *  Created on: Aug 3, 2019
 *      Author: agustin
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <LiquidCrystal.h>

typedef enum
{
    TEMP_PRESSURE = 0,
    AIR_QUALITY = 1,
    SETTINGS = 2,
	PRIORITY = 3//Leave this at last, overrides any display mode and show the message, not an actual visual mode
} displayMode;


//This structure defines data samples displayed on the screen so screen menus can go back to them without waiting for a new sensor reading
typedef struct displayedDataSample
{
	char line1[17];
	char line2[17];
	displayMode mode;
};


void LCDPrint(const char [], const char [], bool , displayMode);
void LCDPrint(const char[], const char[]);
void LCDPrint(const char[], const char[], bool);
void LCDPrint(const char[]);
void LCDInit();
void readPhotoSensor();
void LCDClear();
void SwitchDisplayRegularData();

void DisplayStoredData(displayMode);

void SetDisplayMode(displayMode mode);
displayMode GetDisplayMode();

#endif /* DISPLAY_H_ */
