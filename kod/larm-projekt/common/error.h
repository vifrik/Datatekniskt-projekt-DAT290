#ifndef ERROR_H
#define ERROR_H

void error_send(uchar id, uchar dir, uchar error);
void error_handler(CANMsg *msg);

#endif