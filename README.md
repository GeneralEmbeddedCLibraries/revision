# **Revision**
Revision module provides device information in form of SW & HW version, application header and additional project informations. 

## **Dependencies**
This module needs only ANSI C standard libraries. All programs needed for addtional project information generation are provided within same repository. There is no need for any separate installation of tools.

## **General Embedded C Libraries Ecosystem**
In order to be part of *General Embedded C Libraries Ecosystem* this module must be placed in following path: 

```
root/revision/revision/"module_space"
```

## **API**
| API Functions | Description | Prototype |
| --- | ----------- | ----- |
| **version_get_sw**            | Get SW version                  | uint32_t version_get_sw(uint8_t * const p_major, uint8_t * const p_minor, uint8_t * const p_develop, uint8_t * const p_test) |****
| **version_get_hw**            | Get HW version                  | uint32_t version_get_hw(uint8_t * const p_major, uint8_t * const p_minor, uint8_t * const p_develop, uint8_t * const p_test) |
| **version_get_sw_str**        | Get SW versioning string        | const char* version_get_sw_str(void) |
| **version_get_hw_str**        | Get HW versioning string        | const char* version_get_hw_str(void) |
| **version_get_proj_info_str** | Get project information string  | const char* version_get_proj_info_str(void) |

## **Define Software & Hardware version**
Software and hardware version can be changed in configuration file ***ver_cfg.h***:

```C
/******************************************************************************
 * 	SET SOFTWARE VERSION
 *****************************************************************************/
#define VER_SW_MAJOR      0
#define VER_SW_MINOR      5
#define VER_SW_DEVELOP    1
#define VER_SW_TEST       0

/******************************************************************************
 * 	SET HARDWARE VERSION
 *****************************************************************************/
#define VER_HW_MAJOR      1
#define VER_HW_MINOR      0
#define VER_HW_DEVELOP    1
#define VER_HW_TEST       0
```

In embedded world there is very tight connection between HW and SW. Knowing what kind of software is compatible with any specific hardware version is critical. Therefore part of revision module is also HW version. Based on that informations one can derive compatibility of software and hardware. 

