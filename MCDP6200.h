/*
 * RegistersWrite.h
 *
 * Created: 3/1/2021 12:37:50 PM
 *  Author: karl.yamashita
 */ 


#ifndef REGISTERSWRITE_H_
#define REGISTERSWRITE_H_

#include "Retimer.h"

// comment out for retimers 4-5-6 group

// defines
#define MCDP6200_SLAVE_ID_14 0x14 // 0b10100
#define MCDP6200_SLAVE_ID_15 0x15
#define MCDP6200_SLAVE_ID_16 0x16
#define MCDP6200_SLAVE_ID_17 0x17

// defines for Retimer Tx and Rx Register arrays
enum {
	GEN2_HOST,
	GEN2_DEVICE,
	GEN1_HOST,
	GEN1_DEVICE
}GEN_Type;


// RX base address / Eye Opening Monitor base address / RX Equalizer Status base address
enum {
	All_DP_RECEIVER_ADDR = 0x0100, // write only
	ALL_USB_RX_BASE_ADDR = 0x1E00, // write only
	UFP_USB_RX_BASE_ADDR = 0x1100,
	UFP_DPRX_ML0_ADDR = 0x1200,
	UFP_DPRX_ML1_ADDR = 0x1300,
	UFP_DPRX_ML2_ADDR = 0x1400,
	UFP_DPRX_ML3_ADDR = 0x1500,
	DFP_USB_RX1_BASE_ADDR = 0x2000,
	DFP_USB_RX2_BASE_ADDR = 0x2100
}RX_Base_Address;

// TX base address
enum {
	All_DPTX_ADDR = 0x0200, // write only
	ALL_USB_TX_BASE_ADDR = 0x1F00, // write only
	DFP_DPTX_ML3_ADDR = 0x2200,
	DFP_DPTX_ML0_ADDR = 0x2300,
	DFP_DPTX_ML2_ADDR = 0x2400,
	DFP_DPTX_ML1_ADDR = 0x2500,
	UFP_USB_TX_BASE_ADDR = 0x1000,
	DFP_USB_TX1_BASE_ADDR = 0x2400,
	DFP_USB_TX2_BASE_ADDR = 0x2500
}TX_Base_Address;


// IC Status / Configurations Registers
enum {
	IC_RT_CONFIG_ADDR = 0x30C,
	DP_RT_CONFIG_ADDR = 0x350,
	OPMODE_CONF_ADDR = 0x504,
	IC_REV_ADDR = 0x510,
	RO_TUNE_MANUAL_ADDR = 0x281C,
	RO_CTUNE_STATUS_ADDR = 0x283C,
	USB_RTSSM_STATUS0_ADDR = 0x320,
	USB_RTSSM_STATUS1_ADDR = 0x324,
	USB_RTSSM_STATUS2_ADDR = 0x328,
	USB_RTSSM_STATUS3_ADDR = 0x32C,
	USB_RTSSM_STATUS4_ADDR = 0x330,
	USB_RTSSM_STATUS5_ADDR = 0x334,
	USB_RTSSM_STATUS6_ADDR = 0x338
}IC_Status_Config_Registers;


// DisplayPort Receiver Logic Register
enum{
	DPRX_ERR_CNT_CTRL_ADDR = 0x61C,
	DP_LT_STATUS_ADDR = 0x62C,
	DPRX_CONTROL_ADDR = 0x660,
	DP_FIFO_CTRL_ADDR = 0x67C
}DisplayPortReceiverLogicRegister;


// DPCD_SNOOP
enum {
	DPCD_SNOOP_0_ADDR = 0x700,
	DPCD_SNOOP_1_ADDR = 0x704,
	DPCD_SNOOP_2_ADDR = 0x708,
	DPCD_SNOOP_3_ADDR = 0x720,
	DPCD_SNOOP_4_ADDR = 0x724,
	DPCD_SNOOP_5_ADDR = 0x728,
	DPCD_SNOOP_6_ADDR = 0x72C,
	DPCD_SNOOP_7_ADDR = 0x750,
	DPCD_SNOOP_8_ADDR = 0x754
}DPCD_SNOOP;


// LT_CONFIG
enum{
	LT_CONFIG_0_ADDR = 0x900,
	LT_CONFIG_1_ADDR = 0x904,
	LT_CONFIG_2_ADDR = 0x908,
	LT_CONFIG_3_ADDR = 0x90C,
	LT_CONFIG_4_ADDR = 0x910,
	LT_CONFIG_5_ADDR = 0x914,
	LT_CONFIG_6_ADDR = 0x918,
	LT_CONFIG_7_ADDR = 0x91C,
	LT_CONFIG_8_ADDR = 0x920,
	LT_CONFIG_9_ADDR = 0x924,
	LT_CONFIG_A_ADDR = 0x928,
	LT_CONFIG_B_ADDR = 0x92C
}LT_CONFIG;


// DPCD_LTTPR_CAP_ID
enum{
	DPCD_LTTPR_CAP_ID_0_ADDR = 0xA00,
	DPCD_LTTPR_CAP_ID_1_ADDR = 0xA04,
	DPCD_LTTPR_CONF_STATUS_13_10_ADDR = 0xB00,
	DPCD_LTTPR_CONF_STATUS_17_14_ADDR = 0xB04,
	DPCD_LTTPR_CONF_STATUS_1B_18_ADDR = 0xB08,
	DPCD_LTTPR_CONF_STATUS_23_20_ADDR = 0xB10,
	DPCD_LTTPR_CONF_STATUS_33_30_ADDR = 0xB20,
	DPCD_LTTPR_CONF_STATUS_37_34_ADDR = 0xB24,
	DPCD_LTTPR_CONF_STATUS_3B_38_ADDR = 0xB28,
	DPCD_LTTPR_CONF_STATUS_3F_3C_ADDR = 0xB2C
}DPCD_LTTPR_CAP_ID;


