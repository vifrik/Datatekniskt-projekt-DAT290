#include "debug.h"
#include "usart.h"

void print_can_msg(CANMsg msg){
	usart_sendl("#############################");
	usart_send("Message id:");
	usart_send_numericl(msg.msgId);
	usart_send("Message direction:");
	usart_send_numericl(msg.dir);
	usart_send("Message nodeId:");
	usart_send_numericl(msg.nodeId);
	usart_send("Message length:");
	usart_send_numericl(msg.length);
	usart_sendl("Message data contents:");
	for(int i = 0; i < msg.length; i++){
		usart_send("Data at index ");
		usart_send_numeric(i);
		usart_send(": ");
		usart_send_numericl(msg.buff[i]);
	}
	usart_sendl("#############################");
}
