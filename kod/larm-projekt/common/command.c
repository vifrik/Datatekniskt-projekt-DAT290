#include <stdlib.h>
#include "usart.h"
#include "command.h"

// 20 bytes for command, 2 bytes for spaces, 1 bytes for each argument
#define BUFFER_LENGTH 24

void buffer_append(uchar buffer[], uchar len, uchar c) {
	for (int i = 0; i < len-1; i++) {
		buffer[i] = buffer[i+1];
	}
	
	buffer[len-1] = c;
}

uchar buffer_contains(uchar buffer[], uchar sub[]) {
	return memmem(buffer, BUFFER_LENGTH, sub, strlen(sub));
}

void extract_arguments(uchar buffer[], Command *cmd) {
	char current_argument = -1;
	char arg_temp_buffer[4];
	
	for (int i = 0; i < BUFFER_LENGTH; i++) {
		if (buffer[i] == 32 || buffer[i] == 13) {		
			if (current_argument == 0)
				cmd->pass = atoi(arg_temp_buffer);
			if (current_argument == 1)
				cmd->arg0 = atoi(arg_temp_buffer);
			if (current_argument == 2)
				cmd->arg1 = atoi(arg_temp_buffer);
			if (current_argument == 3)
				cmd->arg2 = atoi(arg_temp_buffer);
			
			// 48 is ascii for 0
			memset(arg_temp_buffer, 48, sizeof(arg_temp_buffer));
			
			current_argument++;
		} else if (current_argument >= 0) {
			buffer_append(arg_temp_buffer, 4, buffer[i]);
		}
	}
}

uchar command_buffer[BUFFER_LENGTH];

void command_init() {
	memset(command_buffer, 0, sizeof(command_buffer));
}

Command command_handler() {
	uchar c;
	Command cmd = {0xF,0,0xF,0xF,0xF,0xF};
	
	// Om en symbol har matats in i USART
	if (c = _tstchar()) {
		buffer_append(command_buffer, BUFFER_LENGTH, c);
		_outchar(c);
		
		// Om enter är nedtryckt
		if (command_buffer[BUFFER_LENGTH-1] == 13) { //13 == '\n' ???
			extract_arguments(command_buffer, &cmd);
			
			// Om command_buffer innehåller strängen test1
			if (buffer_contains(command_buffer, "active")) {
				cmd.command = ACTIVE;
			}else if (buffer_contains(command_buffer, "tol")) {
				cmd.command = TOL;
			}else if(buffer_contains(command_buffer, "ndoors")) {
				cmd.command = NDOORS;
			}else if (buffer_contains(command_buffer, "test1")) {
				cmd.command = TEST1;
			}else if (buffer_contains(command_buffer, "passcode")) {
				cmd.command = PASSCODE;
			}else if (buffer_contains(command_buffer, "help")) {
				cmd.command = HELP;
			}else if (buffer_contains(command_buffer, "show")) {
				cmd.command = SHOW;
			}else {
				cmd.command = UNKNOWN;
			}
			
			cmd.active = 1;
			
			// Återställ command_buffer
			memset(command_buffer, 0, sizeof(command_buffer));
			_outchar('\n');
		}
	}
	
	return cmd;
}