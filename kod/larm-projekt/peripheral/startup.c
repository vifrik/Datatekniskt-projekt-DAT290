#include "proximity_peripheral.h"
#include "door_peripheral.h"

void main() {
	proximity_peripheral_init();
	proximity_peripheral_think();

	return;
}


// **** Boilerplate startup code ****
void startup(void) __attribute__((naked)) __attribute__((section (".start_section")) );

void startup( void ) {
		__asm volatile(
		" LDR R0,=0x2001C000\n"		/* set stack */
		" MOV SP,R0\n" 
		" BL main\n" 	    /* call main */
		" B ."
	);
	while(1); // should not be needed
}