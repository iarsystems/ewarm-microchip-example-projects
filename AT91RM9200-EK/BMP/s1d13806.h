//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : s1d13806.h
//* Object              : Video Graphic Display Controller Prototyping File.
//*
//* 1.0 09/05/02 ED     : Creation
//* 1.2 13/01/03 FB		: Update on lib V3
//*----------------------------------------------------------------------------

#ifndef s1d13806_h
#define s1d13806_h

#include	"bmp.h"

#define S1D13806_BASE			      ((unsigned char *) BASE_EBI_CS3_ADDRESS )
#define S1D13806_DISP_MEM_BASE	(((int)S1D13806_BASE)  + ((int) 0x200000))

//////////////////////////////////////////////////////////////////////////////
// S1D13806 User Interface Structure Definition
//////////////////////////////////////////////////////////////////////////////
typedef struct	_AT91S_S1D13806_OnChipReg
{
	char	OCR_REV ;			  /* Revision Code Register */
	char	OCR_MISC ;			/* Miscellaneous Register */
	char	Reserved0[2] ;	/* Reserved */
	short	OCR_IOCONF ;		/* GPIO Configuration Register */
	char	Reserved1[2] ;	/* Reserved */
	short	OCR_IOCR ;			/* GPIO Control Register */
	char	Reserved2[2] ;	/* Reserved */
	char	OCR_SR ;			  /* Status Register */
	char	Reserved3[3] ;	/* Reserved */
	char	OCR_MCLK ;			/* Memory Clock Configuration Register */
	char	Reserved4[3] ;	/* Reserved */
	char	OCR_LCDCLK ;		/* LCD Pixel Clock Configuration Register */
	char	Reserved5[3] ;	/* Reserved */
	char	OCR_CRTPCLK ;		/* CRT/TV Pixel Clock Configuration Register */
	char	Reserved6[3] ;	/* Reserved */
	char	OCR_MPCLK ;			/* MediaPlug Clock Configuration Register */
	char	Reserved7 ;			/* Reserved */
	char	OCR_NWS ;			  /* CPU to Memory Wait States Configuration Register */
	char	Reserved8 ;			/* Reserved */
	char	OCR_MEM ;			  /* Memory Configuration Register */
	char	OCR_REFC ;			/* SDRAM Refresh Counter Register */
	char	Reserved9[8] ;	/* Reserved */
	short	OCR_SDRAM ;			/* SDRAM Configuration Register */
	char	Reserved10[4] ;	/* Reserved */
	char	OCR_PTYPE ;			/* Panel Type Configuration Register */
	char	OCR_MOD ;			  /* MOD Rate Configuration Register */
	char	OCR_LCDHDW ;		/* LCD Horizontal Display Width Configuration Register */
	char	Reserved11 ;		/* Reserved */
	char	OCR_LCDHNDP ;		/* LCD Horizontal Non-Display Period Configuration Register */
	char	OCR_TFTFPSP ;		/* TFT FPLINE Start Position Configuration Register */
	char	OCR_TFTFPPW ;		/* TFT FPLINE Pulse Width Configuration Register */
	char	Reserved12 ;		/* Reserved */
	short	OCR_LCDVDH ;		/* LCD Vertical Display Height Configuration Register */
	char	OCR_LCDVNDP ;		/* LCD Vertical Non-Display Period Configuration Register */
	char	OCR_LCDFPSP ;		/* LCD FPFRAME Start Position Configuration Register */
	char	OCR_LCDFPPW ;		/* LCD FPFRAME Pulse Width Configuration Register */
	char	Reserved13 ;		/* Reserved */
	char	OCR_LCDLC ;			/* LCD Line Count Configuration Register */
	char	Reserved14 ;		/* Reserved */
	char	OCR_LCDDM ;			/* LCD Display Mode Configuration Register */
	char	OCR_LCDM ;			/* LCD Miscellaneous Configuration Register */
	char	OCR_LCDDBA_0 ;	/* LCD Display Start Address Configuration Register 0 */
	char	OCR_LCDDBA_1 ;	/* LCD Display Start Address Configuration Register 1 */
	char	OCR_LCDDBA_2 ;	/* LCD Display Start Address Configuration Register 2 */
	char	Reserved15 ;		/* Reserved */
	short	OCR_LCDMAO ;		/* LCD Memory Address Offset Configuration Register */
	char	OCR_LCDPP ;			/* LCD Pixel Planning Configuration Register */
	char	Reserved16 ;		/* Reserved */
	char	OCR_LCDDFHTC ;	/* LCD Display FIFO High Threshold Control Register */
	char	OCR_LCDDFLTC ;	/* LCD Display FIFO Low Threshold Control Register */
	char	Reserved17[4] ;	/* Reserved */
	char	OCR_CRTHDW ;		/* CRT/TV Horizontal Display Width Configuration Register */
	char	Reserved18 ;		/* Reserved */
	char	OCR_CRTHNDP ;		/* CRT/TV Horizontal Non-Display Period Configuration Register */
	char	OCR_CRTHSP ;		/* CRT/TV HRTC Start Position Configuration Register */
	char	OCR_CRTHPW ;		/* CRT/TV HRTC Pulse Width Configuration Register */
	char	Reserved19 ;		/* Reserved */
	short	OCR_CRTVDH ;		/* CRT/TV Vertical Display Height Configuration Register */
	char	OCR_CRTVNDP ;		/* CRT/TV Vertical Non-Display Period Configuration Register */
	char	OCR_CRTVSP ;		/* CRT/TV VRTC Start Position Configuration Register */
	char	OCR_CRTVPW ;		/* CRT/TV VRTC Pulse Width Configuration Register */
	char	OCR_TVOC ;			/* TV Output Control Register */
	char	Reserved20[2] ;	/* Reserved */
	char	OCR_CRTLC ;			/* CRT/TV Line Count Configuration Register */
	char	Reserved21 ;		/* Reserved */
	char	OCR_CRTDM ;			/* CRT/TV Display Mode Configuration Register */
	char	Reserved22 ;		/* Reserved */
	char	OCR_CRTDBA_0 ;	/* CRT/TV Display Start Address Configuration Register 0 */
	char	OCR_CRTDBA_1 ;	/* CRT/TV Display Start Address Configuration Register 1 */
	char	OCR_CRTDBA_2 ;	/* CRT/TV Display Start Address Configuration Register 2 */
	char	Reserved23 ;		/* Reserved */
	short	OCR_CRTMAO ;		/* CRT/TV Memory Address Offset Configuration Register */
	char	OCR_CRTPP ;			/* CRT/TV Pixel Planning Configuration Register */
	char	Reserved24 ;		/* Reserved */
	char	OCR_CRTDFHTC ;	/* CRT/TV Display FIFO High Threshold Control Register */
	char	OCR_CRTDFLTC ;	/* CRT/TV Display FIFO Low Threshold Control Register */
	char	Reserved25[4] ;	/* Reserved */
	char	OCR_LCDICC ;		/* LCD Ink/Cursor Control Register */
	char	OCR_LCDICBA ;		/* LCD Ink/Cursor Start Address Register */
	short	OCR_LCDCXP ;		/* LCD Cursor X Position Register */
	short	OCR_LCDCYP ;		/* LCD Cursor Y Position Register */
	char	OCR_LCDICB_0 ;	/* LCD Ink/Cursor Blue Color Configuration Register 0 */
	char	OCR_LCDICG_0 ;	/* LCD Ink/Cursor Green Color Configuration Register 0 */
	char	OCR_LCDICR_0 ;	/* LCD Ink/Cursor Red Color Configuration Register 0 */
	char	Reserved26 ;		/* Reserved */
	char	OCR_LCDICB_1 ;	/* LCD Ink/Cursor Blue Color Configuration Register 1 */
	char	OCR_LCDICG_1 ;	/* LCD Ink/Cursor Green Color Configuration Register 1 */
	char	OCR_LCDICR_1 ;	/* LCD Ink/Cursor Red Color Configuration Register 1 */
	char	Reserved27 ;		/* Reserved */
	char	OCR_LCDICFT ;		/* LCD Ink/Cursor FIFO Threshold Configuration Register */
	char	Reserved28 ;		/* Reserved */
	char	OCR_CRTICC ;		/* CRT/TV Ink/Cursor Control Register */
	char	OCR_CRTICBA ;		/* CRT/TV Ink/Cursor Start Address Register */
	short	OCR_CRTCXP ;		/* CRT/TV Cursor X Position Register */
	short	OCR_CRTCYP ;		/* CRT/TV Cursor Y Position Register */
	char	OCR_CRTICB_0 ;	/* CRT/TV Ink/Cursor Blue Color Configuration Register 0 */
	char	OCR_CRTICG_0 ;	/* CRT/TV Ink/Cursor Green Color Configuration Register 0 */
	char	OCR_CRTICR_0 ;	/* CRT/TV Ink/Cursor Red Color Configuration Register 0 */
	char	Reserved29 ;		/* Reserved */
	char	OCR_CRTICB_1 ;	/* CRT/TV Ink/Cursor Blue Color Configuration Register 1 */
	char	OCR_CRTICG_1 ;	/* CRT/TV Ink/Cursor Green Color Configuration Register 1 */
	char	OCR_CRTICR_1 ;	/* CRT/TV Ink/Cursor Red Color Configuration Register 1 */
	char	Reserved30 ;		/* Reserved */
	char	OCR_CRTICFT ;		/* CRT/TV Ink/Cursor FIFO Threshold Configuration Register */
	char	Reserved31[113] ;	/* Reserved */
	short	OCR_BBLTC ;			/* BitBLT Control Register */
	char	OCR_BBLTRCCE ;	/* BitBLT ROP Code/Color Expansion Register */
	char	OCR_BBLTO ;			/* BitBLT Operation Register */
	char	OCR_BBLTSSA_0 ;	/* BitBLT Source Start Address Register 0 */
	char	OCR_BBLTSSA_1 ;	/* BitBLT Source Start Address Register 1 */
	char	OCR_BBLTSSA_2 ;	/* BitBLT Source Start Address Register 2 */
	char	Reserved32 ;		/* Reserved */
	char	OCR_BBLTDSA_0 ;	/* BitBLT Destination Start Address Register 0 */
	char	OCR_BBLTDSA_1 ;	/* BitBLT Destination Start Address Register 1 */
	char	OCR_BBLTDSA_2 ;	/* BitBLT Destination Start Address Register 2 */
	char	Reserved33 ;		/* Reserved */
	short	OCR_BBLTMAO ;		/* BitBLT Memory Address Offset Register */
	char	Reserved34[2] ;	/* Reserved */
	short	OCR_BBLTW ;			/* BitBLT Width Register */
	short	OCR_BBLTH ;			/* BitBLT Height Register */
	short	OCR_BBLTBC ;		/* BitBLT Background Color Register */
	char	Reserved35[2] ;	/* Reserved */
	short	OCR_BBLTFC ;		/* BitBLT Foreground Color Register */
	char	Reserved36[198] ;	/* Reserved */
	char	OCR_LUTM ;			/* Look-Up Table Mode Register */
	char	Reserved37 ;		/* Reserved */
	char	OCR_LUTBA ;			/* Look-Up Table Base Address Register */
	char	Reserved38 ;		/* Reserved */
	char	OCR_LUTD ;			/* Look-Up Table Data Register */
	char	Reserved39[11] ;	/* Reserved */
	char	OCR_PSC ;			  /* Power Save Configuration Register */
	char	OCR_PSS ;			  /* Power Save Status Register */
	char	Reserved40[2] ;	/* Reserved */
	char	OCR_WDT ;			  /* CPU to Memory Access Watchdog Timer Configuration Register */
	char	Reserved41[7] ;	/* Reserved */
	char	OCR_DM ;			  /* Display Mode Configuration Register */
	char	Reserved42[3] ;	/* Reserved */
} AT91S_S1D13806_OnChipReg, *AT91PS_S1D13806_OnChipReg;
//////////////////////////////////////////////////////////////////////////////