// RX Configuration registers
enum{
	RX_CTLE_TERM_SB_CTRL_ADDR = 0x18,
	RX_PGA_CTRL_ADDR = 0x1C,
	RX_DFE_TAP_CTRL_ADDR = 0x24,
	RX_DFE_CTR_ADDR = 0x28,
	RX_RBR_RX_Manual_EN_ADDR = 0x70,
	RX_HBR_RX_Manual_EN_ADDR = 0x74,
	RX_HBR2_RX_Manual_EN_ADDR = 0x78,
	RX_HBR3_RX_Manual_EN_ADDR = 0x7C,
	RX_Gen1_RX_Manual_EN_ADDR = 0x80,
	RX_Gen2_RX_Manual_EN_ADDR = 0x84,
	RX_DFE_ADAPT_CTRL_ADDR = 0x88,
	RX_CTLE_ADAPT_MODE_ADDR = 0x8C,
	RX_THR_ADAPT_MODE_ADDR = 0x90,
	RX_EQ_ADAPT_WAIT0_ADDR = 0xA0,
	RX_EQ_ADAPT_WAIT1_ADDR = 0xA4,
	RX_PHYD_CTRL0_ADDR = 0xB4,
	RX_PHYD_MISC_CNTRL0_ADDR = 0xC0,
	RX_CDR_RESET_CTRL_ADDR = 0xD8
};
/*
#define	RX_CTLE_TERM_SB_CTRL_ADDR 0x18
#define	RX_PGA_CTRL_ADDR 0x1C
#define	RX_DFE_TAP_CTRL_ADDR 0x24
#define	RX_DFE_CTR_ADDR 0x28
#define RX_RBR_RX_Manual_EN_ADDR 0x70 // 5
#define RX_HBR_RX_Manual_EN_ADDR 0x74
#define RX_HBR2_RX_Manual_EN_ADDR 0x78
#define RX_HBR3_RX_Manual_EN_ADDR 0x7C
#define	RX_Gen1_RX_Manual_EN_ADDR 0x80
#define	RX_Gen2_RX_Manual_EN_ADDR 0x84 // 10
#define	RX_DFE_ADAPT_CTRL_ADDR 0x88
#define	RX_CTLE_ADAPT_MODE_ADDR 0x8C
#define	RX_THR_ADAPT_MODE_ADDR 0x90
#define RX_EQ_ADAPT_WAIT0_ADDR 0xA0
#define RX_EQ_ADAPT_WAIT1_ADDR 0xA4 // 15
#define	RX_PHYD_CTRL0_ADDR 0xB4
#define	RX_PHYD_MISC_CNTRL0_ADDR 0xC0
#define RX_CDR_RESET_CTRL_ADDR 0xD8
*/

// Receiver Equalizer Status monitor registers
enum{
	RX_ADAPT_MONITOR0_ADDR = 0x94,
	RX_CTLE_ADAPT_CONF_ADDR = 0xB4,
	RX_ADAPT_MONITOR1_ADDR = 0xBC,
	RX_ADAPT_MONITOR2_ADDR = 0xE4
}ReceiverEqualizerStatusMonitorRegisters;
/*
#define	RX_ADAPT_MONITOR0_ADDR 0x94
#define	RX_CTLE_ADAPT_CONF_ADDR 0xB4
#define	RX_ADAPT_MONITOR1_ADDR 0xBC
#define	RX_ADAPT_MONITOR2_ADDR 0xE4
*/

// Eye opening Monitor Registers
enum{
	RXPHY_CTRL0_ADDR = 0x50,
	RX_EOM_ACCUM_INTERVAL_ADDR = 0x54,
	RX_EOM_START_VAL_ADDR = 0x58,
	RX_EOM_CTRL_ADDR = 0xB8,
	RX_PHYD_EOM_DAT_ADDR = 0xC8,
	RX_EOM_CONFIG_ADDR = 0xD4
}EyeOpeningMonitorRegisters;
/*
#define	RXPHY_CTRL0_ADDR 0x50
#define RX_EOM_ACCUM_INTERVAL_ADDR 0x54
#define RX_EOM_START_VAL_ADDR 0x58
#define RX_EOM_CTRL_ADDR 0xB8
#define RX_PHYD_EOM_DAT_ADDR 0xC8 // 5
#define RX_EOM_CONFIG_ADDR 0xD4
*/

// TX Configuration registers
enum{
	TXPHY_CTRL0_ADDR = 0x40,
	TX_GC_CTRL1_ADDR = 0x4C,
	TX_CONF_WINDOW_0_ADDR = 0x54,
	TX_CONF_WINDOW_1_ADDR = 0x58,
	TX_CONF_CTRL_ADDR = 0x5C,
	TX_CONF_MONITOR_0_ADDR = 0x60,
	TX_CONF_MONITOR_1_ADDR = 0x64
}TXConfigurationRegisters;
// end defines

typedef struct{
	uint8_t mainEN;
	uint8_t preEN;
	uint8_t postEN;
	uint8_t pre;
	uint8_t post;
	uint8_t dldoTune; //TX_DLDO_TUNE
}RetimerTxRegister;

typedef struct{
	uint8_t GEN2_CTLE_CAP ;
	uint8_t GEN2_CDR_IGAIN ;
	uint8_t GEN2_CDR_PGAIN ;
	uint8_t GEN2_CTLE_RES ;
	uint8_t GEN2_CTLER_OVR ;
	uint8_t RX_SUM_TAP1_SPI ; 
	uint8_t RX_SUM_TAPWEIGHTSEL1 ;
}RetimerRxRegister;


// structures

