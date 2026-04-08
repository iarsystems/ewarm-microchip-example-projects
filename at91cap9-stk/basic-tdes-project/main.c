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
/// \dir "Basic TDES Project"
///
/// !!!Purpose
///
/// Uses the hardware triple-DES (3DES) to encrypt and decrypt test values in
/// several operating modes
/// It encrypts and decrypts several test values in Electronic CodeBook (ECB)
/// and Cipher Block Chaining (CBC) modes and checks them against the known
/// answers.
///
/// !!!Requirements
///
/// The current supported board list:
/// - at91sam7xc-ek
/// - at91cap9s-dk
///
/// !!!Description
///
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
/// mode: manual, auto or PDC. Each test is also performed in the following 
/// configurations: 
/// - Single-DES
/// - Triple-DES with two keys
/// - Triple-DES with three keys
/// - The test values and the results are displayed on the DBGU. 
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
///   -- Basic Triple DES Project xxx --
///   -- AT91xxxxxx-xx
///   -- Compiled: xxx xx xxxx xx:xx:xx --
///   -I- Testing Single-DES ECB encryption
///   -I- Key1:              0123456789ABCDEF
///   -I- Input:             4E6F772069732074
///   -I- Output (expected): 6A2A19F41ECA854B
///   -I- Output (manual):   6A2A19F41ECA854B (OK)
///   -I- Output (auto):     6A2A19F41ECA854B (OK)
///   -I- Output (PDC):      6A2A19F41ECA854B (OK)
///   
///   -I- Testing Single-DES ECB decryption
///   [...]
///   -I- Testing Triple-DES/2 keys ECB encryption
///   [...]
///   -I- Testing Triple-DES/2 keys ECB decryption
///   [...]
///   -I- Testing Triple-DES/3 keys ECB encryption
///   [...]
///   -I- Testing Triple-DES/3 keys ECB decryption
///   [...]
///   -I- Testing Single-DES CBC encryption
///   [...]
///   -I- Testing Single-DES CBC decryption
///   [...]
///   -I- Testing Triple-DES/2 keys CBC encryption
///   [...]
///   -I- Testing Triple-DES/2 keys CBC decryption
///   [...]
///   -I- Testing Triple-DES/3 keys CBC encryption
///   [...]
///   -I- Testing Triple-DES/3 keys CBC decryption
///   [...]
/// \endcode
///
//-----------------------------------------------------------------------------



//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <tdes/tdes.h>
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
/// Displays a value as an hexadecimal stream on the DBGU.
/// \param pValue  Value to display.
/// \param size  Size of value in bytes.
//------------------------------------------------------------------------------
static void DisplayStream(const unsigned int *pValue, unsigned int size)
{
    const unsigned char *pByte = (const unsigned char *) pValue;
    while (size > 0) {

        printf("%02X", *pByte);
        pByte++;
        size--;
    }
}