typedef struct	_AT91S_S1D13806_MediaPlug
{
	short	MP_LCMD ;			/* MediaPlug LCMD Configuration Register */
	short	MP_RLCMD ;			/* MediaPlug Reserved LCMD Configuration Register */
	short	MP_CMD ;			/* MediaPlug Command Configuration Register */
	short	MP_RCMD ;			/* MediaPlug Reserved Command Configuration Register */
	short	MP_DBA ;			/* MediaPlug Data Base Address */
} AT91S_S1D13806_MediaPlug, *AT91PS_S1D13806_MediaPlug;
//////////////////////////////////////////////////////////////////////////////

typedef struct	_AT91S_S1D13806_BitBLTReg
{
	short	BBR_DBA ;			/* BitBLT Data Base Address */
}AT91S_S1D13806_BitBLTReg, *AT91PS_S1D13806_BitBLTReg;
//////////////////////////////////////////////////////////////////////////////

typedef struct	_AT91S_S1D13806_Desc
{
    AT91S_S1D13806_OnChipReg	S1D13806_OnChipReg ;			/* On-Chip Configuration Registers */
    int							Reserved1[(0x1000-(0x0 + sizeof ( AT91S_S1D13806_OnChipReg )))/4] ;	/* Reserved */
    AT91S_S1D13806_MediaPlug	S1D13806_MediaPlug ;			/* MediaPlug Configuration Registers */
    int							Reserved2[(0x100000-(0x1000 + sizeof ( AT91S_S1D13806_MediaPlug )))/4] ;	/* Reserved */
    AT91S_S1D13806_BitBLTReg	S1D13806_BitBLTReg ;			/* BitBLT Data Registers */
} AT91S_S1D13806_Desc, *AT91PS_S1D13806_Desc;
//////////////////////////////////////////////////////////////////////////////