// ***** IC Status/Configuration
typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned : 16;
		unsigned USB_G2_MODE_SEL : 1;
		unsigned : 3;
		unsigned FAST_U2_EXIT_EN : 1;
		unsigned : 9;
		unsigned XTAL_REF_MODE : 1;
		unsigned REFOUT_EN : 1;
	} Status;
} IC_RT_CONFIG_30C_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned DP_RXEQ_CONT: 4;
		unsigned RTMR_DO_AUX_CONFIG : 1;
		unsigned : 2;
		unsigned DPRX_RST_CONFIG : 1;
		unsigned : 1;
		unsigned RETIMER_CONFIG_RW9 : 1;
		unsigned : 22;
	} Status;
} DP_RT_CONFIG_350_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned : 1;
		unsigned PPOL_OVR_EN : 1;
		unsigned PCONF0_OVR_EN : 1;
		unsigned PCONF1_OVR_EN : 1;
		unsigned PPOL : 1;
		unsigned PCONF0 : 1;
		unsigned PCONF1 : 1;
		unsigned : 1;
		unsigned IC_SOFT_RST : 1;
		unsigned USB_SOFT_RST : 1;
		unsigned DP_SOFT_RST : 1;
		unsigned DP_AUX_SOFT_RST : 1;
		unsigned PPOL_BS_OVR : 1;
		unsigned PCONF0_BS_OVR : 1;
		unsigned PCONF1_BS_OVR : 1;
		unsigned : 1;
		unsigned DP_LT_AMP_PRE_CHK_DIS : 1;
		unsigned DP_LT__SYMB_ERR_CHK_DIS : 1;
		unsigned : 12;
	} Status;
} OPMODE_CONF_504_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned TWISLV0_BS_STATUS : 1;
		unsigned TWISLV1_BS_STATUS : 1;
		unsigned P_CONF0_BS_STATUS : 1;
		unsigned P_CONF1_BS_STATUS : 1;
		unsigned P_POL_BS_STATUS : 1;
		unsigned TM_BS_STATUS : 1;
		unsigned : 2;
		unsigned MINOR_VERSION : 4;
		unsigned MAJOR_VERSION : 4;
		unsigned : 16;
	} Status;
} IC_REV_510_RO;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned :26;
		unsigned RO_CTUNE_SEL:1;
		unsigned RO_RC_TRIM:4;
		unsigned :1;
	} Status;
} RO_TUNE_MANUAL_281C_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RO_CTUNE_DONE : 1;
		unsigned RO_LD_LOCK : 1;
		unsigned RO_CTUNEF_MON : 2;
		unsigned RO_CTUNEC_MON : 2;
		unsigned :26;
	} Status;
} RO_CTUNE_STATUS_283C_RO;

//****** USB Status Monitor
typedef union {
    struct {
        uint32_t u32data;
    } u32RegData;
    struct {
     uint8_t data[4];
    } RegDataArray;
    struct {
        unsigned RO_CTUNE_DONE : 1;
        unsigned RO_LD_LOCK : 1;
        unsigned RO_CTUNEF_MON : 2;
        unsigned RO_CTUNEC_MON : 2;
        unsigned :26;
    } Status;
} USB_RTSSM_STATUS_31C;


typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RxDetect : 1;
		unsigned PollingSpeedDetect : 1;
		unsigned PollingPortConfig : 1;
		unsigned PollingRxEQ : 1;
		unsigned PollingTSx : 1;
		unsigned PollingIdle : 1;
		unsigned ComplianceMode : 1;
		unsigned U0 : 1;
		unsigned U1 : 1;
		unsigned RecoveryTSx : 1;
		unsigned RecoveryIdle : 1;
		unsigned PassThroughLoopback : 1;
		unsigned LocalLoopbackActive : 1;
		unsigned LocalLoopbackExit : 1;
		unsigned HotReset : 1;
		unsigned BLRComplianceMode : 1;
		unsigned RxDetect_duplicate : 1; // this is a duplicate of bit0
		unsigned DCI : 1;
		unsigned U1_Duplicate : 1; // this has duplicate bit name at bit 8
		unsigned U2 : 1;
		unsigned U3 : 1;
		unsigned Idle : 1;
		unsigned IdleLP : 1;
		unsigned USB_P0_RATE : 1;
		unsigned : 8;
	} Status;
} USB_RTSSM_STATUS_320_324_RO; // 0x320 and 0x324 are the same other than one is Status0 and the other is Status1

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned USB_P0_RXDET_HIS : 4;
		unsigned USB_P0_POLSPD_HIS : 4;
		unsigned USB_P0_POLPORT_HIS : 4;
		unsigned USB_P0_POLRXEQ_HIS : 4;
		unsigned USB_P0_POLTSX_HIS : 4;
		unsigned USB_P0_POLIDLE_HIS : 4;
		unsigned USB_P0_COMPL_HIS : 4;
		unsigned USB_P0_U0_HIS : 4;
	} Status;
} USB_RTSSM_STATUS2_328_CRO;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned USB_P0_U1_HIS : 4;
		unsigned USB_P0_U2_HIS : 4;
		unsigned USB_P0_U3_HIS : 4;
		unsigned USB_P0_RECTSX_HIS : 4;
		unsigned USB_P0_RECIDLE_HIS : 4;
		unsigned USB_P0_PTLB_HIS : 4;
		unsigned USB_P0_LLACT_HIS : 4;
		unsigned USB_P0_LLEXIT_HIS : 4;
	} Status;
} USB_RTSSM_STATUS3_32C_CRO;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned USB_P1_RXDET_HIS : 4;
		unsigned USB_P1_POLSPD_HIS : 4;
		unsigned USB_P1_POLPORT_HIS : 4;
		unsigned USB_P1_POLRXEQ_HIS : 4;
		unsigned USB_P1_POLTSX_HIS : 4;
		unsigned USB_P1_POLIDLE_HIS : 4;
		unsigned USB_P1_COMPL_HIS : 4;
		unsigned USB_P1_U0_HIS : 4;
	} Status;
} USB_RTSSM_STATUS4_330_CRO;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned USB_P1_U1_HIS : 4;
		unsigned USB_P1_U2_HIS : 4;
		unsigned USB_P1_U3_HIS : 4;
		unsigned USB_P1_RECTSX_HIS : 4;
		unsigned USB_P1_RECIDLE_HIS : 4;
		unsigned USB_P1_PTLB_HIS : 4;
		unsigned USB_P1_LLACT_HIS : 4;
		unsigned USB_P1_LLEXIT_HIS : 4;
	} Status;
} USB_RTSSM_STATUS5_334_CRO;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned USB_P0_HRST_HIS : 4;
		unsigned USB_P0_BLRC_HIS : 4;
		unsigned : 8;
		unsigned USB_P1_HRST_HIS : 4;
		unsigned USB_P1_BLRC_HIS : 4;
		unsigned : 8;
	} Status;
} USB_RTSSM_STATUS6_338_CRO;

typedef union {
     struct {
         uint8_t data[4];
     } RegDataArray;
    struct {
        unsigned : 17;
        unsigned TheBit: 1;
        unsigned : 14;
    } Status;
} USB_RTSSM_STATUS_34C;

