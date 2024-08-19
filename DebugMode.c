
#include "main.h"
#include "DebugMode.h"


DebugModeStructure debugTelemetry;


int DEBUG_Mode(char *msg)
{
    int status = NO_ERROR;
    char *token;
    char *token2;
   // char *token3;
    char *rest = msg;
    
    token = strtok(rest, ","); // debug pointer
    token2 = strtok(NULL, "\r"); // enable
   // token3 = strtok_r(rest, "\r", &rest); // value
    
    
    switch(atoi(token))
    {
        case 0:
            debugTelemetry.enable = atoi(token2);
            break;
        case 1:

            break;
        case 2:

            break;
        case 3:
                
            break;
        case 4:
            
            break;
        case 5:
            
            break;
        case 6:
            
            break;
        case 7:
            
            break;
        default:
            break;
    }
    
    
    return status;
}
