#ifndef USART_H
#define USART_H


// Some extra functions for USART
void _outchar(char c);
char _tstchar( void );
char _getchar( void );
void usart_send(char *s);
void DUMP(char* s);

#endif