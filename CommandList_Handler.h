/*
 * CommandList_Handler.h
 *
 *  Created on: May 1, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_COMMANDLIST_HANDLER_H_
#define INC_COMMANDLIST_HANDLER_H_


#define CMD_HELP DECLARE_CMD("help", Command_List_Print, "help\r\n" \
						"\thelp <color> k,r,g,y,b,m,c,w")


void CommandList_Init(void);
extern int CommandList_array_size;


#endif /* INC_COMMANDLIST_HANDLER_H_ */
