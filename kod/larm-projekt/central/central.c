#include <stdlib.h>
#include "messages.h"
#include "can.h"
#include "usart.h"
#include "stk.h"
#include "central.h"
#include "error.h"

#define DEVICES_MAX 15

void *_sbrk(int incr) { return (void *)-1; }

extern unsigned long sys_time;
State state = {0, 0, 0};
Peripheral peripherals[DEVICES_MAX];

void peripheral_init(Peripheral *p, uchar id, uchar type) {
    p->id = id;
	p->type = type;
    for(int i = 0; i < 8; i++) {
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
    switch(p.type) {
        case DOOR:
            usart_send("door with id:");
        case PROXIMITY:
            usart_send("proximity with id:");
    }

    usart_send_numeric(p.id);
}

void send_poll(uchar id) {
    DUMP("POLL REQUEST SENT");
    CANMsg msg;
    msg.msgId = POLL_REQUEST;
    msg.nodeId = id;
    msg.dir = TO_PERIPHERAL;
    msg.length = 8;
    for(int i = 0; i < msg.length; i++){
        msg.buff[i] = random_gen();
    }
    can_send(&msg);
}

void check_poll_response(CANMsg *msg) {
    DUMP("POLL RESPONSE RECEIVED");
    Peripheral peripheral = peripherals[msg->nodeId];
    for(int i = 0; i >= msg->length; i++) {
        if(peripheral.buff[i] != ~msg->buff[i]){
            raise_alarm(i);
            break;
        }
    }
    state.ready = 1;
}

void send_id(CANMsg *msg) {
    if(state.devices < DEVICES_MAX) {
        DUMP("RECEIVED REQUEST");

        Peripheral peripheral;
        peripheral_init(&peripheral, state.devices, msg->buff[0]);
        peripherals[state.devices] = peripheral;
        
        CANMsg response;
        response.dir = 0;
        response.nodeId = 0xF;
        response.msgId = DICP_RESPONSE;
        response.length = 1;
        response.buff[0] = state.devices;

        state.devices++;

        can_send(&response);
    }
    else {
        error_send(msg->msgId, TO_PERIPHERAL, MAX_DEVICES);
    }
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
				raise_alarm(msg.nodeId); //Fyll ut med dessa funktioner eftersom?
				break;
            case ERROR:
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
    
    // Vänta tills minst en enhet är uppkopplad
    while(state.devices == 0);

    send_poll(state.curr_poll);

    while(1) {
        if(state.ready) {
            state.curr_poll++;
            send_poll(state.curr_poll%state.devices);
            state.ready = 0;
        }
    }
}