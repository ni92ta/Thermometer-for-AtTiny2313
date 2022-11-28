/*
 * main.c
 *Термометр на ATtiny 2313
 * Created: 3/10/2022 7:28:52 PM
 *  Author: Никита
 *Личные неимущественные и имущественные права принадлежат автору в случае патентоспособности НТ или её частей.
 */ 

//#include <xc.h>
//#include <avr/interrupt.h>
//#define F_CPU 4800000L
//#include <util/delay.h>
#include "main.h"
unsigned char start_tim;//флаг запуска таймера 0-старт,1-стоп
unsigned char start_run_fraim;//флаг запуска бегущей строки
unsigned char time_tim;//переменная для счёта переполнения таймера
unsigned char start_izm;//переменная для начала измерения температуры
unsigned char l;
unsigned char y;
unsigned char v;
unsigned char t;//Переменная для хранения результата измерения внутреннего датчика
unsigned char t2;//Переменная для хранения результата измерения внешнего датчика
unsigned char aa;
unsigned char bb;
unsigned char cc;
unsigned char dd;
unsigned char vpo;
unsigned char kll;
unsigned char chh_h;
unsigned char ch;
unsigned char chh;
unsigned char chhh;
//----------------------------------------------------
int efi[12] = {0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,0b01111111,0b01111011,0b01001110,0b00000000
};//0,1,2,3,4,5,6,7,8,9,С,NULL
/*int eff1[12] = {0b00000000,0b01111110,0b00110000,0b01101101,
	            0b01111001,0b00110011,0b01011011,0b01011111,
				0b01110000,0b01111111,0b00000000,0b00000000
};*/
//int efi[9] = {0b00000000,0b01111110,0b00110000,0b01101101,0b01111001,0b00000000,0b00000000,0b00000000,0b00000000
//};
//int eff1[9] = {0b00000000,0b01111110,0b00110000,0b01101101,0b01111001,0b00000000,0b00000000,0b00000000,0b00000000
	//};
int result_output[9] = {0};//Матрица для записи и вывода значения температуры
int buf[9];//Буфер для хранения сдвигаемой матрицы. Размер равен размеру матрицы с текстом
 //-----------------------функция-инициализации таймера------
 void timer_ini(void)
 {
	TCCR0A |= (1<<WGM01);//устанавливаем режим работы таймера - сброс при совпадении 0b00000010
	TCCR0B |= (1<<CS02) | (1<<CS00) ;//устанавливаем делитель 1024	
	TCNT0 = 0b00000000;//обнуляем таймер 
	//TIMSK = 0b00000001;//|= (1<<OCIE0A);//устанавливаем бит разрешения прерывания счетчика по совпадению
	//TIMSK0 =0b00000000;//&= ~0x01;
	TCNT0 = 0b00000000;// установка начального значения таймера 11101000
	OCR0A = 0b00010110;//записываем в регистр число для сравнения	00110010
	//TIMSK |=  (1<<OCIE0A);//устанавливаем бит разрешения прерывания таймера по совпадению  (1<<TOIE0) |
	//TCCR0B |= (1<<CS02) | (1<<CS00);
 }
 //------------функция-обработчик таймера-------------
 ISR (TIMER0_COMPA_vect)//	TIMER0_COMPA_vect   
 {
time_tim++;
	 TIMSK = 0b00000000;
	 TCCR0B = 0b00000000;//останавливаем таймер
 }
