/*
 * CommandList_Drv.c
 *
 *  Created on: Apr 30, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


//****** DO NOT MODIFY BELOW **************


int puttyIsColor = false;
int colorIs = 37; // default to white

/*
 * Description: User shall call this function in a timed fashion. Every 10ms should suffice.
 * 				Using timed trigger will reduce the uC from being overloaded during printing of all available commands
 * 				and avoid blocking.
 */
void Command_List_Poll(void)
{
	//int array_size = sizeof(Command_List); // defined in CommandList_Handler as extern
	int element_size = sizeof(Command_List[0]);
	int num_elements = CommandList_array_size/element_size;
	char arguments[(UART_DMA_QUEUE_DATA_SIZE/2) - 1] = {0};// so fits in str variable
	static int cmdPtr = 0;
	char strColor[16] = {0};
	char str2[UART_DMA_QUEUE_DATA_SIZE / 2] = {0};
	CommandData_t cData = {0};

	//if(cmd_list_poll_mode)
	{
		sprintf(strColor, "\033[0;%dm", colorIs);

		if(Command_List[cmdPtr].args != NULL)
		{
			sprintf(arguments, "%s", Command_List[cmdPtr].args);
		}

		if(puttyIsColor)
		{
			snprintf((char*)cData.data, COMMAND_NAME_MAX_SIZE, "%s> %s %s\r\n", strColor, Command_List[cmdPtr].command, END_COLOR);
		}
		else
		{
			snprintf((char*)cData.data, COMMAND_NAME_MAX_SIZE, "> %s\r\n", Command_List[cmdPtr].command);
		}

		if(arguments[0] != '\0')
		{
			snprintf(str2, UART_DMA_QUEUE_DATA_SIZE / 2, "\t%s", arguments[0] == '\0' ? "":arguments);
			strcat((char*)cData.data, str2);
		}

		Command_ListNotify(&cData); // see notes at bottom of this file

		if(++cmdPtr == num_elements)
		{
			//cmd_list_poll_mode = 0; // done
			TimerCallbackDisable(&timerCallback, Command_List_Poll);
			cmdPtr = 0; // reset
			sprintf((char*)cData.data, "***** END *****");
			Command_ListNotify(&cData);
		}
	}
}


/*
 * Description: This will start to return all the functions created with the command name and arguments.
 * Input: msg arg can be NULL or 1. retStr not used, leave alone so it'll return OK.
 * Output: None
 *
 */
int Command_List_Print(char* msg, char *retStr)
{
	char *token;
	char *rest = msg;
	char delim[] = ": ,|\r";

	token = strtok_r(rest, delim, &rest);

	if(token != NULL)
	{
		switch(token[0])
		{
		case 'k': // black
			colorIs = 30;
			break;
		case 'r': // red
			colorIs = 31;
			break;
		case 'g': // green
			colorIs = 32;
			break;
		case 'y': // yellow
			colorIs = 33;
			break;
		case 'b': // blue
			colorIs = 34;
			break;
		case 'm': // magenta
			colorIs = 35;
			break;
		case 'c': // cyan
			colorIs = 36;
			break;
		case 'w': // white
			colorIs = 37;
			break;
		default :
			colorIs = 37; // white
			break;
		}
		if(colorIs != 37)
		{
			puttyIsColor = 1;
		}
		else
		{
			puttyIsColor = 0;
		}
	}

	//cmd_list_poll_mode = 1; // start

	TimerCallbackTimerStart(&timerCallback, Command_List_Poll, 10, TIMER_REPEAT);

	return NO_ACK;
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

	return status;
}
*/

