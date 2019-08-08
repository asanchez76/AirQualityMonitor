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
typedef void (*btn_ptr)(void);
btn_ptr btn1_event, btn2_event, btn3_event;

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



		char buf[100];
		sprintf(buf, "[BTN] readButtonState: %d pulseCount", pulseCount);
		Serial.write(buf);



		if (totalMicro > 250 && totalMicro < 350)
		{

			btn1_event();
		}
		if (totalMicro > 500 && totalMicro < 650)
		{

			btn2_event();
		}
		if (totalMicro > 800 && totalMicro < 950)
		{

			btn3_event();
		}

		endTime = 0;pulseCount=0;

	}

}
