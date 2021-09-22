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
#define GEN2_HOST 0
#define GEN2_DEVICE 1
#define GEN1_HOST 2
#define GEN1_DEVICE 3

// TX base address
#define	UFP_USB_TX_BASE_ADDR 0x1000
#define	DFP_USB_TX1_BASE_ADDR 0x2400
#define	DFP_USB_TX2_BASE_ADDR 0x2500
#define	ALL_USB_TX_BASE_ADDR 0x1F00 // write only

// RX base address
#define	UFP_USB_RX_BASE_ADDR 0x1100
#define	DFP_USB_RX1_BASE_ADDR 0x2000
#define	DFP_USB_RX2_BASE_ADDR 0x2100
#define	ALL_USB_RX_BASE_ADDR 0x1E00

// TX registers
#define	TXPHY_CTRL0_0x40 0x40
#define TX_GC_CTRL1_0x4C 0x4C
#define	TX_CONF_WINDOW_0_0x54 0x54
#define	TX_CONF_WINDOW_1_0x58 0x58
#define	TX_CONF_CTRL_0x5C 0x5C
#define	TX_CONF_MONITOR_0_0x60 0x60
#define	TX_CONF_MONITOR_1_0x64 0x64

// RX registers
#define	RX_CTLE_TERM_SB_CTRL_0x18 0x18
#define	RX_PGA_CTRL_0x1C 0x1C
#define	RX_DFE_TAP_CTRL_0x24 0x24
#define	RX_DFE_CTRL_0x28 0x28
#define	RX_Gen1_RX_Manual_EN_0x80 0x80
#define	RX_Gen2_RX_Manual_EN_0x84 0x84
#define	RX_DFE_ADAPT_CTRL_0x88 0x88
#define	RX_CTLE_ADAPT_MODE_0x8C 0x8C
#define	RX_THR_ADAPT_MODE_0x90 0x90
#define	RX_PHYD_CTRL0_0xB4 0xB4
#define	RX_PHYD_MISC_CNTRL0_0xC0 0xC0

// Receiver Equalizer Status monitor registers
#define	RX_ADAPT_MONITOR0_0x94 0x94
#define	RX_CTLE_ADAPT_CONF_0xB4 0xB4
#define	RX_ADAPT_MONITOR1_0xBC 0xBC
#define	RX_ADAPT_MONITOR2_0xE4 0xE4

// Eye opening Monitor Registers
#define	RXPHY_CTRL0_0x50 0x50
#define RX_EOM_ACCUM_INTERVAL_0x54 0x54
#define RX_EOM_START_VAL_0x58 0x58
#define RX_EOM_CTRL_0xB8 0xB8
#define RX_PHYD_EOM_DAT_0xC8 0xC8
#define RX_EOM_CONFIG_0xD4 0xD4

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
		unsigned DPRX_ERR_CNT_EN :1;
		unsigned :31;
	} Status;
} FPRX_ERR_CNT_CTRL_61C_RW;

typedef union {
	struct {
		uint8_t data[4];
	} RegDataArray;
	struct {
		unsigned DPRX_FINAL_LINL_BW :6;
		unsigned :2;
		unsigned DPRX_FINALLANE_COUNT:3;
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
		unsigned :2;
		unsigned linkQualPatternSetLane0:3;
		unsigned :5;
		unsigned linkQualPatternSetLane1:3;
		unsigned :5;
		unsigned linkQualPatternSetLane2:3;
		unsigned :5;
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
		unsigned EOM_EN : 1;
		unsigned EOM_START : 1;
		unsigned EOM_MODE : 3;
		unsigned reserved :2;
		unsigned EQ_ADAPT_STOP : 1;
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
		unsigned EOM_OVR : 1;
		unsigned ERDE_EN_OVR : 1;
		unsigned ERDE_RSLT_CLR_OVR : 1;
		unsigned : 6;
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
		unsigned reserved2 : 21;
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
void MCDP6200_Init(void);
void MCDP6200_SetTxConfigWindow(uint8_t retimerAddress, uint16_t _txRegister, uint8_t rdWr, uint8_t gen);
int RetimerWriteRxRegister(uint8_t address, uint8_t siteNumber, RetimerStatus *retimerStatus);
int RetimerWriteTxRegister(uint8_t address, uint8_t siteNumber, RetimerStatus *retimerStatus);


#endif /* REGISTERSWRITE_H_ */
