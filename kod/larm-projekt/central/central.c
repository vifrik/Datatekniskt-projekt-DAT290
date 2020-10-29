#include "messages.h"
#include "can.h"
#include "usart.h"
#include "stk.h"
#include "central.h"
#include "error.h"
#include "keypad.h"
#include "command.h"
#include "debug.h"
#include "lamp.h"

#define DEVICES_MAX 15
#define INPUT_BUFFER_SIZE 2

extern unsigned long sys_time;
State state	= {0, 0, 0, 1};
uchar passcode[4] = {1, 2, 3, 4};
Peripheral peripherals[DEVICES_MAX];

CANMsg msg_create(uchar msgId, uchar nodeId, uchar dir, uchar length){
	CANMsg msg;
	msg.msgId = msgId;
	msg.nodeId = nodeId;
	msg.dir = dir;
	msg.length = length;
	return msg;
}

void peripheral_init(Peripheral *p, uchar type) {
	p->type = type;
	p->alarm = 0;
	p->units = 0;
    for (int i = 0; i < 8; i++) {
        p->buff[i] = 0;
    }
}

// Ändrade så att den ger id för vilken dörr/rörelseenhet istället för id för periferienheten
// Tänker mig att det skickas med i larmmeddelandet
void raise_alarm(CANMsg *msg) {
	red_lamp_enable();
	Peripheral p = peripherals[msg->nodeId];
	p.alarm = 1;
	usart_send("###  ALARM  ###");
    usart_sendl(" from:");
    switch (p.type) {
        case DOOR:
            usart_send("Door with id:");
			break;
        case PROXIMITY:
            usart_send("Proximity with id:");
			break;
    }

	usart_send_numeric(msg->nodeId);
	usart_send(" from subdevice ");
    usart_send_numericl(msg->buff[0]);
	
}

