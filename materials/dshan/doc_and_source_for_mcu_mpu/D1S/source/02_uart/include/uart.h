#ifndef	_MINI_UART_H
#define	_MINI_UART_H

void uart_init ( void );
char uart_recv ( void );
void uart_send ( char c );
void uart_send_string(char* str);
int sys_uart_printf(const char * fmt, ...);

#endif  /*_MINI_UART_H */
