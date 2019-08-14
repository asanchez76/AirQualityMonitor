/*
 * display.cpp
 *
 *  Created on: Aug 3, 2019
 *      Author: agustin
 */

#include "display.h"
#include "hardwarepins.h"
#include "Arduino.h"


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(lcd_pin_rs, lcd_pin_en, lcd_pin_d4, lcd_pin_d5, lcd_pin_d6, lcd_pin_d7);

displayMode currentMode;

displayedDataSample* storedLastDataSamples = malloc((PRIORITY - TEMP_PRESSURE)*sizeof(storedLastDataSamples));

void SetDisplayMode(displayMode mode)
{
	currentMode = mode;
}

displayMode GetDisplayMode()
{
	return currentMode;
}

void SwitchDisplayRegularData()
{
	//If I'm displaying these two modes, side keys switch between them, to go to settings the user needs to press the middle button (2)
	switch (GetDisplayMode())
	{
	case TEMP_PRESSURE:
		SetDisplayMode(AIR_QUALITY);
		DisplayStoredData(AIR_QUALITY);
		break;
	case AIR_QUALITY:
		SetDisplayMode(TEMP_PRESSURE);
		DisplayStoredData(TEMP_PRESSURE);
		break;
	}
}

void DisplayStoredData(displayMode mode)
{
	displayedDataSample sample = storedLastDataSamples[mode];
	lcd.setCursor(0,0);
	lcd.print(sample.line1);
	lcd.setCursor(0,1);
	lcd.print(sample.line2);
}

void StoreDisplayData(const char line1[], const char line2[], displayMode mode)
{
	//store display lines in an array for later display, this gets refreshed with every sensor reading, so when switching display modes the last available value is displayed
	displayedDataSample sample;
	strcpy(sample.line1, line1);
	strcpy(sample.line2, line2);
	sample.mode = mode;
	storedLastDataSamples[mode] = sample;
}

void LCDPrint(const char line1[], const char line2[], bool clean, displayMode mode)
{
	//store displayed data in memory for display context switching
	StoreDisplayData(line1, line2, mode);
	//display this data only if on the correct mode, otherwise it will just store it for later, unless its PRIORITY
	if (mode == GetDisplayMode() || mode == PRIORITY)
	{
		if (strlen(line1)>0)
		{
			if (clean)
			{
			  lcd.setCursor(0,0);
			  lcd.print("                ");
			}
			lcd.setCursor(0,0);
			lcd.print(line1);
		}
		if (strlen(line2)>0)
		{
			if (clean)
			{
				lcd.setCursor(0,1);
				lcd.print("                ");
			}
			lcd.setCursor(0,1);
			lcd.print(line2);
		}
	}
}

void LCDPrint(const char line1[], const char line2[], bool clean)
{
	LCDPrint(line1, line2, clean, PRIORITY);
}

void LCDPrint(const char line1[], const char line2[])
{
	LCDPrint(line1, line2, true);
}

void LCDPrint(const char line1[])
{
	LCDPrint(line1,"");
}

void LCDClear()
{
	lcd.clear();
}

void LCDInit()
{
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);

	pinMode(lcdBacklightPin, OUTPUT);  // Set lepPin - 9 pin as an output
	pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
	readPhotoSensor();
	SetDisplayMode(TEMP_PRESSURE); //the default mode when the device starts, will show temperature & humidity readings.
}

void readPhotoSensor()
{
	//takes a reading of the photocell and reacts by sending a PWM value to the display VCC pin
	int resistorvalue = analogRead(pResistor);

	int output = resistorvalue / 5;
	analogWrite(lcdBacklightPin,output);
}
