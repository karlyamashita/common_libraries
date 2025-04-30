/*
 * CommandList_Drv.h
 *
 *  Created on: Apr 30, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_COMMANDLIST_DRV_H_
#define INC_COMMANDLIST_DRV_H_


#define CommandDataSize 512

typedef struct
{
	uint8_t data[CommandDataSize];
	uint32_t size;
}CommandData_t;

typedef int (*FunctionGet)(char *, char *);
typedef int (*FunctionSet)(char *);
typedef int (*FuncCallback)(CommandData_t *);  // instead of pointer to data/size, create a data structure?

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


#endif /* INC_COMMANDLIST_DRV_H_ */
