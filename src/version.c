////////////////////////////////////////////////////////////////////////////////
/*!
*@file      version.h
*@brief     Revision of software & hardware
*@author    Ziga Miklosic
*@date      20.04.2022
*@project	Angle simulator
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
 * 	Software version definitions
 */
#define VERSION_SW( maj, min, dev, tst ) \
\
static const char *		gs_sw_ver_str 			= "Software Version " #maj "." #min "." #dev "." #tst "\rBuilted " __DATE__ " " __TIME__;		\
static const uint32_t 	gu32_sw_ver_num			= (( maj << 24 ) | ( min << 16 ) | ( dev << 8 ) | ( tst ));

/**
 * 	Hardware version definitions
 */
#define VERSION_HW( maj, min, dev, tst )  \
\
static const char *		gs_hw_ver_str	 		= "Hardware Version " #maj "." #min "." #dev "." #tst "\r";		\
static const uint32_t 	gu32_hw_ver_num 		= (( maj << 24 ) | ( min << 16 ) | ( dev << 8 ) | ( tst ));

/**
 * 	Set SW version
 */
VERSION_SW( VER_SW_MAJOR, VER_SW_MINOR, VER_SW_DEVELOP, VER_SW_TEST );

/**
 * 	Set HW version
 */
VERSION_HW( VER_HW_MAJOR, VER_HW_MINOR, VER_HW_DEVELOP, VER_HW_TEST );


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
