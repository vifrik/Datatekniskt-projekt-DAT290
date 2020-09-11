// Contains functions for initializing CAN-port 1 and to send/receive messages.
// Also contains some simple USART communication functions.

#ifndef CAN_H
#define CAN_H

typedef unsigned char uchar;

// CANMsg represents a message that is to be sent or has been received. 
// msgId is the 7 most significatnt bits of the 11 arbitration bits in a CAN-message.
//   This means that lower msgIds should have priority over higher ones if there is a 
//   collision on the CAN-bus (not tested)
// nodeId is intended to represent the ID of the sending device. 
//   This uses the four least significant bits of the 11 arbitration-bist in a CAN-message.
//   (this should mean that lower nodeId have priority accesst to the bus if msgId is identical.
typedef struct {
	uchar msgId;  //Valid values: 0-127
	uchar nodeId; //Valid values: 0-15
	uchar length; //Valid values: 0-8
	uchar buff[8]; // A message carries at most 8 bytes of data
} CANMsg;

typedef void (*VoidFunction)(void);

// Exported CAN functions
void can1_init(VoidFunction interrupt);
int can_receive(CANMsg *msg);
int can_send(CANMsg *msg);

// Some extra functions for USART
void _outchar(char c);
char _tstchar( void );
char _getchar( void );
void usart_send(char *s);
void DUMP(char* s);
    
#endif
