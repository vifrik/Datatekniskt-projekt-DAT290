#include <stdlib.h>
#include "messages.h"
#include "can.h"
#include "usart.h"
#include "stk.h"
#include "central.h"

void *_sbrk(int incr) { return (void *)-1; }

extern unsigned long sys_time;
int newId = 0;
int counter = 0;
int ready = 0;
PERIPHERAL peripherals[15];

void init_peripheral(PERIPHERAL *p) {
    p->type = 0;
    for(int i = 0; i < 8; i++) {
        p->buff[i] = 0;
    }
}

// Genererar ett slumpmässigt 16-bitars tal med hjälp av standard c-funktioner.
uchar random_gen(void) {
    srand(sys_time); 
    return (uchar)(rand() % 256); 
}

void raise_alarm(void) {
	//Do things
	DUMP("PANIC");
}

void send_poll(uchar id) {
    DUMP("POLL REQUEST SENT");
    CANMsg msg;
    msg.msgId = POLL_REQUEST;
    msg.nodeId = id;
    msg.dir = 0;
    msg.length = 8;
    for(int i = 0; i < msg.length; i++){
        msg.buff[i] = random_gen();
    }
    can_send(&msg);
}

void check_poll_response(CANMsg *msg) {
    DUMP("POLL RESPONSE RECEIVED");
   // delay(1000000);
    for(int i = 0; i >= msg->length; i++) {
        if(peripherals[msg->nodeId].buff[i] != ~msg->buff[i]){
            raise_alarm();
        }
    }
    ready = 1;
}

void periphery_init(Periphery *p, uchar id, uchar type) {
	p->id = id;
	p->type = type;
}

Periphery units[5]; //5 är antagen max antal enheter här

//type är typen på den nyanslutna enheten
//
uchar dicp_next_id(uchar type) {
	uchar id;
	uchar length = sizeof(units)/sizeof(units[0]);
	uchar i = 0;
	while(i <= length) {
		if(units[i].type == 0) { //vet inte riktigt hur vi visar typer än, men så som det är just nu så ska ingen ha typ 0
			id = i;
			break;
		} else if(i == length) {
			//shit hits the fan
		}
		i++;
	}
	//Lägg till ny enhet i arrayen.
	Periphery *p;
	periphery_init(p, id, type);
	units[id] = *p;
	
	return length;
}

void send_id(CANMsg *msg) {
    if(newId < 16){
        DUMP("RECEIVED REQUEST");

        PERIPHERAL peripheral;
        init_peripheral(&peripheral);
        peripheral.type = msg->buff[0];
        peripherals[newId] = peripheral;
        
        CANMsg response;
        response.dir = 0;
        response.nodeId = 0xF;
        response.msgId = DICP_RESPONSE;
        response.length = 1;
        response.buff[0] = newId;
        newId++;
        can_send(&response);
        DUMP("DICP RESPONSE SENT");
    }
    else {
        //TODO error
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
    
    while(newId == 0); 
    send_poll(0); 
    while(1) {
        if(ready) {
                counter++;
                send_poll(counter%newId);
                ready = 0;
        }
    }
}