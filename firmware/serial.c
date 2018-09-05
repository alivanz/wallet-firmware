#ifdef __AVR_ARCH__

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

// Serial
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define BAUD 9600
#include <util/setbaud.h>

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void uart_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}
char uart_getchar(void) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

void serial_init(){
  uart_init();
  // fill in the UART file descriptor with pointer to writer.
  static FILE uart_output;
  static FILE uart_input;
  static FILE uart_io;
  // fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  fdev_setup_stream(&uart_input,  NULL, uart_getchar, _FDEV_SETUP_READ);
  fdev_setup_stream(&uart_io,     uart_putchar, uart_getchar, _FDEV_SETUP_RW);
  // The uart is the standard output device STDOUT.
  stdout = &uart_output;
  stdin  = &uart_input;
}

#endif
