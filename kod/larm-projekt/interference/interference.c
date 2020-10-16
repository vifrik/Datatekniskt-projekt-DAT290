#include "interference.h"
#include "can.h"
#include "usart.h"
#include "shared_peripheral.h"
#include "messages.h"
#include "stk.h"

void interference_init(void) {
	state_init();
	//state.id = 1;
	stk_init();
	can1_init(interference_receiver());
}

void interference_receiver(void) {
	DUMP("CAN message received: ");
}

//Läser in en sträng från USART tills det kommer en ny rad.
//Tar in det första avlästa värdet från konsolen och adressen till var strängen skall ligga.
//Returnerar längden på den inlästa strängen
uchar get_string(uchar c, uchar *address) {
	uchar *s;
	s = address;
	uchar input = c;
	uchar length = 0;
	
	while(input != '\n') {
		*s = input;
		s++;
		length++;
		_outchar(input);
		input = _getchar();
	}
	*s = '\0';
	_outchar('\n');
	
	return length;
}

uchar delayDone;

void delay_done(void) {
	delayDone = 0;
}

void clear_string(uchar *address, uchar length) {
	uchar *iAddress;
	iAddress = address;
	for(uchar i = 0; i < length; i++) {
		*iAddress++ = '\0';
	}
}

void interference_think(void) {
	CANMsg *msg;
	canmsg_init(msg);
	msg->msgId = POLL_RESPONSE;
	callback_init(delay_done);
	
	uchar *s;
	uchar *sAddress;
	sAddress = s;
	uchar length = 0;
	uchar c;
	unsigned int usDelay = 1000000;
	while(1) {
		DUMP_numeric(usDelay);
		//can_send(msg);
		delayDone = 1;
		delay_no_block(usDelay);
		while(delayDone) {
			c = _tstchar();
			if(c) {
				//*sAddress = '\0';
				//clear_string(sAddress, length);
				length = get_string(c, sAddress);
				s = sAddress;
				usDelay = atoi(s);
				break;
			}
		}
	}
}