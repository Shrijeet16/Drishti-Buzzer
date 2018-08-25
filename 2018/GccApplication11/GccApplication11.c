/*
 * GccApplication9.c
 *
 * Created: 31-Jul-18 7:05:35 PM
 *  Author: Sinal
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 8000000UL
#define baud 51

volatile unsigned char recieve_char;
volatile int count=1;
volatile unsigned char INT_Number[4]="0000";


unsigned char usart_recieve_char()
{
	while (! (UCSR0A & (1 << RXC0)) );
	return UDR0;
		
}


void usart_initialization()
{
	//sei();
	
	UBRR0H = (unsigned char) (baud >> 8);
	UBRR0L = (unsigned char) (baud);
	
	UCSR0B = 0b00011000;
	UCSR0C = 0b00000110;
	
}

void usart_transmit_char(unsigned char data)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0=data;
}



void usart_transmit_string(char *str)
{
	while(*str > 0)
	{
		usart_transmit_char(*str);
		str++;
	}
	
}

void usart_interrupt_enable()
{
	UCSR0B |= (1<<RXCIE0);
	
	recieve_char = UDR0;
	//usart_transmit_char('a');
	
}



int main(void)
{
	int count1=0 ;
	int p;
	
	sei();
	
	TCCR1B |= (1<<CS12);
	
	Reset();
	
	interrupt_init();				
	usart_initialization();
	while(1)
    {		
		if (TCNT1>31250)
		{
			count1++;
			TCNT1=0;
			
		}
		
		if (count1==5)
		{
			p++;
			count1=0;
			usart_transmit_string(INT_Number);
			if (p%10==0)
			{
				Reset();
			}
						
		}
    }
}




/*--------------------------------------------------------------------------------------------------------------------------------------------*/




void interrupt_init()
{
	DDRD = 0x00;	//for INT input
	
	PORTD =0x00;	//for low 0V

	
	EICRA=0x00;//register clear
	EICRA=0xFF;//all INT0,1,2,3 as rising edge
	
	
	
	EIMSK=0x00;//register clear
	EIMSK=0x0F;//Enable INT0,1,2,3
	
}


ISR(INT0_vect)
{
	//usart_transmit_string("I got INT0");
	//EIFR|=(1<<INTF0);
	//convert into character at that particuar position
	if (INT_Number[0]=='0')
	{
		//usart_transmit_string("I got INT0 and inside if");
		INT_Number[0]=(char)(count + '0');
		count++;
		if(count%4!=0)
		count=count%4;
		else
		count=4;

	}
}

ISR(INT1_vect)
{
	//EIFR|=(1<<INTF1);
//	usart_transmit_string("I got INT1");
	if (INT_Number[1]=='0')
	{
		//usart_transmit_string("I got INT1 and inside if");
		INT_Number[1]=(char)(count + '0');
		count++;
		if(count%4!=0)
		  count=count%4;
		else
		  count=4;
	
	}	
}

ISR(INT2_vect)
{
		//usart_transmit_string("I got INT2");
		if (INT_Number[2]=='0')
		{
			//usart_transmit_string("I got INT2 and inside if");
	INT_Number[2]=(char)(count + '0');
	count++;
	if(count%4!=0)
	count=count%4;
	else
	count=4;
	}	
}

ISR(INT3_vect)
{
	//	usart_transmit_string("I got INT3");
		if (INT_Number[3]=='0')
		{
		//	usart_transmit_string("I got INT3 and inside if");	INT_Number[3]=(char)(count + '0');
		count++;
		if(count%4!=0)
		count=count%4;
		else
		count=4;
	}
}



 void Reset()
{
	DDRB  = 0b01000000;
	PORTB = 0b00000000;
	//for(int i=0;i<3;++i)
	INT_Number[4] = 0 ;
	count =1;
}

