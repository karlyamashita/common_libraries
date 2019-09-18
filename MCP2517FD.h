#include "LSP.h"
#ifndef MCP2517FD_H
#define MCP2517FD_H


/*
C = Command(4bit), A = Address(12bit), D = Data(1 to n bytes), N = Number of Bytes(1 byte), CRC(2 bytes)
*/
#define MCP_RESET 0b0000
#define MCP_READ 0b0011
#define MCP_WRITE 0b0010
#define MCP_READ_CRC 0b1011
#define MCP_WRITE_CRC 0b1010
#define MCP_WRITE_SAFE 0b1100



#define MCP_OSC_ADDR 0xE00 // 32 bytes
#define MCP_IOCON_ADDR 0xE04 
#define MCP_CRC_ADDR 0xE08
#define MCP_ECCCON_ADDR 0xE0C
#define MCP_ECCSTAT_ADDR 0xE10

#define MCP_C1CON_ADDR 0x00
#define MCP_C1NBTCFG_ADDR 0x04
#define MCP_C1DBTCFG_ADDR 0x08
#define MCP_C1TDC_ADDR 0x0C
#define MCP_C1TBC_ADDR 0x10
#define MCP_C1TSCON_ADDR 0x14
#define MCP_C1VEC_ADDR 0x18
#define MCP_C1INT_ADDR 0x1C
#define MCP_C1RXIF_ADDR 0x20
#define MCP_C1RXOVIF_ADDR 0x28
#define MCP_C1TXATIF_ADDR 0x2C



#define MCP_C1RXIF_ADDR 0x20




typedef union CiCON_Register {
	struct {
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned TXBWS:4;
		unsigned ABAT:1;
		unsigned REQOP:3;
		unsigned OPMOD:3;
		unsigned TXQEN:1;
		unsigned STEF:1;
		unsigned SERR2LOM:1;
		unsigned ESIGM:1;
		unsigned RTXAT:1;
		unsigned :3;
		unsigned BRSDIS:1;
		unsigned BUSY:1;
		unsigned WFT:2;
		unsigned WAKFIL:1;
		unsigned :1;
		unsigned PXEDIS:1;
		unsigned ISOCRCEN:1;
		unsigned DNCNT:5;
	}Status;
}CiCON_Register;


typedef union CiNBTCFG_Register {
	struct {
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		
		unsigned BRP:8;
		unsigned TSEG1:8;
		unsigned :1;
		unsigned TSEG2:7;
		unsigned :1;
		unsigned SJW:7;
	}Status;
}CiNBTCFG_Register;

typedef union CiDBTCFG_Register {
	struct {
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	} Bytes;
	struct {
		unsigned BRP:8;
		unsigned :3;
		unsigned TSEG1:5;
		unsigned :4;
		unsigned TSEG2:4;
		unsigned :4;
		unsigned SJW:4;
	}Status;
}CiDBTCFG_Register;

typedef union CiTDC_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		unsigned :7;
		unsigned EDGFLTEN:1;
		unsigned SID11EN:1;
		unsigned :6;	
		unsigned TDCMOD:2;
		unsigned :1;
		unsigned TDCO:7;
		unsigned :2;
		unsigned TDCV:6;
	}Status;
}CiTDC_Register;

typedef union CiTBC_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned TBC:32; 
	}Status;
}CiTBC_Register;

typedef union CiTSCON_Register {
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		unsigned :13;
		unsigned TSRES:1;
		unsigned TSEOF:1;
		unsigned TBCEN:1;
		unsigned :6;
		unsigned TBCPRE:10;
	}
}CiTSCON_Register;

typedef union CiVEC_Register {
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned :1;
		unsigned RXCODE:7;
		unsigned :1;
		unsigned TXCODE:7;
		unsigned :3;
		unsigned FILHIT:5;
		unsigned :1;
		unsigned ICODE:7;
	}Status;
}CiVEC_Register;

typedef union CiINT_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		unsigned IVMIE:1;
		unsigned WAKIE:1;
		unsigned CERRIE:1;
		unsigned SERRIE:1;
		unsigned RXOVIE:1;
		unsigned TXAFIE:1;
		unsigned SPICRCIE:1;
		unsigned ECCIE:1;
		unsigned :3;
		unsigned TEFIE:1;
		unsigned MODIE:1;
		unsigned TVCIE:1;
		unsigned RXIE:1;
		unsigned TXIE:1;
		unsigned IVMIF:1;
		unsigned WAKIF:1;
		unsigned CERRIF:1;
		unsigned SERRIF:1;
		unsigned RXOVIF:1;
		unsigned TXATIF:1;
		unsigned SPICRCIF:1;
		unsigned ECCIF:1;
		unsigned :3;
		unsigned TEFIF:1;
		unsigned MODIF:1;
		unsigned TBCIF:1;
		unsigned RXIF:1;
		unsigned TXIF :1;
	}Status;
}CiINT_Register;

typedef union CiRXIF_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned RFIF:31;
		unsigned :1; 
	}Status;
}CiRXIF_Register;

typedef union CiRXOVIF_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned RFOVIF:31;
		unsigned :1; 
	}Status;
}CiRXOVIF_Register;

typedef union CiTXIF_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned TFIF:32;
	}Status;
}CiTXIF_Register;

typedef union CiTXATIF_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned TFATIF:32;
	}Status;
}CiTXATIF_Register;

