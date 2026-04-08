/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
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
     
//-----------------------------------------------------------------------------
/// \dir "Basic EMAC project"
///
/// !!!Purpose
///
/// This project uses the Ethernet MAC (EMAC) and the on-board Ethernet transceiver available
/// on Atmel evaluation kits. It enables the device to respond to a ping command sent by a host computer. 
/// 
/// !!!Hardware requirements
///
/// The system reset signal NRST is connected to both the Ethernet PHY and the JTAG/ICE interface. 
/// This prevents the project from working properly when a JTAG probe is connected. 
/// To disconnect NRST from the ICE interface, the following modifications must be performed: 
///  - AT91SAM7X/XC-EK: cut strap S2. 
///  - AT91SAM9XE-EK: remove resistor R14 
///  - AT91SAM9260-EK: remove resistor R14 
///  - AT91SAM9263-EK: remove resistor R18 
///
/// !!!See
///
/// - emac: EMAC interface driver
/// - macb: PHY interface driver
///
/// !!!Description
///
/// Upon startup, the program will configure the EMAC with a default IP and MAC addresses 
/// and then ask the transceiver to auto-negotiate the best mode of operation. Once this is done, 
/// it will start monitoring incoming packets and processing them whenever appropriate. 
/// The basic will only answer to two kinds of packets: 
///
/// - It will reply to ARP requests with its MAC address, 
/// - and to ICMP ECHO request so the device can be PING'ed. 
///
/// To test that the board responds correctly to ping requests, type the following 
/// command-line on a computer connected to the same network as the board: 
/// \code 
///   ping 10.159.245.156
/// \endcode
///
/// !!!Usage
///
/// -# Build the program and download it inside the evaluation board. Please
///    refer to the
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
///    SAM-BA User Guide</a>, the
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
///    GNU-Based Software Development</a> application note or to the
///    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
///    IAR EWARM User Guide</a>, depending on your chosen solution.
/// -# On the computer, open and configure a terminal application
///    (e.g. HyperTerminal on Microsoft Windows) with these settings:
///   - 115200 bauds
///   - 8 bits of data
///   - No parity
///   - 1 stop bit
///   - No flow control
/// -# Connect an Ethernet cable between the evaluation board and the network. 
///     The board may be connected directly to a computer; in this case, 
///     make sure to use a cross/twisted wired cable such as the one provided with the evaluation kit. 
/// -# Start the application. It will display the following message on the DBGU: 
///    \code   
///     -- Basic EMAC Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     MAC 00:45:56:78:9a:bc
///     IP 10.159.245.156
///    \endcode
/// -# The program will then auto-negotiate the mode of operation and start receiving packets, 
///      displaying feedback on the DBGU. To display additional information, press any key in the terminal application. 
/// -# To test that the board responds to ICMP ECHO requests, type the following command line in a shell: 
///     \code 
///      ping 10.159.245.156
///     \endcode
///
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// basic-emac-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Init EMAC driver structure
///    - Init MACB driver
///    - Polling the packets
///    - Handle input frame
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//         Headers
//-----------------------------------------------------------------------------

#include "MiniIp.h"
#include <board.h>
#include <pio/pio.h>
#include <irq/irq.h>
#include <dbgu/dbgu.h>
#include <usart/usart.h>
#include <emac/emac.h>
#include <drivers/macb/macb.h>
//#include <stdio.h>
#include <string.h>
#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

//-----------------------------------------------------------------------------
//         Local Define
//-----------------------------------------------------------------------------

#ifdef __ICCARM__          // IAR
#define __attribute__(...) // IAR
#endif                     // IAR

/// EMAC packet processing offset
#define EMAC_RCV_OFFSET     0

/// Delay before a link check
#define EMAC_LINK_CHECK_DELAY       1000000

//-----------------------------------------------------------------------------
//         Local variables
//-----------------------------------------------------------------------------

/// EMAC power control pin
#if !defined(BOARD_EMAC_POWER_ALWAYS_ON)
static const Pin emacPwrDn[] = {BOARD_EMAC_PIN_PWRDN};
#endif

/// The PINs' on PHY reset
static const Pin emacRstPins[] = {BOARD_EMAC_RST_PINS};

/// The PINs for EMAC
static const Pin emacPins[] = {BOARD_EMAC_RUN_PINS};

/// The MAC address used for demo
static unsigned char MacAddress[6] = {0x00, 0x45, 0x56, 0x78, 0x9a, 0xac};

/// The IP address used for demo (ping ...)
static unsigned char IpAddress[4] = {10, 159, 241, 248};

/// The MACB driver instance
static Macb gMacb;

/// Buffer for Ethernet packets
static unsigned char EthBuffer[EMAC_FRAME_LENTGH_MAX];


