#include "stk.h"
#include "can.h"

#define MAX_BUFF_SIZE 64

CANMsg busTraffic[MAX_BUFF_SIZE];
unsigned char count = 0;


// Enheten tar emot all busstrafik och sparar alla meddelanden i en array
void replay_receiver(void) {

	CANMsg msg;
	can_receive(&msg);	

	busTraffic[count] = msg;
	count++;
}

void replay_init(void){
	can1_init(replay_receiver);
}


// Väntar tills arrayn med meddelanden är full, skickar sedan alla meddelanden på bussen och nollställer sedan count.
void replay_think(void){
	while(1){
		if(count == MAX_BUFF_SIZE){
			for(int i = 0; i < MAX_BUFF_SIZE; i++){
				can_send(&busTraffic[i]);
			}
			count = 0;
		}
	}
}