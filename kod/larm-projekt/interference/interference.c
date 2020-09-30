#include "interference.h"
#include "can.h"
#include "usart.h"
#include "shared_peripheral.h"
#include "messages.h"
#include "stk.h"

void interference_init(void) {
	state_init();
	stk_init();
	//more??
}

uchar* get_string(uchar c) {
	uchar *s;
	uchar input = c;
	uchar *output = s;
	
	while(input != '\n') {
		*s = input;
		s++;
		input = _getchar();
	}
	
	return output;
}

void interference_think(void) {
	CANMsg *msg;
	canmsg_init(msg);
	msg->msgId = POLL_RESPONSE;
	
	uchar *s;
	uchar c;
	unsigned int usDelay;
	while(1) {
		c = _tstchar();
		if(c) {
			*s = '\0';
			s = get_string(c);
		}
		usDelay = atoi(s);
		DUMP_numeric(usDelay);
		can_send(msg);
		delay(usDelay);
	}
}