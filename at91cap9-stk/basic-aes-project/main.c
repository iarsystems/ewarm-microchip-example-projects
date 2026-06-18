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
/// \dir "Basic AES Project"
///
/// !!!Purpose
///
/// This application demonstrates the Advanced Encryption Standard (AES) 
/// peripheral integrated in some AT91 microcontrollers. It encrypts and
/// decrypts several test values in Electronic CodeBook (ECB) and Cipher Block
/// Chaining (CBC) modes and checks them against the known answers.
///
/// !!!Requirements
///
/// The current supported board list:
/// - at91sam7xc-ek
/// - at91cap9s-dk
///
/// !!!Description
///
/// Uses the hardware AES to encrypt and decrypt test values in several 
/// operating modes
/// - Performs ECB (Electronic codebook) encryption & decryption tests
/// - Performs CBC (Cipher Block Chaining) encryption & decryption tests
///
/// When launched, the program will perform four tests: 
/// Encryption using ECB, with check against a known answer.
/// Decryption using ECB, with check against a known answer.
/// Encryption using CBC, with check against a known answer.
/// Decryption using CBC, with check against a known answer.
/// 
/// Each encryption/decryption process is done three times, to check each start
/// mode: manual, auto or PDC. The test values and the results are also 
/// displayed on the DBGU. 
///
/// Note: The test values are based on the ones provided by the
///  <a href="http://csrc.nist.gov/archive/aes/rijndael">NIST</a>
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
/// -# Start the application.
/// -# In the terminal window, the following text should appear:
/// \code
///    -- Basic AES Project xxx --
///    -- AT91xxxxxx-xx
///    -- Compiled: xxx xx xxxx xx:xx:xx --
///    -I- Testing ECB encryption
///    --------------------------
///    -I- Key:                    000102030405060708090A0B0C0D0E0F
///    -I- Plain text (initial):   000102030405060708090A0B0C0D0E0F
///    -I- Cipher text (expected): 0A940BB5416EF045F1C39458C653EA5A
///    -I- Cipher text (manual):   0A940BB5416EF045F1C39458C653EA5A (OK)
///    -I- Cipher text (auto):     0A940BB5416EF045F1C39458C653EA5A (OK)
///    -I- Cipher text (PDC):      0A940BB5416EF045F1C39458C653EA5A (OK)
///    
///    -I- Testing ECB decryption
///    --------------------------
///    -I- Key:                    000102030405060708090A0B0C0D0E0F
///    -I- Cipher text (initial):  0A940BB5416EF045F1C39458C653EA5A
///    -I- Plain text (expected):  000102030405060708090A0B0C0D0E0F
///    -I- Plain text (manual):    000102030405060708090A0B0C0D0E0F (OK)
///    -I- Plain text (auto):      000102030405060708090A0B0C0D0E0F (OK)
///    -I- Plain text (PDC):       000102030405060708090A0B0C0D0E0F (OK)
///    
///    -I- Testing CBC encryption
///    --------------------------
///    -I- Key:                    8A05FC5E095AF4848A08D328D3688E3D
///    -I- IV:                     8A05FC5E095AF4848A08D328D3688E3D
///    -I- Plain text (initial):   204F17E2444381F6114FF53934C0BCD3
///    -I- Cipher text (expected): 192D9B3AA10BB2F7846CCBA0085C657A
///    -I- Cipher text (manual):   192D9B3AA10BB2F7846CCBA0085C657A (OK)
///    -I- Cipher text (auto):     192D9B3AA10BB2F7846CCBA0085C657A (OK)
///    -I- Cipher text (PDC):      192D9B3AA10BB2F7846CCBA0085C657A (OK)
///    
///    -I- Testing CBC decryption
///    --------------------------
///    -I- Key:                    FACA37E0B0C85373DF706E73F7C9AF86
///    -I- IV:                     52D0C29FF8793A519BD6A8289FC80E6A
///    -I- Cipher text (initial):  FACA37E0B0C85373DF706E73F7C9AF86
///    -I- Plain text (expected):  F5372F9735C5685F1DA362AF6ECB2940
///    -I- Plain text (manual):    F5372F9735C5685F1DA362AF6ECB2940 (OK)
///    -I- Plain text (auto):      F5372F9735C5685F1DA362AF6ECB2940 (OK)
///    -I- Plain text (PDC):       F5372F9735C5685F1DA362AF6ECB2940 (OK)
/// \endcode
///
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <aes/aes_p.h>
#include <utility/trace.h>
#include <pmc/pmc.h>
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Displays a byte array as an hexadecimal stream.
/// \param pData  Data to display.
/// \param size  Size of data in bytes.
//------------------------------------------------------------------------------
static void DisplayStream(const unsigned int *pData, unsigned int size)
{
    unsigned char *pByte = (unsigned char *) pData;

    while (size > 0) {

        printf("%02X", *pByte);
        pByte++;
        size--;
    }
}

