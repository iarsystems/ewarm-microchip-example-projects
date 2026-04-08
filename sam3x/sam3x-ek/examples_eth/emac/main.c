/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/**
 *  \page emac EMAC Example
 *
 *  \section Purpose
 *
 *  This example uses the Ethernet MAC (EMAC) and the on-board Ethernet
 *  transceiver available on Atmel evaluation kits. It enables the device to
 *  respond to a ping command sent by a host computer.
 *
 *  \section Requirements
 *
 *  - AT91 microcontrollers with EMAC feature.
 *  - On-board ethernet interface.
 *
 *  \section Description
 *
 *  Upon startup, the program will configure the EMAC with a default IP and
 *  MAC address and then ask the transceiver to auto-negotiate the best mode
 *  of operation. Once this is done, it will start to monitor incoming packets
 *  and processing them whenever appropriate.
 *
 *  The basic will only answer to two kinds of packets:
 *
 *  - It will reply to ARP requests with its MAC address,
 *  - and to ICMP ECHO request so the device can be PING'ed.
 *
 *  You may use 'ping' command to check if the board responds correctly to
 *  ping requests.
 *
 *  \section Usage
 *
 *  -# Build the program and download it inside the evaluation board. Please
 *     refer to the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/6421B.pdf">
 *     SAM-BA User Guide</a>, the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *     GNU-Based Software Development</a>
 *     application note or to the
 *     <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *     IAR EWARM User Guide</a>,
 *     depending on your chosen solution.
 *  -# On the computer, open and configure a terminal application
 *     (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *    - 115200 bauds
 *    - 8 bits of data
 *    - No parity
 *    - 1 stop bit
 *    - No flow control
 *  -# Connect an Ethernet cable between the evaluation board and the network.
 *      The board may be connected directly to a computer; in this case,
 *      make sure to use a cross/twisted wired cable such as the one provided
 *      with the evaluation kit.
 *  -# Start the application. It will display the following message on the terminal:
 *     \code
 *      -- EMAC Example xxx --
 *      -- AT91xxxxxx-xx
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *      MAC 00:45:56:78:9a:bc
 *      IP 10.159.245.156
 *     \endcode
 *  -# The program will then auto-negotiate the mode of operation and start
 *     receiving packets, displaying feedback on the terminal. To display additional
 *     information, press any key in the terminal application.
 *  -# To test that the board responds to ICMP ECHO requests, type the following
 *     command line in a shell:
 *      \code
 *       ping 10.159.245.156
 *      \endcode
 *     Response to 'ping' cmd will appear in the shell.
 *
 *  \note
 *  Make sure the IP adress of the device(EK board) and the computer are in the same network.
 *
 *  \section References
 *  - emac/main.c
 *  - macb.h
 *  - emacd.h
 *  - emac.h
 */

/** \file
 *
 *  This file contains all the specific code for the emac example.
 *
 */

/*---------------------------------------------------------------------------
 *         Headers
 *---------------------------------------------------------------------------*/

#include <board.h>
#include <string.h>

#include "MiniIp.h"

/*---------------------------------------------------------------------------
 *         Local Define
 *---------------------------------------------------------------------------*/

#ifdef __ICCARM__          /* IAR */
#define __attribute__(...) /* IAR */
#endif                     /* IAR */

/** EMAC packet processing offset */
#define EMAC_RCV_OFFSET     0

/** Number of buffer for RX */
#define RX_BUFFERS  16
/** Number of buffer for TX */
#define TX_BUFFERS  8

/*---------------------------------------------------------------------------
 *         Local variables
 *---------------------------------------------------------------------------*/

/** The PINs for EMAC */
static const Pin gEmacPins[] = {BOARD_EMAC_PINS};
/** The MAC address used for demo */
static uint8_t gMacAddress[6] = {0x00, 0x45, 0x56, 0x78, 0x9a, 0xbc};

/** The IP address used for demo (ping ...) */
static uint8_t gIpAddress[4] = {10, 159, 245, 156};

/** The EMAC driver instance */
static sEmacd gEmacd;

/** The MACB driver instance */
static Macb gMacb;

