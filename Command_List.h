/*
 * Command_List.h
 *
 *  Created on: Apr 10, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_COMMAND_LIST_H_
#define INC_COMMAND_LIST_H_



typedef int (*FunctionGet)(char *, char *);
typedef int (*FunctionSet)(char *);
typedef int (*FuncCallback)(uint8_t *data, uint32_t *size);  // instead of pointer to data/size, create a data structure?


typedef struct
{
	char *command;
	char *args;
	char *notes;
	FunctionGet func_g;
	FunctionSet func_s;
	FuncCallback func_cb;
}Command_t;

void Command_List_Poll(void);
int Command_List_Print(char* msg, char *retStr);


#endif /* INC_CMD_LIST_H_ */