// ****** DisplayPort Receiver Logic Register
typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned DPRX_ERR_CNT_EN :1;
		unsigned :31;
	} Status;
} FPRX_ERR_CNT_CTRL_61C_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned DPRX_FINAL_LINK_BW :6;
		unsigned :2;
		unsigned DPRX_FINAL_LANE_COUNT:3;
		unsigned :2;
		unsigned DPRX_FINAL_ENH_EN:1;
		unsigned DPRX_CURR_LT_PATRN:3;
		unsigned :25;
	} Status;
} DP_LT_STATUS_62C_RO;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned DPRX_EN :1;
		unsigned :3;
		unsigned ALIGN_CTLR:1;
		unsigned :7;
		unsigned DPRX_FEFIFO_RESET_AUTO_EN:1;
		unsigned :1;
		unsigned LTTPR_FIFO_GATING:1;
		unsigned :17;
	} Status;
} DPRX_CONTROL_660_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ALIGNED_DATA_SEL :1;
		unsigned DPTX_SKEW_CTLR:1;
		unsigned :30;
	} Status;
} DP_FIFO_CTRL_67C_RW;

// ****** DPCD_SNOOP
typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned trainingPatternSelection :4;
		unsigned recoveredClockOutEN:1;
		unsigned scramblingDisable:1;
		unsigned symbolErrorCountSel:2;
		unsigned voltageSwingSetLane0:2;
		unsigned maxSwingReachedLane0:1;
		unsigned preEmphasisSetLane0:2;
		unsigned maxPreEmphasisReachedLane0:1;
		unsigned :2;
		unsigned voltageSwingSetLane1:2;
		unsigned maxSwingReachedLane1:1;
		unsigned preEmphasisSetLane1:2;
		unsigned maxPreEmphasisReachedLane1:1;
		unsigned :2;
		unsigned voltageSwingSetLane2:2;
		unsigned maxSwingReachedLane2:1;
		unsigned preEmphasisSetLane2:2;
		unsigned maxPreEmphasisReachedLane2:1;
		unsigned :2;
	} Status;
} DPCD_SNOOP_0_700_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned voltageSwingSetLane3 :2;
		unsigned maxSwingReachedLane3:1;
		unsigned preEmphasisSetLane3:2;
		unsigned maxPreEmphasisReachedLane3:1;
		unsigned :2;
		unsigned linkQualPatternSetLane0:3;
		unsigned :5;
		unsigned linkQualPatternSetLane1:3;
		unsigned :5;
		unsigned linkQualPatternSetLane2:3;
		unsigned :5;
	} Status;
} DPCD_SNOOP_1_704_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned linkQualPatternSetLane3 :3;
		unsigned :29;
	} Status;
} DPCD_SNOOP_2_708_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned Lane0_cr_done :1;
		unsigned Lane0_eq_done:1;
		unsigned Lane0_sym_licked:1;
		unsigned :1;
		unsigned Lane1_cr_done :1;
		unsigned Lane1_eq_done:1;
		unsigned Lane1_sym_licked:1;
		unsigned :1;
		unsigned Lane2_cr_done :1;
		unsigned Lane2_eq_done:1;
		unsigned Lane2_sym_licked:1;
		unsigned :1;
		unsigned Lane3_cr_done :1;
		unsigned Lane3_eq_done:1;
		unsigned Lane3_sym_licked:1;
		unsigned :1;
		unsigned Lane_align_status_updated:8;
		unsigned VoltSwingLane0:2;
		unsigned PreEmpLane0:2;
		unsigned VoltSwingLane1:2;
		unsigned PreEmpLane1:2;
	} Status;
} DPCD_SNOOP_3_720_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned VoltSwingLane2:2;
		unsigned PreEmpLane2:2;
		unsigned VoltSwingLane3:2;
		unsigned PreEmpLane3:2;
		unsigned SymErrorCntLane0_low:8;
		unsigned SymErrorCntLane0_high:7;
		unsigned SymErrorCntLane0_valid:1;
		unsigned SymErrorCntLane1_low:8;
	} Status;
} DPCD_SNOOP_4_724_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned SymErrorCntLane1_high:7;
		unsigned SymErrorCntLane1_valid:1;
		unsigned SymErrorCntLane2_low:8;
		unsigned SymErrorCntLane2_high:7;
		unsigned SymErrorCntLane2_valid:1;
		unsigned SymErrorCntLane3_low:8;
	} Status;
} DPCD_SNOOP_5_728_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned SymErroCntLane3_high:7;
		unsigned SymErroCntLane3_valid:1;
		unsigned IEEE_OUT1:8;
		unsigned IEEE_OUT2:8;
		unsigned IEEE_OUT3:8;
	} Status;
} DPCD_SNOOP_6_72C_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned DPRX_DPCD_REV_00000:8;
		unsigned MAX_LINK_RATE_00001:8;
		unsigned MAX_LANE_COUNT_00002:8;
		unsigned :8;
	} Status;
} DPCD_SNOOP_7_750_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned LINK_BW_SET_100:8;
		unsigned LANE_COUNT_SET_101:8;
		unsigned SET_POWER_STATE_600:8;
		unsigned :8;
	} Status;
} DPCD_SNOOP_8_754_RW;

