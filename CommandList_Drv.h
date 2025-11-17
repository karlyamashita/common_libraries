/*
 * CommandList_Array.h
 *
 *  Created on: Apr 30, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_COMMANDLIST_DRV_H_
#define INC_COMMANDLIST_DRV_H_


#define COMMAND_DATA_SIZE 512
#define COMMAND_NAME_MAX_SIZE 48



#define DECLARE_CMD(name, min, max, flags, func, desc, syntax) \
        { name, func, min, max, flags, desc, syntax }


// String buffer and size. This is used to build the message to be sent to the user
typedef struct
{
	uint8_t data[COMMAND_DATA_SIZE];
	uint32_t size;
}CommandData_t;

typedef int (*Function)(char *msg, char *retStr); // the function called should be able to parse get/set commands.

typedef struct// __Command_t
{
	const char *  cmd;              /* command name user types, ie. GO  */
	Function func;       /* actual function to call          */
	int     min_args;               /* min num of args command accepts  */
	int     max_args;               /* max num of args command accepts  */
	int     flags;                  /* command flags (e.g. repeat)      */
	const char *  description;      /* brief description of command     */
	const void *  syntax;           /* syntax of command                */
}Command_t;


#define CMD_HELP DECLARE_CMD("help", 0, 0, 0, Command_List_Print, "show available commands", "help?")


void Command_List_Poll(void);
int Command_List_Print(char *msg, char *retStr);
int Command_List_Parse(char *msg, char *retStr);

#endif /* INC_COMMANDLIST_DRV_H_ */
