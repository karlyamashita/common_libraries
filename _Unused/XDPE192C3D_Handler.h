/*
 * XDPE192C3D_Handler.h
 *
 *  Created on: Sep 25, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_XDPE192C3D_HANDLER_H_
#define INC_XDPE192C3D_HANDLER_H_


#define CMD_PM_OPERATION DECLARE_CMD("pm operation", 0, 0, 0, XDPE192C3D_Handler, "Byte value, see document", "<R>pm operation ?")
#define CMD_PM_STORE_USER_ALL DECLARE_CMD("pm store user all", 0, 0, 0, XDPE192C3D_Handler, "Save relevant changes to NVM", "<W>pm store user all")
#define CMD_PM_VOUT_COMMAND DECLARE_CMD("pm vout command", 150, 2000, 0, XDPE192C3D_Handler, "Set/Get VOUT COMMAND (mV)", \
			"<R>pm vout command ?:<page>\r\n" \
			"<W>pm vout command:<page>,<mV>")
#define CMD_PM_FREQUENCY_SWITCH DECLARE_CMD("pm frequency switch", 2, 2046, 0, XDPE192C3D_Handler, "Set/Get FREQUENCY SWITCH (KHz)", \
			"<R>pm frequency switch ?:<page>\r\n" \
			"<W>pm frequency switch:<page>,<KHz>")
#define CMD_PM_VOUT_OV_FAULT_LIMIT DECLARE_CMD("pm vout ov fault limit", 0, 2000, 0, XDPE192C3D_Handler, "Set/Get OV max fault limit (mV)", \
			"<R>pm vout ov fault limit ?:<page>\r\n" \
			"<W>pm vout ov fault limit:<page>, <mV>")
#define CMD_PM_VOUT_UV_FAULT_LIMIT DECLARE_CMD("pm vout uv fault limit", 0, 0, 0, XDPE192C3D_Handler, "Set/Get UV min fault limit (mV)", \
			"<R>pm vout uv fault limit ?\r\n" \
			"<W>pm vout uv fault limit:<mV>")
#define CMD_PM_IOUT_OC_FAULT_LIMIT DECLARE_CMD("pm iout oc fault limit", 0, 70, 0, XDPE192C3D_Handler, "Set/Get IOUT OC max fault limit (mA)", \
			"<R>pm iout oc fault limit ?\r\n" \
			"<W>pm iout oc fault limit:<mA>")
#define CMD_PM_OT_FAULT_LIMIT DECLARE_CMD("pm ot fault limit", 0, 82, 0, XDPE192C3D_Handler, "Set/Get Over Temperature fault limit", \
			"<R>pm ot fault limit ?\r\n" \
			"<W>pm ot fault limit:<Celsius>")
#define CMD_PM_STATUS_WORD DECLARE_CMD("pm status word", 0, 0, 0, XDPE192C3D_Handler, "Get STATUS WORD", "<R>pm status word ?:0")
#define CMD_PM_READ_TERMPEATURE_1 DECLARE_CMD("pm read temperature 1", 0, 0, 0, XDPE192C3D_Handler, "Get READ TEMPERATURE 1", "<R>pm read temperature 1 ?:0")
#define CMD_PM_READ_TERMPEATURE_2 DECLARE_CMD("pm read temperature 2", 0, 0, 0, XDPE192C3D_Handler, "Get READ TEMPERATURE 2", "<R>pm read temperature 2 ?:0")

#define CMD_RAIL_COUNT DECLARE_CMD("rail count", 0, 0, 0, XDPE192C3D_Configuration_Handler, "Set/Get rail count", \
		"<R>rail count ?\r\n" \
		"<W>rail count:<count>")



#define XDP_SLAVE_ADDRESS (0x62 << 1)


typedef struct
{
	uint32_t railCount; // 1 or 2
	bool read_vout_iout_enable;
	int exponent;
}XDPE192C3D_Configuration_t;


// these 2 structs are mainly for debug
typedef union
{
	struct
	{
		uint8_t flags;
		uint32_t :8;
	}Bytes;
	struct
	{
		uint32_t v1:1;
		uint32_t v2:1;
		uint32_t i1:1;
		uint32_t i2:1;
		uint32_t t1:1;
		uint32_t t2:1;
		uint32_t :2;
		uint32_t :8;
		uint32_t :16;
		uint16_t voltage_1;
		uint16_t voltage_2;
		uint16_t current_1;
		uint16_t current_2;
		uint16_t temperature2_1;
		uint16_t temperature2_2;

		float f_voltage_1;
		float f_voltage_2;
		float f_current_1;
		float f_current_2;
		float f_temperature2_1;
		float f_temperature2_2;
	}Flags_Voltages;
}XDPE192C3D_I2C_DataRdy_t;

typedef struct
{
	uint32_t v1;
	uint32_t v2;
	uint32_t i1;
	uint32_t i2;
	uint32_t t1;
	uint32_t t2;
	uint32_t hal_busy_count;
	uint32_t hal_ok_count;
	uint32_t hal_error_count;
	uint32_t error_callback_count;
}XDP_CallbackCount_t;

int XDPE192C3D_Configuration_Init(XDPE192C3D_Configuration_t *config);

int XDPE192C3D_Handler(char *msg, char *retStr);
int XDPE192C3D_RegisterGet(char *msg, char *retStr);
int XDPE192C3D_RegisterSet(char *msg);
int XDPE192C3D_GetCommandPtr(char *msg, uint8_t *ptr);

int XDPE192C3D_IsReady(I2C_GenericDef *i2c, uint32_t Trials);

int XDPE192C3D_SetPage(I2C_GenericDef *i2c, uint8_t page);
int XDPE192C3D_Validate_Page(XDPE192C3D_Configuration_t *config, uint32_t page);

void XDPE192C3D_Poll_READ_VOUT_IOUT(void);

void XDPE192C3D_Voltage_1_Callback(I2C_GenericDef *i2c);
void XDPE192C3D_Voltage_2_Callback(I2C_GenericDef *i2c);
void XDPE192C3D_Current_1_Callback(I2C_GenericDef *i2c);
void XDPE192C3D_Current_2_Callback(I2C_GenericDef *i2c);

void XDPE192C3D_Temperature2_Callback_1(I2C_GenericDef *i2c);
void XDPE192C3D_Temperature2_Callback_2(I2C_GenericDef *i2c);

void XDPE192C3D_CheckVoltagesCurrentTemperature(void);


// configuration
int XDPE192C3D_Configuration_Handler(char *msg, char *retStr);
int XDPE192C3D_Configuration_Get(char *msg, char *retStr);
int XDPE192C3D_Configuration_Set(char *msg);

#endif /* INC_XDPE192C3D_HANDLER_H_ */
