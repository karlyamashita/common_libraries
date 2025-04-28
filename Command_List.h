/*
 * Command_List.h
 *
 *  Created on: Apr 10, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_COMMAND_LIST_H_
#define INC_COMMAND_LIST_H_



typedef int (*FunctionType)(char *, char *);
typedef int (*FuncCallback)(uint8_t *data, uint32_t *size);


typedef struct
{
	char *command;
	char *args;
	char *notes;
	FunctionType func;
	FuncCallback func_cb;
}Command_t;



int Print_Functions(char* msg, char *retStr);


#endif /* INC_CMD_LIST_H_ */
