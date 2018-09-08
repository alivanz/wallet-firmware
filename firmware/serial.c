#ifdef __AVR_ARCH__

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Serial
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define BAUD 9600
#include <util/setbaud.h>

#define BUFLEN 256
static char rxbuf[BUFLEN];
static char txbuf[BUFLEN];
static volatile int l,r;
static volatile int x,y;

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
    UCSR0B |= (1<<RXCIE0) | _BV(UDRIE0);
    l = 0;
    r = 0;
    x = 0;
    y = 0;
}

void uart_putchar(char c) {
  if((x==y) && bit_is_set(UCSR0A, UDRE0)){
    UDR0 = c;
    return;
  }
  txbuf[y++] = c;
  y = y % BUFLEN;
}

ISR(USART_RX_vect){
  rxbuf[r++] = UDR0;
  r = r % BUFLEN;
}

ISR(USART_UDRE_vect){
  if(x!=y){
    UDR0 = txbuf[x++];
    x = x % BUFLEN;
  }
}

char uart_getchar(void) {
  char c;
  // wait until data available
  while(l==r);
  c = rxbuf[l++];
  l = l % BUFLEN;
  return c;
}

void serial_init(){
  uart_init();
  DDRB = 0xff;
  PORTB = 0xff;
  sei();
  // while(1);
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
