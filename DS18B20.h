/*
 * DS18B20.h
 *
 * Created: 09.10.2022 14:33:49
 *  Author: Никита
 */ 


#ifndef WIREE_H_
#define WIREE_H_
#define F_CPU 8000000L

#include "main.h"
#define NOID 0xCC //Пропустить идентификацию
#define T_CONVERT 0x44 //Код измерения температуры
#define READ_DATA 0xBE //Передача байтов ведущему
#define READ_ROM 0x33//чтение ROM
#define SEARCH_ROM 0xF0//поиск ROM
#define MATCH_ROM 0x55//соответствие ROM
#define PORTTEMP PORTD //вывод
#define DDRTEMP DDRD //настройка на вход-0/выход-1
#define PINTEMP PIND //ввод
#define BITTEMP 5

char dt_testdevice(void);
void dt_sendbit(char bt);
void dt_sendbyte(unsigned char bt);
char dt_readbit(void);
unsigned char dt_readbyte(void);
int dt_check(void);
char converttemp (unsigned int tt);

#endif /* WIREE_H_ */