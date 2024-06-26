#ifndef NCA_FROM_SDAR_H
#define NCA_FROM_SDAR_H


void SetNCA_From_SDAR_PresentFlag(bool status);
bool GetNCA_From_SDAR_PresentFlag(void);
void SendNCA_FROM_SDAR(uint32_t delay);

void Emulate_NCA_From_SDAR(void);
#endif // NCA_FROM_SDAR_H
