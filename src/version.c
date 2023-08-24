// Copyright (c) 2023 Ziga Miklosic
// All Rights Reserved
// This software is under MIT licence (https://opensource.org/licenses/MIT)
////////////////////////////////////////////////////////////////////////////////
/*!
*@file      version.h
*@brief     Revision of software & hardware
*@author    Ziga Miklosic
*@date      24.08.2022
*@version   V1.3.0
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

#if ( 1 == VER_CFG_USE_PROJ_INFO_EN )
#include "proj_info.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 *  Application header version
 */
#define VER_APP_HEADER_VER          ( 1 )

/**
 * 	Convert value to string
 */
#define VER_STR_HELPER(x) 		#x
#define VER_STR(x) 				VER_STR_HELPER(x)

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * 	Software string and number
 */
static const char *		gs_sw_ver_str 			= "Software Version " VER_STR(VER_SW_MAJOR) "." VER_STR(VER_SW_MINOR) "." VER_STR(VER_SW_DEVELOP) "." VER_STR(VER_SW_TEST) " (Builted on " __DATE__ " " __TIME__ ")";
static const uint32_t 	gu32_sw_ver_num			= (( VER_SW_MAJOR << 24 ) | ( VER_SW_MINOR << 16 ) | ( VER_SW_DEVELOP << 8 ) | ( VER_SW_TEST ));

/**
 *  Hardware string and number
 */
static const char *     gs_hw_ver_str           = "Hardware Version " VER_STR(VER_HW_MAJOR) "." VER_STR(VER_HW_MINOR) "." VER_STR(VER_HW_DEVELOP) "." VER_STR(VER_HW_TEST);
static const uint32_t   gu32_hw_ver_num         = (( VER_HW_MAJOR << 24 ) | ( VER_HW_MINOR << 16 ) | ( VER_HW_DEVELOP << 8 ) | ( VER_HW_TEST ));


/**
 * 	Application header informations
 */
static volatile const ver_app_header_t __attribute__ (( section( VER_APP_HEAD_SECTION ))) g_app_header =
{
    .sw_ver     = gu32_sw_ver_num,
	.hw_ver     = gu32_hw_ver_num,
	.app_size	= 0,	                /* Calculated by post-build script */
	.app_crc	= 0,	                /* Calculated by post-build script */
	.ver        = VER_APP_HEADER_VER,
	.crc        = 0,                    /* Calculated by post-build script */
};


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
		*p_major = (uint8_t)(( gu32_sw_ver_num >> 24U ) & 0xFFU );
	}
	if ( NULL != p_minor )
	{
		*p_minor = (uint8_t)(( gu32_sw_ver_num >> 16U ) & 0xFFU );
	}
	if ( NULL != p_develop )
	{
		*p_develop = (uint8_t)(( gu32_sw_ver_num >> 8U ) & 0xFFU );
	}
	if ( NULL != p_test )
	{
		*p_test = (uint8_t)(( gu32_sw_ver_num >> 0U ) & 0xFFU );
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
		*p_major = (uint8_t)(( gu32_hw_ver_num >> 24U ) & 0xFFU );
	}
	if ( NULL != p_minor )
	{
		*p_minor = (uint8_t)(( gu32_hw_ver_num >> 16U ) & 0xFFU );
	}
	if ( NULL != p_develop )
	{
		*p_develop = (uint8_t)(( gu32_hw_ver_num >> 8U ) & 0xFFU );
	}
	if ( NULL != p_test )
	{
		*p_test = (uint8_t)(( gu32_hw_ver_num >> 0U ) & 0xFFU );
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
/**
 * 	@brief		Get project detailed information string
 *
 * @return 		proj_info_str - Project info string
 */
////////////////////////////////////////////////////////////////////////////////
const char* version_get_proj_info_str(void)
{
	const char* proj_info_str = NULL;

	#if ( 1 == VER_CFG_USE_PROJ_INFO_EN )
		proj_info_str = proj_info_get_str();
	#else
		proj_info_str = "Not used...";
	#endif

	return proj_info_str;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
