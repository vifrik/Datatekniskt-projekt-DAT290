// A small CAN-application that sends a hello, prints any messages it receives and 
//   continually requests words to send via USART. 
#include "can.h"

//
// This function should be called only when a CAN message is ready to be received
//   currently, it simply prints the message (must be a properly terminated string)
//
void receiver(void) {
	DUMP("CAN message received: ");
	
    CANMsg msg;
    if (can_receive(&msg))
	    DUMP((char*)msg.buff);
    else
        DUMP("***Error: Something went wrong :(");
}


void main() {
	DUMP("\nStarting application");

    can1_init(receiver);
	
	DUMP("CAN initated");

	CANMsg msg;
    msg.msgId = 1;
    msg.nodeId = 1;
    msg.length = 6;
    msg.buff[0] = 'H';
    msg.buff[1] = 'e';
    msg.buff[2] = 'l';
    msg.buff[3] = 'l';
    msg.buff[4] = 'o';
    msg.buff[5] = 0;
    can_send(&msg);
    
	DUMP("Hello message sent");

    while(1){
      usart_send("Enter a seven letter message\n");
      for(int i = 0; i < 7; i++)
          msg.buff[i] = _getchar();
      msg.buff[7] = '\0';
      msg.length=8;
      can_send(&msg);
      usart_send("Message sent: ");
      usart_send(msg.buff);
      usart_send("\n");
    }

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