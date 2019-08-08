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

void LCDPrint(const char line1[], const char line2[], bool clean)
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
}

void readPhotoSensor()
{
	//takes a reading of the photocell and reacts by sending a PWM value to the display VCC pin
	int resistorvalue = analogRead(pResistor);

	/*
	if (resistorvalue > 100){
		digitalWrite(ledPin, HIGH);
	}
	else{
		digitalWrite(ledPin, LOW);
	}
	*/
	int output = resistorvalue / 5;
	analogWrite(lcdBacklightPin,output);

	/*
	char buf[100];
	sprintf(buf, "[DISP] readPhotoSensor triggered with value: %d, dim to:%d\r\n", resistorvalue, output);
	Serial.write(buf);
	*/
}
