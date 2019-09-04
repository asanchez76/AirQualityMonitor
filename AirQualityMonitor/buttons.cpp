/*
 * buttons.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: agustin
 */

#include "Arduino.h"
#include "hardwarepins.h"
#include "buttons.h"

volatile boolean started = false;
volatile boolean ignore_IRQ = false;
volatile unsigned long startTime = 0;
volatile unsigned long endTime = 0;
int totalMicro;
typedef void (*btn_ptr)(void);
btn_ptr btn1_event, btn2_event, btn3_event;

unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers
unsigned long lastDebounceTime = 0;  // the last time the button was 'pressed'

void initButtons(void (*btn1_ptr)(void), void (*btn2_ptr)(void), void (*btn3_ptr)(void))
{
	attachInterrupt (digitalPinToInterrupt (IRQ_BUTTONS_PIN), IRQbuttonPressed, CHANGE);
	//store function pointers to be executed when the timer hits
	btn1_event = btn1_ptr;
	btn2_event = btn2_ptr;
	btn3_event = btn3_ptr;
}

void IRQbuttonPressed()
{
	//The IRQ line receives a square signal, need to calculate a frequency to determine which button was pressed
	//this function is called every time the pulse raises or falls, the oscillator may have slight variations in frequencies, so use ranges.

	if (!ignore_IRQ)
	{
		if (started)
			endTime = micros ();
		else
			startTime = micros ();
		started = !started;
		totalMicro = endTime - startTime;
	}
}


void readButtonsState()
{
	// whatever the reading is at, it's been there for longer than the debounce
	// delay, so take it as the actual current state:
	ignore_IRQ = true;
	if (endTime)
	{
		totalMicro = abs(totalMicro);
		/*
		char buf[100];
		sprintf(buf, "[BTN] %d,%d", millis() - lastDebounceTime, totalMicro);
		Serial.write(buf);
		*/
		if ((millis() - lastDebounceTime) > debounceDelay) {

			if (totalMicro > 200 && totalMicro < 350)
			{
				btn1_event();
			}
			if (totalMicro > 350 && totalMicro < 650)
			{
				btn2_event();
			}
			if (totalMicro > 650 && totalMicro < 950)
			{
				btn3_event();
			}
		}
		startTime = endTime  = 0;
		lastDebounceTime = millis();
	}
	ignore_IRQ = false;
}