/* Standard configurations definitions */
#define	CRT_Display_Conf_640_480		0x0
#define	CRT_Display_Conf_800_600		0x1

/* Standard Display Resolution definitions */
#define	Display_Def_0			          0x0		/* 640  x 480 */
#define	Display_Def_1			          0x1		/* 800  x 600 */

/* Standard Character Definition definitions */
#define Char_Def_0				          0x0		/* 8 x 16 */

/* Standard Character Definition definitions */
#define Lut_Def_0				            0x0		/* LUT8 */

/* Video Graphic Controller Display Memory Size definition */
#define S1D13806_DISP_MEM_SIZE	0x140000

/* Colors Word Masks */
#define BLUE_WORD_MASK			0x001F
#define GREEN_WORD_MASK			0x07E0
#define RED_WORD_MASK			  0xF800

/*-------------------------------------------------------*/
/* S1D13806 Revision Code Register Bits Definition       */
/*-------------------------------------------------------*/

#define		S1D_PRD_MASK			0xFC		/* Product Code Mask */
#define		S1D_PRD_SHIFT			0x02		/* Product Code Shift */

#define		S1D_REV_MASK			0x03		/* Revision Code Mask */

/*-------------------------------------------------------*/
/* S1D13806 Miscellaneous Register Bits Definition       */
/*-------------------------------------------------------*/

