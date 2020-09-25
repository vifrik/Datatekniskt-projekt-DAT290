#include <stdlib.h>
#include "messages.h"
#include "can.h"
#include "usart.h"
#include "stk.h"
#include "central.h"
#include "error.h"
#include "keypad.h"

#define DEVICES_MAX 15

void *_sbrk(int incr) { return (void *)-1; }

extern unsigned long sys_time;
State state = {0, 0, 0, 0};
Peripheral peripherals[DEVICES_MAX];

void peripheral_init(Peripheral *p, uchar type) {
	p->type = type;
    for (int i = 0; i < 8; i++) {
        p->buff[i] = 0;
    }
}

// Genererar ett slumpmässigt 16-bitars tal med hjälp av standard c-funktioner.
uchar random_gen(void) {
    srand(sys_time); 
    return (uchar)(rand() % 256); 
}

void raise_alarm(uchar id) {
	Peripheral p = peripherals[id];
	usart_send("###  ALARM  ###");
    usart_send("from:");
    switch (p.type) {
        case DOOR:
            usart_send("door with id:");
        case PROXIMITY:
            usart_send("proximity with id:");
    }

    usart_send_numeric(id);
}

void poll_request(uchar id) {
    DUMP("POLL REQUEST SENT");
    CANMsg msg;
    msg.msgId = POLL_REQUEST;
    msg.nodeId = id;
    msg.dir = TO_PERIPHERAL;
    msg.length = 8;
    for (int i = 0; i < msg.length; i++){
        msg.buff[i] = random_gen();
        peripherals[id].buff[i] = msg.buff[i];
    }
    can_send(&msg);
}

void poll_response_handler(CANMsg *msg) {
	DUMP("POLL RESPONSE RECEIVED");
	Peripheral p = peripherals[msg->nodeId];	
    for (int i = 0; i >= msg->length; i++) {
        if (p.buff[i] != ~msg->buff[i]) {
			raise_alarm(msg->nodeId);
			break;
        }
    }
    state.ready = 1;
}

void dicp_request_handler(CANMsg *msg) {
    if (state.devices < DEVICES_MAX) {
        DUMP("RECEIVED REQUEST");

        Peripheral p;
        peripheral_init(&p, msg->buff[0]);
        peripherals[state.devices] = p;
        
        CANMsg response;
		response.msgId = DICP_RESPONSE;
		response.nodeId = 0xF;
        response.dir = TO_PERIPHERAL;
        response.length = 1;
        response.buff[0] = state.devices;

        state.devices++;

        can_send(&response);
    } else {
        error_send(msg->msgId, TO_PERIPHERAL, MAX_DEVICES);
    }
}

// active = ACTIVE_ON aktiverar periferienhet p
// active = ACTIVE_OFF deaktiverar
void active_toggle(uchar id, uchar active) {
	CANMsg msg;
	msg.msgId = active;
	msg.nodeId = id;
	msg.dir = TO_PERIPHERAL;
	msg.length = 0;
}

void receiver(void) {
	DUMP("CAN message received: ");
	
    CANMsg msg;
    can_receive(&msg);
	DUMP_numeric(msg.msgId);
	if (msg.dir == 1) {
		switch (msg.msgId) {
			case ALARM:
				raise_alarm(msg.nodeId); //Fyll ut med dessa funktioner eftersom?
				break;
            case ERROR:
                break;
			case ALARM_OFF:
				break;
            case POLL_REQUEST:
                break;
			case POLL_RESPONSE:
				poll_response_handler(&msg);
				break;
			case DICP_REQUEST:
				dicp_request_handler(&msg);
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

void init() {
    //can1_init(receiver);
    keyboard_init();
}

uchar same(uchar arr1[], uchar arr2[]) {
	for (int i = 0; i < 4; i++) {
		if (arr1[i] != arr2[i])
		return 0;
	}	
	return 1;
}

void think(void) {
    // Vänta tills minst en enhet är uppkopplad
    //while(!state.devices);

    uchar passcode[4] = {1,2,3,4};
    uchar keypad[5] = {0,0,0,0};

    while(1) {
		// Keypad- och USART-logik här
        keyboard_input(keypad);
		
		DUMP_numeric_list(keypad, 4);
		
		/*if (state.ready) {
			poll_request(state.curr_poll++%state.devices);
			state.ready = 0;
		}*/
	}
}