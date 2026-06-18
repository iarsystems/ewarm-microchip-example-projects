/*******************************************************************************
 * (c) Copyright 2006 Actel Corporation
 */
#ifndef __COREAI_REGISTERS_H
#define __COREAI_REGISTERS_H	1

/*------------------------------------------------------------------------------
 * ACM_CTRL_STATUS register details
 */
#define ACM_CTRL_STATUS_REG_OFFSET	0x00

/*
 * ACMRESET bits.
 */
#define ACMRESET_OFFSET   0x00
#define ACMRESET_MASK     0x0001
#define ACMRESET_SHIFT    0

/*
 * ACMRDSTART bits.
 */
#define ACMRDSTART_OFFSET   0x00
#define ACMRDSTART_MASK     0x0002
#define ACMRDSTART_SHIFT    1

/*
 * ACMRESETBUSY bits.
 */
#define ACMRESETBUSY_OFFSET   0x00
#define ACMRESETBUSY_MASK     0x0004
#define ACMRESETBUSY_SHIFT    2

/*
 * ACMRDBUSY bits.
 */
#define ACMRDBUSY_OFFSET   0x00
#define ACMRDBUSY_MASK     0x0008
#define ACMRDBUSY_SHIFT    3

/*
 * ACMWRBUSY bits.
 */
#define ACMWRBUSY_OFFSET   0x00
#define ACMWRBUSY_MASK     0x0010
#define ACMWRBUSY_SHIFT    4

/*------------------------------------------------------------------------------
 * ACM_ADDR register details
 */
#define ACM_ADDR_REG_OFFSET	0x04

/*
 * ACMADDR bits.
 */
#define ACMADDR_OFFSET   0x04
#define ACMADDR_MASK     0x00FF
#define ACMADDR_SHIFT    0

/*------------------------------------------------------------------------------
 * ACM_DATA register details
 */
#define ACM_DATA_REG_OFFSET	0x08

/*
 * ACMDATA bits.
 */
#define ACMDATA_OFFSET   0x08
#define ACMDATA_MASK     0xFF
#define ACMDATA_SHIFT    0

/*------------------------------------------------------------------------------
 * ADC_CTRL_1 register details
 */
#define ADC_CTRL_1_REG_OFFSET	0x0C

/*
 * MODE bits.
 */
#define MODE_OFFSET   0x0C
#define MODE_MASK     0x07
#define MODE_SHIFT    0

/*
 * VAREFSEL bits.
 */
#define VAREFSEL_OFFSET   0x0C
#define VAREFSEL_MASK     0x10
#define VAREFSEL_SHIFT    4

/*
 * PWRDN bits.
 */
#define PWRDN_OFFSET   0x0C
#define PWRDN_MASK     0x20
#define PWRDN_SHIFT    5

/*
 * ADCRESET bits.
 */
#define ADCRESET_OFFSET   0x0C
#define ADCRESET_MASK     0x40
#define ADCRESET_SHIFT    6

/*
 * TVC bits.
 */
#define TVC_OFFSET   0x0C
#define TVC_MASK     0xFF00U
#define TVC_SHIFT    8

/*------------------------------------------------------------------------------
 * ADC_CTRL_2 register details
 */
#define ADC_CTRL_2_REG_OFFSET	0x10

/*
 * STC bits.
 */
#define STC_OFFSET   0x10
#define STC_MASK     0x00FF
#define STC_SHIFT    0

/*
 * CHNUMBER bits.
 */
#define CHNUMBER_OFFSET   0x10
#define CHNUMBER_MASK     0x1F00
#define CHNUMBER_SHIFT    8

/*
 * ADCSTART bits.
 */
#define ADCSTART_OFFSET   0x10
#define ADCSTART_MASK     0x2000
#define ADCSTART_SHIFT    13

/*------------------------------------------------------------------------------
 * ADC_CTRL_3 register details
 */
#define ADC_CTRL_3_REG_OFFSET	0x14

/*
 * CMSTB bits.
 */
#define CMSTB_OFFSET   0x14
#define CMSTB_MASK     0x03FF
#define CMSTB_SHIFT    0

/*------------------------------------------------------------------------------
 * ADC_CTRL_4 register details
 */
#define ADC_CTRL_4_REG_OFFSET	0x18

/*
 * TMSTB bits.
 */
#define TMSTB_OFFSET   0x18
#define TMSTB_MASK     0x03FF
#define TMSTB_SHIFT    0

/*
 * TMSTBINT bits.
 */
#define TMSTBINT_OFFSET   0x18
#define TMSTBINT_MASK     0x0400
#define TMSTBINT_SHIFT    10

/*------------------------------------------------------------------------------
 * ADC_CTRL_5 register details
 */
#define ADC_CTRL_5_REG_OFFSET	0x1C

/*
 * GDON bits.
 */
#define GDON_OFFSET   0x1C
#define GDON_MASK     0x03FF
#define GDON_SHIFT    0

