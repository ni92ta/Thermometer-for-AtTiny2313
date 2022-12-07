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
int temp;//Переменная для хранения результата измерения внутреннего датчика
//int t2;//Переменная для хранения результата измерения внешнего датчика
unsigned char digit3;
unsigned char digit2;
unsigned char digit1;
unsigned char digit4;
unsigned char vpo;
unsigned char kll;
unsigned char chh_h;
unsigned char ch;
unsigned char chh;
//----------------------------------------------------
int efi[12] = {0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,0b01111111,0b01111011,0b00000001,0b00000000
};//0,1,2,3,4,5,6,7,8,9,-,NULL
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
	if (time_tim == 15){//задаём время до начала бегущей строки 40~20сек 
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
			  result_output[1] = digit1;//0
			  result_output[2] = digit1;//1
			  result_output[3] = digit2;//2
			  result_output[4] = digit3;//3
			  result_output[5] = digit4;//4
			 break;
			 	   case 20:
				    result_output[1] = digit1;//1
			        result_output[2] = digit2;//2
			        result_output[3] = digit3;//3
			        result_output[4] = digit4;//4
			        result_output[5] = 0;//0
			  	    result_output[6] = 0;//0
			  		result_output[7] = 0;//0
			  		result_output[8] = 0;//0
			  		result_output[9] = 0;//0
			 	   break;
						 case 30:
						  result_output[1] = digit2;//2
						  result_output[2] = digit3;//3
						  result_output[3] = digit4;//4
						  result_output[4] = 0;//0
						  result_output[5] = 0;//0
						  result_output[6] = 0;//0
						  result_output[7] = 0;//0
						  result_output[8] = 0;//0
						  result_output[9] = digit1;//1
						 break;
						       case 40:
						        result_output[1] = digit3;//3
								result_output[2] = digit4;//4
								result_output[3] = 0;//0
								result_output[4] = 0;//4
								result_output[5] = 0;//0
								result_output[6] = 0;//0
								result_output[7] = 0;//0
								result_output[8] = digit1;//1
								result_output[9] = digit2;//2
							   break;
									 case 50:
						  if (kll == 0){//0
							  temp = dt_check(1);//измеряем температуру внутреннего датчика 1
							  if (temp&0b00000001) {
								   chh_h = 5; 
							  }
							  else {
								  chh_h = 0;
							  }
							  temp = (temp/2);
								  ch = temp%10;//единицы
								  chh = temp%100/10;//десятки
								 // chhh = temp%1000/100;//сотни
								 chhh = 11;
								  
								  							 // cc = efi[11];
								  							  /*  if (chhh == 0){//убираем первый разряд (ноль)
								  							  	  cc = efi[12];
								  							   }
								  							  else{
								  							  	  cc = efi[chhh];
								  							   }*/
								  
								  
						  }		
						  if (kll == 1){//1
							  temp = dt_check(2);//измеряем температуру внешнего датчика 2
							  //temp = (((temp>>1)*10)+((temp%10)));
							   							  							
							   						/*	 ch = t2%10;//единицы
							   							 chh = t2%100/10;//десятки
							   							 chhh = t2%1000/100;//сотни
							   							 chh_h = t2/1000;
							   */
													      //  cc = efi[10];
														    chh_h = temp%10;
														    ch = temp%100/10;//единицы
														    chh = temp%1000/100;//десятки
														  //  cc = efi[10];
														 // chhh = 10;
														   
														   
						  }		
							 

							  digit4 = efi[chh_h];
							  digit3 = efi[ch] | 0b10000000;//с точкой
							  digit2 = efi[chh];
                              digit1 = efi[chhh];
							  //digit1 = efi[8];	
			  
						 result_output[1] = digit4;//4
						 result_output[2] = 0;//2
						 result_output[3] = 0;//3
						 result_output[4] = 0;//4
						 result_output[5] = 0;//0
						 result_output[6] = 0;//0
						 result_output[7] = digit1;//1
						 result_output[8] = digit2;//2
						 result_output[9] = digit3;//3
						 break;

								/* case 100:
								 for (unsigned char i=1;i<=6;i++){
									  result_output[i] = 0b00000000;//0 
								 }						
								 break;*/
								 
								
								 
								 
								 
								case 110:					 
			  result_output[1] = 0;//0
			  result_output[2] = digit1;//1
			  result_output[3] = digit2;//2
			  result_output[4] = digit3;//3
			  result_output[5] = digit4;//4
			  vpo = 10;
				break;																
		 } 
 }
//------Переключение сегмента светодиодного индикатора-----
	for (int i = 0; i < 350; ++i) {
		PORTB = result_output[l];//включение сегментов дисплея
		_delay_us(750);//пауза для задания скорости движения бегущей строки 1000us
		switch (l){
			case 0:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);//установкой битов выключаем разряды
			break;
			case 1:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);//установкой битов выключаем разряды
			PORTD &= ~(1<<2);//сбросом бита включаем 1 разряд (1234)
			break;
			case 2:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);//установкой битов выключаем разряды
			PORTD &= ~(1<<1);//сбросом бита включаем 2 разряд (1234)
			break;
			case 3:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);//установкой битов выключаем разряды
			PORTD &= ~(1<<0);//сбросом бита включаем 3 разряд (1234)
			break;
			case 4:
			PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);//установкой битов выключаем разряды
			PORTD &= ~(1<<3);//сбросом бита включаем 4 разряд (1234)
			break;
						case 5:
						PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);//установкой битов выключаем разряды
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
		// out_led ();
	}
}
//PORTB |= 0x10;//установка бита
//PORTB &= ~0x1;//сброс бита

 //--------------Вывод информации на индикатор-----------
 /*void out_led (void)
 {

	 for (int i = 1; i>=0; i--)
	 {
		 PORTB = efi[1];//первая цифра
		 PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
		 PORTD &= ~(1<<3);//первый сегмент
		 _delay_ms(2);
		 PORTB = efi[2];//третья цифра
		 PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
		 PORTD &= ~(1<<0);//третий сегмент
		 _delay_ms(5);
		 PORTB = efi[3];//вторая цифра
		 PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
		 PORTD &= ~(1<<1);//второй сегмент
		 _delay_ms(2);
		 PORTB = efi[4];//первая цифра
		 PORTD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
		 PORTD &= ~(1<<2);//первый сегмент
		 _delay_ms(2);
	 }
 }*/