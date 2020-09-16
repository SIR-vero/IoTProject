/*
 * seatTest2.c
 *
 * Created: 11/15/2019 3:18:57 PM
 * Author : Ayush Verma
 */ 

#define F_CPU 12000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#define dl _delay_ms(2000)
void adc()
{
	ADCSRA = 0b10000111;	// Enable ADC
	ADMUX = 0b11000000;		// for channel zero(ADC0)
}
int as,L;
#ifndef _Buzzer_
#define _Buzzer_
void buzzer()
{
	PORTD|=(1<<7);
	_delay_ms(100);
	PORTD&=~(1<<7);
	_delay_ms(100);
}
#endif

#ifndef _UART_H_
#define _UART_H_

void tt( char t)
{
	while(!(UCSRA&(1<<UDRE)));				// hold until conversion is completed
	UDR=t;									// PUT data into UDR registor
}
void usart_tx( char *tx)
{
	while(*tx!='\0')						// transfer data until we get NULL character ('\0')
	{
		tt(*tx);
		tx++;								// next character
	}
}
void u_init()
{
	UBRRL=12;					// PUT that value for 9600 BAUDRATE by chart
	UCSRB|=(1<<TXEN)|(1<<RXEN);				// transmitt enable
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	// setting for 8 bit character size (ex: 'A' = 01000001)
}




#endif
char arr1[10];

int main(void)
{
    /* Replace with your application code */
	adc();								// ADC initialization
	u_init();
	int attend=0;
	int flag=0;
	char arr[10];
	//char update[40] = {"GET /update?key=DGXX4163Z7XEXE2I&field3="};
	DDRB = 0B00000000;
	usart_tx("AT\r\n");
	_delay_ms(500);
	usart_tx("AT+CWMODE=3\r\n");
	_delay_ms(700);
	usart_tx("AT+CWJAP=\"Verma\",\"12345678\"\r\n");
	_delay_ms(3000);
	usart_tx("AT+CIPMUX=1\r\n");
	_delay_ms(1000);
	usart_tx("AT+CIPSERVER=1,80\r\n");
	_delay_ms(2000);
	
    while (1) 
    {
		ADCSRA|=(1<<ADSC);				// Start conversion
		while(!(ADCSRA&(1<<ADIF)));
		as=ADC;
		//itoa(as,arr1,10);
		//usart_tx(arr1);					//Now transmitting the decimal value from controller
		//usart_tx("\r\n");				// (\r --> for new row)(\n --> for new line) both to press enter
		//_delay_ms(1000);
		PORTB=0B00000000;				// delay of one second
		if(as<=130&&flag==0)
		{
			flag=1;
			
			for(int i=0;i<3;i++){
				usart_tx("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
				_delay_ms(1000);
				usart_tx("AT+CIPSEND=4,43\r\n");
				_delay_ms(300);
				usart_tx("GET /update?key=DGXX4163Z7XEXE2I&field1=1\r\n");
				_delay_ms(1000);
				//usart_tx("AT+CIPCLOSE=4\r\n");
				//_delay_ms(1000);
			}
		}
		else if(as>130&&flag==1)
		{
			flag=0;
			
			for(int i=0;i<3;i++){
				usart_tx("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
				_delay_ms(1000);
				usart_tx("AT+CIPSEND=4,43\r\n");
				_delay_ms(300);
				usart_tx("GET /update?key=DGXX4163Z7XEXE2I&field1=0\r\n");
				_delay_ms(1000);
				//usart_tx("AT+CIPCLOSE=4\r\n");
				//_delay_ms(1000);
			}
		}
		
		if((PINB&0x01)==0x01){
			while(1){
				if((PINB&0B00000010)==0B00000010){
					attend++;
					itoa(attend,arr,10);
					L = strlen(arr)+42;
					itoa(L,arr1,10);
					//usart_tx(arr1);
					//usart_tx("\r\n");
					for(int i=0;i<3;i++)
					{
						usart_tx("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
						_delay_ms(1000);
						usart_tx("AT+CIPSEND=4,");
						usart_tx(arr1);
						usart_tx("\r\n");
						_delay_ms(300);
						usart_tx("GET /update?key=DGXX4163Z7XEXE2I&field3=");
						usart_tx(arr);
						usart_tx("\r\n");
						_delay_ms(1000);
						//usart_tx("AT+CIPCLOSE=4\r\n");
						//_delay_ms(1000);
					}
					break;
				}
			}
			_delay_ms(500);
			PORTB=0B00000000;
		}
		if((PINB&0B00000010)==0B00000010){
			while(1){
				if((PINB&0x01)==0x01){
					attend--;
					itoa(attend,arr,10);
					L = strlen(arr)+42;
					itoa(L,arr1,10);
					//usart_tx(arr1);
					//usart_tx("\r\n");
					for(int i=0;i<3;i++)
					{
						usart_tx("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
						_delay_ms(1000);
						usart_tx("AT+CIPSEND=4,");
						usart_tx(arr1);
						usart_tx("\r\n");
						_delay_ms(300);
						usart_tx("GET /update?key=DGXX4163Z7XEXE2I&field3=");
						usart_tx(arr);
						usart_tx("\r\n");
						_delay_ms(1000);
						//usart_tx("AT+CIPCLOSE=4\r\n");
						//_delay_ms(1000);
					}
					break;
				}
			}
			_delay_ms(500);
			PORTB=0B00000000;
		}
		
		
    }
}