#define		S1D_RME					0x00		/* Register/Memory Access Enabled */
#define		S1D_RMD					0x80		/* Register/Memory Access Disabled */

/*--------------------------------------*/
/* S1D13806 GPIOs Bits Definition       */
/*--------------------------------------*/

#define		S1D_GPIO0				(1 << 0)	/* GPIO 0 */
#define		S1D_GPIO1				(1 << 1)	/* GPIO 1 */
#define		S1D_GPIO2				(1 << 2)	/* GPIO 2 */
#define		S1D_GPIO3				(1 << 3)	/* GPIO 3 */
#define		S1D_GPIO4				(1 << 4)	/* GPIO 4 */
#define		S1D_GPIO5				(1 << 5)	/* GPIO 5 */
#define		S1D_GPIO6				(1 << 6)	/* GPIO 6 */
#define		S1D_GPIO7				(1 << 7)	/* GPIO 7 */

#define		S1D_GPIO8				(1 << 0)	/* GPIO 8 */
#define		S1D_GPIO9				(1 << 1)	/* GPIO 9 */
#define		S1D_GPIO10			(1 << 2)	/* GPIO 10 */
#define		S1D_GPIO11			(1 << 3)	/* GPIO 11 */
#define		S1D_GPIO12			(1 << 4)	/* GPIO 12 */

/*-----------------------------------------------------------*/
/* S1D13806 Memory Clock Configuration Bits Definition       */
/*-----------------------------------------------------------*/

#define		S1D_MCK_DIV2			0x10		/* Memory Clock Divide by 2 Enable */

#define		S1D_CLKI3_SRC			0x02		/* Memory Clock Source is CLKI3 */

/*-------------------------------------------------------------------------------------------------*/
/* S1D13806 Memory, LCD Pixel, CRT/TV Pixel & MediaPlug Clocks Configuration Bits Definition       */
/*-------------------------------------------------------------------------------------------------*/

#define		S1D_CLKI_SRC			0x00		/* Clock Source is CLKI */
#define		S1D_BUSCLK_SRC		0x01		/* Clock Source is BUSCLK */
#define		S1D_CLKI2_SRC			0x02		/* Clock Source is CLKI2 */
#define		S1D_MCLK_SRC			0x03		/* Clock Source is MCLK */

