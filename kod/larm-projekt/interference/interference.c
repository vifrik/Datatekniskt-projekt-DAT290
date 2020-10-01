#include "interference.h"
#include "can.h"
#include "usart.h"
#include "shared_peripheral.h"
#include "messages.h"
#include "stk.h"

void interference_init(void) {
	state_init();
	stk_init();
	can1_init(interference_receiver());
	//mer? inte helt säker på vad som behövs
}

void interference_receiver(void) {
	DUMP("CAN message received: ");
}

//Läser in en sträng från USART tills det kommer en ny rad.
//Tar in det första avlästa värdet from konsolen och adressen till var strängen skall ligga.
/*uchar**/void get_string(uchar c, uchar *address) { //Bug: ibland blir all input 0
	uchar *s;
	s = address;
	uchar input = c;
	//uchar *output;
	//output = s;
	//DUMP("here");
	
	while(input != '\n') {
		*s = input;
		s++;
		_outchar(input);
		input = _getchar();
	}
	//DUMP("\nhere too");
	_outchar('\n');
	
	//return output;
}

void interference_think(void) {
	CANMsg *msg;
	canmsg_init(msg);
	msg->msgId = POLL_RESPONSE;
	//DUMP("\nBegin");
	
	uchar *s;
	uchar *sAddress;
	sAddress = s;
	//*s = 0;
	uchar c;
	unsigned int usDelay = 1000000;
	while(1) {
		/*c = _tstchar();
		if(c) {
			//DUMP("Yup");
			//DUMP_numeric(c);
			/**s*///*sAddress = '\0';
			/*s = *///get_string(c, sAddress);
			//s = sAddress;
			//DUMP("After get");
			//usDelay = atoi(s);
			//DUMP_numeric(usDelay);
		//}*/
		//usDelay = atoi(s);
		//DUMP("One");
		//DUMP_numeric_list(s, 1);//sizeof(s));
		//DUMP("Two");
		DUMP_numeric(usDelay);
		can_send(msg);
		delay(usDelay);
		//delay_no_block(usDelay);
		//while(/*väntar på delayen*/) {
			c = _tstchar();
			if(c) {
				get_string(c, sAddress);
				s = sAddress;
				usDelay = atoi(s);
			}
		//}
	}
}