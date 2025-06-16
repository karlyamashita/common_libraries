/*
 * CommandList_Drv.h
 *
 *  Created on: Apr 30, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_COMMANDLIST_DRV_H_
#define INC_COMMANDLIST_DRV_H_


#define COMMAND_DATA_SIZE 512
#define COMMAND_NAME_MAX_SIZE 48



#define DECLARE_CMD(name, func, desc) { name, func, desc }


// String buffer and size. This is used to build the message to be sent to the user
typedef struct
{
	uint8_t data[COMMAND_DATA_SIZE];
	uint32_t size;
}CommandData_t;

typedef int (*Function)(char *msg, char *retStr); // the function called should be able to parse get/set commands.

typedef struct// __Command_t
{
	const char *command;
	Function func;
	const char *args;
}Command_t;


#define START_COLOR_BLACK "\033[0;30m"
#define START_COLOR_RED "\033[0;31m"
#define START_COLOR_GREEN "\033[0;32m"
#define START_COLOR_YELLOW "\033[0;33m"
#define START_COLOR_BLUE "\033[0;34m"
#define START_COLOR_MAGENTA "\033[0;35m"
#define START_COLOR_CYAN "\033[0;36m"
#define START_COLOR_WHITE "\033[0;37m"

#define END_COLOR "\033[m"


void Command_List_Poll(void);
int Command_List_Print(char* msg, char *retStr);


#endif /* INC_COMMANDLIST_DRV_H_ */
