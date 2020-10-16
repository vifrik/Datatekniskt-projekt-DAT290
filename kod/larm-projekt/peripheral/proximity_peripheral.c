#include "stk.h"
#include "can.h"
#include "messages.h"
#include "proximity_sensor.h"
#include "vibration_sensor.h"
#include "shared_peripheral.h"

#define SAMPLE_SIZE 128

extern State state;
unsigned long tolerance = 10;

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
				update_tolerance(&msg);
				break;
			case ACTIVE_ON:
				state.active = 1;
				break;
			case ACTIVE_OFF:
				state.active = 0;
				break;
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

unsigned int sample[SAMPLE_SIZE];

void vibration_callback(void) {
	if (!state.alarm && state.id != 0xF) {
		DUMP("abc123");
		alarm_raise(1);
	}
}

// Initialisering av rörelsesensor och CAN
void proximity_peripheral_init(void) {
	DUMP("Proximity");
	state_init();
	proximity_init();
	vibration_init();
	vibration_callback_init(vibration_callback);
	can1_init(proximity_receiver);
	request_id(PROXIMITY, 2);
	
	stk_init();
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
		
		
		// Flyttar fram alla värden ett steg
		for (int i = 0; i < SAMPLE_SIZE - 1; i++) {
			sample[i] = sample[i+1];
		}
		
		
		// Uppdaterar sista värdet i sample
		sample[SAMPLE_SIZE - 1] = proximity_read();
		//if(!state.alarm) DUMP_numeric(sample[SAMPLE_SIZE - 1]);
		
		unsigned int sampleMean = sample_mean(sample);
		unsigned int sampleVar = sample_variance(sample, sampleMean);
		
		// Om vi har räknat fem värden och vi inte har larmat och urvalsvariansen är större än tio -> larma
		if (!state.alarm && all_nonzero(sample, SAMPLE_SIZE) && sampleVar > tolerance) {
			DUMP_numeric(sampleVar);
			DUMP_numeric(sampleMean);
			alarm_raise(0);
		}
		delay(2500);
		
	}
}