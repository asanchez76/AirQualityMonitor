/*
 * buttons.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: agustin
 */

#include "Arduino.h"
#include "hardwarepins.h"
#include "buttons.h"

volatile boolean started;
volatile unsigned long startTime;
volatile unsigned long endTime;
volatile unsigned long pulseCount = 0;

void initButtons()
{
	attachInterrupt (digitalPinToInterrupt (IRQ_BUTTONS_PIN), IRQbuttonPressed, CHANGE);
}

void IRQbuttonPressed()
{
	//The IRQ line receives a square signal, need to calculate a frequency to determine which button was pressed
	//this function is called every time the pulse raises or falls, the oscillator may have slight variations in frequencies, so use ranges.

	//Serial.print("IRQbuttonPressed()");
	pulseCount++;
	if (started)
	    endTime = micros ();
	else
	    startTime = micros ();
	started = !started;
}

void readButtonsState()
{
	if (endTime)
	{
		//pulse train ended
		unsigned long totalMicro = endTime - startTime;
		endTime = 0;

		char buf[100];
		sprintf(buf, "[BTN] readButtonState: %d microseconds", totalMicro);
		Serial.write(buf);
	}

}
