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
 *      Image header version
 */
#define VER_IMAGE_HEADER_VER          ( 1 )

/**
 *     Convert value to string
 */
#define VER_STR_HELPER(x)           #x
#define VER_STR(x)                  VER_STR_HELPER(x)

/**
 *      Image header validation results
 */
typedef enum
{
    eVER_RESULT_NONE = 0,   /**<Validation not jet performed */
    eVER_RESULT_VALID,      /**<Image header valid */
    eVER_RESULT_INVALID,    /**<Image header in-valid/corrupted */
} ver_result_t;

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
 *     Image header informations
 */
static volatile const ver_image_header_t __attribute__ (( section( VER_IMAGE_HEAD_SECTION ))) g_image_header =
{
    .ctrl =
    {
        .crc = 0U,                       /**<Filled by post-build script */
        .ver = VER_IMAGE_HEADER_VER,
    },

    .data =
    {
   		.sw_ver     = gu32_sw_ver_num,
   		.hw_ver     = gu32_hw_ver_num,
   		.image_size = 0U,               /**<Filled by post-build script */
   		.image_crc  = 0U,               /**<Filled by post-build script */
   		.enc_type   = 0U,               /**<Filled by post-build script */
   		.sig_type   = 0U,               /**<Filled by post-build script */
   		.signature  = {0},              /**<Filled by post-build script */
   		.hash       = {0},              /**<Filled by post-build script */
   		.git_sha    = {0},              /**<Filled by post-build script */
    },
};

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static uint8_t version_calc_crc(const uint8_t * const p_data, const uint16_t size);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*       Calculate CRC-8
*
* @param[in]    p_data  - Pointer to data
* @param[in]    size    - Size of data to calc crc
* @return       crc8    - Calculated CRC
*/
////////////////////////////////////////////////////////////////////////////////
static uint8_t version_calc_crc(const uint8_t * const p_data, const uint16_t size)
{
    const   uint8_t poly    = 0x07U;    // CRC-8-CCITT
    const   uint8_t seed    = 0xB6U;    // Custom seed
            uint8_t crc8    = seed;

    for (uint16_t i = 0; i < size; i++)
    {
        crc8 = ( crc8 ^ p_data[i] );

        for (uint16_t j = 0U; j < 8U; j++)
        {
            if ( crc8 & 0x80U )
            {
                crc8 = (( crc8 << 1U ) ^ poly );
            }
            else
            {
                crc8 = ( crc8 << 1U );
            }
        }
    }

    return crc8;
}

////////////////////////////////////////////////////////////////////////////////
/**
*       Check image header integrity
*
* @return       validy_result - Result of data integrity check
*/
////////////////////////////////////////////////////////////////////////////////
static ver_result_t version_check_img_header(void)
{
    static ver_result_t validy_result = eVER_RESULT_NONE;

    // Perform integrity check only once
    if ( eVER_RESULT_NONE == validy_result )
    {
        // Calculate CRC
        // NOTE: Skip CRC at the end and start calculation at version field!
        const uint8_t crc_calc = version_calc_crc((uint8_t*) &g_image_header.ctrl.ver, ( sizeof(ver_image_header_t) - 1U ));

        // Image header valid
        if ( crc_calc == g_image_header.ctrl.crc )
        {
            validy_result = eVER_RESULT_VALID;
        }
        else
        {
            validy_result = eVER_RESULT_INVALID;
        }
    }

    return validy_result;
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup VERSION_API
* @{ <!-- BEGIN GROUP -->
*
*   Following function are part of Version API.
*/
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
/**
 *  @brief  Get image header informations
 *
 *  @note   Function return NULL if image header is corrupted or missing!
 *
 *  @note   This functions return image header linker into internal flash,
 *          location defined with "VER_IMAGE_HEAD_SECTION" configuration.
 *
 * @return  p_image_header - Pointer to image header
 */
////////////////////////////////////////////////////////////////////////////////
const ver_image_header_t * version_get_img_header(void)
{
    ver_image_header_t * p_image_header = NULL;

    // Check image header
    const ver_result_t img_header_valid = version_check_img_header();

    // Return data only if header valid
    if ( eVER_RESULT_VALID == img_header_valid )
    {
        p_image_header = (ver_image_header_t*) &g_image_header;
    }

    return p_image_header;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
