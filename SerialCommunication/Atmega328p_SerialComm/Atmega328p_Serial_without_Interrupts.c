#include<avr/io.h>
#define F_CPU 16000000UL
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

int main(void){
	char recieved_byte;
	
	UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0H  = ( BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	
	for(;;){
		// wait until a byte is ready to read
		while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}
		
		// grab the byte from the serial port
		recieved_byte = UDR0;
		
		// wait until the port is ready to be written to
		while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
		
		// write the byte to the serial port
		UDR0 = recieved_byte;
	}
	return 0;   /* never reached */
}