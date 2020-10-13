#include "stk.h"
#include "can.h"
#include "messages.h"
#include "proximity_sensor.h"
#include "shared_peripheral.h"

extern State state;
uchar tolerance;

// Hanterar CAN-meddelanden
void proximity_receiver(void) {
	DUMP("CAN message received: ");

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
unsigned char sample_mean(unsigned char data[]) {
	unsigned char sum = 0;
	
	for (int i = 0; i < 5; i++) {
		sum += data[i];
	}
	
	return sum/5;
}

// Urvalsvarians
unsigned char sample_variance(unsigned char data[], unsigned char sum) {
	unsigned char variance = 0;
	
	for (int i = 0; i < 5; i++) {
		variance += ((data[i]-sum)*(data[i]-sum));
	}
	
	return variance/4;
}

unsigned char sample[5];

void vibration_callback(void) {
	if (!state.alarm) {
		alarm_raise(1);
	}
}

// Initialisering av rörelsesensor och CAN
void proximity_peripheral_init(void) {
	state_init();
	
	proximity_init();
	vibration_init();
	vibration_callback_init(vibration_callback);
	//can1_init(proximity_receiver);
	//request_id(PROXIMITY, 2);
	
	stk_init();
}

unsigned char counter = 5;

// Huvudslinga för periferienhet
void proximity_peripheral_think(void) {	
	while(1) {
		
		//uchar vibration = vibration_read();
		
		// Flyttar fram alla värden ett steg
		for (int i = 0; i < 4; i++) {
			sample[i] = sample[i+1];
		}
		
		// Uppdaterar sista värdet i sample
		sample[4] = proximity_read();
		
		unsigned char sampleMean = sample_mean(sample);
		unsigned char sampleVar = sample_variance(sample, sampleMean);
		
		// Om vi har räknat fem värden och vi inte har larmat och urvalsvariansen är större än tio -> larma
		if (!counter && !state.alarm && sampleVar > tolerance) {
			alarm_raise(1);
		}
		
		if (counter)
			counter--;
	}
}