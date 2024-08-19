/* 
 * File:   DebugMode.h
 * Author: karl.yamashita
 *
 * Created on June 1, 2023, 1:05 PM
 */

#ifndef DEBUGMODE_H
#define	DEBUGMODE_H

#ifdef	__cplusplus
extern "C" {
#endif

    
typedef struct
{
    bool enable;
    uint8_t value;
}DebugModeStructure;
    
    
int DEBUG_Mode(char *msg);


#ifdef	__cplusplus
}
#endif

#endif	/* DEBUGMODE_H */