//-----------------------------------------------------------------------------
//         Local functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/// Emac interrupt handler
//-----------------------------------------------------------------------------
static void ISR_Emac(void)
{
    EMAC_Handler();
}

//-----------------------------------------------------------------------------
/// Display the IP packet
/// \param pIpHeader Pointer to the IP header
//-----------------------------------------------------------------------------
static void DisplayIpPacket(PIpHeader pIpHeader, unsigned int size)
{
    printf("======= IP %4d bytes, HEADER ==========\n\r", size);
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

    switch(pIpHeader->ip_p) {

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


//-----------------------------------------------------------------------------
/// Process the received ARP packet
/// Just change address and send it back
/// \param pData The data to process
/// \param size The data size
//-----------------------------------------------------------------------------
static void arp_process_packet(unsigned char* pData, unsigned int size)
{
    unsigned int i;
    unsigned char emac_rc = EMAC_TX_OK;

    PEthHeader   pEth = (PEthHeader)pData;
    PArpHeader   pArp = (PArpHeader)(pData + 14 + EMAC_RCV_OFFSET);

    if (SWAP16(pArp->ar_op) == ARP_REQUEST) {

        // ARP REPLY operation
        pArp->ar_op =  SWAP16(ARP_REPLY);               

        // Fill the dest address and src address
        for (i = 0; i <6; i++) {
            // swap ethernet dest address and ethernet src address          
            pEth->et_dest[i] = pEth->et_src[i];
            pEth->et_src[i]  = MacAddress[i];
            pArp->ar_tha[i]  = pArp->ar_sha[i];
            pArp->ar_sha[i]  = MacAddress[i];
        }                                   
        // swap sender IP address and target IP address
        for (i = 0; i<4; i++) {
            pArp->ar_tpa[i] = pArp->ar_spa[i];
            pArp->ar_spa[i] = IpAddress[i];
        }   
        emac_rc = EMAC_Send( (pData + EMAC_RCV_OFFSET), 
                             size,
                             (EMAC_TxCallback)0);
        if (emac_rc != EMAC_TX_OK) {
            printf("E: ARP Send - 0x%x\n\r", emac_rc);
        }
    }           
}

//-----------------------------------------------------------------------------
/// Process the received IP packet
/// Just change address and send it back
/// \param pData The data to process
/// \param size The data size
//-----------------------------------------------------------------------------
static void ip_process_packet(unsigned char* pData, unsigned int size)
{
    unsigned int i;
    unsigned int icmp_len;
    int emac_rc = EMAC_TX_OK;

    PEthHeader   pEth = (PEthHeader)pData;
    PIpHeader    pIpHeader = (PIpHeader)(pData + 14 + EMAC_RCV_OFFSET);

    PIcmpEchoHeader pIcmpEcho = (PIcmpEchoHeader)((char *)pIpHeader + 20);

    switch(pIpHeader->ip_p) {

        case IP_PROT_ICMP:     

            // if ICMP_ECHO_REQUEST ==> resp = ICMP_ECHO_REPLY
            if(pIcmpEcho->type == ICMP_ECHO_REQUEST) {
                pIcmpEcho->type = ICMP_ECHO_REPLY;
                pIcmpEcho->code = 0;
                pIcmpEcho->cksum = 0;

                // Checksum of the ICMP Message             
                icmp_len = (SWAP16(pIpHeader->ip_len) - 20);
                if (icmp_len % 2) {
                    *((unsigned char *)pIcmpEcho + icmp_len) = 0;
                    icmp_len ++;
                }
                icmp_len = icmp_len / sizeof(unsigned short);

                pIcmpEcho->cksum =
                    SWAP16(IcmpChksum((unsigned short *) pIcmpEcho, icmp_len));
                // Swap IP Dest address and IP Source address
                for(i = 0; i <4; i++) {
                    pIpHeader->ip_dst[i] = pIpHeader->ip_src[i];
                    pIpHeader->ip_src[i] = IpAddress[i];
                }
                // Swap Eth Dest address and Eth Source address
                for(i = 0; i <6; i++) {

                    // swap ethernet dest address and ethernet src addr
                    pEth->et_dest[i] = pEth->et_src[i];
                    pEth->et_src[i]  = MacAddress[i];
                }
                // send the echo_reply
                emac_rc = EMAC_Send( (pData + EMAC_RCV_OFFSET),
                                     SWAP16(pIpHeader->ip_len)+14+EMAC_RCV_OFFSET,
                                     (EMAC_TxCallback)0);
                if (emac_rc != EMAC_TX_OK) {
                    printf("E: ICMP Send - 0x%x\n\r", emac_rc);
                }
            }
        break; // case IP_PROT_ICMP

        default:
        break;
    }
}

//-----------------------------------------------------------------------------
/// Process the received EMAC packet
/// \param pData The data to process
/// \param size The data size
//-----------------------------------------------------------------------------
static void eth_process_packet(unsigned char* pData, unsigned int size)
{
    unsigned short pkt_format;

    PEthHeader   pEth = (PEthHeader)(pData + EMAC_RCV_OFFSET);
    PIpHeader    pIpHeader = (PIpHeader)(pData + 14 + EMAC_RCV_OFFSET);
    IpHeader     ipHeader;

    pkt_format = SWAP16(pEth->et_protlen);
    switch (pkt_format) {

        // ARP Packet format
        case ETH_PROT_ARP:

            // Process the ARP packet
            arp_process_packet(pData, size);

            // Dump for ARP packet
            // printf("=== ARP %3d bytes ===\n\r", size);
        break; // case ETH_PROT_ARP

        // IP protocol frame
        case ETH_PROT_IP:

            // Backup the header
            memcpy(&ipHeader, pIpHeader,sizeof(IpHeader));

            // Process the IP packet
            ip_process_packet(pData, size);

            // Dump the IP header
            // DisplayIpPacket(&ipHeader, size);
        break; // case ETH_PROT_IP
                    
        default:
            printf("=== Defalt pkt_format= 0x%X===\n\r", pkt_format);
        break;
    }// switch (SWAP16(pEth->et_protlen))
}

//-----------------------------------------------------------------------------
//         Global functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Default main() function. Initializes the DBGU and writes a string on the
/// DBGU.
//-----------------------------------------------------------------------------
int main(void)
{
    Macb       *pMacb = &gMacb;
    unsigned int errCount = 0;
    unsigned int frmSize;
    EmacStats    stats;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic EMAC Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    
    // Display MAC & IP settings
    printf("-- MAC %x:%x:%x:%x:%x:%x\n\r",
           MacAddress[0], MacAddress[1], MacAddress[2],
           MacAddress[3], MacAddress[4], MacAddress[5]);
    printf("-- IP  %d.%d.%d.%d\n\r",
           IpAddress[0], IpAddress[1], IpAddress[2], IpAddress[3]);

#if !defined(BOARD_EMAC_POWER_ALWAYS_ON)
    // clear PHY power down mode
    PIO_Configure(emacPwrDn, 1);
#endif

    // Init EMAC driver structure
    EMAC_Init(AT91C_ID_EMAC, MacAddress, EMAC_CAF_DISABLE, EMAC_NBC_DISABLE);

    // Setup EMAC buffers and interrupts
    IRQ_ConfigureIT(AT91C_ID_EMAC, (0x2 << 5), ISR_Emac);
    IRQ_EnableIT(AT91C_ID_EMAC);

    // Init MACB driver
    MACB_Init(pMacb, BOARD_EMAC_PHY_ADDR);

    // PHY initialize
    if (!MACB_InitPhy(pMacb, BOARD_MCK,
                        emacRstPins, PIO_LISTSIZE(emacRstPins),
                        emacPins, PIO_LISTSIZE(emacPins))) {

        printf("P: PHY Initialize ERROR!\n\r");
        return -1;
    }

    // Auto Negotiate
    if (!MACB_AutoNegotiate(pMacb)) {

        printf("P: Auto Negotiate ERROR!\n\r");
        return -1;
    }

    while( MACB_GetLinkSpeed(pMacb, 1) == 0 ) {

        errCount++;
    }
    printf("P: Link detected \n\r");

    printf("Press a key for statistics\n\r");

    while(1) {

        // Display Statistics
        if ( USART_IsDataAvailable((AT91S_USART *)AT91C_BASE_DBGU) ) {

            DBGU_GetChar();
            EMAC_GetStatistics(&stats, 1);
            printf("=== EMAC Statistics ===\n\r");
            printf(" .tx_packets = %d\n\r", stats.tx_packets);
            printf(" .tx_comp = %d\n\r", stats.tx_comp);
            printf(" .tx_errors = %d\n\r", stats.tx_errors);
            printf(" .collisions = %d\n\r", stats.collisions);
            printf(" .tx_exausts = %d\n\r", stats.tx_exausts);
            printf(" .tx_underruns = %d\n\r", stats.tx_underruns);
            printf(" .rx_packets = %d\n\r", stats.rx_packets);
            printf(" .rx_eof = %d\n\r", stats.rx_eof);
            printf(" .rx_ovrs = %d\n\r", stats.rx_ovrs);
            printf(" .rx_bnas = %d\n\r", stats.rx_bnas);
        }

        // Process packets
        if( EMAC_RX_OK != EMAC_Poll( EthBuffer, sizeof(EthBuffer), &frmSize) ) {

            continue;
        }
        
        if (frmSize > 0) {

            // Handle input frame
            eth_process_packet(EthBuffer, frmSize);
        }
    }

    return 0;
}