/** Buffer for Ethernet packets */
static uint8_t gEthBuffer[EMAC_FRAME_LENTGH_MAX];

/** TX descriptors list */
#ifdef __ICCARM__          /* IAR */
#pragma data_alignment=8   /* IAR */
#endif                     /* IAR */
static sEmacTxDescriptor gTxDs[TX_BUFFERS];
/** TX callbacks list */
static fEmacdTransferCallback gTxCbs[TX_BUFFERS];
/** RX descriptors list */
#ifdef __ICCARM__          /* IAR */
#pragma data_alignment=8   /* IAR */
#endif                     /* IAR */
static sEmacRxDescriptor gRxDs[RX_BUFFERS];
/** Send Buffer */
/* Section 3.6 of AMBA 2.0 spec states that burst should not cross 1K Boundaries.
   Receive buffer manager writes are burst of 2 words => 3 lsb bits of the address
   shall be set to 0 */
#ifdef __ICCARM__          /* IAR */
#pragma data_alignment=8   /* IAR */
#endif                     /* IAR */
static uint8_t gpTxBuffer[TX_BUFFERS * EMAC_TX_UNITSIZE] __attribute__((aligned(8)));

#ifdef __ICCARM__          /* IAR */
#pragma data_alignment=8   /* IAR */
#endif                     /* IAR */
/** Receive Buffer */
static uint8_t gpRxBuffer[RX_BUFFERS * EMAC_RX_UNITSIZE] __attribute__((aligned(8)));

/*---------------------------------------------------------------------------
 *         Local functions
 *---------------------------------------------------------------------------*/

/**
 * Display the IP packet
 * \param pIpHeader Pointer to the IP header
 * \param dwSize    The data size
 */
static void _DisplayIpPacket(PIpHeader pIpHeader, uint32_t dwSize)
{
    printf("======= IP %4d bytes, HEADER ==========\n\r", (int)dwSize);
    printf(" IP Version        = v.%d",(pIpHeader->ip_hl_v & 0xF0) >> 4);
    printf("\n\r Header Length     = %d",pIpHeader->ip_hl_v & 0x0F);
    printf("\n\r Type of service   = 0x%x",pIpHeader->ip_tos);
    printf("\n\r Total IP Length   = 0x%X",
        (((pIpHeader->ip_len)>>8)&0xff) + (((pIpHeader->ip_len)<<8)&0xff00) );
    printf("\n\r ID                = 0x%X",
        (((pIpHeader->ip_id)>>8)&0xff) + (((pIpHeader->ip_id)<<8)&0xff00) );
    printf("\n\r Header Checksum   = 0x%X",
        (((pIpHeader->ip_sum)>>8)&0xff) + (((pIpHeader->ip_sum)<<8)&0xff00) );
    printf("\n\r Protocol          = ");

    switch(pIpHeader->ip_p)
    {
        case IP_PROT_ICMP:
            printf( "ICMP");
        break;

        case IP_PROT_IP:
            printf( "IP");
        break;

        case IP_PROT_TCP:
            printf("TCP");
        break;

        case IP_PROT_UDP:
            printf("UDP");
        break;

        default:
            printf( "%d (0x%X)", pIpHeader->ip_p, pIpHeader->ip_p);
        break;
    }

    printf( "\n\r IP Src Address    = %d:%d:%d:%d",
             pIpHeader->ip_src[0],
             pIpHeader->ip_src[1],
             pIpHeader->ip_src[2],
             pIpHeader->ip_src[3]);

    printf( "\n\r IP Dest Address   = %d:%d:%d:%d",
             pIpHeader->ip_dst[0],
             pIpHeader->ip_dst[1],
             pIpHeader->ip_dst[2],
             pIpHeader->ip_dst[3]);
    printf("\n\r----------------------------------------\n\r");
}

/**
 * Process the received ARP packet. Just change address and send it back.
 * \param pData  The data to process
 * \param dwSize The data size
 */