//------------------------------------------------------------------------------
/// Performs ECB encryption & decryption tests.
//------------------------------------------------------------------------------
static void TestEcb(void)
{
    const unsigned int pKey[] =    {0x03020100, 0x07060504, 0x0B0A0908, 0x0F0E0D0C};
    const unsigned int pPlain[] =  {0x03020100, 0x07060504, 0x0B0A0908, 0x0F0E0D0C};
    const unsigned int pCipher[] = {0xB50B940A, 0x45F06E41, 0x5894C3F1, 0x5AEA53C6};
    unsigned int pData[4];

    // Encryption
    printf("\n\r");
    printf("-I- Testing ECB encryption\n\r");
    printf("--------------------------\n\r");
    printf("-I- Key:                    ");
    DisplayStream(pKey, sizeof(pKey));
    printf("\n\r-I- Plain text (initial):   ");
    DisplayStream(pPlain, sizeof(pPlain));
    printf("\n\r-I- Cipher text (expected): ");
    DisplayStream(pCipher, sizeof(pCipher));
    printf("\n\r");

    // Manual encryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(AT91C_AES_CIPHER, AT91C_AES_SMOD_MANUAL, AT91C_AES_OPMOD_ECB);
    AES_SetKey(pKey);
    AES_SetInputData(pPlain);
    AES_Start();
    while ((AES_GetStatus() & AT91C_AES_DATRDY) != AT91C_AES_DATRDY);
    AES_GetOutputData(pData);

    printf("-I- Cipher text (manual):   ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pCipher, sizeof(pCipher))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // Auto encryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(AT91C_AES_CIPHER, AT91C_AES_SMOD_AUTO, AT91C_AES_OPMOD_ECB);
    AES_SetKey(pKey);
    AES_SetInputData(pPlain);
    while ((AES_GetStatus() & AT91C_AES_DATRDY) != AT91C_AES_DATRDY);
    AES_GetOutputData(pData);

    printf("-I- Cipher text (auto):     ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pCipher, sizeof(pCipher))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // PDC encryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(AT91C_AES_CIPHER, AT91C_AES_SMOD_PDC, AT91C_AES_OPMOD_ECB);
    AES_SetKey(pKey);
    AES_SetInputBuffer(pPlain);
    AES_SetOutputBuffer(pData);
    AES_Start();
    while ((AES_GetStatus() & AT91C_AES_ENDRX) != AT91C_AES_ENDRX);
    
    printf("-I- Cipher text (PDC):      ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pCipher, sizeof(pCipher))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // Decryption
    printf("\n\r");
    printf("-I- Testing ECB decryption\n\r");
    printf("--------------------------\n\r");
    printf("-I- Key:                    ");
    DisplayStream(pKey, sizeof(pKey));
    printf("\n\r-I- Cipher text (initial):  ");
    DisplayStream(pCipher, sizeof(pCipher));
    printf("\n\r-I- Plain text (expected):  ");
    DisplayStream(pPlain, sizeof(pPlain));
    printf("\n\r");

    // Manual decryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(0, AT91C_AES_SMOD_MANUAL, AT91C_AES_OPMOD_ECB);
    AES_SetKey(pKey);
    AES_SetInputData(pCipher);
    AES_Start();
    while ((AES_GetStatus() & AT91C_AES_DATRDY) != AT91C_AES_DATRDY);
    AES_GetOutputData(pData);

    printf("-I- Plain text (manual):    ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pPlain, sizeof(pPlain))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // Auto decryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(0, AT91C_AES_SMOD_AUTO, AT91C_AES_OPMOD_ECB);
    AES_SetKey(pKey);
    AES_SetInputData(pCipher);
    while ((AES_GetStatus() & AT91C_AES_DATRDY) != AT91C_AES_DATRDY);
    AES_GetOutputData(pData);

    printf("-I- Plain text (auto):      ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pPlain, sizeof(pPlain))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // PDC decryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(0, AT91C_AES_SMOD_PDC, AT91C_AES_OPMOD_ECB);
    AES_SetKey(pKey);
    AES_SetInputBuffer(pCipher);
    AES_SetOutputBuffer(pData);
    AES_Start();
    while ((AES_GetStatus() & AT91C_AES_ENDRX) != AT91C_AES_ENDRX);
    
    printf("-I- Plain text (PDC):       ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pPlain, sizeof(pPlain))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");
}

//------------------------------------------------------------------------------
/// Performs CBC encryption & decryption tests. This uses the Monte Carlo test
/// values, so each encryption/decryption is done 10000 times.
//------------------------------------------------------------------------------
static void TestCbc(void)
{
    const unsigned int pKey1[] =    {0x5EFC058A, 0x84F45A09, 0x28D3088A, 0x3D8E68D3};
    const unsigned int pVector1[] = {0x5EFC058A, 0x84F45A09, 0x28D3088A, 0x3D8E68D3};
    const unsigned int pPlain1[] =  {0xE2174F20, 0xF6814344, 0x39F54F11, 0xD3BCC034};
    const unsigned int pCipher1[] = {0x3A9B2D19, 0xF7B20BA1, 0xA0CB6C84, 0x7A655C08};
    const unsigned int pKey2[] =    {0xE037CAFA, 0x7353C8B0, 0x736E70DF, 0x86AFC9F7};
    const unsigned int pVector2[] = {0x9FC2D052, 0x513A79F8, 0x28A8D69B, 0x6A0EC89F};
    const unsigned int pCipher2[] = {0xE037CAFA, 0x7353C8B0, 0x736E70DF, 0x86AFC9F7};
    const unsigned int pPlain2[] =  {0x972F37F5, 0x5F68C535, 0xAF62A31D, 0x4029CB6E};
    unsigned int pCurrentVector[4];
    unsigned int pCurrentPlain[4];
    unsigned int pCurrentCipher[4];
    unsigned int pData[4];
    unsigned int i;

    // Encryption
    printf("\n\r");
    printf("-I- Testing CBC encryption\n\r");
    printf("--------------------------\n\r");
    printf("-I- Key:                    ");
    DisplayStream(pKey1, sizeof(pKey1));
    printf("\n\r-I- IV:                     ");
    DisplayStream(pVector1, sizeof(pVector1));
    printf("\n\r-I- Plain text (initial):   ");
    DisplayStream(pPlain1, sizeof(pPlain1));
    printf("\n\r-I- Cipher text (expected): ");
    DisplayStream(pCipher1, sizeof(pCipher1));
    printf("\n\r");

    // Manual encryption
    AES_Configure(AT91C_AES_CIPHER, AT91C_AES_SMOD_MANUAL, AT91C_AES_OPMOD_CBC);
    AES_SetKey(pKey1);
    memcpy(pCurrentVector, pVector1, sizeof(pVector1));
    memcpy(pCurrentPlain, pPlain1, sizeof(pPlain1));
    for (i=0; i < 10000; i++) {

        AES_SetVector(pCurrentVector);
        AES_SetInputData(pCurrentPlain);
        AES_Start();
        while ((AES_GetStatus() & AT91C_AES_DATRDY) != AT91C_AES_DATRDY);
        AES_GetOutputData(pData);
        memcpy(pCurrentPlain, pCurrentVector, sizeof(pCurrentVector));
        memcpy(pCurrentVector, pData, sizeof(pData));
    }

    printf("-I- Cipher text (manual):   ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pCipher1, sizeof(pCipher1))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // Auto encryption
    AES_Configure(AT91C_AES_CIPHER, AT91C_AES_SMOD_AUTO, AT91C_AES_OPMOD_CBC);
    AES_SetKey(pKey1);
    memcpy(pCurrentVector, pVector1, sizeof(pVector1));
    memcpy(pCurrentPlain, pPlain1, sizeof(pPlain1));
    for (i=0; i < 10000; i++) {

        AES_SetVector(pCurrentVector);
        AES_SetInputData(pCurrentPlain);
        while ((AES_GetStatus() & AT91C_AES_DATRDY) != AT91C_AES_DATRDY);
        AES_GetOutputData(pData);
        memcpy(pCurrentPlain, pCurrentVector, sizeof(pCurrentVector));
        memcpy(pCurrentVector, pData, sizeof(pData));
    }

    printf("-I- Cipher text (auto):     ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pCipher1, sizeof(pCipher1))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // PDC encryption
    AES_Configure(AT91C_AES_CIPHER, AT91C_AES_SMOD_PDC, AT91C_AES_OPMOD_CBC);
    AES_SetKey(pKey1);
    memcpy(pCurrentVector, pVector1, sizeof(pVector1));
    memcpy(pCurrentPlain, pPlain1, sizeof(pPlain1));
    for (i=0; i < 10000; i++) {

        AES_SetVector(pCurrentVector);
        AES_SetInputBuffer(pCurrentPlain);
        AES_SetOutputBuffer(pData);
        AES_Start();
        while ((AES_GetStatus() & AT91C_AES_ENDRX) != AT91C_AES_ENDRX);
        AES_GetOutputData(pData);
        memcpy(pCurrentPlain, pCurrentVector, sizeof(pCurrentVector));
        memcpy(pCurrentVector, pData, sizeof(pData));
    }
    
    printf("-I- Cipher text (PDC):      ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pCipher1, sizeof(pCipher1))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // Decryption
    printf("\n\r");
    printf("-I- Testing CBC decryption\n\r");
    printf("--------------------------\n\r");
    printf("-I- Key:                    ");
    DisplayStream(pKey2, sizeof(pKey2));
    printf("\n\r-I- IV:                     ");
    DisplayStream(pVector2, sizeof(pVector2));
    printf("\n\r-I- Cipher text (initial):  ");
    DisplayStream(pCipher2, sizeof(pCipher2));
    printf("\n\r-I- Plain text (expected):  ");
    DisplayStream(pPlain2, sizeof(pPlain2));
    printf("\n\r");

    // Manual decryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(0, AT91C_AES_SMOD_MANUAL, AT91C_AES_OPMOD_CBC);
    AES_SetKey(pKey2);
    memcpy(pCurrentVector, pVector2, sizeof(pVector2));
    memcpy(pCurrentCipher, pCipher2, sizeof(pCipher2));
    for (i=0; i < 10000; i++) {

        AES_SetVector(pCurrentVector);
        AES_SetInputData(pCurrentCipher);
        AES_Start();
        while ((AES_GetStatus() & AT91C_AES_DATRDY) != AT91C_AES_DATRDY);
        AES_GetOutputData(pData);
        memcpy(pCurrentVector, pCurrentCipher, sizeof(pCurrentCipher));
        memcpy(pCurrentCipher, pData, sizeof(pData));
    }

    printf("-I- Plain text (manual):    ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pPlain2, sizeof(pPlain2))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // Auto decryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(0, AT91C_AES_SMOD_AUTO, AT91C_AES_OPMOD_CBC);
    AES_SetKey(pKey2);
    memcpy(pCurrentVector, pVector2, sizeof(pVector2));
    memcpy(pCurrentCipher, pCipher2, sizeof(pCipher2));
    for (i=0; i < 10000; i++) {

        AES_SetVector(pCurrentVector);
        AES_SetInputData(pCurrentCipher);
        while ((AES_GetStatus() & AT91C_AES_DATRDY) != AT91C_AES_DATRDY);
        AES_GetOutputData(pData);
        memcpy(pCurrentVector, pCurrentCipher, sizeof(pCurrentCipher));
        memcpy(pCurrentCipher, pData, sizeof(pData));
    }

    printf("-I- Plain text (auto):      ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pPlain2, sizeof(pPlain2))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // PDC decryption
    memset(pData, 0, sizeof(pData));
    AES_Configure(0, AT91C_AES_SMOD_PDC, AT91C_AES_OPMOD_CBC);
    AES_SetKey(pKey2);
    memcpy(pCurrentVector, pVector2, sizeof(pVector2));
    memcpy(pCurrentCipher, pCipher2, sizeof(pCipher2));
    for (i=0; i < 10000; i++) {

        AES_SetVector(pCurrentVector);
        AES_SetInputBuffer(pCurrentCipher);
        AES_SetOutputBuffer(pData);
        while ((AES_GetStatus() & AT91C_AES_ENDRX) != AT91C_AES_ENDRX);
        memcpy(pCurrentVector, pCurrentCipher, sizeof(pCurrentCipher));
        memcpy(pCurrentCipher, pData, sizeof(pData));
    }

    printf("-I- Plain text (PDC):       ");
    DisplayStream(pData, sizeof(pData));
    if (!memcmp(pData, pPlain2, sizeof(pPlain2))) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Performs several tests on the AES peripheral to make sure it generates the
/// correct output using test vectors.
//------------------------------------------------------------------------------
int main()
{
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic AES Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Enable AES peripheral clock
#ifdef AT91C_ID_AES
    PMC_EnablePeripheral( AT91C_ID_AES );
#elif AT91C_ID_AESTDES
    PMC_EnablePeripheral( AT91C_ID_AESTDES );
#elif AT91C_ID_TDES
    PMC_EnablePeripheral( AT91C_ID_TDES );
#else
#error AES undefined
#endif

    // Tests
    TestEcb();
    TestCbc();

    return 0;
}

