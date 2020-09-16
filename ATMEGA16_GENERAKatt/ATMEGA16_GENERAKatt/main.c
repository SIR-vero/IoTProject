#define F_CPU 1000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */
	UBRRL = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate value */
	UBRRH = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits*/
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
	return(UDR);			/* Return the byte*/
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)		/* Send string till null */
	{
		UART_TxChar(str[j]);
		j++;
	}
}

void ADC_Init()
{
	DDRA=0x0;			/* Make ADC port as input */
	ADCSRA = 0x87;			/* Enable ADC, fr/128  */
	ADMUX = 0x40;			/* Vref: Avcc, ADC channel: 0 */
	
}

int ADC_Read(char channel)
{
	int Ain,AinLow;
	
	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and 
					Multiply with weight */
	Ain = Ain + AinLow;				
	return(Ain);			/* Return digital value*/
}

char arr1[10];
int as,L;
int main(void)
{
	/* Replace with your application code */
	ADC_Init();								// ADC initialization
	UART_init(4800);
	int attend=0;
	int flag=0;
	char arr[10];
	//char update[40] = {"GET /update?key=DGXX4163Z7XEXE2I&field3="};
	DDRB = 0B00000000;
	UART_SendString("AT\r\n");
	_delay_ms(500);
	//UART_SendString("AT+CWMODE=3\r\n");
	//_delay_ms(700);
//	UART_SendString("AT+CWJAP=\"Verma\",\"12345678\"\r\n");
//	_delay_ms(5000);
	UART_SendString("AT+CIPMUX=1\r\n");
	_delay_ms(1000);
	UART_SendString("AT+CIPSERVER=1,80\r\n");
	_delay_ms(2000);
	
	while (1)
	{
		as=ADC_Read(0);
		//itoa(as,arr1,10);
		//UART_SendString(arr1);					//Now transmitting the decimal value from controller
		//UART_SendString("\r\n");				// (\r --> for new row)(\n --> for new line) both to press enter
		//_delay_ms(1000);
		PORTB=0B00000000;				// delay of one second
		if(as>=200&&flag==0)
		{
			flag=1;
			
			for(int i=0;i<3;i++){
				UART_SendString("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
				_delay_ms(1000);
				UART_SendString("AT+CIPSEND=4,43\r\n");
				_delay_ms(300);
				UART_SendString("GET /update?key=DGXX4163Z7XEXE2I&field2=1\r\n");
				_delay_ms(1000);
				//UART_SendString("AT+CIPCLOSE=4\r\n");
				//_delay_ms(1000);
			}
		}
		else if(as<200&&flag==1)
		{
			flag=0;
			
			for(int i=0;i<3;i++){
				UART_SendString("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
				_delay_ms(1000);
				UART_SendString("AT+CIPSEND=4,43\r\n");
				_delay_ms(300);
				UART_SendString("GET /update?key=DGXX4163Z7XEXE2I&field2=0\r\n");
				_delay_ms(1000);
				//UART_SendString("AT+CIPCLOSE=4\r\n");
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
					//UART_SendString(arr1);
					//UART_SendString("\r\n");
					for(int i=0;i<3;i++)
					{
						UART_SendString("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
						_delay_ms(1000);
						UART_SendString("AT+CIPSEND=4,");
						UART_SendString(arr1);
						UART_SendString("\r\n");
						_delay_ms(300);
						UART_SendString("GET /update?key=DGXX4163Z7XEXE2I&field4=");
						UART_SendString(arr);
						UART_SendString("\r\n");
						_delay_ms(1000);
						//UART_SendString("AT+CIPCLOSE=4\r\n");
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
					//UART_SendString(arr1);
					//UART_SendString("\r\n");
					for(int i=0;i<3;i++)
					{
						UART_SendString("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
						_delay_ms(1000);
						UART_SendString("AT+CIPSEND=4,");
						UART_SendString(arr1);
						UART_SendString("\r\n");
						_delay_ms(300);
						UART_SendString("GET /update?key=DGXX4163Z7XEXE2I&field4=");
						UART_SendString(arr);
						UART_SendString("\r\n");
						_delay_ms(1000);
						//UART_SendString("AT+CIPCLOSE=4\r\n");
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