// ****** LT_CONFIG
typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned DPRX_AUX_CLK_LOW:4;
		unsigned :4;
		unsigned DPRX_AUX_CLK_HIGH:4;
		unsigned :4;
		unsigned DPRX_AUX_CLK_EN:1;
		unsigned :1;
		unsigned AUX_POLARITY:1;
		unsigned :6;
		unsigned AUX_MOD_CTRL:2;
		unsigned :1;
		unsigned ADJ_REQ_RPT_NUM:4;
	} Status;
} LT_CONFIG_0_900_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned LTTPR_RD_INT:8;
		unsigned FORCE_ML0_VSL:2;
		unsigned FORCE_ML0_PEL:2;
		unsigned FORCE_ML1_VSL:2;
		unsigned FORCE_ML1_PEL:2;
		unsigned FORCE_ML2_VSL:2;
		unsigned FORCE_ML2_PEL:2;
		unsigned FORCE_ML3_VSL:2;
		unsigned FORCE_ML3_PEL:2;
		unsigned :8;
	} Status;
} LT_CONFIG_1_904_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned :10;
		unsigned FORCE_TX_PARAM:1;
		unsigned FULL_TRANSPARENT_EN:1;
		unsigned :2;
		unsigned PRE_EMP_LEVEL_3_DIS:1;
		unsigned :17;
	} Status;
} LT_CONFIG_2_908_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned :24;
		unsigned DPRX_RD_INT:7;
		unsigned AMP_LEVEL_3_DIS:1;
	} Status;
} LT_CONFIG_3_90C_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ADJ_SWING_REQ_LN0_0:2;
		unsigned ADJ_SWING_REQ_LN1_0:2;
		unsigned ADJ_PRE_EMP_REQ_LN0_0:2;
		unsigned ADJ_PRE_EMP_REQ_LN1_0:2;
		unsigned ADJ_SWING_REQ_LN2_0:2;
		unsigned ADJ_SWING_REQ_LN3_0:2;
		unsigned ADJ_PRE_EMP_REQ_LN2_0:2;
		unsigned ADJ_PRE_EMP_REQ_LN3_0:2;
		unsigned ADJ_REQ_1:16;
	} Status;
} LT_CONFIG_4_910_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ADJ_REQ_2:16;
		unsigned ADJ_REQ_3:16;
	} Status;
} LT_CONFIG_5_914_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ADJ_REQ_4:16;
		unsigned ADJ_REQ_5:16;
	} Status;
} LT_CONFIG_6_918_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ADJ_REQ_6:16;
		unsigned ADJ_REQ_7:16;
	} Status;
} LT_CONFIG_7_91C_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ADJ_REQ_8:16;
		unsigned ADJ_REQ_9:16;
	} Status;
} LT_CONFIG_8_920_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ADJ_REQ_10:16;
		unsigned ADJ_REQ_11:16;
	} Status;
} LT_CONFIG_9_924_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ADJ_REQ_12:16;
		unsigned ADJ_REQ_13:16;
	} Status;
} LT_CONFIG_A_928_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned ADJ_REQ_14:16;
		unsigned ADJ_REQ_15:16;
	} Status;
} LT_CONFIG_B_92C_RW;

// ****** DPCD_LTTPR_CAP_ID
typedef union  {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned LT_TUNABLE_REV:8;
		unsigned MAX_LINK_RATE:8;
		unsigned PHY_RPTR_CNT:8;
		unsigned PHY_RPTR_MODE:8;
	} Status;
} DPCD_LTTPR_CAP_ID_0_A00_RW;

typedef union  {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned MAX_LANE_COUNT:5;
		unsigned :27;
	} Status;
} DPCD_LTTPR_CAP_ID_1_A04_RW;

// ****** DPCD_LTTPR_CONF_STATUS
typedef union{
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned trainingPatternSelection :4;
		unsigned recoveredClockOutEN:1; // >> 4
		unsigned scramblingDisable:1; // >> 5
		unsigned symbolErrorCountSel:2; // > 6
		unsigned voltageSwingSetLane0:2; // >> 8
		unsigned maxSwingReachedLane0:1; // >> 10
		unsigned preEmphasisSetLane0:2; // >> 11
		unsigned maxPreEmphasisReachedLane0:1; // >> 13
		unsigned :2;
		unsigned voltageSwingSetLane1:2; // >> 16
		unsigned maxSwingReachedLane1:1; // >> 18
		unsigned preEmphasisSetLane1:2; // >> 19
		unsigned maxPreEmphasisReachedLane1:1; // >> 21
		unsigned :2;
		unsigned voltageSwingSetLane2:2; // >> 24
		unsigned maxSwingReachedLane2:1; // >> 26
		unsigned preEmphasisSetLane2:2; // >> 27
		unsigned maxPreEmphasisReachedLane2:1; // > 29
		unsigned :2;
	} Status;
} DPCD_LTTPR_CONF_STATUS_13_10_B00_RW;

typedef union{
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned voltageSwingSetLane3 :2;
		unsigned maxSwingReachedLane3:1;
		unsigned preEmphasisSetLane3:2;
		unsigned maxPreEmphasisReachedLane3:1;
		unsigned :26;
	} Status;
} DPCD_LTTPR_CONF_STATUS_17_14_B04_RW;

typedef union{
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned linkQualPatternSetLane3 :3;
		unsigned :29;
	} Status;
} DPCD_LTTPR_CONF_STATUS_1B_18_B08_RW;

typedef union{
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned trainRdInterval :7;
		unsigned :1;
		unsigned voltSwingLevel3:1;
		unsigned preEmpLevel3:1;
		unsigned :22;
	} Status;
} DPCD_LTTPR_CONF_STATUS_23_20_B10_RO;

typedef union{
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned Lane0_cr_done :1;
		unsigned Lane0_eq_done:1;
		unsigned Lane0_sym_licked:1;
		unsigned :1;
		unsigned Lane1_cr_done :1;
		unsigned Lane1_eq_done:1;
		unsigned Lane1_sym_licked:1;
		unsigned :1;
		unsigned Lane2_cr_done :1;
		unsigned Lane2_eq_done:1;
		unsigned Lane2_sym_licked:1;
		unsigned :1;
		unsigned Lane3_cr_done :1;
		unsigned Lane3_eq_done:1;
		unsigned Lane3_sym_licked:1;
		unsigned :1;
		unsigned Lane_align_status_updated:8;
		unsigned VoltSwingLane0:2;
		unsigned PreEmpLane0:2;
		unsigned VoltSwingLane1:2;
		unsigned PreEmpLane1:2;
	} Status;
} DPCD_LTTPR_CONF_STATUS_33_30_B20_RO;

typedef union{
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned VoltSwingLane2:2;
		unsigned PreEmpLane2:2;
		unsigned VoltSwingLane3:2;
		unsigned PreEmpLane3:2;
		unsigned SymErrorCntLane0_low:8;
		unsigned SymErrorCntLane0_high:7;
		unsigned SymErrorCntLane0_valid:1;
		unsigned SymErrorCntLane1_low:8;
	} Status;
} DPCD_LTTPR_CONF_STATUS_37_34_B24_RO;

typedef union{
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned SymErrorCntLane1_high:7;
		unsigned SymErrorCntLane1_valid:1;
		unsigned SymErrorCntLane2_low:8;
		unsigned SymErrorCntLane2_high:7;
		unsigned SymErrorCntLane2_valid:1;
		unsigned SymErrorCntLane3_low:8;
	} Status;
} DPCD_LTTPR_CONF_STATUS_3B_38_B28_RO;

typedef union{
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned SymErrorCntLane3_high:7;
		unsigned SymErrorCntLane3_valid:1;
		unsigned IEEE_OUT1:8;
		unsigned IEEE_OUT2:8;
		unsigned IEEE_OUT3:8;
	} Status;
} DPCD_LTTPR_CONF_STATUS_3F_3C_B2C_RO;

