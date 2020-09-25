#include "usart.h"

#define __DUMP_ENABLED      // Comment out to disable deubug messages over USART

typedef struct USART {
    volatile unsigned short sr;
    volatile unsigned short Unused0;
    volatile unsigned short dr;
    volatile unsigned short Unused1;
    volatile unsigned short brr;
    volatile unsigned short Unused2;
    volatile unsigned short cr1;
    volatile unsigned short Unused3;
    volatile unsigned short cr2;
    volatile unsigned short Unused4;
    volatile unsigned short cr3;
    volatile unsigned short Unused5;
    volatile unsigned short gtpr;
} USART;

#define USART1  ((USART *) 0x40011000) 

void _outchar( char c ) {
    /* write character to usart1 */
    while (( USART1->sr & 0x80)==0);
    USART1->dr = (unsigned short) c;
    if( c == '\n')
            _outchar('\r');
}

char _tstchar(void) {
    /* see if character arrived at usart1,
        if not, return 0
        else return character
        */
    if( (USART1->sr & 0x20)==0)
        return 0;
    return (char) USART1->dr;
}

char _getchar(void) {
    /* wait until character arrived at usart1,
        return character
        */
    while( (USART1->sr & 0x20)==0)
        ;
    return (char) USART1->dr;
}

void usart_send(char* s){
   while (*s != '\0')
       _outchar(*(s++));
}

// Översätter char till int så det synns på Eterm8
void usart_send_numeric(int n) {
	char buffer[20];
	itoa(n,buffer,10);
	usart_send(buffer);
}

void DUMP(char *s) {
#ifdef __DUMP_ENABLED
   usart_send(s);
   _outchar('\n');
#endif
}

// Översätter char till int så det synns på Eterm8 (Debugg syfte)
void DUMP_numeric(int n) {
	unsigned char buffer[16];
	itoa(n,buffer,10);
	DUMP(buffer);
}

void DUMP_numeric_list(char* s, unsigned char length) {
	for (int i = 0; i < length; i++) {
		char buffer[16];
		itoa(*(s++), buffer, 10);
		_outchar(buffer[0]);
	}
	_outchar('\n');
}