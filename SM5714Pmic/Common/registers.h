#ifndef __SM5714_H__
#define __SM5714_H__

//
// Charger Register definitions
//
enum chg_status_regs {
    SM5714_CHG_REG_STATUS1      = 0x0D,
    SM5714_CHG_REG_STATUS2      = 0x0E,
    SM5714_CHG_REG_STATUS3      = 0x0F,
    SM5714_CHG_REG_STATUS4      = 0x10,
    SM5714_CHG_REG_STATUS5      = 0x11,
};

enum chg_cntl_regs {
    SM5714_CHG_REG_CNTL1 = 0x13,
    SM5714_CHG_REG_VBUSCNTL = 0x15,
    SM5714_CHG_REG_CHGCNTL2 = 0x18,
    SM5714_CHG_REG_CHGCNTL4 = 0x1A,
    SM5714_CHG_REG_CHGCNTL5 = 0x1B,
};

//
// Charger Register definitions
//

enum typec_pdic_rid {
	REG_RID_UNDF = 0x00,
	REG_RID_255K = 0x03,
	REG_RID_301K = 0x04,
	REG_RID_523K = 0x05,
	REG_RID_619K = 0x06,
	REG_RID_OPEN = 0x07,
	REG_RID_MAX = 0x08,
};

enum typec_usbpd_reg {
	SM5714_REG_INT1 = 0x01,
	SM5714_REG_INT2 = 0x02,
	SM5714_REG_INT3 = 0x03,
	SM5714_REG_INT4 = 0x04,
	SM5714_REG_INT5 = 0x05,
	SM5714_REG_INT_MASK1 = 0x06,
	SM5714_REG_INT_MASK2 = 0x07,
	SM5714_REG_INT_MASK3 = 0x08,
	SM5714_REG_INT_MASK4 = 0x09,
	SM5714_REG_INT_MASK5 = 0x0A,
	SM5714_REG_STATUS1 = 0x0B,
	SM5714_REG_STATUS2 = 0x0C,
	SM5714_REG_STATUS3 = 0x0D,
	SM5714_REG_STATUS4 = 0x0E,
	SM5714_REG_STATUS5 = 0x0F,
	SM5714_REG_JIGON_CONTROL = 0x17,
	SM5714_REG_FACTORY = 0x18,
	SM5714_REG_ADC_CNTL1 = 0x19,
	SM5714_REG_ADC_CNTL2 = 0x1A,
	SM5714_REG_SYS_CNTL = 0x1B,
	SM5714_REG_COMP_CNTL = 0x1C,
	SM5714_REG_CLK_CNTL = 0x1D,
	SM5714_REG_USBK_CNTL = 0x1E,
	SM5714_REG_CORR_CNTL1 = 0x20,
	SM5714_REG_CORR_CNTL4 = 0x23,
	SM5714_REG_CORR_CNTL5 = 0x24,
	SM5714_REG_CORR_CNTL6 = 0x25,
	SM5714_REG_CC_STATUS = 0x28,
	SM5714_REG_CC_CNTL1 = 0x29,
	SM5714_REG_CC_CNTL2 = 0x2A,
	SM5714_REG_CC_CNTL3 = 0x2B,
	SM5714_REG_CC_CNTL4 = 0x2C,
	SM5714_REG_CC_CNTL5 = 0x2D,
	SM5714_REG_CC_CNTL6 = 0x2E,
	SM5714_REG_CC_CNTL7 = 0x2F,
	SM5714_REG_CABLE_POL_SEL = 0x33,
	SM5714_REG_GEN_TMR_L = 0x35,
	SM5714_REG_GEN_TMR_U = 0x36,
	SM5714_REG_PD_CNTL1 = 0x38,
	SM5714_REG_PD_CNTL2 = 0x39,
	SM5714_REG_PD_CNTL4 = 0x3B,
	SM5714_REG_PD_CNTL5 = 0x3C,
	SM5714_REG_RX_SRC = 0x41,
	SM5714_REG_RX_HEADER_00 = 0x42,
	SM5714_REG_RX_HEADER_01 = 0x43,
	SM5714_REG_RX_PAYLOAD = 0x44,
	SM5714_REG_RX_BUF = 0x5E,
	SM5714_REG_RX_BUF_ST = 0x5F,
	SM5714_REG_TX_HEADER_00 = 0x60,
	SM5714_REG_TX_HEADER_01 = 0x61,
	SM5714_REG_TX_PAYLOAD = 0x62,
	SM5714_REG_TX_BUF_CTRL = 0x63,
	SM5714_REG_TX_REQ = 0x7E,
	SM5714_REG_BC12_DEV_TYPE = 0x88,
	SM5714_REG_TA_STATUS = 0x89,
	SM5714_REG_CORR_CNTL9 = 0x92,
	SM5714_REG_CORR_TH3 = 0xA4,
	SM5714_REG_CORR_TH6 = 0xA7,
	SM5714_REG_CORR_TH7 = 0xA8,
	SM5714_REG_CORR_OPT4 = 0xC8,
	SM5714_REG_PROBE0 = 0xD0,
	SM5714_REG_PD_STATE0 = 0xD5,
	SM5714_REG_PD_STATE1 = 0xD6,
	SM5714_REG_PD_STATE2 = 0xD7,
	SM5714_REG_PD_STATE3 = 0xD8,
	SM5714_REG_PD_STATE4 = 0xD9,
	SM5714_REG_PD_STATE5 = 0xDA
};

#endif