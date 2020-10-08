#ifndef USART_H
#define USART_H


// Some extra functions for USART
void _outchar(char c);
char _tstchar( void );
char _getchar( void );
void usart_send(char *s);
void usart_sendl(char*s);
void usart_send_numeric(int n);
void usart_send_numericl(int n);
void DUMP(char* s);
void DUMP_numeric(int n);
void DUMP_numeric_list(char* s, unsigned char length);

#endif