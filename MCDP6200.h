/* 
 * File:   MCDP6200.h
 * Author: karl.yamashita
 *
 * Created on January 14, 2021, 1:12 PM
 */

#ifndef MCDP6200_H
#define	MCDP6200_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <atmel_start.h>

    // defines
#define MCDP6200_SLAVE_ID_14 0x14 // 0b10100
#define MCDP6200_SLAVE_ID_15 0x15
#define MCDP6200_SLAVE_ID_16 0x16
#define MCDP6200_SLAVE_ID_17 0x17    

enum{
	READ,
	WRITE
};

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
    
    // structures
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned reserved : 16;
            unsigned USB_G2_MODE_SEL : 1;
            unsigned reserved1 : 3;
            unsigned FAST_U2_EXIT_EN : 1;
            unsigned reserved2 : 9;
            unsigned XTAL_REF_MODE : 1;
            unsigned REFOUT_EN : 1;
        } Status;
    } IC_RT_CONFIG_30C_RW;

    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned reserved : 1;
            unsigned PPOL_OVR_EN : 1;
            unsigned PCONF0_OVR_EN : 1;
            unsigned PCONF1_OVR_EN : 1;
            unsigned PPOL : 1;
            unsigned PCONF0 : 1;
            unsigned PCONF1 : 1;
            unsigned reserved1 : 1;
            unsigned IC_SOFT_RST : 1;
            unsigned USB_SOFT_RST : 1;
            unsigned DP_SOFT_RST : 1;
            unsigned DP_AUX_SOFT_RST : 1;
            unsigned PPOL_BS_OVR : 1;
            unsigned PCONF0_BS_OVR : 1;
            unsigned PCONF1_BS_OVR : 1;
            unsigned reserved2 : 1;
            unsigned DP_LT_AMP_PRE_CHK_DIS : 1;
            unsigned DP_LT__SYMB_ERR_CHK_DIS : 1;
            unsigned reserved3 : 12;
        } Status;
    } OPMODE_CONF_504_RW;

    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned TWISLV0_BS_STATUS : 1;
            unsigned TWISLV1_BS_STATUS : 1;
            unsigned P_CONF0_BS_STATUS : 1;
            unsigned P_CONF1_BS_STATUS : 1;
            unsigned P_POL_BS_STATUS : 1;
            unsigned TM_BS_STATUS : 1;
            unsigned reserved : 2;
            unsigned MINOR_VERSION : 4;
            unsigned MAJOR_VERSION : 4;
            unsigned reserved1 : 16;
        } Status;
    } IC_REV_510_RO;

    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            uint16_t reserved;
            uint8_t reserved1;
            unsigned reserved2 : 2;
            unsigned RO_CTUNE_SEL : 1;
            unsigned RO_RC_TRIM : 4;
            unsigned reserved3 : 1;
        } Status;
    } RO_TUNE_MANUAL_281C_RW;

    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned RO_CTUNE_DONE : 1;
            unsigned RO_LD_LOCK : 1;
            unsigned RO_CTUNEF_MON : 2;
            unsigned RO_CTUNEC_MON : 2;
            uint16_t reserved;
            uint8_t reserved1;
            unsigned reserved2 : 2;
        } Status;
    } RO_CTUNE_STATUS_283C_RO;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

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
            unsigned U1_Deuplicate : 1; // this has duplicate bit name at bit 8
            unsigned U2 : 1;
            unsigned U3 : 1;
            unsigned Idle : 1;
            unsigned IdleLP : 1;
            unsigned reserved : 1;
            unsigned USB_P0_RATE : 1;
            unsigned reserved1 : 8;
        } Status;
    } USB_RTSSM_STATUS0_320_RO;
    
    typedef struct {
        
    } USB_RTSSM1_TYPEC;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

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
            unsigned U1_Deuplicate : 1; // this has duplicate bit name at bit 8
            unsigned U2 : 1;
            unsigned U3 : 1;
            unsigned Idle : 1;
            unsigned IdleLP : 1;
            unsigned reserved : 1;
            unsigned USB_P1_RATE : 1;
            unsigned reserved1 : 8;
        } Status;
    } USB_RTSSM_STATUS1_324_RO;

    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

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
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

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
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

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
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

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
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned USB_P0_HRST_HIS : 4;
            unsigned USB_P0_BLRC_HIS : 4;
            unsigned reserved: 8;
            unsigned USB_P1_HRST_HIS : 4;
            unsigned USB_P1_BLRC_HIS : 4;
            unsigned reserved1 : 8;
        } Status;
    } USB_RTSSM_STATUS6_338_CRO;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            uint16_t reserved;
            unsigned reserved1 : 8;
            unsigned RX_CTLE_TERM_LO : 1;
            unsigned reserved2 : 8;
            unsigned reserved3 : 7;
        } Status;
    } RX_CTLE_TERM_SB_CTRL_18_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            uint16_t reserved;
            unsigned reserved1 : 8;
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
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned RX_SUM_TAP1_SPI:6;
            unsigned reserved : 2;
            unsigned RX_SUM_TAP2_SPI : 6;
            unsigned reserved1 : 2;
            unsigned RX_SUM_TAP3_SPI : 6;
            unsigned reserved2 :2;
            unsigned RX_SUM_TAP4_SPI :6;
            unsigned reserved3 :2;
        } Status;
    } RX_DFE_TAP_CTRL_24_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned reserved : 1;
            unsigned RX_SUM_TAPWEIGHTSEL1 : 1;
            unsigned RX_SUM_TAPWEIGHTSEL2 : 1;
            unsigned RX_SUM_TAPWEIGHTSEL3 : 1;
            unsigned RX_SUM_TAPWEIGHTSEL4 : 1;
            unsigned reserved1 :3;
            uint16_t reserved2;
            uint8_t reserved3;
        } Status;
    } RX_DFE_CTRL_28_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned GEN1_CTLE_CAP : 4;
            unsigned GEN1_CTLE_ALGO : 4;
            unsigned GEN1_CTLE_RES : 6; // ch1 (18dB) : 001010
                                        // ch2 (23dB) : 001110
                                        // ch3 (23dB) : 000101
                                        // ch4 (12dB) : 010001 (PGA should be 001)
                                        // 42inch (25dB) : 011010
            unsigned GEN1_CTLER_OVR : 1;
            unsigned reserved : 1;
            unsigned GEN1_CLK_TUNE : 5;
            unsigned GEN1_PI_CLOAD : 3;
            unsigned GEN1_CDR_IGAIN : 3;
            unsigned reserved1 : 1;
            unsigned GEN1_CDR_PGAIN : 2;
            unsigned reserved2 : 2;
        } Status;
    } RX_Gen1_RX_Manual_EN_80_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned GEN2_CTLE_CAP : 4;
            unsigned GEN2_CTLE_ALGO : 4;
            unsigned GEN2_CTLE_RES : 6;
            unsigned GEN2_CTLER_OVR : 1;
            unsigned reserved : 1;
            unsigned GEN2_CLK_TUNE : 5;
            unsigned GEN2_PI_CLOAD : 3;
            unsigned GEN2_CDR_IGAIN : 3;
            unsigned reserved1 : 1;
            unsigned GEN2_CDR_PGAIN : 2;
            unsigned reserved2 : 2;
        } Status;
    } RX_Gen2_RX_Manual_EN_84_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            uint16_t reserved;
            unsigned GEN1_EQ_MODE_TAP : 3;
            unsigned reserved1 : 1;
            unsigned GEN2_EQ_MODE_TAP : 3;
            unsigned reserved2 : 1;
            unsigned GEN1_EQ_WAIT_TAP : 4;
            unsigned GEN2_EQ_WAIT_TAP : 4;
        } Status;
    } RX_DFE_ADAPT_CTRL_88_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            uint16_t reserved;
            unsigned GEN1_EQ_MODE_CTLE : 3;
            unsigned GEN2_EQ_MODE_CTLE : 3;
            unsigned reserved1 : 1;
            unsigned GEN1_EQ_WAIT_CTLE : 4;
            unsigned GEN2_EQ_WAIT_CTLE : 4;
        } Status;
    } RX_CTLE_ADAPT_MODE_8C_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            uint16_t reserved;
            unsigned GEN1_EQ_MODE_THR : 3;
            unsigned reserved1 : 1;
            unsigned GEN2_EQ_MODE_THR : 3;
            unsigned reserved2 : 1;
            unsigned GEN1_EQ_WAIT_THR : 4;
            unsigned GEN2_EQ_WAIT_THR : 4;
        } Status;
    } RX_THR_ADAPT_MODE_90_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned RX_CTLE_RES_MIN0 : 6;
            unsigned RX_CTLE_RES_MAX0 : 6;
            unsigned RX_CTLE_RES_MIN1 : 6;
            unsigned RX_CTLE_RES_MAX1 : 6;
            unsigned reserved : 1;
            unsigned AdaptCodeMuxSel : 4;
            unsigned reserved1 : 3;
        } Status;
    } RX_PHYD_CTRL0_B4_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
        //    unsigned EQ_ADAPT_CTRL : 24; // TODO - need structure
            unsigned reserved : 4;
            unsigned GEN1_DFE_ADAPT_EN : 1;
            unsigned GEN2_DFE_ADAPT_EN : 1;
            unsigned reserved1 : 2;
        } Status;
    } RX_PHYD_MISC_CNTRL0_C0_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
        //    unsigned EQ_ADAPT_CTRL : 24; // TODO - need structure
            uint8_t reserved;
            unsigned reserved1 : 1;
            unsigned RX_SUMMER_EBTUNE_CAL : 3;
            unsigned RX_ODAC_IBTUNE_CAL : 3;
            unsigned RX_SUMMER_ATT_EN_CAL : 1;
            unsigned RX_THRESH_IBTUNE_CAL : 3;
            unsigned reserved2 : 1;
            unsigned RX_CTLE_PGA_CAL : 2;
            unsigned GM_CTLE_CAL : 2;
            unsigned reserved3 : 1;
            unsigned RX_TDAC_EBTUNE_CAL : 3;
            unsigned RX_TAP1_EBTUNE_CAL : 3;
            unsigned reserved4 : 1;
        } Status;
    } RX_ADAPT_MONITOR0_90_RO;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned RX_CTLE_RES_MIN0 : 6;
            unsigned RX_CTLE_RES_MAX0 : 6;
            unsigned RX_CTLE_RES_MIN1 : 6;
            unsigned RX_CTLE_RES_MAX1 : 6;
            unsigned reserved : 1;
            unsigned AdaptCodeMuxSel : 4;
            unsigned reserved1 : 3;
        } Status;
    } RX_CTLE_ADAPT_CONF_B4_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned RX_SLICER_THE_LMS : 6;
            unsigned RX_SLICER_THO_LMS : 6;
            unsigned reserved : 4;
            unsigned SS_ACODE : 6;
            unsigned RX_CTLE_RES_LMS : 6;
            unsigned reserved1 : 4;
        } Status;
    } RX_ADAPT_MONITOR1_BC_RO;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned RX_SUM_TAP1_LMS : 6;
            unsigned RX_SUM_TAP2_LMS : 6;
            unsigned RX_SUM_TAP3_LMS : 6;
            unsigned RX_SUM_TAP4_LMS : 6;
            unsigned reserved : 8;
        } Status;
    } RX_ADAPT_MONITOR2_E4_RO;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned RX_RST : 1;
            unsigned RX_INVERT : 1;
            unsigned RX_SWAP : 2;
            unsigned RX_SWAP_ERR : 2;
            unsigned RX_INVERT_ERR : 1;
            unsigned reserved : 4;
            unsigned RSTZ_EOM : 1;
            uint16_t reserved1;
            unsigned reserved2 :4;
        } Status;
    } RX_PHY_CTRL0_50_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;
    } RX_EOM_ACCUM_INTERVAL_54_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned EOM_THR_INIT : 6;
            unsigned reserved : 2;
            unsigned EOM_PHOS_INIT : 7;
            unsigned reserved1 :1;
            uint16_t reserved2;
        } Status;
    } RX_EOM_START_VAL_58_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned EOM_EN : 1;
            unsigned EOM_START : 1;
            unsigned EOM_MODE : 3;
            unsigned reserved :2;
            unsigned EQ_ADAPT_STOP : 1;
            uint16_t reserved1;
            uint8_t reserved2;
        } Status;
    } RX_EOM_CTRL_B8_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned reserved : 5;
            unsigned EOM_DONE : 1;
            unsigned EOM_THRMAX : 6;
            unsigned EOM_THRMIN : 6;
            unsigned EOM_PHOSMAX : 7;
            unsigned EOM_PHOSMIN : 7;
        } Status;
    } RX_PHYD_EOM_DAT_C8_RO;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
        //    unsigned EOM_EYELIM : 17; // TODO - make structure
            unsigned EOM_HSTEP : 3;
            unsigned EOM_VSTEP : 3;
            unsigned EOM_OVR : 1;
            unsigned ERDE_EN_OVR : 1;
            unsigned ERDE_RSLT_CLR_OVR : 1;
            unsigned reserved : 6;
        } Status;
    } RX_EOM_CONFIG_D4_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned TX_SOFT_RESET : 1;
            unsigned reserved : 7;
            unsigned TX_INVERT : 1;
            unsigned reserved1 : 1;
            unsigned FIFO_AUTO_RESET_EN : 1;
            unsigned reserved2 : 5;
            uint16_t reserved3;
        } Status;
    } TX_PHY_CTRL0_40_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned reserved : 3;
            unsigned TXGC_GAIN_USBGEN1 : 4;
            unsigned reserved1 : 1;
            uint8_t reserved2;
            uint16_t reserved3;
        } Status;
    } TX_GC_CTRL1_4C_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned TX_VML_SWING_EN : 5;
            unsigned reserved : 3;
            unsigned TX_VML_MAIN_EN : 5;
            unsigned reserved1 : 3;
            unsigned TX_VML_PRE_EN : 3;
            unsigned reserved2 : 1;
            unsigned TX_VML_POST_EN : 4;
            unsigned TX_VML_PRE : 3;
            unsigned reserved3 : 1;
            unsigned TX_VML_POST : 4;
        } Status;
    } TX_CONF_WINDOW_0_54_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned TX_P2S_OUTP_HIGH_EN : 1;
            unsigned TX_P2S_OUTN_HIGH_EN : 1;
            unsigned TX_P2S_TEST_EN : 1;
            unsigned TX_P2S_ACTIVE_EN : 1;
            unsigned TX_P2S_PRE_PD : 1;
            unsigned TX_P2S_POST_PD : 1;
            unsigned TX_DYNAMIC_COMP_EN : 1;
            unsigned reserved : 1;
            unsigned TX_DYNAMIC_COMP_TUNE : 5;
            unsigned TX_DLDO_TUNE : 3;
            unsigned TX_CAL_SWING_MODE : 2;
            unsigned TX_CLK_SEL : 2;
            unsigned TX_CLK_TUNE : 5;
            unsigned reserved1 : 3;
            unsigned TX_PI_CLOAD : 3;
            unsigned reserved2 : 1;
        } Status;
    } TX_CONF_WINDOW_1_58_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned CONF_SEL : 5;
            unsigned reserved : 2;
            unsigned RD_WR_SEL : 1;
            uint8_t reserved1;
            uint16_t reserved2;
        } Status;
    } TX_CONF_CTRL_5C_RW;
    
    typedef union {

        struct {
            uint32_t data;
        } RegisterData;

        struct {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        } Bytes;

        struct {
            unsigned TX_VML_SWING_EN : 5;
            unsigned reserved : 3;
            unsigned TX_VML_MAIN_EN : 5;
            unsigned reserved1 : 3;
            unsigned TX_VML_PRE_EN : 3;
            unsigned reserved2 : 1;
            unsigned TX_VML_POST_EN : 4;
            unsigned TX_VML_PRE : 3;
            unsigned reserved3 : 1;
            unsigned TX_VML_POST : 4;
        } Status;
    } TX_CONF_MONITOR_0_60_RW;
	
	typedef union {
		 struct {
			 uint32_t data;
		 } RegisterData;

		 struct {
			 uint8_t byte0;
			 uint8_t byte1;
			 uint8_t byte2;
			 uint8_t byte3;
		 } Bytes;
		
	} TX_CONF_MONITOR_1_64_RW;
        
    // end structures  
    
    // prototypes
    void MCDP6200_Init(void);
    i2c_error_t MCDP6200_Write_Register_Data(uint8_t address, uint16_t _register, uint32_t *data);
    void MCDP6200_SetTxConfigWindow(uint8_t retimerAddress, uint16_t _txRegister, uint8_t rdWr, uint8_t gen);
	
	
#ifdef	__cplusplus
}
#endif

#endif	/* MCDP6200_H */

