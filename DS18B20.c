/*
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
//int identifi [8] = {0b00001000,0b01110000,0b01111101,0b01101000,0b01000000,0b00000000,0b00010000,0b01101000};	
//====================================
void ds18x2_sel(unsigned char dat){//отправляем ROM адрес датчика
dt_sendbyte(MATCH_ROM);
if (dat==1){//1
//-------Датчик внутренний DS1820---------------
		dt_sendbyte(0x10);
		dt_sendbyte(0x4E);
		dt_sendbyte(0xBE);
		dt_sendbyte(0x57);
		dt_sendbyte(0x02);
		dt_sendbyte(0x08);
		dt_sendbyte(0x0);
		dt_sendbyte(0x17);
		}
if (dat==2){//2	
//-------Датчик внешний DS18B20---------------					
	    dt_sendbyte(0x28);			
		dt_sendbyte(0x44);
		dt_sendbyte(0xD1);
		dt_sendbyte(0xE9);
		dt_sendbyte(0x05);
		dt_sendbyte(0x00);
		dt_sendbyte(0x00);
		dt_sendbyte(0xCB);	
	    }			
}
char dt_testdevice(void) //dt - digital termomether | определим, есть ли устройство на шине
{
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
	_delay_us(500); //задержка как минимум на 480 микросекунд, но хватит и 420, тк это с учетом времени прошедших команд
	return dt; //вернем результат
}
//========================================
void dt_sendbit(char bt)//функция записи бита на устройство
{
DDRTEMP |= 1<<BITTEMP;  //притягиваем шину
_delay_us(2); //задержка как минимум на 2 микросекунд
if(bt)
DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
_delay_us(65); //задержка как минимум на 65 микросекунд
DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
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
char bt; //переменная хранения бита
DDRTEMP |= 1<<BITTEMP; //притягиваем шину
_delay_us(2); //задержка как минимум на 2 микросекунды
DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
_delay_us(13);
bt = (PINTEMP & (1<<BITTEMP))>>BITTEMP; //читаем бит
_delay_us(45);
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
int dt_check(unsigned char izm)//функция преобразования показаний датчика в температуру
{
	unsigned char bt;//переменная для считывания байта
	unsigned int tt=0;
	if(dt_testdevice()==1) //если устройство нашлось
	{		
ds18x2_sel(izm);
//Чтение ROM
/*dt_sendbyte(READ_ROM);
for(i=0;i<8;i++){
	bt = dt_readbyte();
}*/
		dt_sendbyte(READ_DATA); //даем команду на чтение данных с устройства
		bt = dt_readbyte(); //читаем младший бит
		tt = dt_readbyte(); //читаем старший бит MS
		dt_testdevice();//снова используем  те же манипуляции с шиной что и при проверке ее присутствия
		dt_sendbyte(NOID);
		dt_sendbyte(T_CONVERT);//44 измеряем температуру
		_delay_ms(1);//в 12битном режиме преобразования - 750 милисекунд
if (izm==1){//1
		tt = bt;///2;//2;//для датчика 1820
}
if (izm==2) {//2
	   tt=(tt<<4)|bt;//для датчика 18B20 для отрицательной температуры от 65536 отнять измеренную (((tt<<8)|bt)>>4);
	// tt=0;
	 // tt=bt;
	   tt=tt>>4;
	   //tt=((tt>>1)*10+((tt%2)*5)); //((tt>>1)*10+((tt%2)*5))
	   //ttt = ((t>>1)*7+((t%10)));//((t>>1)*7+((t%10)));
}
	}
	return tt;
}
//===========================================

