/*
 * CommandList_Drv.c
 *
 *  Created on: Apr 30, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


/*
 * Description: This will start to return all the functions created with the command name and arguments.
 * Input: msg and retStr not used but needed as a function call.
 * Output: None
 *
 */
int Command_List_Print(char *msg, char *retStr)
{
	TimerCallbackTimerStart(&timerCallback, Command_List_Poll, 10, TIMER_REPEAT);

	return NO_ACK;
}

/*
 * Description: User shall call this function in a timed fashion. Every 10ms should suffice.
 * 				Using timed trigger will reduce the uC from being overloaded during printing of all available commands
 * 				and avoid blocking.
 */
void Command_List_Poll(void)
{
	static int cmdPtr = 0;
	//char strColor[16] = {0};
	char str2[128] = {0};
	CommandData_t cData = {0};

	// copy command
	snprintf((char*)cData.data, COMMAND_NAME_MAX_SIZE, "> %s\r\n", command_list_array[cmdPtr].cmd);

	// copy description
	if(command_list_array[cmdPtr].description[0] != '\0')
	{
		sprintf((char*)str2, "<DESCRIPTION> %s\r\n", command_list_array[cmdPtr].description);
	}
	strcat((char*)cData.data, str2);

	// copy min,max, flags
	sprintf(str2, "<ARG> min:%d, max:%d, flags:%d\r\n", command_list_array[cmdPtr].min_args, command_list_array[cmdPtr].max_args, command_list_array[cmdPtr].flags);
	strcat((char*)cData.data, str2);

	// copy syntax
	if(command_list_array[cmdPtr].syntax != NULL)
	{
		snprintf(str2, UART_DMA_QUEUE_DATA_SIZE, "%s\r\n", (char*)command_list_array[cmdPtr].syntax);
		strcat((char*)cData.data, str2);
	}

	cData.size = strlen((char*)cData.data);
	if(cData.size >= UART_DMA_QUEUE_DATA_SIZE)
	{
		cData.size = UART_DMA_QUEUE_DATA_SIZE - 1;
	}
	Command_ListNotify(&cData); // see notes at bottom of this file

	if(++cmdPtr == (commandList_array_size/commandList_element_size))
	{
		memset(&cData, 0, sizeof(cData));
		TimerCallbackDisable(&timerCallback, Command_List_Poll);
		cmdPtr = 0; // reset
		sprintf((char*)cData.data, "***** END *****");
		cData.size = strlen((char*)cData.data);
		Command_ListNotify(&cData);
	}
}

/*
 * Description: Parse through Command List Array for matching command.
 *
 */
int Command_List_Parse(char *msg, char *retStr)
{
	int status = NO_ERROR;
	char *token;
	char *rest = msg;
	char delim[] = " ?:,\r";
	char strcpy[64] = {0};
	int i = 0;
	int totalElements = commandList_array_size / commandList_element_size;
	char *result;

	memcpy(&strcpy, msg, strlen(msg));

	// get command
	token = strtok_r(rest, delim, &rest);

	for(i = 0; i < totalElements; i++)
	{
		result = strstr(command_list_array[i].cmd, token);
		if(result)
		{
			rest = strcpy;
			rest += strlen(token);
			status = command_list_array[i].func(rest, retStr);
			return status;
		}
	}
	status = COMMAND_UNKNOWN;

	return status;
}

/*
 * User will need to create a function (Command_ListNotify) to receive CommandData_t data structure.
 * This is done so it's a generic function call outside of this file. But inside the function,
 * the actual uC UART transmit routine is used. So that way Command_List_Poll above does not need to be modified.
 */
/*
int Command_ListNotify(CommandData_t *msg)
{
	int status = NO_ERROR;

	// User will need to send msg using the uC UART transmit driver.
	UART_DMA_NotifyUser(&uart2_msg, (char*)msg->data, msg->size, true);

	return status;
}


// Be sure to register this call in PollingInit()
TimerCallbackRegisterOnly(&timerCallback, Command_List_Poll);

*/