//------------------------------------------------------------------------------
/// Performs the Monte Carlo test with the given configuration.
/// \param cipher  Encrypts if 1, decrypts if 0.
/// \param opmod  Operating mode.
/// \param pKey1  Pointer to key #1.
/// \param pKey2  Pointer to key #2 (if 0, single-DES is used).
/// \param pKey3  Pointer to key #3 (if 0, triple-DES with two keys is used).
/// \param pVector  Initialization vector.
/// \param pInput  Input data.
/// \param pOutput  Output data.
//------------------------------------------------------------------------------
static void Test(
    unsigned char cipher,
    unsigned int opmod,
    const unsigned int *pKey1,
    const unsigned int *pKey2,
    const unsigned int *pKey3,
    const unsigned int *pVector,
    const unsigned int *pInput,
    const unsigned int *pOutput)
{
    unsigned int keymod;
    unsigned int tdesmod;
    unsigned int pCurrent[2];
    unsigned int pIV[2];
    unsigned int pResult[2];
    unsigned int i;

    printf("-I- Testing ");

    // DES mode
    if (pKey2 && pKey3) {

        printf("Triple-DES/3 keys");
        keymod = 0;
        tdesmod = AT91C_TDES_TDESMOD;
    }
    else if (pKey2) {

        printf("Triple-DES/2 keys");
        keymod = AT91C_TDES_KEYMOD;
        tdesmod = AT91C_TDES_TDESMOD;
    }
    else {

        printf("Single-DES");
        keymod = 0;
        tdesmod = 0;
    }

    printf(" ");

    // Operating mode
    switch (opmod) {
    case AT91C_TDES_OPMOD_ECB: printf("ECB"); break;
    case AT91C_TDES_OPMOD_CBC: printf("CBC"); break;
    case AT91C_TDES_OPMOD_CFB: printf("CFB"); break;
    case AT91C_TDES_OPMOD_OFB: printf("OFB"); break;
    }

    printf(" ");

    // Cipher/decipher
    if (cipher == AT91C_TDES_CIPHER) {
    
        printf("encryption");
    }
    else {

        printf("decryption");
    }

    printf("\n\r");

    // Keys
    printf("-I- Key1:              ");
    DisplayStream(pKey1, 8);
    if (pKey2) {
    
        printf("\n\r-I- Key2:              ");
        DisplayStream(pKey2, 8);
    }
    if (pKey3) {

        printf("\n\r-I- Key3:              ");
        DisplayStream(pKey3, 8);
    }

    // Vector
    if (pVector) {

        printf("\n\r-I- IV:                ");
        DisplayStream(pVector, 8);
    }

    // Input & output data
    printf("\n\r-I- Input:             ");
    DisplayStream(pInput, 8);
    printf("\n\r-I- Output (expected): ");
    DisplayStream(pOutput, 8);
    printf("\n\r");

    // Manual mode
    memset(pResult, 0, 8);
    TDES_Configure(cipher, tdesmod, keymod, AT91C_TDES_SMOD_MANUAL, opmod);
    TDES_SetKeys(pKey1, pKey2, pKey3);
    if (pVector) {

        memcpy(pIV, pVector, 8);
    }
    memcpy(pCurrent, pInput, 8);
    for (i=0; i < 10000; i++) {
    
        TDES_SetInputData(pCurrent);
        if (pVector) {

            TDES_SetVector(pIV);
        }
        TDES_Start();
        while ((TDES_GetStatus() & AT91C_TDES_DATRDY) != AT91C_TDES_DATRDY);
        TDES_GetOutputData(pResult);
        if (pVector && cipher) {

            memcpy(pCurrent, pIV, 8);
            memcpy(pIV, pResult, 8);
        }
        else {

            memcpy(pIV, pCurrent, 8);
            memcpy(pCurrent, pResult, 8);
        }
    }

    printf("-I- Output (manual):   ");
    DisplayStream(pResult, 8);
    if (!memcmp(pResult, pOutput, 8)) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");

    // Auto mode
    memset(pResult, 0, 8);
    TDES_Configure(cipher, tdesmod, keymod, AT91C_TDES_SMOD_AUTO, opmod);
    TDES_SetKeys(pKey1, pKey2, pKey3);
    if (pVector) {

        memcpy(pIV, pVector, 8);
    }
    memcpy(pCurrent, pInput, 8);
    for (i=0; i < 10000; i++) {

        if (pVector) {

            TDES_SetVector(pIV);
        }
        TDES_SetInputData(pCurrent);
        while ((TDES_GetStatus() & AT91C_TDES_DATRDY) != AT91C_TDES_DATRDY);
        TDES_GetOutputData(pResult);
        if (pVector && cipher) {

            memcpy(pCurrent, pIV, 8);
            memcpy(pIV, pResult, 8);
        }
        else {

            memcpy(pIV, pCurrent, 8);
            memcpy(pCurrent, pResult, 8);
        }
    }

    printf("-I- Output (auto):     ");
    DisplayStream(pResult, 8);
    if (!memcmp(pResult, pOutput, 8)) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r");
    
    // PDC
    memset(pResult, 0, 8);
    TDES_Configure(cipher, tdesmod, keymod, AT91C_TDES_SMOD_PDC, opmod);
    TDES_SetKeys(pKey1, pKey2, pKey3);
    if (pVector) {

        memcpy(pIV, pVector, 8);
    }
    memcpy(pCurrent, pInput, 8);
    for (i=0; i < 10000; i++) {

        if (pVector) {

            TDES_SetVector(pIV);
        }
        TDES_SetInputBuffer(pCurrent, 8);
        TDES_SetOutputBuffer(pResult, 8);
        TDES_Start();
        while ((TDES_GetStatus() & AT91C_TDES_ENDRX) != AT91C_TDES_ENDRX);
        if (pVector && cipher) {

            memcpy(pCurrent, pIV, 8);
            memcpy(pIV, pResult, 8);
        }
        else {

            memcpy(pIV, pCurrent, 8);
            memcpy(pCurrent, pResult, 8);
        }
    }

    printf("-I- Output (PDC):      ");
    DisplayStream(pResult, 8);
    if (!memcmp(pResult, pOutput, 8)) {

        printf(" (OK)");
    }
    else {

        printf(" (ERROR)");
    }
    printf("\n\r\n\r");
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Default main() function. Initializes the DBGU and writes a string on the
/// DBGU.
//------------------------------------------------------------------------------
int main()
{
    unsigned int pKey1[2];
    unsigned int pKey2[2];
    unsigned int pKey3[2];
    unsigned int pVector[2];
    unsigned int pInput[2];
    unsigned int pOutput[2];

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Triple DES Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Power-up TDES
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
    // Single-DES ECB encrypt
    pKey1[0] = 0x67452301; pKey1[1] = 0xEFCDAB89;
    pInput[0] = 0x20776F4E; pInput[1] = 0x74207369;
    pOutput[0] = 0xF4192A6A; pOutput[1] = 0x4B85CA1E;
    Test(AT91C_TDES_CIPHER,
         AT91C_TDES_OPMOD_ECB,
         pKey1,
         0, 0, 0,
         pInput,
         pOutput);

    // Single-DES ECB decrypt
    pKey1[0] = 0x67452301; pKey1[1] = 0xEFCDAB89;
    pInput[0] = 0x20776F4E; pInput[1] = 0x74207369;
    pOutput[0] = 0x2f4fd6cd; pOutput[1] = 0x5dc12794;
    Test(0,
         AT91C_TDES_OPMOD_ECB,
         pKey1,
         0, 0, 0,
         pInput,
         pOutput);

    // Triple-DES/2 keys ECB encryption
    pKey1[0] = 0x67452301; pKey1[1] = 0xEFCDAB89;
    pKey2[0] = 0x89674523; pKey2[1] = 0x01efcdab;
    pInput[0] = 0x20776F4E; pInput[1] = 0x74207369;
    pOutput[0] = 0x5b9fe603; pOutput[1] = 0x42eb58fa;
    Test(AT91C_TDES_CIPHER,
         AT91C_TDES_OPMOD_ECB,
         pKey1,
         pKey2,
         0, 0,
         pInput,
         pOutput);

    // Triple-DES/2 keys ECB decryption
    pKey1[0] = 0x67452301; pKey1[1] = 0xEFCDAB89;
    pKey2[0] = 0x89674523; pKey2[1] = 0x01efcdab;
    pInput[0] = 0x20776F4E; pInput[1] = 0x74207369;
    pOutput[0] = 0xfac89669; pOutput[1] = 0xebaba247;
    Test(0,
         AT91C_TDES_OPMOD_ECB,
         pKey1,
         pKey2,
         0, 0,
         pInput,
         pOutput);

    // Triple-DES/3 keys ECB encryption
    pKey1[0] = 0x67452301; pKey1[1] = 0xEFCDAB89;
    pKey2[0] = 0x89674523; pKey2[1] = 0x01efcdab;
    pKey3[0] = 0xab896745; pKey3[1] = 0x2301efcd;
    pInput[0] = 0x20776F4E; pInput[1] = 0x74207369;
    pOutput[0] = 0xb8e817dd; pOutput[1] = 0x32d237b4;
    Test(AT91C_TDES_CIPHER,
         AT91C_TDES_OPMOD_ECB,
         pKey1,
         pKey2,
         pKey3,
         0,
         pInput,
         pOutput);

    // Triple-DES/3 keys ECB decryption
    pKey1[0] = 0x67452301; pKey1[1] = 0xEFCDAB89;
    pKey2[0] = 0x89674523; pKey2[1] = 0x01efcdab;
    pKey3[0] = 0xab896745; pKey3[1] = 0x2301efcd;
    pInput[0] = 0x20776F4E; pInput[1] = 0x74207369;
    pOutput[0] = 0x76392583; pOutput[1] = 0x0a1a0944;
    Test(0,
         AT91C_TDES_OPMOD_ECB,
         pKey1,
         pKey2,
         pKey3,
         0,
         pInput,
         pOutput);

    // Single-DES CBC encrypt
    pKey1[0] = 0x67452301; pKey1[1] = 0xefcdab89;
    pVector[0] = 0x78563412; pVector[1] = 0xefcdab90;
    pInput[0] = 0x20776f4e; pInput[1] = 0x74207369;
    pOutput[0] = 0xf65af154; pOutput[1] = 0xb4a4e3eb;
    Test(AT91C_TDES_CIPHER,
         AT91C_TDES_OPMOD_CBC,
         pKey1,
         0, 0,
         pVector,
         pInput,
         pOutput);

    // Single-DES CBC decrypt
    pKey1[0] = 0x67452301; pKey1[1] = 0xefcdab89;
    pVector[0] = 0x78563412; pVector[1] = 0xefcdab90;
    pInput[0] = 0x20776f4e; pInput[1] = 0x74207369;
    pOutput[0] = 0xb9409f12; pOutput[1] = 0xb35600d2;
    Test(0,
         AT91C_TDES_OPMOD_CBC,
         pKey1,
         0, 0,
         pVector,
         pInput,
         pOutput);
    
    // Triple-DES/2 keys CBC encryption
    pKey1[0] = 0x67452301; pKey1[1] = 0xefcdab89;
    pKey2[0] = 0x89674523; pKey2[1] = 0x01efcdab;
    pVector[0] = 0x78563412; pVector[1] = 0xefcdab90;
    pInput[0] = 0x20776f4e; pInput[1] = 0x74207369;
    pOutput[0] = 0x56117635; pOutput[1] = 0x4d8ea15f;
    Test(AT91C_TDES_CIPHER,
         AT91C_TDES_OPMOD_CBC,
         pKey1,
         pKey2,
         0,
         pVector,
         pInput,
         pOutput);
    
    // Triple-DES/2 keys CBC decryption
    pKey1[0] = 0x67452301; pKey1[1] = 0xefcdab89;
    pKey2[0] = 0x89674523; pKey2[1] = 0x01efcdab;
    pVector[0] = 0x78563412; pVector[1] = 0xefcdab90;
    pInput[0] = 0x20776f4e; pInput[1] = 0x74207369;
    pOutput[0] = 0x9afc0e47; pOutput[1] = 0x93e38e6b;
    Test(0,
         AT91C_TDES_OPMOD_CBC,
         pKey1,
         pKey2,
         0,
         pVector,
         pInput,
         pOutput);
    
    // Triple-DES/3 keys CBC encryption
    pKey1[0] = 0x67452301; pKey1[1] = 0xefcdab89;
    pKey2[0] = 0x89674523; pKey2[1] = 0x01efcdab;
    pKey3[0] = 0xab896745; pKey3[1] = 0x2301efcd;
    pVector[0] = 0x78563412; pVector[1] = 0xefcdab90;
    pInput[0] = 0x20776f4e; pInput[1] = 0x74207369;
    pOutput[0] = 0x851f19cb; pOutput[1] = 0x3984edd1;
    Test(AT91C_TDES_CIPHER,
         AT91C_TDES_OPMOD_CBC,
         pKey1,
         pKey2,
         pKey3,
         pVector,
         pInput,
         pOutput);
    
    // Triple-DES/3 keys CBC decryption
    pKey1[0] = 0x67452301; pKey1[1] = 0xefcdab89;
    pKey2[0] = 0x89674523; pKey2[1] = 0x01efcdab;
    pKey3[0] = 0xab896745; pKey3[1] = 0x2301efcd;
    pVector[0] = 0x78563412; pVector[1] = 0xefcdab90;
    pInput[0] = 0x20776f4e; pInput[1] = 0x74207369;
    pOutput[0] = 0x63cfcec5; pOutput[1] = 0x4c51ecec;
    Test(0,
         AT91C_TDES_OPMOD_CBC,
         pKey1,
         pKey2,
         pKey3,
         pVector,
         pInput,
         pOutput);

    return 0;
}

