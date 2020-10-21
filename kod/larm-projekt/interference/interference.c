#include "stk.h"
#include "can.h"
#include "messages.h"
#include "usart.h"

// CONFIG
// Uppdatera denna för att ändra delay
unsigned int delay_time = 500000;
//END CONFIG

CANMsg msg;

void interference_receiver(void) {}

void timer_callback(void) {
	can_send(&msg);
	usart_sendl("Sent");
	delay_no_block(delay_time);
}

void interference_init(void) {
	msg.nodeId = 0xF;
	msg.msgId = ERROR;
	msg.dir = TO_CENTRAL;
	msg.length = 0;
	msg.buff[0] = 0;
	
	stk_init();
	systick_callback_init(timer_callback);
	timer_callback();
	
	can1_init(interference_receiver);
}