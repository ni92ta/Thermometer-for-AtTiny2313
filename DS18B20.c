﻿/*
 * DS18B20.c
 *
 * Created: 09.10.2022 14:33:30
 *  Author: Никита
 */ 
#include "DS18B20.h"
#define F_CPU 8000000L
char i;
char bt;
char dt;
char dt_testdevice(void) //dt - digital termomether | определим, есть ли устройство на шине
{
	//	char stektemp=SREG;// сохраним значение стека
	//	cli(); //запрещаем прерывание
	DDRTEMP |= 1<<BITTEMP;  //притягиваем шину  |= 0x20; 
	_delay_us(500); //задержка как минимум на 480 микросекунд
	DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину  &= ~0x20;
	_delay_us(65); //задержка как минимум на 60 микросекунд
	if ((PINTEMP & (1<<BITTEMP))==0)//проверяем, ответит ли устройство
	{
		dt=1;//устройство есть
	}
	else {
		dt=0;//устройства нет
	}
	//SREG = stektemp;// вернем значение стека
	_delay_us(500); //задержка как минимум на 480 микросекунд, но хватит и 420, тк это с учетом времени прошедших команд
	return dt; //вернем результат
}
//========================================
void dt_sendbit(char bt)//функция записи бита на устройство
{
		//char stektemp=SREG;// сохраним значение стека
		//cli(); //запрещаем прерывание
DDRTEMP |= 1<<BITTEMP;  //притягиваем шину
_delay_us(2); //задержка как минимум на 2 микросекунд
if(bt)
DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
_delay_us(65); //задержка как минимум на 65 микросекунд
DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
//SREG = stektemp;// вернем значение стека
}
//========================================
void dt_sendbyte(unsigned char bt)//функция записи байта на устройство
{
char i;
for(i=0;i<8;i++)//посылаем отдельно каждый бит на устройство
{
	if((bt & (1<<i)) == 1<<i)//посылаем 1
	dt_sendbit(1);
	else //посылаем 0
	dt_sendbit(0);
}	
}
//========================================
char dt_readbit(void)//функция чтения бита с устройства
{
//char stektemp=SREG;// сохраним значение стека
//cli(); //запрещаем прерывание
char bt; //переменная хранения бита
DDRTEMP |= 1<<BITTEMP; //притягиваем шину
_delay_us(2); //задержка как минимум на 2 микросекунды
DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
_delay_us(13);
bt = (PINTEMP & (1<<BITTEMP))>>BITTEMP; //читаем бит
_delay_us(45);
//SREG = stektemp;// вернем значение стека
return bt; //вернем результат	
}
//========================================
unsigned char dt_readbyte(void)//функция чтения байта с устройства
{
	char c=0;
	char i;
	for(i=0;i<8;i++)
	c|=dt_readbit()<<i; //читаем бит
	return c;
}
//========================================
int dt_check(void)//функция преобразования показаний датчика в температуру
{
	
	unsigned char bt;//переменная для считывания байта
	unsigned int tt=0;
	if(dt_testdevice()==1) //если устройство нашлось
	{
		dt_sendbyte(NOID);//CC пропустить идентификацию, тк у нас только одно устройство на шине
		dt_sendbyte(T_CONVERT);//44 измеряем температуру
		_delay_ms(1); //в 12битном режиме преобразования - 750 милисекунд
		dt_testdevice(); //снова используем  те же манипуляции с шиной что и при проверке ее присутствия
		dt_sendbyte(NOID);//CC пропустить идентификацию, тк у нас только одно устройство на шине
		dt_sendbyte(READ_DATA); //даем команду на чтение данных с устройства
		/*dt_sendbyte(READ_ROM);//поиск ROM
		for (i=0;i<=7;i++){
			dt_sendbyte(READ_DATA); //даем команду на чтение данных с устройства
		_delay_us(250);	
		}*/
		
		bt = dt_readbyte(); //читаем младший бит
		tt = dt_readbyte(); //читаем старший бит MS
		for (i=0;i<=6;i++){
			dt_readbyte();
		}
		tt = bt;//(tt<<8)|bt;//сдвигаем старший влево, младший пишем на его место, тем самым получаем общий результат
	}
	return tt;
}
//===========================================
char converttemp (unsigned int tt)//преобразование температуры в единицы
{
	char t = tt/2;//>>3;//сдвиг и отсечение части старшего байта
	//char t = tt;
	return t;
}