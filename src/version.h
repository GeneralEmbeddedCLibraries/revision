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
/**
*@addtogroup VERSION
* @{ <!-- BEGIN GROUP -->
*
*     Revision API
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VERSION_H_
#define __VERSION_H_

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

#include "../../version_cfg.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 *     Module version
 */
#define VER_VER_MAJOR       ( 1 )
#define VER_VER_MINOR       ( 4 )
#define VER_VER_DEVELOP     ( 0 )

/**
 *  Encryption type
 */
typedef enum
{
    eVER_ENC_TYPE_NONE = 0,     /**<No encryption */
    eVER_ENC_TYPE_AES_CTR,      /**<AES-CTR encryption type */

    eVER_ENC_TYPE_NUM_OF,
} ver_enc_type_t;

/**
 *  Signature type
 */
typedef enum
{
    eVER_SIG_TYPE_NONE = 0,     /**<No encryption */
    eVER_SIG_TYPE_ECSDA,        /**<ECSDA signature type */

    eVER_SIG_TYPE_NUM_OF,
} ver_sig_type_t;

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
        uint8_t  res[230];      /**<Reserved space in application header */
    } data;

} ver_app_header_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
uint32_t    version_get_sw              (uint8_t * const p_major, uint8_t * const p_minor, uint8_t * const p_develop, uint8_t * const p_test);
uint32_t    version_get_hw              (uint8_t * const p_major, uint8_t * const p_minor, uint8_t * const p_develop, uint8_t * const p_test);
const char* version_get_sw_str          (void);
const char* version_get_hw_str          (void);
const char* version_get_proj_info_str   (void);

#endif // __VERSION_H_

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