#define		S1D_SRC_DIV1			0x00		/* Clock Source is not divided */
#define		S1D_SRC_DIV2			0x10		/* Clock Source is divided by 2 */
#define		S1D_SRC_DIV3			0x20		/* Clock Source is divided by 3 */
#define		S1D_SRC_DIV4			0x30		/* Clock Source is divided by 4 */

/*--------------------------------------------------*/
/* S1D13806 Wait State Select Bits Definition       */
/*--------------------------------------------------*/

#define		S1D_WS_0				0x00		/* Configuration 0 programmed */
#define		S1D_WS_1				0x01		/* Configuration 1 programmed */
#define		S1D_WS_2				0x02		/* Configuration 2 programmed */

/*-----------------------------------------------------*/
/* S1D13806 SDRAM Initialisation Bits Definition       */
/*-----------------------------------------------------*/

#define		S1D_SDRAM_INIT			0x80		/* SDRAM Initialisation Command */

/*-----------------------------------------------  -----*/
/* S1D13806 SDRAM Refresh Counter Bits Definition       */
/*-------------------------------------------  ---------*/

#define		S1D_RR_0				0x00		/* Configuration 0 programmed */
#define		S1D_RR_1				0x01		/* Configuration 1 programmed */
#define		S1D_RR_2				0x02		/* Configuration 2 programmed */
#define		S1D_RR_3				0x03		/* Configuration 3 programmed */

/*-----------------------------------------------------*/
/* S1D13806 SDRAM Timing Control Bits Definition       */
/*-----------------------------------------------------*/

#define		S1D_TC_0_0				0x00		/* Configuration 0 programmed register 0 */
#define		S1D_TC_0_1				0x01		/* Configuration 0 programmed register 1 */
#define		S1D_TC_1_0				0x00		/* Configuration 1 programmed register 0 */
#define		S1D_TC_1_1				0x12		/* Configuration 1 programmed register 1 */
#define		S1D_TC_2_0				0x11		/* Configuration 2 programmed register 0 */
#define		S1D_TC_2_1				0x13		/* Configuration 2 programmed register 1 */

/*-------------------------------------------*/
/* S1D13806 Panel Type Bits Definition       */
/*-------------------------------------------*/

#define		S1D_EL_PAN_EN			0x80		/* EL Panel Mode Enabled */
#define		S1D_TFT_DF				0x40		/* Switch TFT Standard (0) or 2X (1) Data Format */

#define		S1D_PAN_DW_0			0x00		/* Panel Data Width Configuration 0 programmed */
#define		S1D_PAN_DW_1			0x10		/* Panel Data Width Configuration 1 programmed */
#define		S1D_PAN_DW_2			0x20		/* Panel Data Width Configuration 2 programmed */

#define		S1D_PAN_DF				0x08		/* Switch Panel Data Format 1 (0) or 2 (1) */
#define		S1D_PAN_CM				0x04		/* Switch Panel monochrome (0) or color (1) */
#define		S1D_PAN_DS				0x02		/* Switch Panel single (0) or dual (1) */
#define		S1D_PAN_TL				0x01		/* Switch Panel LCD (0) or TFT (1) */

/*---------------------------------------------------*/
/* S1D13806 FPLINE & FPFRAME Polarity Definition     */
/*---------------------------------------------------*/

#define		S1D_FPPOL				0x80		/* Switch TFT active low and LCD active high (0) or
													TFT active high and LCD active low (1) */

/*--------------------------------------------------------------*/
/* S1D13806 Vertical Non-Display Period Status Definition       */
/*--------------------------------------------------------------*/

#define		S1D_VNDPS_MASK			0x80		/* Status Mask */

#define		S1D_VNDP				0x80		/* LCD Panel Vertical Non-Display Output */
#define		S1D_VDP					0x00		/* LCD Panel Output is in a Vertical Display Output */

/*----------------------------------------------------------*/
/* S1D13806 LCD & CRT/TV Display Mode Bits Definition       */
/*----------------------------------------------------------*/

#define		S1D_LCDDIS			0x80		/* Switch LCD Display Enabled (0) or Disabled (1) */
#define		S1D_LCDSWIVEL1	0x10		/* Switch LCD SwivelView Disabled (0) or Enabled (1) */

#define		S1D_4BPP				0x02		/* LCD & CRT/TV 4 Bit-per-pixel selection */
#define		S1D_8BPP				0x03		/* LCD & CRT/TV 8 Bit-per-pixel selection */
#define		S1D_16BPP				0x05		/* LCD & CRT/TV 16 Bit-per-pixel selection */

