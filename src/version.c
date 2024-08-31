// Copyright (c) 2024 Ziga Miklosic
// All Rights Reserved
// This software is under MIT licence (https://opensource.org/licenses/MIT)
////////////////////////////////////////////////////////////////////////////////
/*!
*@file      version.h
*@brief     Revision of software & hardware
*@author    Ziga Miklosic
*@date      15.02.2024
*@version   V1.4.0
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
#include <stdio.h>
#include "version.h"

#if ( 1 == VER_CFG_USE_PROJ_INFO_EN )
#include "proj_info.h"
#endif

#if ( 1 == VER_CFG_BOOT_PRESENT )
    #include "middleware/boot/boot/src/boot.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#if ( 1 == VER_CFG_BOOT_PRESENT )
    /**
     *  Compatibility check with BOOT module
     *
     *  Support version V0.2.x up
     */

    // TODO: Update compatibility checks!
    _Static_assert( 0 == BOOT_VER_MAJOR );
    _Static_assert( 2 <= BOOT_VER_MINOR );
#endif

/**
 *  Application header version
 */
#define VER_APP_HEADER_VER          ( 1 )

/**
 *     Convert value to string
 */
#define VER_STR_HELPER(x)           #x
#define VER_STR(x)                  VER_STR_HELPER(x)

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 *     Software string and number
 */
static const char *     gs_sw_ver_str   = "Software Version " VER_STR(VER_SW_MAJOR) "." VER_STR(VER_SW_MINOR) "." VER_STR(VER_SW_DEVELOP) "." VER_STR(VER_SW_TEST) " (Builted on " __DATE__ " " __TIME__ ")";
static const uint32_t   gu32_sw_ver_num = (( VER_SW_MAJOR << 24 ) | ( VER_SW_MINOR << 16 ) | ( VER_SW_DEVELOP << 8 ) | ( VER_SW_TEST ));

/**
 *  Hardware string and number
 */
static const char *     gs_hw_ver_str   = "Hardware Version " VER_STR(VER_HW_MAJOR) "." VER_STR(VER_HW_MINOR) "." VER_STR(VER_HW_DEVELOP) "." VER_STR(VER_HW_TEST);
static const uint32_t   gu32_hw_ver_num = (( VER_HW_MAJOR << 24 ) | ( VER_HW_MINOR << 16 ) | ( VER_HW_DEVELOP << 8 ) | ( VER_HW_TEST ));

/**
 *     Application header informations
 */
static volatile const ver_app_header_t __attribute__ (( section( VER_APP_HEAD_SECTION ))) g_app_header =
{
    .ctrl =
    {
        .crc = 0U,                       /**<Filled by post-build script */
        .ver = VER_APP_HEADER_VER,
    },

    .data =
    {
   		.sw_ver     = gu32_sw_ver_num,
   		.hw_ver     = gu32_hw_ver_num,
   		.app_size   = 0U,               /**<Filled by post-build script */
   		.app_crc    = 0U,               /**<Filled by post-build script */
   		.enc_type   = 0U,               /**<Filled by post-build script */
   		.sig_type   = 0U,               /**<Filled by post-build script */
    },
};

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Get software version
 *
 * @return      Application software version
 */
////////////////////////////////////////////////////////////////////////////////
ver_t version_get_sw(void)
{
    const ver_t sw_ver = { .U = gu32_sw_ver_num };
    return sw_ver;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Get hardware version
 *
 * @return      Hardware version
 */
////////////////////////////////////////////////////////////////////////////////
ver_t version_get_hw(void)
{
    const ver_t hw_ver = { .U = gu32_hw_ver_num };
    return hw_ver;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Get bootloader software version
 *
 * @return      Bootloader SW version
 */
////////////////////////////////////////////////////////////////////////////////
ver_t version_get_boot(void)
{
    ver_t boot_ver = { .U = 0 };

    #if ( 1 == VER_CFG_BOOT_PRESENT )
        if ( eBOOT_OK != boot_shared_mem_get_boot_ver( &boot_ver.U ))
        {
            boot_ver.U = 0;
        }
    #endif

    return boot_ver;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Get SW version string
 *
 * @return  gs_sw_ver_str - Software version string
 */
////////////////////////////////////////////////////////////////////////////////
const char* version_get_sw_str(void)
{
    return gs_sw_ver_str;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Get HW version string
 *
 * @return  gs_hw_ver_str - Software version string
 */
////////////////////////////////////////////////////////////////////////////////
const char* version_get_hw_str(void)
{
    return gs_hw_ver_str;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Get bootloader SW version string
 *
 * @return  gs_hw_ver_str - Software version string
 */
////////////////////////////////////////////////////////////////////////////////
const char* version_get_boot_str(void)
{
    const ver_t boot_ver = version_get_boot();
    static char boot_ver_str[64] = "Bootloader not present!";

    if ( 0 != boot_ver.U )
    {
        // Assembly boot version string
        snprintf(boot_ver_str, sizeof(boot_ver_str), "Bootloader (SW) Version %d.%d.%d.%d", boot_ver.maj, boot_ver.min, boot_ver.dev, boot_ver.test );
    }

    return boot_ver_str;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Get project detailed information string
 *
 * @return proj_info_str - Project info string
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
