#include "LSP.h"
#ifdef USE_DIAGNOSTIC_TOOL

#include "main.h"
#include "DiagnosticTool.h"
#include "can.h"

unsigned char diagnosticStatus[8];
unsigned char diagnosticToolFlag = 0;
unsigned char stopCommunicationsFlag = 0;

void ServiceDiagnosticTool(unsigned char *data) {
	int i;
	for(i = 0; i < 8; i++){
		diagnosticStatus[i] = data[i];
	}		
	diagnosticToolFlag = 1;
}

void CheckDiagnosticTool(callbackDiagnosticTool diagnosticToolCallback) {
	if(!diagnosticToolFlag) return;
	diagnosticToolFlag = 0;
	diagnosticToolCallback(diagnosticStatus);
}

void DiagnosticToolCallback(unsigned char *status) {
	static unsigned char oldStatus[8];
	if(memcmp(oldStatus, status,8) == 0) {
		return;
	}

		// stop communicating until bus is quiet for 20 seconds.
	if(status[1] == 0x34) {
		stopCommunicationsFlag = 1;
	} else {
		stopCommunicationsFlag = 0;
	}
}

unsigned char GetDiagnosticToolStatus(void) {
	return stopCommunicationsFlag;
}

#endif // USE_DIAGNOSTIC_TOOL


