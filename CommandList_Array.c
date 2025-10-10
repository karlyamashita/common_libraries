/*
 * CommandList_Handler.c
 *
 *  Created on: May 1, 2025
 *      Author: karl.yamashita
 */

#include "main.h"



/*
 * User shall add new instances to array. This needs to be extern in header file, typically main.h
 */
const Command_t Command_List[] =
{
	CMD_HELP,
	PM_OPERATION,
	PM_STORE_USER_ALL,
	PM_VOUT_COMMAND,
	PM_VOUT_OV_FAULT_LIMIT,
	PM_VOUT_UV_FAULT_LIMIT,
	PM_IOUT_OC_FAULT_LIMIT
};



// need to place here for CommandList_Drv.c while extern placed in header file. "extern int CommandList_array_size;"
int CommandList_array_size = sizeof(Command_List);
int element_size = sizeof(Command_List[0]);
