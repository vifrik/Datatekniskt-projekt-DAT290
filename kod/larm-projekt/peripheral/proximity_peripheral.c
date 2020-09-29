#include "stk.h"
#include "can.h"
#include "messages.h"
#include "proximity_sensor.h"
#include "shared_peripheral.h"

extern State state;

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

// Initialisering av rörelsesensor och CAN
void proximity_peripheral_init(void) {
	state_init();
	
	proximity_init();
	can1_init(proximity_receiver);
	request_id(PROXIMITY);
	
	stk_init();
}

unsigned char counter = 5;

// Huvudslinga för periferienhet
void proximity_peripheral_think(void) {	
	while(1) {
		
		// Flyttar fram alla värden ett steg
		for (int i = 0; i < 4; i++) {
			sample[i] = sample[i+1];
		}
		
		// Uppdaterar sista värdet i sample
		sample[4] = proximity_read();
		
		unsigned char sampleMean = sample_mean(sample);
		unsigned char sampleVar = sample_variance(sample, sampleMean);
		
		DUMP("Reading:");
		DUMP_numeric(sampleMean);
		DUMP_numeric(sampleVar);
		DUMP_numeric(sample[4]);
		DUMP("");

		
		// Om vi har räknat fem värden och vi inte har larmat och urvalsvariansen är större än tio -> larma
		if (!counter && !state.alarm && sampleVar > 10) {
			alarm_raise();
		}
		
		if (counter)
			counter--;
		
		// Fördröj med 10^-6s = 1s
		delay(1000000);
	}
}