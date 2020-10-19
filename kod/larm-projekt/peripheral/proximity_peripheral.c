#include "stk.h"
#include "can.h"
#include "messages.h"
#include "proximity_sensor.h"
#include "vibration_sensor.h"
#include "shared_peripheral.h"
#include "lamp.h"
#include "debug.h"

#define SAMPLE_SIZE 10

unsigned int sample[SAMPLE_SIZE];

extern State state;
uchar tolerance = 10;
uchar distance_active = 1;
uchar vibration_active = 1;

void update_tolerance_proximity(CANMsg *msg) {
	tolerance = msg->buff[1];
}

void proximity_set_active(CANMsg *msg) {
	print_can_msg(*msg);
	msg->buff[1] ? usart_send("Enabled ") : usart_send("Disabled ");
	msg->buff[0] ? usart_sendl("vibration sensor") : usart_sendl("distance sensor");
	
	if (msg->buff[0]) {
		vibration_active = msg->buff[1];
	} else {
		distance_active = msg->buff[1];
	}
}

// Hanterar CAN-meddelanden
void proximity_receiver(void) {
	//DUMP("CAN message received: ");

	CANMsg msg;
	can_receive(&msg);
	

	// Om meddelandets mottagare matchar mitt id
	if (msg.nodeId == state.id) {
		switch(msg.msgId) {
			case ALARM:
				break;
			case ALARM_OFF:
				alarm_lower();
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
				update_tolerance_proximity(&msg);
				break;
			case ACTIVATE:
				proximity_set_active(&msg);
		}
	}
}

// Urvalsmedelvärde
unsigned int sample_mean(unsigned int data[]) {
	unsigned long sum = 0;
	
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		sum += data[i];
	}
	
	return sum/SAMPLE_SIZE;
}

// Urvalsvarians
unsigned int sample_variance(unsigned int data[], unsigned int sampleMean) {
	unsigned long variance = 0;
	
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		variance += ((data[i]-sampleMean)*(data[i]-sampleMean));
	}
	
	return variance/(SAMPLE_SIZE - 1);
}

void proximity_callback(unsigned int distance) {
	DUMP("123");
	// Flyttar fram alla värden ett steg
	for (int i = 0; i < SAMPLE_SIZE - 1; i++) {
		sample[i] = sample[i+1];
	}
		
	// Uppdaterar sista värdet i sample
	sample[SAMPLE_SIZE - 1] = distance;
	
}

void vibration_callback(void) {
	if (!state.alarm && state.id != 0xF && vibration_active) {
		alarm_raise(1);
	}
}

// Initialisering av rörelsesensor och CAN
void proximity_peripheral_init(void) {
	state_init();
	lamp_init();

	proximity_init();
	proximity_callback_init(proximity_callback);

	vibration_init();
	vibration_callback_init(vibration_callback);
	
	stk_init();
	
	can1_init(proximity_receiver);
	request_id(PROXIMITY, 2);
}

uchar all_nonzero(unsigned int values[], uchar length){
	for(int i = 0; i < length; i++){
		if(values[i] == 0) return 0;
	}
	return 1;
}

void set_all_zero(unsigned int values[], uchar length){
	for(int i = 0; i < length; i++){
		values[i] = 0;
	}
}

//unsigned char counter = SAMPLE_SIZE * 10;

// Huvudslinga för periferienhet
void proximity_peripheral_think(void) {
	while(state.id == 0xF);
	set_all_zero(sample, SAMPLE_SIZE);
	while(1) {
		unsigned int sampleMean = sample_mean(sample);
		unsigned int sampleVar = sample_variance(sample, sampleMean);
		
		// Om vi har räknat fem värden och vi inte har larmat och urvalsvariansen är större än tio -> larma
		if (!state.alarm && all_nonzero(sample, SAMPLE_SIZE) && sampleVar > tolerance && distance_active) {
			DUMP_numeric(sampleVar);
			DUMP_numeric(sampleMean);
			alarm_raise(0);
		}
		
		proximity_read();
		delay(25000);
	}
}