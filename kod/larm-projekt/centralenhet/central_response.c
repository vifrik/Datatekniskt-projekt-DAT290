#include "messages.h"
#include "can.h"
#include "usart.h"
void raise_alarm(void) {
	//Do things
	DUMP("PANIC");
}

void check_poll_response(CANMsg *msg) {
	//if(msg.buff = )
	//Kolla så svar från rätt nod
	//Kolla om alarm
	//Kolla så responsen är korrekt
}

void send_id(CANMsg *msg) {
	DUMP("RECEIVED REQUEST");
	CANMsg response;
	response.nodeId = 0; //Kalla någon funktion som håller reda på någon array av alla inkopplade periferienheter
	response.msgId = DICP_RESPONSE;
	response.length = 1;
	response.buff[0] = 1;
	can_send(&response);
	DUMP("DICP RESPONSE SENT");
}

void receiver(void) {
	DUMP("CAN message received: ");
	
    CANMsg msg;
    can_receive(&msg);
	DUMP_numeric(msg.msgId);
	DUMP_numeric(ALARM);
	if (msg.dir == 1) {
		switch (msg.msgId) {
			case ALARM:
				raise_alarm(); //Fyll ut med dessa funktioner eftersom?
				break;
			case ALARM_OFF:
				break;
            case POLL_REQUEST:
                break;
			case POLL_RESPONSE:
				check_poll_response(&msg);
				break;
			case DICP_REQUEST:
				send_id(&msg);
				break;
			case DICP_RESPONSE:
				break;
			case TOL_SET:
				break;
			case ACTIVE_ON:
				break;
			case ACTIVE_OFF:
				break;
		}
		
	}
}

void init(void) {
	can1_init(receiver);
}