//-------------Вывод кадра---------------------------
void frame_out (void){
							
	if (time_tim==15){//задаём время до начала бегущей строки 40~20сек 
		start_run_fraim=0;
		start_tim = 1;
		 time_tim=0;
		 v++;
		 kll=0;
		 if (v>=2) {
			 kll=1;
			 v=0;
		 }
	}


	start_run_fraim++;	
 if (start_run_fraim<=10) {//количество проходов бегущей строки 20~2раза
	 buf[9] = result_output[1]; // Считали 1ю колонку в буфер
	 for (int v = 1; v<=10; v++)
	 {
		 vpo ++;
		 result_output[v] = result_output[v+1]; // Сдвинули матрицу на один столбец влево
	 }
	 result_output[9] = buf[9]; // Записали содержимое буфера (первую колонку) в конец матрицы
	  switch (vpo)
		 {
			 case 10:
			  result_output[1] = 0;//0
			  result_output[2] = cc;//1
			  result_output[3] = bb;//2
			  result_output[4] = aa;//3
			  result_output[5] = dd;//4
			 break;
			 	   case 20:
				    result_output[1] = cc;//0
			        result_output[2] = bb;//2
			        result_output[3] = aa;//3
			        result_output[4] = dd;//40b01001110
			        result_output[5] = 0;//0
			  	    result_output[6] = 0;//0
			  		result_output[7] = 0;//0
			  		result_output[8] = 0;//0
			  		result_output[9] = 0;//0
			 	   break;
						 case 30:
						  result_output[1] = bb;//0
						  result_output[2] = aa;//2
						  result_output[3] = dd;//30b01001110
						  result_output[4] = 0;//4
						  result_output[5] = 0;//0
						  result_output[6] = 0;//0
						  result_output[7] = 0;//0
						  result_output[8] = 0;//0
						  result_output[9] = cc;//0
						 break;
						       case 40:
						        result_output[1] = aa;//0
								result_output[2] = dd;//20b01001110
								result_output[3] = 0;//3
								result_output[4] = 0;//4
								result_output[5] = 0;//0
								result_output[6] = 0;//0
								result_output[7] = 0;//0
								result_output[8] = cc;//0
								result_output[9] = bb;//0
							   break;
									 case 50:
						  if (kll==0){//0
							  t = dt_check(1);//измеряем температуру внутреннего датчика 1
							  // chh_h=0b01001110;//дробная часть
							   dd=0b01001110;
							   							  ch = t%10;//единицы
							   							  chh = t%100/10;//десятки
							   							  chhh = t%1000/100;//сотни
							   
						  }		
						  if (kll==1){//1
							  t = dt_check(2); //измеряем температуру внешнего датчика 2
							   
							   							   							  chh_h = t%10;//единицы
							   							   							  ch = t%100/10;//десятки
							   							   							  chh = t%1000/100;//сотни
							                                                          chhh = t/1000;//сотни
							   
							                             // chh_h=t%10;//дробная часть
							   							//  ch = t%100/10;//единицы
							   							 // chh = t%1000/100;//десятки
							   							 // chhh = t/1000;//сотни
															 dd=efi[chh_h];
							   
						  }		
							 

							  
							  aa = efi[ch];
							  bb = efi[chh];
							  if (chhh == 0){//убираем первый разряд (ноль)
								  cc = efi[12];
							  }
							  else{
								  cc = efi[chhh];
							  }
						  
						 result_output[1] = dd;//00b01001110
						 result_output[2] = 0;//2
						 result_output[3] = 0;//3
						 result_output[4] = 0;//4
						 result_output[5] = 0;//0
						 result_output[6] = 0;//0
						 result_output[7] = cc;//cc
						 result_output[8] = bb;//bb
						 result_output[9] = aa;//aa
						 break;
								 case 60:
								 result_output[1] = 0b00000000;//0 
							     result_output[2] = 0b00000000;//0
							     result_output[3] = 0b00000000;//0
								 result_output[4] = 0b00000000;//0
								 result_output[5] = 0b00000000;//0
								 result_output[6] = 0b00000000;//0						
								 break;
								case 110:					 
			  result_output[1] = 0;//0
			  result_output[2] = cc;//1
			  result_output[3] = bb;//2
			  result_output[4] = aa;//3
			  result_output[5] = dd;//40b01001110
			  vpo = 10;
				break;																
		 } 
 }
//------Переключение сегмента светодиодного индикатора-----
	for (int i = 0; i < 350; ++i) {
		PORTB = result_output[l];
		_delay_us(750);//пауза для задания скорости движения бегущей строки 1000us
		switch (l){
			case 0:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
			break;
			case 1:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
			PORTD &= ~(1<<2);
			break;
			case 2:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
			PORTD &= ~(1<<1);
			break;
			case 3:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
			PORTD &= ~(1<<0);
			break;
			case 4:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
			PORTD &= ~(1<<3);
			break;
						case 5:
						PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
						break;
							}
		l++;
		y++;
		if (y == 9) {
			y = 0;
			l -= 9;
			}		
			}

	
if (start_tim == 0){
		TIMSK |= (1<<OCIE0A);//устанавливаем бит разрешения прерывания таймера по совпадению
		TCCR0B |= (1<<CS02) | (1<<CS00);
		TCNT0 = 0b00000000;//обнуляем таймер									 
}
}
//-----------------------------------------------------
int main(void)
{
    sei();//разрешаем глобальные прерывания
	DDRB = 0b11111111;// определяем выходы мк (1 выход; 0 вход); 
	DDRD = 0b01001111;// определяем выходы мк (1 выход; 0 вход);
    while(1)
    {	
		frame_out();	
	}
}
//PORTB |= 0x10;//установка бита
//PORTB &= ~0x1;//сброс бита