/*-----------------------------------------------------------*/
/* S1D13806 LCD Miscellaneous Register Bits Definition       */
/*-----------------------------------------------------------*/

#define		S1D_LCDDEN				0x02		/* Switch Dithering Enabled (0) or Disabled (1) */
#define		S1D_DPANEN				0x01		/* Switch Dual Panel Buffer Enabled (0) or Disabled (1) */

/*------------------------------------------------*/
/* S1D13806 HRTC & VRTC Polarity Definition       */
/*------------------------------------------------*/

#define		S1D_RTCPOL				0x80		/* Switch HRTC (or VRTC) pulse active low (0) or
													HRTC (or VRTC) pulse active high (1) */

/*-----------------------------------------------------------*/
/* S1D13806 TV Output Control Register Bits Definition       */
/*-----------------------------------------------------------*/

#define		S1D_CHROFILEN			0x20		/* Switch TV Chrominance Filter Disabled (0) or Enabled (1) */
#define		S1D_LUFILEN				0x10		/* Switch TV Luminance Filter Disabled (0) or Enabled (1) */
#define		S1D_DOLSEL				0x08		/* Select IREF reduced when set (1) */
#define		S1D_SVCSEL				0x02		/* Select Composite Output when reset (0), S-Video when set (1) */
#define		S1D_PALNTSEL			0x01		/* Select NTSC Format Output when reset (0), PAL Format Output when set (1) */

/*----------------------------------------------------*/
/* S1D13806 CRT/TV Display Mode Bits Definition       */
/*----------------------------------------------------*/

#define		S1D_CRTDIS				0x80		/* Switch CRT/TV Display Enabled (0) or Disabled (1) */

/*-------------------------------------------------------------------------*/
/* S1D13806 LCD & CRT/TV Ink/Cursor Control Register Bits Definition       */
/*-------------------------------------------------------------------------*/

#define		S1D_NOINKCUR		0x00		/* LCD & CRT/TV Ink/Cursor circuitry disabled */
#define		S1D_CUREN				0x01		/* LCD & CRT/TV Cursor Enabled */
#define		S1D_INKEN				0x02		/* LCD & CRT/TV Ink Enabled */

/*---------------------------------------------------------*/
/* S1D13806 BitBLT Control Registers Bits Definition       */
/*---------------------------------------------------------*/

#define		S1D_BBLTAS				0x80		/* 2D Operation initiated & busy */
#define		S1D_BBLTFNES			0x40		/* BitBLT FIFO Not Empty Status */
#define		S1D_BBLTHFS				0x20		/* BitBLT Half Full Status */
#define		S1D_BBLTFS				0x10		/* BitBLT FIFO Full Status */
#define		S1D_BBLTDLS				0x02		/* Destination BitBLT stored as a contiguous linear block of memory */
#define		S1D_BBLTSLS				0x01		/* Source BitBLT stored as a contiguous linear block of memory */

#define		S1D_BBLTCFS				0x01		/* Switch between 8 Bit-per-pixel 2D Operation Color Format (0) or
													16 Bit-per-pixel (1) */

/*------------------------------------------------------------*/
/* S1D13806 Look-Up Table Mode Register Bits Definition       */
/*------------------------------------------------------------*/

#define		S1D_LCDLUTRW_CRTLUTW	0x00	/* LCD LUT Read & Write, CRT/TV LUT Write */
#define		S1D_LCDLUTRW			0x01		  /* LCD LUT Read & Write */
#define		S1D_CRTLUTRW			0x02		  /* CRT/TV LUT Read & Write */

/*---------------------------------------------------------*/
/* S1D13806 Power Save Configuration Bits Definition       */
/*---------------------------------------------------------*/

#define		S1D_PSMD				0x10		/* Switch Power Save Mode Disabled (0) or Enabled (1) */
#define		S1D_PSME				0x11		/* Switch Power Save Mode Disabled (0) or Enabled (1) */

/*--------------------------------------------------*/
/* S1D13806 Power Save Status Bits Definition       */
/*--------------------------------------------------*/

#define		S1D_LCDPSS				0x02		/* Indicates LCD Panel Powered (0) or not (1) */
#define		S1D_MCPSS				0x01		/* Indicates Memory Controller Powered (0) or not (1) */

/*-------------------------------------------------------------*/
/* S1D13806 Common Display Mode Register Bits Definition       */
/*-------------------------------------------------------------*/

