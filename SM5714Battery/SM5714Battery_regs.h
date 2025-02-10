/*
 * SM5714Battery_regs.h
 *
 * Header of Siliconmitus SM5714 PMIC Fuel Gauge registers
 */

#ifndef SM5714BATTERY_REGS
#define SM5714BATTERY_REGS

// Register definitions
#define SM5714_FG_REG_DEVICE_ID           0x00 // Read device ID
#define SM5714_FG_REG_CTRL				  0x01 // Control register
#define SM5714_FG_REG_INTFG               0x02
#define SM5714_FG_REG_STATUS              0x03
#define SM5714_FG_REG_INTFG_MASK          0x04

#define SM5714_FG_REG_SRAM_PROT		      0x8B
#define SM5714_FG_REG_SRAM_RADDR		  0x8C // Register for holding SRAM read address
#define SM5714_FG_REG_SRAM_RDATA		  0x8D // Read address for getting back data
#define SM5714_FG_REG_SRAM_WADDR		  0x8E
#define SM5714_FG_REG_SRAM_WDATA		  0x8F

// SRAM Addresses as static constants instead of macros
// NOT top level I2C registers. They're sub-addresses inside PMIC's SRAM region.
static USHORT SM5714_FG_ADDR_SRAM_SOC         = 0x00; // State of Charge
static USHORT SM5714_FG_ADDR_SRAM_OCV	        = 0x01; // Voltage now
static USHORT SM5714_FG_ADDR_SRAM_VBAT		= 0x03;
static USHORT SM5714_FG_ADDR_SRAM_VSYS		= 0x04;
static USHORT SM5714_FG_ADDR_SRAM_CURRENT	    = 0x05; // Current now
static USHORT SM5714_FG_ADDR_SRAM_TEMPERATURE	= 0x07; // Temperature
static USHORT SM5714_FG_ADDR_SRAM_VBAT_AVG	= 0x08;
static USHORT SM5714_FG_ADDR_SRAM_CURRENT_AVG = 0x09;
static USHORT SM5714_FG_ADDR_SRAM_STATE       = 0x15;
static USHORT SM5714_FG_ADDR_SRAM_SOC_CYCLE	= 0x87; // Cycle count

#endif // SM5714BATTERY_REGS

