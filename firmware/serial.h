#ifdef __AVR_ARCH__

void serial_init();
void uart_putchar(char c);
char uart_getchar(void);

#endif