#define		S1D_LCDSWIVEL0			0x40		/* Switch LCD SwivelView Disabled (0) or Enabled (1) */

#define		S1D_NDM					0x00		/* No Display Mode */
#define		S1D_LCDOM				0x01		/* LCD Only Display Mode */
#define		S1D_CRTOM				0x02		/* CRT Only Display Mode */
#define		S1D_EISD0M			0x03		/* EISD (CRT & LCD) Display Mode */
#define		S1D_TVFFOFFM		0x04		/* TV with flicker filter off Display Mode */
#define		S1D_EISD1M			0x05		/* EISD (TV with flicker filter off & LCD) Display Mode */
#define		S1D_TVFFONM			0x06		/* TV with flicker filter on Display Mode */
#define		S1D_EISD2M			0x07		/* EISD (TV with flicker filter on & LCD) Display Mode */

//////////////////////////////////////////////////////////////////////////////
// Standard Descriptor Structure Definition
//////////////////////////////////////////////////////////////////////////////

typedef struct	_AT91S_S1D13806_DisplayDefDesc
{
	short	hdw ;			/* Horizontal Display Width */
	short	vdw ;			/* Vertical Display Width */
	short	pixel;			/* Pixel size */
} AT91S_S1D13806_DisplayDefDesc, *AT91PS_S1D13806_DisplayDefDesc;
//////////////////////////////////////////////////////////////////////////////

typedef struct	_AT91S_S1D13806_CharDefDesc
{
	int		hnbp ;			/* Horizontal Number of Pixels per Character */
	int		vnbp ;			/* Vertical Number of Pixels per Character */
} AT91S_S1D13806_CharDefDesc, *AT91PS_S1D13806_CharDefDesc;
//////////////////////////////////////////////////////////////////////////////

typedef struct	_AT91S_S1D13806_DataDesc
{
	int						Present_lcd_start_address ;		/* Current LCD Start Address */
	int						Present_crt_start_address ;		/* Current CRT Start Address */
	int						Present_nb_char_on_line ;		/* Current Line Number of Characters */
	char					*Present_display_mem_ptr ;		/* Current Display Memory Pointer */
} AT91S_S1D13806_DataDesc, *AT91PS_S1D13806_DataDesc;
//////////////////////////////////////////////////////////////////////////////

typedef	struct	_AT91S_S1D13806_RegDesc
{
	short	ioconf ;		/* GPIO Initialization */
	short	iocr ;			/* GPIO Initialization */
	char	mclk ;			/* Clock Initialization */
	char	lcdclk ;		/* Clock Initialization */
	char	crtpclk ;		/* Clock Initialization */
	char	mpclk ;			/* Clock Initialization */
	char	nws ;			  /* CPU Wait States */
	char	refc ;			/* SDRAM Refres Counter */
	short	sdram ;			/* SDRAM Timing Control */
	char	ptype ;			/* Panel Configuration */
	char	mod ;			  /* Mode Rate */
	char	lcdhndp ;		/* LCD Horizontal Non-Display Period */
	char	tftfpsp ;		/* FPLINE Start Position */
	char	tftfppw ;		/* FPLINE Pulse Width */
	char	lcdvndp ;		/* LCD Vertical Non-Display Period */
	char	lcdfpsp ;		/* FPFRAME Start Position */
	char	lcdfppw ;		/* FPFRAME Pulse Width */
	char	lcddm ;			/* LCD Display mode */
	char	lcdm ;			/* LCD Miscellaneous */
	short	lcdmao ;		/* LCD Memory Address Offset */
	char	crthndp ;		/* CRT/TV Horizontal Non-Display Period */
	char	crthsp ;		/* HRTC Start Position */
	char	crthpw ;		/* HRTC Pulse Width */
	char	crtvndp ;		/* CRT/TV Vertical Non-Display Period */
	char	crtvsp ;		/* VRTC Start Position */
	char	crtvpw ;		/* VRTC Pulse Width */
	char	tvoc ;			/* TV Output */
	char	crtdm ;			/* CRT Display Mode */
	short	crtmao ;		/* CRT Memory Address Offset */
	char	lcdicc ;		/* LCD Ink/Cursor Control */
	char	lcdicba ;		/* LCD Ink/Cursor Start Address */
	char	lcdicb0 ;		/* LCD Ink/Cursor Blue Color 0 */
	char	lcdicg0 ;		/* LCD Ink/Cursor Green Color 0 */
	char	lcdicr0 ;		/* LCD Ink/Cursor Red Color 0 */
	char	lcdicb1 ;		/* LCD Ink/Cursor Blue Color 1 */
	char	lcdicg1 ;		/* LCD Ink/Cursor Green Color 1 */
	char	lcdicr1 ;		/* LCD Ink/Cursor Red Color 1 */
	char	crticc ;		/* CRT/TV Ink/Cursor Control */
	char	crticba ;		/* CRT/TV Ink/Cursor Start Address */
	char	crticb0 ;		/* CRT/TV Ink/Cursor Blue Color 0 */
	char	crticg0 ;		/* CRT/TV Ink/Cursor Green Color 0 */
	char	crticr0 ;		/* CRT/TV Ink/Cursor Red Color 0 */
	char	crticb1 ;		/* CRT/TV Ink/Cursor Blue Color 1 */
	char	crticg1 ;		/* CRT/TV Ink/Cursor Green Color 1 */
	char	crticr1 ;		/* CRT/TV Ink/Cursor Red Color 1 */
	char	lutm ;			/* Look-Up Table Mode */
	char	psc ;			  /* Power Save Configuration */
	char	dm ;			  /* Display Mode */
} AT91S_S1D13806_RegDesc, *AT91PS_S1D13806_RegDesc;
//////////////////////////////////////////////////////////////////////////////