// ****** Receiver configuration registers
typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned : 16;
		unsigned RX_CTLE_TERM_LO : 1;
		unsigned : 15;
	} Status;
} RX_CTLE_TERM_SB_CTRL_18_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned : 24;
		unsigned RX_GEN1_PGA : 2;
		unsigned RX_GEN1_PGA_ATT : 1;
		unsigned RX_GEN1_PGA_OVR : 1;
		unsigned RX_GEN2_PGA :2;
		unsigned RX_GEN2_PGA_ATT :1;
		unsigned RX_GEN2_PGA_OVR :1;
	} Status;
} RX_PGA_CTRL_1C_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RX_SUM_TAP1_SPI:6;
		unsigned : 2;
		unsigned RX_SUM_TAP2_SPI : 6;
		unsigned : 2;
		unsigned RX_SUM_TAP3_SPI : 6;
		unsigned :2;
		unsigned RX_SUM_TAP4_SPI :6;
		unsigned :2;
	} Status;
} RX_DFE_TAP_CTRL_24_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned : 1;
		unsigned RX_SUM_TAPWEIGHTSEL1 : 1;
		unsigned RX_SUM_TAPWEIGHTSEL2 : 1;
		unsigned RX_SUM_TAPWEIGHTSEL3 : 1;
		unsigned RX_SUM_TAPWEIGHTSEL4 : 1;
		unsigned :27;
	} Status;
} RX_DFE_CTRL_28_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RBR_CTLE_CAP:4;
		unsigned RBR_CTLE_ALGO : 4;
		unsigned RBR_CTLE_RES : 6;
		unsigned RBR_CTLER_OVR : 1;
		unsigned : 1;
		unsigned RBR_CLK_TUNE :5;
		unsigned RBR_PI_CLOAD:3;
		unsigned RBR_CDR_IGAIN:3;
		unsigned :1;
		unsigned RBR_CDR_PGAIN:2;
		unsigned :2;
	} Status;
} RX_RBR_RX_Manual_EN_70_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned HBR_CTLE_CAP:4;
		unsigned HBR_CTLE_ALGO : 4;
		unsigned HBR_CTLE_RES : 6;
		unsigned HBR_CTLER_OVR : 1;
		unsigned : 1;
		unsigned HBR_CLK_TUNE :5;
		unsigned HBR_PI_CLOAD:3;
		unsigned HBR_CDR_IGAIN:3;
		unsigned :1;
		unsigned HBR_CDR_PGAIN:2;
		unsigned :2;
	} Status;
} RX_HBR_RX_Manual_EN_74_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned HBR2_CTLE_CAP:4;
		unsigned HBR2_CTLE_ALGO : 4;
		unsigned HBR2_CTLE_RES : 6;
		unsigned HBR2_CTLER_OVR : 1;
		unsigned : 1;
		unsigned HBR2_CLK_TUNE :5;
		unsigned HBR2_PI_CLOAD:3;
		unsigned HBR2_CDR_IGAIN:3;
		unsigned :1;
		unsigned HBR2_CDR_PGAIN:2;
		unsigned :2;
	} Status;
} RX_HBR2_RX_Manual_EN_78_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned HBR3_CTLE_CAP:4;
		unsigned HBR3_CTLE_ALGO : 4;
		unsigned HBR3_CTLE_RES : 6;
		unsigned HBR3_CTLER_OVR : 1;
		unsigned : 1;
		unsigned HBR3_CLK_TUNE :5;
		unsigned HBR3_PI_CLOAD:3;
		unsigned HBR3_CDR_IGAIN:3;
		unsigned :1;
		unsigned HBR3_CDR_PGAIN:2;
		unsigned :2;
	} Status;
} RX_HBR3_RX_Manual_EN_7C_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned GEN1_CTLE_CAP : 4;
		unsigned GEN1_CTLE_ALGO : 4;
		unsigned GEN1_CTLE_RES : 6; // ch1 (18dB) : 001010
									// ch2 (23dB) : 001110
									// ch3 (23dB) : 000101
									// ch4 (12dB) : 010001 (PGA should be 001)
									// 42inch (25dB) : 011010
		unsigned GEN1_CTLER_OVR : 1;
		unsigned : 1;
		unsigned GEN1_CLK_TUNE : 5;
		unsigned GEN1_PI_CLOAD : 3;
		unsigned GEN1_CDR_IGAIN : 3;
		unsigned : 1;
		unsigned GEN1_CDR_PGAIN : 2;
		unsigned : 2;
	} Status;
} RX_Gen1_RX_Manual_EN_80_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned GEN2_CTLE_CAP : 4;
		unsigned GEN2_CTLE_ALGO : 4;
		unsigned GEN2_CTLE_RES : 6;
		unsigned GEN2_CTLER_OVR : 1;
		unsigned : 1;
		unsigned GEN2_CLK_TUNE : 5;
		unsigned GEN2_PI_CLOAD : 3;
		unsigned GEN2_CDR_IGAIN : 3;
		unsigned : 1;
		unsigned GEN2_CDR_PGAIN : 2;
		unsigned : 2;
	} Status;
} RX_Gen2_RX_Manual_EN_84_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned :16;
		unsigned GEN1_EQ_MODE_TAP : 3;
		unsigned : 1;
		unsigned GEN2_EQ_MODE_TAP : 3;
		unsigned : 1;
		unsigned GEN1_EQ_WAIT_TAP : 4;
		unsigned GEN2_EQ_WAIT_TAP : 4;
	} Status;
} RX_DFE_ADAPT_CTRL_88_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned :16;
		unsigned GEN1_EQ_MODE_CTLE : 3;
		unsigned GEN2_EQ_MODE_CTLE : 3;
		unsigned : 1;
		unsigned GEN1_EQ_WAIT_CTLE : 4;
		unsigned GEN2_EQ_WAIT_CTLE : 4;
	} Status;
} RX_CTLE_ADAPT_MODE_8C_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned :16;
		unsigned GEN1_EQ_MODE_THR : 3;
		unsigned : 1;
		unsigned GEN2_EQ_MODE_THR : 3;
		unsigned : 1;
		unsigned GEN1_EQ_WAIT_THR : 4;
		unsigned GEN2_EQ_WAIT_THR : 4;
	} Status;
} RX_THR_ADAPT_MODE_90_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RBR_EQ_WAIT_THR : 4;
		unsigned HBR_EQ_WAIT_THR : 4;
		unsigned HBR2_EQ_WAIT_THR : 4;
		unsigned HBR3_EQ_WAIT_THR : 4;
		unsigned RBR_EQ_WAIT_CTLE : 4;
		unsigned HBR_EQ_WAIT_CTLE : 4;
		unsigned HBR2_EQ_WAIT_CTLE : 4;
		unsigned HBR3_EQ_WAIT_CTLE : 4;
	} Status;
} RX_E_ADAPT_WAIT0_A0_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RBR_EQ_WAIT_TAP : 4;
		unsigned HBR_EQ_WAIT_TAP : 4;
		unsigned HBR2_EQ_WAIT_TAP : 4;
		unsigned HBR3_EQ_WAIT_TAP : 4;
		unsigned  : 16;
	} Status;
} RX_E_ADAPT_WAIT1_A4_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RX_CTLE_RES_MIN0 : 6;
		unsigned RX_CTLE_RES_MAX0 : 6;
		unsigned RX_CTLE_RES_MIN1 : 6;
		unsigned RX_CTLE_RES_MAX1 : 6;
		unsigned : 1;
		unsigned AdaptCodeMuxSel : 4;
		unsigned : 3;
	} Status;
} RX_PHYD_CTRL0_B4_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
	    unsigned EQ_ADAPT_CTRL : 24; // TODO - need structure
		unsigned : 4;
		unsigned GEN1_DFE_ADAPT_EN : 1;
		unsigned GEN2_DFE_ADAPT_EN : 1;
		unsigned : 2;
	} Status;
} RX_PHYD_MISC_CNTRL0_C0_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned CDR_RESET_MODE : 1;
		unsigned : 7;
		unsigned CDR_RESET_DURATION : 5;
		unsigned : 19;
	} Status;
} RX_CDR_RESET_CTRL_D8_RW;

