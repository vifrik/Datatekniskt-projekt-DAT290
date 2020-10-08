#ifndef COMMAND_H
#define COMMAND_H

typedef unsigned char uchar;

enum COMMAND {
	ACTIVE,
	TOL,
	NDOORS,
	TEST1,
	UNKNOWN,
	HELP,
	SHOW
};

typedef struct {
	uchar command;
	uchar arg0;
	uchar arg1;
	uchar arg2;
} Command;

void command_init();
Command command_handler();

#endif