typedef	struct	_AT91S_S1D13806_DisplayDesc
{
	int								s1d13806_base_add ;				/* Device Base Address in EBI Space */
	int								s1d13806_disp_mem_base_add ;	/* Display Memory Base Address in EBI Space */
	AT91PS_S1D13806_RegDesc			RegDesc ;						/* Registers Description */
	AT91PS_S1D13806_DisplayDefDesc	DisplayDefDesc ;				/* Display Definition Description */
	AT91PS_S1D13806_CharDefDesc		CharDefDesc ;					/* Character Definition Description */
	AT91PS_S1D13806_DataDesc		DataDesc ;						/* Data Descriptor */
} AT91S_S1D13806_DisplayDesc, *AT91PS_S1D13806_DisplayDesc;
//////////////////////////////////////////////////////////////////////////////

typedef	struct	_AT91S_S1D13806_BitmapDesc
{
	short	type ;			  /* BM - bitmap type indicator */
	int		FileSize ;
	short	Reserved1 ;
	short	Reserved2 ;
	int		ImageOffset ;	/* Image Data Offset - after Bitmap Descriptor */
	int		HeaderSize ;	/* Bitmap Descriptor Size */
	int		width ;		  	/* Horizontal Display Width */
	int		height ;		  /* Vertical Display Width */
	short	planes ;		  /* 1 */
	short	BitsPerPixel ;/* Bit-per-Pixel Representation */
	int		CompressionType ;
	int		ImageSize ;		/* Image Data Size */
	int		xPerMetre ;
	int		yPerMetre ;
	int		ColoursUsed ;
	int		ImportantColours ;
} AT91S_S1D13806_BitmapDesc, *AT91PS_S1D13806_BitmapDesc;
//////////////////////////////////////////////////////////////////////////////

/*-----------------------------------*/
/* Bitmap File Constants Definitions */
/*-----------------------------------*/

#define		BMP_FILE_TYPE			0x4D42
#define		BMP_16BPP				  16
#define		BMP_24BPP				  24
#define		BMP_HEADER_SIZE		( sizeof ( AT91S_S1D13806_BitmapDesc ) )

/* Time constants definition */
#define		TIMEOUT_OF_200us		2	// (2*100)us

/* Function Prototyping s1d13806.c */
extern void AT91F_S1D13806_Init ( AT91PS_S1D13806_Desc	s1d13806_base,
                           int s1d13806_disp_mem_base,
                           unsigned int conf,
                           unsigned int disp_def,
                           unsigned int char_def,
                           unsigned int lut_def,
                           unsigned int mck_khz );
extern void AT91F_S1D13806_16bpp_print_string ( AT91PS_S1D13806_Desc s1d13806_base, char *buff ) ;
extern int AT91F_S1D13806_GetVdw(void);
extern int AT91F_S1D13806_GetHdw(void);
extern int AT91F_S1D13806_GePixelSize(void);
extern void ShowBmp (const GUI_BITMAP *bmp);

#endif /* s1d13806_h */
