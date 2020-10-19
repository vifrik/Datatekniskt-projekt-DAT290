#include "interference.h"
#include "can.h"
#include "usart.h"
#include "messages.h"
#include "stk.h"

// Default 1s delay
unsigned int delay_time = 10000;
CANMsg msg;

void append_char_buff(unsigned char buffer[], unsigned char length, unsigned char c) {
	for (int i = 0; i < length - 1; i++) {
		buffer[i] = buffer[i+1];
	}

	buffer[length-1] = c;

	_outchar(c);
}

void clear_buff(unsigned char buffer[], unsigned char length) {
	for (int i = 0; i < length; i++) {
		buffer[i] = 48;
	}
}

void interference_receiver(void) {
	//DUMP("CAN message received: ");
}

void timer_callback() {
	can_send(&msg);
	DUMP("Sent");
	delay_no_block(delay_time);
}

void interference_init(void) {
	can1_init(interference_receiver);
	
	msg.nodeId = 0;
	msg.msgId = POLL_RESPONSE;
	msg.dir = TO_CENTRAL;
	msg.length = 0;
	clear_buff(msg.buff, 8);
	
	stk_init();
	systick_callback_init(timer_callback);
	timer_callback();
}

void interference_think(void) {
	unsigned char charBuffer[10] = {48,48,48,48,48,48,48,48,48,48};

	// Main loop
	while(1) {
		unsigned char c = _tstchar();
		DUMP_numeric(c);
		if (c == 13 || c == 10) {
			usart_send("Updated delay to: ");
			for (int i = 0; i<10;i++) _outchar(charBuffer[i]);
			usart_send_numericl(delay_time = atoi(charBuffer));
			clear_buff(charBuffer,10);
		} else if (c) append_char_buff(charBuffer, 10, c);
	}
}