static void _ProcessArpPacket( uint8_t* pData, uint32_t dwSize )
{
    uint32_t i ;
    uint8_t emac_rc = EMACD_OK;

    PEthHeader   pEth = (PEthHeader)pData;
    PArpHeader   pArp = (PArpHeader)(pData + 14 + EMAC_RCV_OFFSET) ;

     if ( SWAP16( pArp->ar_op ) == ARP_REQUEST )
    {
        /* ARP REPLY operation */
        pArp->ar_op =  SWAP16(ARP_REPLY);

        /* Fill the dest address and src address */
        for (i = 0; i <6; i++)
        {
            /* swap ethernet dest address and ethernet src address */
            pEth->et_dest[i] = pEth->et_src[i];
            pEth->et_src[i]  = gMacAddress[i];
            pArp->ar_tha[i]  = pArp->ar_sha[i];
            pArp->ar_sha[i]  = gMacAddress[i];
        }
        /* swap sender IP address and target IP address */
        for (i = 0; i<4; i++)
        {
            pArp->ar_tpa[i] = pArp->ar_spa[i];
            pArp->ar_spa[i] = gIpAddress[i];
        }
        emac_rc = EMACD_Send(&gEmacd,
                             (pData + EMAC_RCV_OFFSET),
                             dwSize,
                             NULL);
        if (emac_rc != EMACD_OK)
        {
            printf("E: ARP Send - 0x%x\n\r", emac_rc);
        }
    }
}

/**
 * Process the received IP packet. Just change address and send it back.
 * \param pData  The data to process
 * \param dwSize The data size
 */
static void _ProcessIpPacket( uint8_t* pData, uint32_t dwSize )
{
    uint32_t i ;
    uint32_t icmp_len ;
    int32_t emac_rc = EMACD_OK;

    dwSize = dwSize; /* stop warning */

    PEthHeader   pEth = (PEthHeader)pData;
    PIpHeader    pIpHeader = (PIpHeader)(pData + 14 + EMAC_RCV_OFFSET);

    PIcmpEchoHeader pIcmpEcho = (PIcmpEchoHeader)((int8_t *)pIpHeader + 20);

    switch ( pIpHeader->ip_p )
    {
        case IP_PROT_ICMP:

            /* if ICMP_ECHO_REQUEST ==> resp = ICMP_ECHO_REPLY */
            if(pIcmpEcho->type == ICMP_ECHO_REQUEST)
            {
                pIcmpEcho->type = ICMP_ECHO_REPLY;
                pIcmpEcho->code = 0;
                pIcmpEcho->cksum = 0;

                /* Checksum of the ICMP Message */
                icmp_len = (SWAP16(pIpHeader->ip_len) - 20);
                if (icmp_len % 2)
                {
                    *((uint8_t *)pIcmpEcho + icmp_len) = 0;
                    icmp_len ++;
                }
                icmp_len = icmp_len / sizeof(uint16_t);

                pIcmpEcho->cksum =
                    SWAP16(IcmpChksum((uint16_t *) pIcmpEcho, icmp_len));
                /* Swap IP Dest address and IP Source address */
                for(i = 0; i <4; i++)
                {
                    pIpHeader->ip_dst[i] = pIpHeader->ip_src[i];
                    pIpHeader->ip_src[i] = gIpAddress[i];
                }
                /* Swap Eth Dest address and Eth Source address */
                for(i = 0; i <6; i++)
                {
                    /* swap ethernet dest address and ethernet src addr */
                    pEth->et_dest[i] = pEth->et_src[i];
                    pEth->et_src[i]  = gMacAddress[i];
                }
                /* send the echo_reply */
                emac_rc = EMACD_Send(&gEmacd,
                                     (pData + EMAC_RCV_OFFSET),
                                     SWAP16(pIpHeader->ip_len)+14+EMAC_RCV_OFFSET,
                                     NULL);
                if (emac_rc != EMACD_OK)
                {
                    printf("E: ICMP Send - 0x%x\n\r", emac_rc);
                }
            }
        break; /* case IP_PROT_ICMP */

        default:
        break;
    }
}

/**
 * Process the received EMAC packet.
 * \param pData  The data to process
 * \param dwSize The data size
 */
