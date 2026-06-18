
        PUBLIC __FlashHeader0_CRC
        PUBLIC __FlashHeader1_CRC
        PUBLIC __FW_CRC
        PUBLIC __Patch_CRC
        PUBLIC __IMAGE_0_CRC

        EXTERN FW_Block_Holder$$Limit
        EXTERN FW_Alignment$$Base

        EXTERN Patch_Block_Holder$$Limit
        EXTERN Patch_Alignment$$Base
        
        EXTERN IMAGE_0_Block_Holder$$Limit
        EXTERN IMAGE_0_Alignment$$Base
        
        EXTERN PROC_STACK$$Base
        EXTERN PROC_STACK$$Length
        EXTERN __iar_program_start
        
        ; Declare all loadable sections here
        SECTION FW_Header_Start:CONST
        SECTION FW_Header_End:CONST
        SECTION FW_Image:CONST
        SECTION Patch_Image:CONST
        SECTION IMAGE_0_Holder:CONST
        
        SECTION FlashHeader0:CONST:ROOT(2)
        DATA

        ; Atmel SAMB11 bootloader SPI Flash table.
        ; This table is required by the bootloader in order to successfully load
        ; a user program from the external SPI flash to RAM at startup.
        ; If the table is corrupted, the user images will not be loaded into RAM.

        ; Images are placed in continers referred to as sections by the linker.
        ; This is not to be confused with the term section used in the bootloader 
        ; table specification by Atmel
        ; CRC32 checksums must be calculated for the flash header and for each image.
        ; Checksums can be calculated using ielftool and patched into the elf file.

__FlashHeader0: ; Flash header
        DCD 0x40000  ; Total flash size
        DCD 0x3       ; Number of loadable sections
        
        ; Section table begins here
        DCD FW_Block_Holder$$Limit - 0x1000 ; Image size for this section, including image header data.
                                         ; All images within the section must be of the same size
        DCD 0x1                          ; Number of images in this loadable section
 
        DCD Patch_Block_Holder$$Limit - 0x4000 ; Image size for this section. 
                                         ; All images within the section must be of the same size
        DCD 0x1                          ; Number of images in this loadable section
        
        DCD IMAGE_0_Block_Holder$$Limit - 0x5000 ; Image size for this section. 
                                         ; All images within the section must be of the same size       
        DCD 0x1                          ; Number of images in this loadable section
        ; More table entries here...
        
__FlashHeader0_CRC:
        DCD 0x0 ; The flash header's CRC32 checksum. Place manually or by ielftool
        
        
        SECTION FlashHeader1:CONST:ROOT(2)
        DATA
        
__FlashHeader1: ; Duplicate of the first header
        DCD 0x40000  ; Total flash size
        DCD 0x3       ; Number of loadable sections
        
        ; Section table begins here
        DCD FW_Block_Holder$$Limit - 0x1000 ; Image size for this section, including image header data.
                                         ; All images within the section must be of the same size
        DCD 0x1                          ; Number of images in this loadable section
        
        DCD Patch_Block_Holder$$Limit - 0x4000 ; Image size for this section. 
                                         ; All images within the section must be of the same size
        DCD 0x1                          ; Number of images in this loadable section
        
        DCD IMAGE_0_Block_Holder$$Limit - 0x5000 ; Image size for this section. 
                                         ; All images within the section must be of the same size    
        DCD 0x1                          ; Number of images in this loadable section
        ; More table entries here...
        
__FlashHeader1_CRC:
        DCD 0x0 ; The flash header's CRC32 checksum. Place manually or by ielftool
        
        ; 0-padding must be added here until the end of the flash header section
        
        ; Align the loadable section area to start at the beginning 
        ; of the next flash sector at 0x1000
        
        ; This section should be filled with contents matching the specification of
        ; of the type loadable_Section_t as specified by Atmel.
        

        SECTION FW_Header_Start:CONST:ROOT(2)
        DATA
        REQUIRE FW_Alignment$$Base
        
__FWImageSection:  

        ; A loadable section containing a FW patch image
        DCD 0x1  ; Version 1
        DCD 0x2E6C ; Image size 
        
        ; First entry
        DCD 0x2D30 ; Entry length
        DCD 0x10005184 ; Entry base address
        
        SECTION FW_Image:CONST:ROOT(2)
        DATA
        
        SECTION FW_Header_End:CONST:ROOT(2)
        DATA
        
__FW_CRC:
        DCD 0x0 ; The FW images' CRC32 checksum. Place manually or by ielftool
    
        SECTION Patch_Image:CONST:ROOT(2)
        DATA
        REQUIRE Patch_Alignment$$Base
        
__PatchImageSection:
        
        ; A loadable section containing SAMB11 data patches
        DCD 0x1  ; Version 1
        DCD 0x3C ; Image size 
        
        ; Entry 1
        DCD 0x8  ; Entry length
        DCD 0x10040018 ; Entry base address
        DCD 0x5 ; Data
        
        ; Entry 2
        DCD 0x8  ; Entry length
        DCD 0x10040030 ; Entry base address
        DCD __iar_program_start - 1 ; Data  ;; Application start address
        
        ; Entry 3
        DCD 0x8  ; Entry length
        DCD 0x10040028 ; Entry base address
        DCD PROC_STACK$$Base ; Data             ;; Application stack start address
        
        ; Entry 4
        DCD 0x8  ; Entry length
        DCD 0x1004002C ; Entry base address
        DCD PROC_STACK$$Length ; Data           ;; Application stack size (bytes)
        
        ; Entry 5
        DCD 0x8  ; Entry length
        DCD 0x1004001C ; Entry base address
        DCD 0x101 ; Data                    ;; Some other parameter
        
        __Patch_CRC:
        DCD 0x0 ; The patch images' CRC32 checksum. Place manually or by ielftool
    

        SECTION IMAGE_0_Header_Start:CONST:ROOT(2)
        DATA
        REQUIRE IMAGE_0_Alignment$$Base
        
__UserAppSection:  

        ; A loadable section containing a user application image
        DCD 0x1  ; Version 1
        DCD SIZEOF(IMAGE_0_Holder) + 0x8 ; Image size 
        
        ; First entry
        DCD SIZEOF(IMAGE_0_Holder) + 0x4  ; Entry length

        DCD 0x10008000 ; Entry base address
        
        SECTION IMAGE_0_Holder:CONST:ROOT(2)
        DATA    
        
        SECTION IMAGE_0_Header_End:CONST:ROOT(2)
        DATA

        __IMAGE_0_CRC:
        DCD 0x0 ; The FW images' CRC32 checksum. Place manually or by ielftool
        
        END