/*------------------------------------------------------------------------------
 * ADC_STATUS register details
 */
#define ADC_STATUS_REG_OFFSET	0x20

/*
 * RESULT bits.
 */
#define RESULT_OFFSET   0x20
#define RESULT_MASK     0x0FFF
#define RESULT_SHIFT    0

/*
 * DATAVALID bits.
 */
#define DATAVALID_OFFSET   0x20
#define DATAVALID_MASK     0x1000
#define DATAVALID_SHIFT    12

/*
 * BUSY bits.
 */
#define BUSY_OFFSET   0x20
#define BUSY_MASK     0x2000
#define BUSY_SHIFT    13

/*
 * SAMPLE bits.
 */
#define SAMPLE_OFFSET   0x20
#define SAMPLE_MASK     0x4000
#define SAMPLE_SHIFT    14

/*
 * CALIBRATE bits.
 */
#define CALIBRATE_OFFSET   0x20
#define CALIBRATE_MASK     0x8000U
#define CALIBRATE_SHIFT    15

/*------------------------------------------------------------------------------
 * READ_FIFO_DATA_OUTPUT register details
 */
#define READ_FIFO_DATA_OUTPUT_REG_OFFSET	0x24

/*
 * DOUT bits.
 */
#define DOUT_OFFSET   0x24
#define DOUT_MASK     0x0FFF
#define DOUT_SHIFT    0

/*------------------------------------------------------------------------------
 * READ_FIFO_STATUS register details
 */
#define READ_FIFO_STATUS_REG_OFFSET	0x28

/*
 * FULL bits.
 */
#define FULL_OFFSET   0x28
#define FULL_MASK     0x0001
#define FULL_SHIFT    0

/*
 * AFULL bits.
 */
#define AFULL_OFFSET   0x28
#define AFULL_MASK     0x0002
#define AFULL_SHIFT    1

/*
 * EMPTY bits.
 */
#define EMPTY_OFFSET   0x28
#define EMPTY_MASK     0x0004
#define EMPTY_SHIFT    2

/*
 * AEMPTY bits.
 */
#define AEMPTY_OFFSET   0x28
#define AEMPTY_MASK     0x0008
#define AEMPTY_SHIFT    3

/*------------------------------------------------------------------------------
 * IRQ_ENABLE register details
 */
#define IRQ_ENABLE_REG_OFFSET	0x2C

/*
 * INTEN bits.
 */
#define INTEN_OFFSET   0x2C
#define INTEN_MASK     0xFFFF
#define INTEN_SHIFT    0

/*------------------------------------------------------------------------------
 * IRQ_STATUS register details
 */
#define IRQ_STATUS_REG_OFFSET	0x30

/*
 * INT0 bits.
 */
#define INT0_OFFSET   0x30
#define INT0_MASK     0x0001
#define INT0_SHIFT    0

/*
 * INT1 bits.
 */
#define INT1_OFFSET   0x30
#define INT1_MASK     0x0002
#define INT1_SHIFT    1

/*
 * INT2 bits.
 */
#define INT2_OFFSET   0x30
#define INT2_MASK     0x0004
#define INT2_SHIFT    2

/*
 * INT3 bits.
 */
#define INT3_OFFSET   0x30
#define INT3_MASK     0x0008
#define INT3_SHIFT    3

/*
 * INT4 bits.
 */
#define INT4_OFFSET   0x30
#define INT4_MASK     0x0010
#define INT4_SHIFT    4

/*
 * INT5 bits.
 */
#define INT5_OFFSET   0x30
#define INT5_MASK     0x0020
#define INT5_SHIFT    5

/*
 * INT6 bits.
 */
#define INT6_OFFSET   0x30
#define INT6_MASK     0x0100
#define INT6_SHIFT    8

/*
 * INT7 bits.
 */
#define INT7_OFFSET   0x30
#define INT7_MASK     0x0200
#define INT7_SHIFT    9

/*
 * INT8 bits.
 */
#define INT8_OFFSET   0x30
#define INT8_MASK     0x0400
#define INT8_SHIFT    10

/*
 * INT9 bits.
 */
#define INT9_OFFSET   0x30
#define INT9_MASK     0x0800
#define INT9_SHIFT    11

/*
 * INT10 bits.
 */
#define INT10_OFFSET   0x30
#define INT10_MASK     0x1000
#define INT10_SHIFT    12

/*
 * INT11 bits.
 */
#define INT11_OFFSET   0x30
#define INT11_MASK     0x2000
#define INT11_SHIFT    13

/*
 * INT12 bits.
 */
#define INT12_OFFSET   0x30
#define INT12_MASK     0x4000
#define INT12_SHIFT    14

/*
 * INT13 bits.
 */
#define INT13_OFFSET   0x30
#define INT13_MASK     0x7FFF
#define INT13_SHIFT    0

#endif	/* __COREAI_REGISTERS_H */