static void _ProcessEthPacket( uint8_t* pData, uint32_t dwSize )
{
    uint16_t pkt_format;

    PEthHeader   pEth = (PEthHeader)(pData + EMAC_RCV_OFFSET);
    PIpHeader    pIpHeader = (PIpHeader)(pData + 14 + EMAC_RCV_OFFSET);
    IpHeader     ipHeader;

    pkt_format = SWAP16( pEth->et_protlen ) ;
    switch ( pkt_format )
    {
        /* ARP Packet format */
        case ETH_PROT_ARP:
            /* Process the ARP packet */
            _ProcessArpPacket(pData, dwSize);

         break; /* case ETH_PROT_ARP */

        /* IP protocol frame */
        case ETH_PROT_IP:
            /* Backup the header */
            memcpy(&ipHeader, pIpHeader,sizeof(IpHeader));

            /* Process the IP packet */
            _ProcessIpPacket(pData, dwSize);

            /* Dump the IP header */
            _DisplayIpPacket(&ipHeader, dwSize);
        break; /* case ETH_PROT_IP */

        default:
            printf("=== Default pkt_format= 0x%X===\n\r", pkt_format);
        break;
    }
}

/*---------------------------------------------------------------------------
 *         Global functions
 *---------------------------------------------------------------------------*/
void EMAC_IrqHandler(void)
{
    EMACD_Handler(&gEmacd);
}

/**
 *  \brief emac example entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
    sEmacd    *pEmacd = &gEmacd;
    Macb      *pMacb = &gMacb ;
    uint32_t  dwErrCount = 0 ;
    uint32_t  dwFrmSize ;
    volatile uint32_t  dwDly;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    printf("-- EMAC Example %s --\n\r", SOFTPACK_VERSION);
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /* Display MAC & IP settings */
    printf("-- MAC %x:%x:%x:%x:%x:%x\n\r",
           gMacAddress[0], gMacAddress[1], gMacAddress[2],
           gMacAddress[3], gMacAddress[4], gMacAddress[5]);
    printf("-- IP  %d.%d.%d.%d\n\r",
           gIpAddress[0], gIpAddress[1], gIpAddress[2], gIpAddress[3]);

    /* Reset PHY */
    RSTC_SetExtResetLength(RSTC, 13); /* (2^(13+1))/32768 */
    RSTC_ExtReset(RSTC);
    while(RSTC_GetNrstLevel(RSTC) == 0) {}
    /* Wait for PHY to be ready (CAT811: Max400ms) */
    dwDly = BOARD_MCK / 1000 / 3 * 400;
    while(dwDly--);

    /* Init EMAC driver structure */
    EMACD_Init(pEmacd, EMAC, ID_EMAC, 0, 0);
    EMACD_InitTransfer(pEmacd,
        gpRxBuffer, gRxDs, RX_BUFFERS,
        gpTxBuffer, gTxDs, gTxCbs, TX_BUFFERS);
    EMAC_SetAddress(gEmacd.pHw, 0, gMacAddress);

    /* Enable Interrupt */
    NVIC_EnableIRQ( EMAC_IRQn ) ;

    /* Init MACB driver */
    MACB_Init(pMacb, pEmacd, BOARD_EMAC_PHY_ADDR);

    /* PHY initialize */
    if (!MACB_InitPhy(pMacb, BOARD_MCK,
                      gEmacPins, PIO_LISTSIZE(gEmacPins)))
    {
        printf( "P: PHY Initialize ERROR!\n\r" ) ;
        return -1 ;
    }

    /* Auto Negotiate, work in RMII mode */
    if (!MACB_AutoNegotiate(pMacb, BOARD_EMAC_MODE_RMII))
    {

        printf( "P: Auto Negotiate ERROR!\n\r" ) ;
        return -1 ;
    }

    while ( MACB_GetLinkSpeed( pMacb, 1 ) == 0 )
    {
        dwErrCount++;
    }
    printf( "P: Link detected \n\r" ) ;

    while ( 1 )
    {
        /* Process packets */
        if( EMACD_OK != EMACD_Poll(pEmacd, gEthBuffer, sizeof(gEthBuffer), &dwFrmSize) )
        {
            continue ;
        }

        if (dwFrmSize > 0)
        {
            /* Handle input frame */
            _ProcessEthPacket( gEthBuffer, dwFrmSize ) ;
        }
    }
}