// ****** Receiver Equalizer Status Monitoring Register
typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned :9;
		unsigned RX_SUMMER_EBTUNE_CAL : 3;
		unsigned RX_ODAC_IBTUNE_CAL : 3;
		unsigned RX_SUMMER_ATT_EN_CAL : 1;
		unsigned RX_THRESH_IBTUNE_CAL : 3;
		unsigned : 1;
		unsigned RX_CTLE_PGA_CAL : 2;
		unsigned GM_CTLE_CAL : 2;
		unsigned : 1;
		unsigned RX_TDAC_EBTUNE_CAL : 3;
		unsigned RX_TAP1_EBTUNE_CAL : 3;
		unsigned : 1;
	} Status;
} RX_ADAPT_MONITOR0_94_RO;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RX_CTLE_RES_MIN0 : 6;
		unsigned RX_CTLE_RES_MAX0 : 6;
		unsigned RX_CTLE_RES_MIN1 : 6;
		unsigned RX_CTLE_RES_MAX1 : 6;
		unsigned : 1;
		unsigned AdaptCodeMuxSel : 4;
		unsigned : 3;
	} Status;
} RX_CTLE_ADAPT_CONF_B4_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RX_SLICER_THE_LMS : 6;
		unsigned RX_SLICER_THO_LMS : 6;
		unsigned : 4;
		unsigned SS_ACODE : 6;
		unsigned RX_CTLE_RES_LMS : 6;
		unsigned : 4;
	} Status;
} RX_ADAPT_MONITOR1_BC_RO;

typedef union {
	struct {
		uint32_t data;
	} RegisterData;
	struct {
		unsigned RX_SUM_TAP1_LMS : 6;
		unsigned RX_SUM_TAP2_LMS : 6;
		unsigned RX_SUM_TAP3_LMS : 6;
		unsigned RX_SUM_TAP4_LMS : 6;
		unsigned : 8;
	} Status;
} RX_ADAPT_MONITOR2_E4_RO;

// ****** Eye Opening Monitor Registers

typedef union {
    struct {
             uint8_t data[4];
         } RegDataArray;
        struct {
             unsigned RX_OFFSET_DTH_EVEN_SPI : 6;
             unsigned RX_OFFSET_DTH_EVEN_DBGEN : 1;
             unsigned :1;
             unsigned RX_OFFSET_DTH_ODD_SPI : 6;
             unsigned RX_OFFSET_DTH_ODD_DBGEN : 1;
             unsigned : 18;
        } Status;
} RX_ETH_SLICER_CTRL_2C_RW;

typedef union {
    struct {
             uint8_t data[4];
         } RegDataArray;
        struct {
             unsigned RX_EYESCAN_EN:1;
             unsigned RX_CDR_FCOUNT_MNTR_EN:1;
             unsigned : 6;
             unsigned RX_PCODE_EYESCANOOFFSET : 7;
             unsigned : 17;
        } Status;
} RX_CDR_OFFSET_CTRL_34_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned RX_RST : 1;
		unsigned RX_INVERT : 1;
		unsigned RX_SWAP : 2;
		unsigned RX_SWAP_ERR : 2;
		unsigned RX_INVERT_ERR : 1;
		unsigned : 4;
		unsigned RSTZ_EOM : 1;
		unsigned :20;
	} Status;
} RX_PHY_CTRL0_50_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned ERDD_BER_TH_XG : 32;
	} Status;
} RX_EOM_ACCUM_INTERVAL_54_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned EOM_THR_INIT : 6;
		unsigned : 2;
		unsigned EOM_PHOS_INIT : 7;
		unsigned :17;
	} Status;
} RX_EOM_START_VAL_58_RW;

typedef union {
     struct {
         uint8_t data[4];
     } RegDataArray;
    struct {
        unsigned : 4;
        unsigned ERDD_PATSEL_XG: 3;
        unsigned : 5;
        unsigned ERDD_RSLT_CLR_XG:1;
    } Status;
} RX_XG_ERDD_CTRL0_6C_RW;

