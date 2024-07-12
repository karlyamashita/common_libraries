
#ifndef DIAGNOSTIC_TOOL_H
#define DIAGNOSTIC_TOOL_H

typedef void (*callbackDiagnosticTool)(unsigned char[8]);

void ServiceDiagnosticTool(unsigned char *data);
void CheckDiagnosticTool(callbackDiagnosticTool diagnosticToolCallback);
void DiagnosticToolCallback(unsigned char *status);
unsigned char GetDiagnosticToolStatus(void);

#endif //DIAGNOSTIC_TOOL_H
