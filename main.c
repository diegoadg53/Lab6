/*
 * Lab6.c
 *
 * Created: 4/24/2024 5:35:13 AM
 * Author : diego
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

void initUART57600(void);
void writeUART(char caracter);
void writeTextUART(char* texto);
void showMenu(void);
void init_ADC(void);

volatile char bufferRX;
volatile uint8_t bandera_lectura = 0;
volatile uint8_t valor_pot = 0;

int main(void)
{
    initUART57600();
	DDRD |= 0xFC;
	DDRB |= 0x03;
	init_ADC();
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
	//writeTextUART("Hola Mundo aaaaa");
	
	PORTD &= ~(0xFC);
	PORTB &= ~(0x03);
	
    while (1) 
    {
		showMenu();
		while (bandera_lectura == 0)
		;
		bandera_lectura = 0;
		if (bufferRX == '1'){
			char str_valor_pot[5];
			sprintf(str_valor_pot, "%d", valor_pot);
			writeTextUART(str_valor_pot);
		} else if (bufferRX == '2') {
			writeTextUART("Escriba el valor ASCII a mandar: ");
			writeTextUART("");
			while (bandera_lectura == 0)
			;
			bandera_lectura = 0;
			while (!(UCSR0A & (1<<UDRE0)));
			UDR0 = bufferRX;
			PORTD = (bufferRX<<2) & 0xFC;
			PORTB = (bufferRX>>6) & 0x03;
			
		} else {
			writeTextUART("Valor inválido, por favor aprenda a seguir instrucciones");	
		}
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

void showMenu(void){
	writeTextUART("");
	writeTextUART("Escriba el número correspondiente a la acción a ejecutar:");
	writeTextUART("");
	writeTextUART("1. Leer potenciómetro");
	writeTextUART("");
	writeTextUART("2. Enviar ASCII");
	writeTextUART("");
}

void init_ADC(void){
	ADMUX = 0;
	
	ADMUX |= (1<<REFS0);
	
	ADMUX |= (1<<ADLAR);
	
	ADMUX |= 0x07;
	
	ADCSRA = 0;
	
	ADCSRA = (1<<ADEN);
	
	ADCSRA |= (1<<ADATE);
	
	ADCSRA |= (1<<ADIE);
	
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	ADCSRB = 0;
	
	ADCSRA |= (1<<ADSC);
}

ISR(USART_RX_vect){
	bandera_lectura = 1;
	bufferRX = UDR0;
	
	//while (!(UCSR0A & (1<<UDRE0)));
	//UDR0 = bufferRX;
	
	/*PORTD = (bufferRX<<2) & 0xFC;
	PORTB = (bufferRX>>6) & 0x03;*/
	
}

ISR(ADC_vect){
	valor_pot = ADCH;
}