typedef union {
    struct {
        uint8_t data[4];
    } RegDataArray;
   struct {
       uint32_t ERDE_DAT_ERRCNT_BER_XG;
   } Status;
} RX_XG_ERDE_STATUS1_AC_RO;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned EOM_EN : 1;
		unsigned EOM_START : 1;
		unsigned EOM_MODE : 3;
		unsigned PCODE_EOMOFFSET_DBG:1;
		unsigned OFSET_EOM_DBG:1;
		unsigned EQ_ADAPT_STOP : 1;
		unsigned :6;
		unsigned MODE_SW_OVER:1;
		unsigned FIFO32T20_CLR:1;
		unsigned EQ_EN_OVR_THR:1;
		unsigned EQ_EN_OVR_TAP:1;
		unsigned EQ_EN_OVR_CTLE:1;
		unsigned :3;
		unsigned EQ_RST_THR:1;
		unsigned EQ_RST_TAP:1;
		unsigned EQ_RST_CTLE:1;
		unsigned EQ_POLSW:1;
		unsigned OFFCAL_EN:1;
		unsigned OFFCAL_STRST:1;
		unsigned OFFCAL_IB_CTRL:1;
		unsigned OFFCAL_CTLE_PDZ:2;
		unsigned OFFCAL_CTLE_PDZ_OVR:1;
		unsigned :24;
	} Status;
} RX_EOM_CTRL_B8_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned : 5;
		unsigned EOM_DONE : 1;
		unsigned EOM_THRMAX : 6;
		unsigned EOM_THRMIN : 6;
		unsigned EOM_PHOSMAX : 7;
		unsigned EOM_PHOSMIN : 7;
	} Status;
} RX_PHYD_EOM_DAT_C8_RO;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
	    unsigned EOM_EYELIM : 17; // TODO - make structure
		unsigned EOM_HSTEP : 3;
		unsigned EOM_VSTEP : 3;
		unsigned EOM_OVR : 1; // bit 23

		unsigned ERDE_EN_OVR : 1;
		unsigned ERDE_RSLT_CLR_OVR : 1;
		unsigned DBG_DIVCLK_EN :1;
		unsigned CG_ERD_EN : 1;

		unsigned CG_EQ_EN_OVR :1;
		unsigned CG_OFFCAL_EN_OVR :1;
		unsigned CG_OEM_EN :1;
		unsigned RATEMUX_RSTZ_OVR:1;
	} Status;
} RX_EOM_CONFIG_D4_RW;

// ****** Transmitter Configuration Registers
typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned TX_SOFT_RESET : 1;
		unsigned : 7;
		unsigned TX_INVERT : 1;
		unsigned : 1;
		unsigned FIFO_AUTO_RESET_EN : 1;
		unsigned : 21;
	} Status;
} TX_PHY_CTRL0_40_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned : 3;
		unsigned TXGC_GAIN_USBGEN1 : 4;
		unsigned : 1;
		unsigned :24;
	} Status;
} TX_GC_CTRL1_4C_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned TX_VML_SWING_EN : 5;
		unsigned : 3;
		unsigned TX_VML_MAIN_EN : 5;
		unsigned : 3;
		unsigned TX_VML_PRE_EN : 3;
		unsigned : 1;
		unsigned TX_VML_POST_EN : 4;
		unsigned TX_VML_PRE : 3;
		unsigned : 1;
		unsigned TX_VML_POST : 4;
	} Status;
} TX_CONF_WINDOW_0_54_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned TX_P2S_OUTP_HIGH_EN : 1;
		unsigned TX_P2S_OUTN_HIGH_EN : 1;
		unsigned TX_P2S_TEST_EN : 1;
		unsigned TX_P2S_ACTIVE_EN : 1;
		unsigned TX_P2S_PRE_PD : 1;
		unsigned TX_P2S_POST_PD : 1;
		unsigned TX_DYNAMIC_COMP_EN : 1;
		unsigned : 1;
		unsigned TX_DYNAMIC_COMP_TUNE : 5;
		unsigned TX_DLDO_TUNE : 3;
		unsigned TX_CAL_SWING_MODE : 2;
		unsigned TX_CLK_SEL : 2;
		unsigned TX_CLK_TUNE : 5;
		unsigned : 3;
		unsigned TX_PI_CLOAD : 3;
		unsigned : 1;
	} Status;
} TX_CONF_WINDOW_1_58_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned CONF_SEL : 5;
		unsigned : 2;
		unsigned RD_WR_SEL : 1;
		unsigned :24;
	} Status;
} TX_CONF_CTRL_5C_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	struct {
		unsigned TX_VML_SWING_EN : 5;
		unsigned : 3;
		unsigned TX_VML_MAIN_EN : 5;
		unsigned : 3;
		unsigned TX_VML_PRE_EN : 3;
		unsigned : 1;
		unsigned TX_VML_POST_EN : 4;
		unsigned TX_VML_PRE : 3;
		unsigned : 1;
		unsigned TX_VML_POST : 4;
	} Status;
} TX_CONF_MONITOR_0_60_RW;

typedef union {
	 struct {
		 uint8_t data[4];
	 } RegDataArray;
	 struct {
		unsigned TX_P2S_OUTP_HIGH_EN : 1;
		unsigned TX_P2S_OUTN_HIGH_EN : 1;
		unsigned TX_P2S_TEST_EN : 1;
		unsigned TX_P2S_ACTIVE_EN : 1;
		unsigned TX_P2S_PRE_PD : 1;
		unsigned TX_P2S_POST_PD : 1;
		unsigned TX_DYNAMIC_COMP_EN : 1;
		unsigned : 1;
		unsigned TX_DYNAMIC_COM_TUNE : 5;
		unsigned TX_DLDO_TUNE : 3;
		unsigned TX_CAL_SWING_MODE:2;
		unsigned TX_CLK_SEL:2;
		unsigned TX_CLK_TUNE:5;
		unsigned :3;
		unsigned TX_PI_CLOAD:3;
		unsigned :1;
	} Status;
} TX_CONF_MONITOR_1_64_RW;
// end register structures

// prototypes
int MCDP6200_Init(void);
int SetMCDP6200Register(char *msg);
int GetMCDP6200Register(char *msg, char *str);

int SetMCDP6200_RTMR_P_POL(char *msg);
int SetMCDP6200_RTMR_DIS_N(char *msg);

void MCDP6200_GPIO_Pin_RTMR_DIS_N_Enable(void);
void MCDP6200_GPIO_Pin_RTMR_DIS_N_Disable(void);

void MCDP6200_GPIO_Pin_RTMR_P_POL_Enable(void);
void MCDP6200_GPIO_Pin_RTMR_P_POL_Disable(void);

void MCDP6200_GPIO_Pin_RTMR_DIS_N_StartPulse(void);


#endif /* REGISTERSWRITE_H_ */