void alarm_lower(void) {
	red_lamp_disable();
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

void poll_alarm(uchar id){
	usart_send("###  POLL ALARM  ###");
	usart_send("from:");
	usart_send_numeric(id);
}


void timeout_alarm(uchar id){
	red_lamp_enable();
	usart_send("###  TIMEOUT ALARM  ###");
	usart_send("from:");
	usart_send_numeric(id);
}

// Skicka ett poll-meddelande till periferienhet med id, id
void poll_request(uchar id) {
    CANMsg msg;
	msg = msg_create(POLL_REQUEST, id, TO_PERIPHERAL, 8);
    for (int i = 0; i < msg.length; i++){
        msg.buff[i] = (sys_time & (0xFF << (8*i))) >> (8*i);
        peripherals[id].buff[i] = msg.buff[i];
    }
    can_send(&msg);
}

// Hanterar "poll response"-meddelanden
void poll_response_handler(CANMsg *msg) {
	//Ignorera ifall enheten inte finns med i listan av enheter
	if(msg->nodeId < state.devices){
		Peripheral p = peripherals[msg->nodeId];	
		for (int i = 0; i < msg->length; i++) {
			// Om den skickade buffern inte stämmer överrens med den inkommande inverterad, larma
			if (p.buff[i] != (uchar)~msg->buff[i]) {
				poll_alarm(msg->nodeId);
				break;
			}
		}
		state.ready = 1;	
	}
}

void dicp_request_handler(CANMsg *msg) {
    if (state.devices < DEVICES_MAX) {
        usart_sendl("RECEIVED REQUEST");
        Peripheral p;
        peripheral_init(&p, msg->buff[0]);

		p.units = msg->buff[1];
        peripherals[state.devices] = p;
        
        CANMsg response;
		response = msg_create(DICP_RESPONSE, 0xF, TO_PERIPHERAL, 1);
        response.buff[0] = state.devices;

        state.devices++;

        can_send(&response);
		usart_sendl("ID SENT");
    } else {
        error_send(msg->msgId, TO_PERIPHERAL, MAX_DEVICES);
    }
}

void active_toggle(uchar nodeId, uchar unitId, uchar active) {
	CANMsg msg;
	msg = msg_create(ACTIVATE, nodeId, TO_PERIPHERAL, 2);
	msg.buff[0] = unitId;
	msg.buff[1] = active;
	can_send(&msg);
}

void set_tolerance(uchar nodeId, uchar unitId, uchar tol){
	CANMsg msg = msg_create(TOL_SET, nodeId, TO_PERIPHERAL, 2);
	msg.buff[0] = unitId;
	msg.buff[1] = tol;
	can_send(&msg);
}

void set_ndoors(uchar nodeId, uchar ndoors){
	CANMsg msg = msg_create(DOORS_SET, nodeId, TO_PERIPHERAL, 1);
	msg.buff[0] = ndoors;
	peripherals[nodeId].units = ndoors;
	can_send(&msg);
}

void set_passcode(unsigned int newpass, unsigned int newpass1) {
	int tempPass = newpass;
	
	// nya lösen stämmer överrens
	if (newpass == newpass1) {
		for (int i = 3; i >= 0; i--) {
			passcode[i] = tempPass % 10;
			tempPass /= 10;
		}
		
		usart_sendl("Passcode is set!");
	}
}


void help_msg(void) {
	usart_send("\nEnter commands into the console to configure system, press 'enter' to submit command\n");
	usart_send("These are the available options:\n");
	usart_send("Activate or Deactivate unit:\n Type 'active passcode nodeId 0/1'\n");
	usart_send("Set Tolerance:\n Type 'tol passcode nodeId unitId time'\n");
	usart_send("Set number of doors:\n Type 'nDoors passcode nodeId numberOfDoors'\n");
	usart_send("Show connected units:\n Type 'show passcode'\n");
	usart_send("Change passcode:\n Type 'passcode oldcode newcode newcode'\n");
}

void show_units(void) {
	usart_send("\nConnected units are:\n");
	for(uchar i = 0; i < state.devices; i++) {
		Peripheral p = peripherals[i];
		usart_send("\n");
		switch (p.type) {
			case DOOR:
				usart_send("Door with id: ");
				usart_send_numeric(i);
				usart_send(" and ");
				usart_send_numeric(p.units);
				usart_send(" connected doors\n");
				break;
			case PROXIMITY:
				usart_send("Proximity with id: ");
				usart_send_numeric(i);
				usart_send(" and ");
				usart_send_numeric(p.units);
				usart_send(" connected sensors\n");
				break;
		}
    }
}

void receiver(void) {
    CANMsg msg;
    can_receive(&msg);

	if (msg.dir == TO_CENTRAL) {
		switch (msg.msgId) {
			case ALARM:
				raise_alarm(&msg); //Fyll ut med dessa funktioner eftersom?
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
		}
	}
}

void timeout_handler(void) {
	uchar id = (state.curr_poll-1)%state.devices;
	timeout_alarm(id);
	peripherals[id].alarm = 1;
	state.ready = 1;
}

uchar equal(char input[], char passcode[]) {
	for (int i = 0; i < 4; i++) {
		if (input[i] != passcode[i])
			return 0;
	}
	return 1;
}


void command_parser(Command cmd) {
	if (!cmd.active) return;
	cmd.active = 0;
	
	unsigned int pass = passcode[0]*1000+passcode[1]*100+passcode[2]*10+passcode[3];
	if (pass != cmd.pass && cmd.command != HELP) {
		usart_sendl("Wrong passcode, use help");
		return;
	}

	switch(cmd.command) {
		case ACTIVE:
			active_toggle(cmd.arg0, cmd.arg1, cmd.arg2);
			break;
		case TOL:
			set_tolerance(cmd.arg0, cmd.arg1, cmd.arg2);
			break;
		case NDOORS:
			set_ndoors(cmd.arg0, cmd.arg1);
			break;
		case TEST1:
			break;
		case UNKNOWN:
			usart_sendl("Unknown command!\n");
			break;
		case PASSCODE:
			set_passcode(cmd.arg0, cmd.arg1);
			break;
		case HELP:
			help_msg();
			break;
		case SHOW:
			show_units();
			break;
	}
}

void init(void) {
    can1_init(receiver);
    keyboard_init();
	lamp_init();
	
	stk_init();
	systick_callback_init(timeout_handler);
	
	command_init();
}

void think(void) {
    // Vänta tills minst en enhet är uppkopplad
    while(!state.devices);

    uchar keypad[4] = {0xFF,0xFF,0xFF,0xFF};

    while(1) {
		// Keypad- och USART-logik här
        keyboard_input(keypad);
		command_parser(command_handler());
		
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