#include "can.h"
#include "usart.h"
#include "messages.h"

void error_send(uchar id, uchar dir, uchar error) {
    CANMsg msg;

    msg.msgId = ERROR;
    msg.nodeId = id;
    msg.dir = 0;
    msg.length = 1;
    msg.buff[0] = error;

    can_send(&msg);

}

void error_handler(CANMsg *msg) {
    uchar error_code = msg->buff[0];

    usart_send("###  ERROR  ###");
    usart_send_numeric(error_code);

    switch(error_code) {
        case MAX_DEVICES:
            usart_send("Max peripheral devices reached");
            break;
    }
}