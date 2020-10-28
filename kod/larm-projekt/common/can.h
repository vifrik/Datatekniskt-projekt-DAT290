// Contains functions for initializing CAN-port 1 and to send/receive messages.
// Also contains some simple USART communication functions.

#ifndef CAN_H
#define CAN_H

typedef unsigned char uchar;

// CANMsg represents a message that is to be sent or has been received.
// dir is the most significant bit of the 11 arbitration bits in a CAN-message.
//   It signals whether a message is comming to or from the Central unit.
//   1 = Central -> Periphery / 0 = Periphery -> Central.
// msgId is the 6 most significant bits after 1 the first bit reserved for the dir bit .
//   This means that lower msgIds should have priority over higher ones if there is a 
//   collision on the CAN-bus (not tested)
// nodeId is intended to represent the ID of the sending device. 
//   This uses the four least significant bits of the 11 arbitration-bist in a CAN-message.
//   (this should mean that lower nodeId have priority accesst to the bus if msgId is identical.
typedef struct {
	uchar dir;		//Valid values: 0-1	1 = Central->Periphery / 0 = Periphery->Central
	uchar msgId;	//Valid values: 0-63
	uchar nodeId;	//Valid values: 0-15
	uchar length;	//Valid values: 0-8
	uchar buff[8]; // A message carries at most 8 bytes of data
} CANMsg;

typedef struct  {

} AlarmMsg;

typedef void (*VoidFunction)(void);

// Exported CAN functions
void can1_init(VoidFunction interrupt);
int can_receive(CANMsg *msg);
int can_send(CANMsg *msg);
    
#endif