### **Version notation guideline**
It is prefered to used "Semantic Versioning" (more about that [here](https://semver.org/spec/v2.0.0.html)). Brief description of version notation:
 - MAJOR: Change when code is not back-compatable due to API change
 - MINOR: Change after adding new features and code must be still back-compatable
 - DEVELOP: Change after fixing minor bugs and code must be still back-compatable
 - TEST: Change after providing a special (usally for internal use only) SW in order to validate system design. Of course code must be still back-compatable

## **Application header**
Application header contains information about SW version, application size and output image (.hex file) CRC value. Therefore it can be used for data integrity validation. Checksum and application size makes end build application more suitable for bootloader support. Additionally SW version of running application can be acquired easily by looking into outputed .hex file at specific location all thanks to application header.

### Application header structure:

![](doc/pic/Application_Header_Structure_V1.png)

### Definition of application header:
```C
/**
 *  Application header
 *
 * @note    Purpose of application header is to store informations
 *          of software in HEX output file at specific location. This
 *          gives you the insights of the application itself by looking
 *          only into output file such as Intel HEX type or binary.
 *
 *  Size: 256 bytes
 */
typedef struct __VER_PACKED__
{
    /**     Control fields
     *
     *  Sizeof: 8 bytes
     *
     *  @note   Are fixed, shall not be change during different versions of application header
     */
    struct
    {
        uint8_t     crc;                /**<Application header CRC8 */
        uint8_t     ver;                /**<Application header version */
        uint8_t     res[6];             /**<Reserved fields */
    } ctrl;

    /**     Data fields
     *
     *  Sizeof: 248 bytes
     *
     *  @note   Data fields can be re-sized between different versions of application header.
     */
    struct
    {
        uint32_t sw_ver;        /**<Software (application) version */
        uint32_t hw_ver;        /**<Hardware version */
        uint32_t app_size;      /**<Size of application in bytes - shall be calculated by post-build script */
        uint32_t app_crc;       /**<Application CRC32 - calculated by post-build script */
        uint8_t  enc_type;      /**<Encryption type. Shall be value of @ver_enc_type_t. Filled by post-build script */
        uint8_t  sig_type;      /**<Signature type. Shall be value of @ver_sig_type_t. Filled by post-build script */
        uint8_t  sig_hash[32];  /**<Signature hash value. Filled by post-build script */
        uint8_t  res[198];      /**<Reserved space in application header */
    } data;
} ver_app_header_t;
```

## **Configuration for arm-gcc toolchain**
### **Adding memory layout**

In order to put application header at specified location new memory layout needs to be created. When using arm-gcc based toolchain this is done in ***.ld*** file. For example, when using STM32F103 this file is named *STM32F103VETX_FLASH.ld*

Add new section to memory layout:
```
/*******************************************************************************
*		USER DEFINED MEMORY LOCATIONS 
*******************************************************************************/

/* Application header address & size */
APP_HEADER_ADDR	= 0x08020000;
APP_HEADER_SIZE	= 256;	/*bytes*/

/* Build informations address & size */
PROJ_INFO_ADDR	= ( 0x08020000 + APP_HEADER_SIZE );
PROJ_INFO_SIZE	= 2048;	/*bytes*/

/* Total size of all user define regions */
USER_REGION_SIZE = ( APP_HEADER_SIZE + PROJ_INFO_SIZE );


/* Memories definition */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   		LENGTH = 64K
  FLASH   (rx)    : ORIGIN = 0x08000000,   		LENGTH = 512K-USER_REGION_SIZE
  APP_HEADER (r)  : ORIGIN = APP_HEADER_ADDR, LENGTH = APP_HEADER_SIZE			
  PROJ_INFO (r)   : ORIGIN = PROJ_INFO_ADDR,  LENGTH = PROJ_INFO_SIZE		
}
```
NOTE: Don't forget to reduce size of FLASH memory in order to fit in user defined memory regions!

### **Creating memory section**
Under sections inside linker script add new section for application header. Note that name of symbol inside section must match value of ***VER_APP_HEAD_SECTION*** specified inside ***ver_cfg.h***.

1. Add new section inside linker script:
```
/* Sections */
SECTIONS
{

    /* A lot of stuff before... */


    /* Application header section */
    .app_header :
    {
    APP_HEADER_START = .;
    *(.app_header)				/*Application header */
    *(.app_header*)				/*Application header */
    KEEP (*(.app_header*))
    APP_HEADER_END = .;
    } >APP_HEADER


    /* A lot of stuff after... */
}

```

2. Change configuration inside ***ver_cfg.h*** to match linker settings:
```C
/**
 * 	Section name of app header
 */
#define VER_APP_HEAD_SECTION			( ".app_header" )
```

## **Project Informations**
Project informations contains details about IDE configurations, PC host and current git worktree status. Such an informations can be very usefull later on, as it can be obtained from device in run-time or even by inspecting release output files such as Intel HEX. Therefore this approach can shorten time to find a bug and provide a higher level of code traceability.

As an output to project information process two C code files are created: ***proj_info.c*** and
***proj_info.h***. Detailed informations are created in string format and can be found in generated source file. 

Next section describe steps how to achieve automation of project information creation. In case project information string can be placed anywhere in the memory, step number 2 can be left out.

### **1. Change config file**
Enable project information generation by setting **VER_CFG_USE_PROJ_INFO_EN** to 1.

```C
/**
 * 	Enable/Disable project informations
 */
#define VER_CFG_USE_PROJ_INFO_EN		( 1 )
```

### **2. Creating memory section**
Under sections inside linker script add new section for project informations. Note that name of symbol inside section must match value of ***VER_APP_PROJ_INFO_SECTION*** specified inside ***ver_cfg.h***.

2.1 Add new section inside linker script:
```
/* Sections */
SECTIONS
{

    /* A lot of stuff before... */


    /* Projec information section */
  .proj_info :
  {
	PROJ_INFO_START = .;
	*(.proj_info)				/*Project info string*/
	*(.proj_info*)				/*Project info string */
	KEEP (*(.proj_info*))
	FILL(0x0000);
	PROJ_INFO_END = .;
  } >PROJ_INFO


    /* A lot of stuff after... */
}

```

2.2 Change configuration inside ***ver_cfg.h*** to match linker settings:
```C
/**
 * 	Project info section
 */
#define VER_APP_PROJ_INFO_SECTION 		( ".proj_info" )

```

### **3. Setup pre-build task**

This example is applicable when using STM32CubeIDE, but it should gives you a guidence to other vendors IDEs.
In order to automate process of generate project information go to: *Properties->C/C++Build->Settings->BuildSteps*

Paste folowing command under Pre-Build steps and change only root name path:
```
../my_src/revision/revision/utils/delivery/proj_info.exe -f ../my_src/revision/revision/src/proj_info.c -n ${ProjName} -c ${ConfigName}  -pc ${COMPUTERNAME} -os '${HostOsName}'
```

**NOTICE: Output generated file location and name must not be changed!**


---
### **Example of Automatically Generated Output files**

#### **Source file**
```C
// Copyright (c) 2022 Ziga Miklosic
// All Rights Reserved
// This software is under MIT licence (https://opensource.org/licenses/MIT)
////////////////////////////////////////////////////////////////////////////////
/*!
*@file      proj_info.c
*@brief     Project informations
*@author    Ziga Miklosic
*@date      13.06.2022
*@time      18:14:52
*
*@note     This is automatically generated file!
*/
////////////////////////////////////////////////////////////////////////////////
/*!
 * @addtogroup PROJ_INFO
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdlib.h>

#include "proj_info.h"
#include "../../version_cfg.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 *   Project information string
 */
static volatile const char __attribute__ (( section( VER_APP_PROJ_INFO_SECTION ))) gs_proj_info[VER_APP_PROJ_INFO_SIZE] = "\
=================================================\r\n\
	PROJECT INFORMATIONS\r\n\
  generated by ""proj_info.py""  V0.0.1\r\n\
=================================================\r\n\
 Project name: Wardrobe_Lighting_SW\r\n\
 Build config: Debug\r\n\
      PC name: ZIGAMIKLOSIC\r\n\
      Host OS: Windows 10\r\n\
       Author: zigam\r\n\
        Email: ziga.miklosic@gmail.com\r\n\
.\r\n\
*** Git  informations ***\r\n\
       Origin: git@github.com:ZiGaMi/Wardrobe_Lighting_SW.git\r\n\
       Branch: feature/build_info_testing\r\n\
   Commit SHA: 7cd03fc\r\n\
=================================================\r\n\
";

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * 	@brief		Get build info string
 *
 * @return 		gs_proj_info - Project information string
 */
////////////////////////////////////////////////////////////////////////////////
const char* proj_info_get_str(void)
{
    return (const char*) gs_proj_info;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
```

#### **Header file**
```C
// Copyright (c) 2022 Ziga Miklosic
// All Rights Reserved
// This software is under MIT licence (https://opensource.org/licenses/MIT)
////////////////////////////////////////////////////////////////////////////////
/*!
*@file      proj_info.h
*@brief     Project informations
*@author    Ziga Miklosic
*@date      13.06.2022
*@time      18:14:52
*
*@note     This is automatically generated file!
*/
////////////////////////////////////////////////////////////////////////////////
/*!
 * @addtogroup PROJ_INFO
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
#ifndef __PROJ_INFO_H_
#define __PROJ_INFO_H_ 

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
 
////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
const char* proj_info_get_str(void);

#endif // __PROJ_INFO_H_

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
```