typedef union CiTXREQ_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned TXREQ:32;
	}Status;
}CiTXREQ_Register;

typedef union CiTREC_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned :10;
		unsigned TXBP:1;
		unsigned RXBP:1;
		unsigned TXWARN:1;
		unsigned RXWARN:1;
		unsigned EWARN:1;
		unsigned TEC:8;
		unsigned REC:8;
	}Status;
}CiTREC_Register;

typedef union CiBDIAG0_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned DTERRCNT:8;
		unsigned DRERRCNT:8;
		unsigned NTERRCNT:8;
		unsigned NRERRCNT:8;
	}Status;
}CiBDIAG0_Register;

typedef union CiBDIAG1_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned DLCMM:1;
		unsigned ESI:1;
		unsigned DCRCERR:1;
		unsigned DSTUFERR:1;
		unsigned DFORMERR:1;
		unsigned :1;
		unsigned DBIT1ERR:1;
		unsigned DBIT0ERR:1;
		unsigned TXVOERR:1;
		unsigned :1;
		unsigned NCRCERR:1;
		unsigned NSTUFERR:1;
		unsigned NFORMERR:1;
		unsigned NACKERR:1;
		unsigned NBIT1ERR:1;
		unsigned NBIT0ERR:1;
		unsigned EFMSGCNT:8;
		unsigned EFMSGCNT:8;
	}Status;
}CiBDIAG1_Register;

typedef union CiTEFCON_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned :3;
		unsigned FSIZE:5;
		unsigned :13;
		unsigned FRESET:1;
		unsigned :1;
		unsigned UINC:1;
		unsigned :2;
		unsigned TEFTSEN:1;
		unsigned :1;
		unsigned TEFOVIE:1;
		unsigned TEFFIE:1;
		unsigned TEFHIE:1;
		unsigned TEFNEIE:1;
	}Status;
}CiTEFCON_Register;

typedef union CiTEFSTA_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned :28;
		unsigned TEFOVIF:1;
		unsigned TEFFIF:1;
		unsigned TEFHIF:1;
		unsigned TEFNEIF:1;
	}Status;
}CiTEFSTA_Register;

typedef union CiTEFUA_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned TEFUA:32;
	}Status;
}CiTEFUA_Register;

typedef union CiTXQCON_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned PLSIZE:3;
		unsigned FSIZE:5;
		unsigned :1;
		unsigned TXAT:2;
		unsigned TXPRI:5;
		unsigned :5;
		unsigned FRESET:1;
		unsigned TXREQ:1;
		unsigned UINC:1;
		unsigned TXEN:1;
		unsigned :2;
		unsigned TXQEIE:1;
		unsigned :1;
		unsigned TXQNIE:1;
	}Status;
}CiTXQCON_Register;

typedef union CiTXQSTA_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned :19;
		unsigned TXQCI:5;
		unsigned TXABT:1;
		unsigned TXLARB:1;
		unsigned TXERR:1;
		unsigned TXZTIF:1;
		unsigned :1;
		unsigned TXQEIF:1;
		unsigned :1;
		unsigned TXQNIF;
	}Status;
}CiTXQSTA_Register;

typedef union CiTXQUA_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned TXQUA:32;
	}Status;
}CiTXQUA_Register;

typedef union CiFIFOCONm_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned PLSIZE:3;
		unsigned FSIZE:5;
		unsigned :1;
		unsigned TXAT:2;
		unsigned TXPRI:5;
		unsigned :5;
		unsigned FRESET:1;
		unsigned TXREQ:1;
		unsigned UINC:1;
		unsigned TXEN:1;
		unsigned RTREN:1;
		unsigned RXTSEN:1;
		unsigned TXATIE:1;
		unsigned RXOVIE:1;
		unsigned TFERFFIE:1;
		unsigned TFHRFHIE:1;
		unsigned TFNRFNIE:1;
	}Status;
}CiFIFOCONm_Register;

typedef union CiFIFOSTAm_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned :19;
		unsigned FIFOCI:5;
		unsigned TXABT:1;
		unsigned TXLARB:1;
		unsigned TXERR:1;
		unsigned TXATIF:1;
		unsigned RXOVIF:1;
		unsigned TFERFFIF:1;
		unsigned TFHRFHIF:1;
		unsigned TFNRFNIF:1;
	}Status;
}CiFIFOSTAm_Register;

typedef union CiFIFOUAm_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned FIFOUA:32;
	}Status;
}CiFIFOUAm_Register;

typedef union CiFLTCONm_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned FLTEN3:1;
		unsigned F3BP:5;
		unsigned FLTEN2:1;	
		unsigned F2BP:5;
		unsigned FLTEN1:1;	
		unsigned F1BP:5;
		unsigned FLTEN0:1;
		unsigned :2;
		unsigned F0BP:5;
	}Status;
}CiFLTCONm_Register;

typedef union CiFLTOBJm_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned :1;
		unsigned EXIDE:1;
		unsigned SID11:1;
		unsigned EID:18;
		unsigned SID:11;
	}Status;
}CiFLTOBJm_Register;

typedef union CiMASKm_Register{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned :1;
		unsigned MEXIDE:1;
		unsigned MSID11:1;
		unsigned MEID:18;
		unsigned MSID:11;
	}Status;
}CiMASKm_Register;

#endif // MCP2517FD_H
