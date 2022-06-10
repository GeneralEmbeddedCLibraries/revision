// Copyright (c) 2022 Ziga Miklosic
// All Rights Reserved
// This software is under MIT licence (https://opensource.org/licenses/MIT)
////////////////////////////////////////////////////////////////////////////////
/*!
*@file      version.h
*@brief     Revision of software & hardware
*@author    Ziga Miklosic
*@date      25.05.2022
*@version	V1.1.0
*/
////////////////////////////////////////////////////////////////////////////////
/*!
 * @addtogroup VERSION
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "../../version_cfg.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * 	Convert value to string
 */
#define VER_STR_HELPER(x) 		#x
#define VER_STR(x) 				VER_STR_HELPER(x)

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

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * 	Software string and number
 */
static const char *		gs_sw_ver_str 			= "Software Version " VER_STR(VER_SW_MAJOR) "." VER_STR(VER_SW_MINOR) "." VER_STR(VER_SW_DEVELOP) "." VER_STR(VER_SW_TEST) "\rBuilted " __DATE__ " " __TIME__;
static const uint32_t 	gu32_sw_ver_num			= (( VER_SW_MAJOR << 24 ) | ( VER_SW_MINOR << 16 ) | ( VER_SW_DEVELOP << 8 ) | ( VER_SW_TEST ));

/**
 * 	Application header informations
 */
static volatile const ver_app_header_t __attribute__ (( section( VER_APP_HEAD_SECTION ))) g_app_header =
{
	.signature 	= VER_APP_HEAD_SIGN,
	.sw_major	= VER_SW_MAJOR,
	.sw_minor	= VER_SW_MINOR,
	.sw_dev		= VER_SW_DEVELOP,
	.sw_test	= VER_SW_TEST,
	.app_size	= 0,	/*Calculated by post-build script */
	.app_crc	= 0,	/*Calculated by post-build script */
};

/**
 * 	Hardware string and number
 */
static const char *		gs_hw_ver_str	 		= "Hardware Version " VER_STR(VER_HW_MAJOR) "." VER_STR(VER_HW_MINOR) "." VER_STR(VER_HW_DEVELOP) "." VER_STR(VER_HW_TEST) "\r";
static const uint32_t 	gu32_hw_ver_num 		= (( VER_HW_MAJOR << 24 ) | ( VER_HW_MINOR << 16 ) | ( VER_HW_DEVELOP << 8 ) | ( VER_HW_TEST ));

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * 	@brief		Get software version
 *
 * @param[out]	p_major		- Pointer to major SW version
 * @param[out]	p_minor		- Pointer to minor SW version
 * @param[out]	p_develop	- Pointer to develop SW version
 * @param[out]	p_test		- Pointer to test SW version
 * @return 		sw_ver		- Complete 32-bit number of SW version
 */
////////////////////////////////////////////////////////////////////////////////
uint32_t version_get_sw(uint8_t * const p_major, uint8_t * const p_minor, uint8_t * const p_develop, uint8_t * const p_test)
{
	const uint32_t sw_ver = gu32_sw_ver_num;

	if ( NULL != p_major )
	{
		*p_major = (( gu32_sw_ver_num >> 24UL ) & 0xFFUL );
	}
	if ( NULL != p_minor )
	{
		*p_minor = (( gu32_sw_ver_num >> 16UL ) & 0xFFUL );
	}
	if ( NULL != p_develop )
	{
		*p_develop = (( gu32_sw_ver_num >> 8UL ) & 0xFFUL );
	}
	if ( NULL != p_test )
	{
		*p_test = (( gu32_sw_ver_num >> 0UL ) & 0xFFUL );
	}

	return sw_ver;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * 	@brief		Get hardware version
 *
 * @param[out]	p_major		- Pointer to major HW version
 * @param[out]	p_minor		- Pointer to minor HW version
 * @param[out]	p_develop	- Pointer to develop HW version
 * @param[out]	p_test		- Pointer to test HW version
 * @return 		sw_ver		- Complete 32-bit number of HW version
 */
////////////////////////////////////////////////////////////////////////////////
uint32_t version_get_hw(uint8_t * const p_major, uint8_t * const p_minor, uint8_t * const p_develop, uint8_t * const p_test)
{
	const uint32_t hw_ver = gu32_hw_ver_num;

	if ( NULL != p_major )
	{
		*p_major = (( gu32_hw_ver_num >> 24UL ) & 0xFFUL );
	}
	if ( NULL != p_minor )
	{
		*p_minor = (( gu32_hw_ver_num >> 16UL ) & 0xFFUL );
	}
	if ( NULL != p_develop )
	{
		*p_develop = (( gu32_hw_ver_num >> 8UL ) & 0xFFUL );
	}
	if ( NULL != p_test )
	{
		*p_test = (( gu32_hw_ver_num >> 0UL ) & 0xFFUL );
	}

	return hw_ver;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * 	@brief		Get SW version string
 *
 * @return 		gs_sw_ver_str - Software version string
 */
////////////////////////////////////////////////////////////////////////////////
const char*	version_get_sw_str(void)
{
	return gs_sw_ver_str;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * 	@brief		Get HW version string
 *
 * @return 		gs_hw_ver_str - Software version string
 */
////////////////////////////////////////////////////////////////////////////////
const char*	version_get_hw_str(void)
{
	return gs_hw_ver_str;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
