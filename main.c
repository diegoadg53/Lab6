/*
 * Lab6.c
 *
 * Created: 4/24/2024 5:35:13 AM
 * Author : diego
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void initUART57600(void);
void writeUART(char caracter);
void writeTextUART(char* texto);

volatile char bufferRX;

int main(void)
{
    initUART57600();
	DDRD |= 0xFC;
	DDRB |= 0x03;
	sei();
	
	/*writeUART('H');
	writeUART('o');
	writeUART('l');
	writeUART('a');
	writeUART(' ');
	writeUART('M');
	writeUART('u');
	writeUART('n');
	writeUART('d');
	writeUART('o');
	writeUART('!');
	//writeUART('\n');
	writeUART(10);
	writeUART(13);*/
	writeTextUART("Hola Mundo aaaaa");
	
	PORTD |= 0xFC;
	PORTB |= 0x03;
	
    while (1) 
    {
		
    }
}

void initUART57600(void){
	// Configurar pines TX y RX
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	//Configurar A modo Fast U2X0 = 1
	UCSR0A = 0;
	UCSR0A |= (1<<U2X0);
	
	//Configurar B Habilitar ISR RX
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	// Paso 4: Configurar C Frame: 8 bits datos, no paridad, 1 bit stop
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	// Baudrate = 57600
	UBRR0 = 34;
	
}

void writeUART(char caracter){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = caracter;
}

void writeTextUART(char* texto){
	uint8_t i;
	for(i = 0; texto[i] != '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = texto[i];
	}
	writeUART(10);
	writeUART(13);
}

ISR(USART_RX_vect){
	bufferRX = UDR0;
	
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = bufferRX;
	
	PORTD = (bufferRX<<2) & 0xFC;
	PORTB = (bufferRX>>6) & 0x03;
	
}