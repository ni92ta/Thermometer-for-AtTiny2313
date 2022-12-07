/*
 * main.h
 *
 * Created: 09.10.2022 14:00:57
 *  Author: Никита
 */ 


#ifndef MAIN_H_
#define MAIN_H_
#include <xc.h>
#include <avr/interrupt.h>
#define F_CPU 8000000L
#include <util/delay.h>
#include "DS18B20.h"

unsigned char chhh;
void dela_y (unsigned char i);
//void out_led (void);

#endif /* MAIN_H_ */