#include "stk.h"
#include "can.h"
#include "messages.h"
#include "door_sensor.h"
#include "shared_peripheral.h"
#include "debug.h"
#include "lamp.h"
#include "door_peripheral.h"

extern State state;
extern unsigned long sys_time;

DoorState door_states[7];
unsigned char number_of_doors = 0;

void door_alarm_lower(void){
	usart_sendl("Lowering alarm");
	red_lamp_disable();
	for(int i = 0; i < number_of_doors; i++){
		door_states[i].alarm = 0;
		door_states[i].opened = 0;
	}
	state.alarm = 0;
}

void update_tolerance(CANMsg *msg) {
	unsigned char door_id = msg->buff[0];
	door_states[door_id].tolerance = msg->buff[1];
}

void ndoors_handler(CANMsg *msg) {
	number_of_doors = msg->buff[0];
	usart_send("Enabling ");
	usart_send_numeric(number_of_doors);
	usart_sendl(" doors");
	for(int i = 0; i < number_of_doors; i++){
		door_states[i].active = 1;
	}
}

void door_set_active(CANMsg *msg){
	uchar id = msg->buff[0];
	door_states[id].active = msg->buff[1];
	if(door_states[id].active){
		green_lamp_disable(id);
	} else {
		green_lamp_enable(id);
	}
}

void init_doors(){
	DoorState ds = {0,0,0,0};
	for(int i = 0; i < 7; i++){
		door_states[i] = ds;
	}
}

// Hanterar CAN-meddelanden
void door_receiver(void) {
	CANMsg msg;
	can_receive(&msg);

	if (msg.nodeId == state.id) {
		switch(msg.msgId) {
			case ALARM:
				break;
			case ALARM_OFF:
				door_alarm_lower();
				break;
			case POLL_REQUEST:
				poll_respond(&msg);
				break;
			case POLL_RESPONSE:
				break;
			case DICP_REQUEST:
				break;
			case DICP_RESPONSE:
				update_id(&msg);
				break;
			case TOL_SET:
				update_tolerance(&msg);
				break;
			case DOORS_SET:
				ndoors_handler(&msg);
				break;
			case ACTIVATE:
				door_set_active(&msg);
		}
	}
}

// Initialisering av dörrenhet
void door_peripheral_init(void) {
	DUMP("Door");
	state_init();
	
	lamp_init();
	door_init();

	can1_init(door_receiver);
	request_id(DOOR, number_of_doors);

	stk_init();
}

// Huvudslinga för periferienhet
void door_peripheral_think(void) {	
	while(1) {
		for(int i = 0; i < number_of_doors; i++ ){
			char door_status = door_read(i);
			
			if(!door_states[i].active) continue;
			if (!door_status){
				if(!state.alarm) red_lamp_disable();
				door_states[i].opened = 0;
			}
			
			if(door_states[i].opened && (sys_time - door_states[i].opened > door_states[i].tolerance * 1000000)){
				if(!door_states[i].alarm){
					alarm_raise(i);
					door_states[i].alarm = 1;
				}
			} else if (!door_states[i].alarm && door_status && !door_states[i].opened) {
				red_lamp_enable();
				door_states[i].opened = sys_time;
			}	
		}
	}
}