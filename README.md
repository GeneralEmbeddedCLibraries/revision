# **Revision**
Revision module provides device information in form of SW & HW version and application header. 

## **Define SW&HW version**
Software and hardware version can be changed in configuration file ***ver_cfg.h***:

```C
/******************************************************************************
 * 	SET SOFTWARE VERSION
 *****************************************************************************/
#define VER_SW_MAJOR			( 0 )
#define VER_SW_MINOR			( 5 )
#define VER_SW_DEVELOP			( 1 )
#define VER_SW_TEST				( 0 )

/******************************************************************************
 * 	SET HARDWARE VERSION
 *****************************************************************************/
#define VER_HW_MAJOR			( 1 )
#define VER_HW_MINOR			( 0 )
#define VER_HW_DEVELOP			( 1 )
#define VER_HW_TEST				( 0 )
```

### **Version notation guideline**
It is prefered to used "Semantic Versioning" (more about that [here](https://semver.org/spec/v2.0.0.html)). Brief description of version notation:
 - MAJOR: Change when code is not back-compatable due to API change
 - MINOR: Change after adding new features and code must be still back-compatable
 - DEVELOP: Change after fixing minor bugs and code must be still back-compatable
 - TEST: Change after providing a special (usally for internal use only) SW in order to validate system design


## **Application header**
Application header contains information about SW version, application size and output image (.hex file) CRC value. Therefore it can be used for data integrity validation. Checksum and application size makes end build application more suitable for bootloader support. Additionally SW version of running application can be acquired easily by looking into outputed .hex file at specific location all thanks to application header.

Definition of application header:
```C
/**
 * 	Application header
 *
 * @note	Purpose of application header is to store informations
 * 			of software in HEX output file at specific location. This
 * 			gives you the insights of the application itself by looking
 * 			only into output file of Intel HEX type.
 *
 *
 * 	Size: 16 bytes
 */
typedef struct
{
	uint32_t 	signature;		/**<Signature - to validate if header is valid */
	uint8_t 	sw_major;		/**<Major SW version */
	uint8_t 	sw_minor;		/**<Minor SW version */
	uint8_t 	sw_dev;			/**<Develop SW version */
	uint8_t 	sw_test;		/**<Test SW version */
	uint32_t	app_size;		/**<Size of application - shall be calculated by post-build script */
	uint32_t	app_crc;		/**<Application CRC32 - calculated by post-build script */
} ver_app_header_t;
```

## **Configuration for arm-gcc toolchain**
### **Adding memory layout**

In order to put application header at specified location new memory layout needs to be created. When using arm-gcc based toolchain this is done in ***.ld*** file. For example, when using STM32F103 this file is named *STM32F103VETX_FLASH.ld*

Add new section to memory layout:
```
/* Memories definition */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 64K
  FLASH   (rx)    : ORIGIN = 0x08000000,   LENGTH = 512K-16
  APP_HEADER (r)  : ORIGIN = 0x08020000,   LENGTH = 16			/* Application header */
}
```
NOTE: Don't forget to reduce size of FLASH memory in order to fit in application header!

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

## Post-Build script
TBD: ...


