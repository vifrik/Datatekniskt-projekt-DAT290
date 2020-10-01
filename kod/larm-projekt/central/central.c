#include "messages.h"
#include "can.h"
#include "usart.h"
#include "stk.h"
#include "central.h"
#include "error.h"
#include "keypad.h"

#define DEVICES_MAX 15

//void *_sbrk(int incr) { return (void *)-1; }

extern unsigned long sys_time;
State state = {0, 0, 0, 1};
Peripheral peripherals[DEVICES_MAX];

CANMsg msg_create(uchar msgId, uchar nodeId, uchar dir, uchar length){
	CANMsg msg;
	msg.msgId = msgId;
	msg.nodeId = nodeId;
	msg.dir = dir;
	msg.length = length;
	
}

void peripheral_init(Peripheral *p, uchar type) {
	p->type = type;
	p->alarm = 0;
	p->units = 0;
    for (int i = 0; i < 8; i++) {
        p->buff[i] = 0;
    }
}

// Genererar ett slumpmässigt 16-bitars tal med hjälp av standard c-funktioner
uchar random_gen(void) {
    //srand(sys_time); 
    //return (uchar)(rand() % 256);
	return 10;
}

// Ändrade så att den ger id för vilken dörr/rörelseenhet istället för id för periferienheten
// Tänker mig att det skickas med i larmmeddelandet
void raise_alarm(CANMsg *msg) {
	Peripheral p = peripherals[msg->nodeId];
	p.alarm = 1;
	usart_send("###  ALARM  ###");
    usart_send("from:");
    switch (p.type) {
        case DOOR:
            usart_send("door with id:");
			break;
        case PROXIMITY:
            usart_send("proximity with id:");
			break;
    }

    usart_send_numeric(msg->buff[0]);
}

void poll_request(uchar id) {
    //DUMP("POLL REQUEST SENT");
    CANMsg msg;
	msg = msg_create(POLL_REQUEST, id, TO_PERIPHERAL, 8);
    for (int i = 0; i < msg.length; i++){
        msg.buff[i] = (sys_time & (0xFF << (8*i))) >> (8*i);
        peripherals[id].buff[i] = msg.buff[i];
    }
	//DUMP("POLL SENT");
    can_send(&msg);
}

void poll_response_handler(CANMsg *msg) {
	//DUMP("POLL RESPONSE RECEIVED");
	//Ignorera ifall enheten inte finns med i listan av enheter
	if(msg->nodeId < state.devices){
		Peripheral p = peripherals[msg->nodeId];	
		for (int i = 0; i >= msg->length; i++) {
			if (p.buff[i] != ~msg->buff[i]) {
				raise_alarm(msg->nodeId);
				break;
			}
		}
		state.ready = 1;	
	}
}

void dicp_request_handler(CANMsg *msg) {
    if (state.devices < DEVICES_MAX) {
        DUMP("RECEIVED REQUEST");

        Peripheral p;
        peripheral_init(&p, msg->buff[0]);
		// Tänker mig att periferienheten även skickar med antalet underenheter
		p.units = msg->buff[1];
        peripherals[state.devices] = p;
        
        CANMsg response;
		response = msg_create(DICP_RESPONSE, 0xF, TO_PERIPHERAL, 1);
        response.buff[0] = state.devices;

        state.devices++;

        can_send(&response);
		DUMP("ID SENT");
    } else {
        error_send(msg->msgId, TO_PERIPHERAL, MAX_DEVICES);
    }
}

// active = ACTIVE_ON aktiverar periferienhet p
// active = ACTIVE_OFF deaktiverar
void active_toggle(uchar nodeId, uchar active) {
	CANMsg msg;
	msg = msg_create(active, nodeId, TO_PERIPHERAL, 0);
	can_send(&msg);
}

void set_tolerance(uchar tol, uchar nodeId, uchar unitId){
	CANMsg msg;
	msg_create(TOL_SET, nodeId, 0, 1);
	msg.buff[0] = unitId;
	msg.buff[1] = tol;
	can_send(&msg);
}

void receiver(void) {
	//DUMP("CAN message received: ");
	
    CANMsg msg;
    can_receive(&msg);
	//DUMP_numeric(msg.msgId);
	//DUMP_numeric(POLL_RESPONSE);
	if (msg.dir == TO_CENTRAL) {
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

void timeout_handler(void) {
	uchar id = (state.curr_poll-1)%state.devices;
	raise_alarm(id);
	peripherals[id].alarm = 1;
	state.ready = 1;
}

void init(void) {
    can1_init(receiver);
    keyboard_init();
	stk_init();
	callback_init(timeout_handler);
}

uchar equal(char input[], char passcode[]) {
	for (int i = 0; i < 4; i++) {
		if (input[i] != passcode[i])
			return 0;
	}
	return 1;
}

void alarm_lower(void) {
	for (int i = 0; i < state.devices; i++) {
		CANMsg msg;
		msg.msgId = ALARM_OFF;
		msg.nodeId = i;
		msg.dir = TO_PERIPHERAL;
		msg.length = 0;	
		
		peripherals[i].alarm = 0;
		
		can_send(&msg);
	}
	
	DUMP("ALARM off");
}

void think(void) {
    // Vänta tills minst en enhet är uppkopplad
    while(!state.devices);

    uchar passcode[4] = {1,2,3,4};
    uchar keypad[4] = {0xFF,0xFF,0xFF,0xFF};

    while(1) {
		// Keypad- och USART-logik här
        keyboard_input(keypad);
		
		if (equal(keypad, passcode)) {
			alarm_lower();
			for (int i = 0; i < 4; i++)
				keypad[i] = 0xFF;
		}
		
		if (state.ready) {
			uchar id = state.curr_poll++%state.devices;
			if (peripherals[id].alarm) continue;
			
			delay_no_block(500000);
		
			poll_request(id);
			state.ready = 0;
			
		}
	}
}