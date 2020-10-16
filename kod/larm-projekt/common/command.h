#ifndef COMMAND_H
#define COMMAND_H

typedef unsigned char uchar;

enum COMMAND {
	ACTIVE,
	TOL,
	NDOORS,
	TEST1,
	UNKNOWN,
	PASSCODE,
	HELP,
	SHOW
};

typedef struct {
	uchar command;
	uchar active;
	unsigned int pass;
	unsigned int arg0;
	unsigned int arg1;
	unsigned int arg2;
} Command;

void command_init();
Command command_handler();

#endif