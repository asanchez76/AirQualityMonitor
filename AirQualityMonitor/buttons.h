/*
 * buttons.h
 *
 *  Created on: Aug 5, 2019
 *      Author: agustin
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

void initButtons(void (*btn1_ptr)(void), void (*btn2_ptr)(void), void (*btn3_ptr)(void));
void IRQbuttonPressed();
void readButtonsState();

#endif /* BUTTONS_H_ */
