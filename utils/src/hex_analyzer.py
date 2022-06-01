# ===============================================================================
# @file:    hex_analyzer.py
# @note:    This sript is for acquiring informations from HEX file
# @author:  Ziga Miklosic
# @date:    01.06.2022
# @brief:   HEX file must has predefined format
# ===============================================================================

# ===============================================================================
#       IMPORTS  
# ===============================================================================
import sys
import os

import argparse
from intelhex import IntelHex

# ===============================================================================
#       SCRIPT VERSIONING
# ===============================================================================
SCRIPT_VER = "V0.0.1"

# HEX file endiannes
HEX_FILE_LITTLE_ENDIAN = True

# ===============================================================================
#       APPLICATION HEADER SETTINGS
# ===============================================================================

# Application header size
APP_HEADER_SIZE = 16

# ------------------------------------------------------------------------
#   Application header location offset realtive to inputed values
# ------------------------------------------------------------------------

# Application header signature offset
APP_HEADER_SIGNATURE_OFFSET     = 0

# Application header bytes offset
APP_HEADER_SW_MAJOR_OFFSET      = 4
APP_HEADER_SW_MINOR_OFFSET      = 5
APP_HEADER_SW_DEVELOP_OFFSET    = 6
APP_HEADER_SW_TEST_OFFSET       = 7

# Application CRC and size
APP_HEADER_SIZE_OFFSET          = 8
APP_HEADER_CRC_OFFSET           = 12




# Tool description
TOOL_DESCRIPTION = \
"hex_analyzer.py %s" % SCRIPT_VER


# ===============================================================================
#       FUNCTIONS
# ===============================================================================

# ===============================================================================
# @brief:   Argument parser
#
# @return:       TODO:...
# ===============================================================================
def arg_parser():

    # Arg parser
    parser = argparse.ArgumentParser( 	description=TOOL_DESCRIPTION, 
                                        epilog="Enjoy the program!")

    # Add arguments
    parser.add_argument("-v",   help="get version", action="store_true" , required=False )
    parser.add_argument("-f",   help="HEX file", metavar="hex_file", type=str, required=False )
    parser.add_argument("-i",   help="get HEX info from application hader", metavar="app_header_addr", type=str, required=False )

    # Get args
    args = parser.parse_args()

    # Convert namespace to dict
    args = vars(args)

    return args["f"], args["i"], args["v"]

# ===============================================================================
# @brief: Get tool version
#
# @return:       void
# ===============================================================================
def print_script_version():
    print("Revision tool version: %s" % SCRIPT_VER)

# ===============================================================================
# @brief: Initialize function generator object
#
# @param[in]:    file   - HEX inputed file 
# @param[in]:    addr   - Application header address inside HEX file
# @return:       void
# ===============================================================================
def hex_info_actions(file, addr):
    print("")
    print("Parsing <%s>..." % file)

    # Create and load hex file
    ih = IntelHex()
    ih.loadhex(file)

    # Get application header info
    header_info = []
    for idx in range(APP_HEADER_SIZE):
        header_info.append( (ih[addr+idx]) )

    # Parse header informations
    if HEX_FILE_LITTLE_ENDIAN:

        # Signature
        app_sign = ( header_info[APP_HEADER_SIGNATURE_OFFSET+0] << 0 )  | ( header_info[APP_HEADER_SIGNATURE_OFFSET+1] << 8 ) | \
                    ( header_info[APP_HEADER_SIGNATURE_OFFSET+2] << 16 ) | ( header_info[APP_HEADER_SIGNATURE_OFFSET+3] << 24 )

        # Application size
        app_size =  ( header_info[APP_HEADER_SIZE_OFFSET+0] << 0 )  | ( header_info[APP_HEADER_SIZE_OFFSET+1] << 8 ) | \
                    ( header_info[APP_HEADER_SIZE_OFFSET+2] << 16 ) | ( header_info[APP_HEADER_SIZE_OFFSET+3] << 24 )

        # Application CRC
        app_crc =  ( header_info[APP_HEADER_CRC_OFFSET+0] << 0 )   | ( header_info[APP_HEADER_CRC_OFFSET+1] << 8 ) | \
                    ( header_info[APP_HEADER_CRC_OFFSET+2] << 16 ) | ( header_info[APP_HEADER_CRC_OFFSET+3] << 24 )
    else:

        # Signature
        app_sign =  ( header_info[APP_HEADER_SIGNATURE_OFFSET+3] << 0 )  | ( header_info[APP_HEADER_SIGNATURE_OFFSET+2] << 8 ) | \
                    ( header_info[APP_HEADER_SIGNATURE_OFFSET+1] << 16 ) | ( header_info[APP_HEADER_SIGNATURE_OFFSET+0] << 24 )

        # Application size
        app_size =  ( header_info[APP_HEADER_SIZE_OFFSET+3] << 0 )  | ( header_info[APP_HEADER_SIZE_OFFSET+2] << 8 ) | \
                    ( header_info[APP_HEADER_SIZE_OFFSET+1] << 16 ) | ( header_info[APP_HEADER_SIZE_OFFSET+0] << 24 )

        # Application CRC
        app_crc =  ( header_info[APP_HEADER_CRC_OFFSET+3] << 0 )   | ( header_info[APP_HEADER_CRC_OFFSET+2] << 8 ) | \
                    ( header_info[APP_HEADER_CRC_OFFSET+1] << 16 ) | ( header_info[APP_HEADER_CRC_OFFSET+0] << 24 )

    print("-------------------------------------------------------------")
    print("     APPLICATION HEADER INFORMATIONS" )
    print("-------------------------------------------------------------")
    print(" -Signature: \t0x%08X" % app_sign )
    print(" -SW version: \tV%d.%d.%d.%d" % ( header_info[APP_HEADER_SW_MAJOR_OFFSET], header_info[APP_HEADER_SW_MINOR_OFFSET], header_info[APP_HEADER_SW_DEVELOP_OFFSET], header_info[APP_HEADER_SW_TEST_OFFSET] ))
    print(" -App size: \t%d kb" % (app_size/1024))
    print(" -App CRC: \t0x%08X" % app_crc )
    print("-------------------------------------------------------------")


# ===============================================================================
# @brief:   Main entry
#
# @return:       void
# ===============================================================================
def main():

    # Get invocation arguments
    hex_file, info_addr, crc_l, ver_f = arg_parser()

    # Version infor
    if ver_f:
        print_script_version()

    # Hex file info
    elif info_addr is not None:

        # Convert to hex
        info_addr = int(info_addr, 16)

        # Get hex info actions
        hex_info_actions(hex_file, info_addr)

    
# ===============================================================================
#       CLASSES
# ===============================================================================

# ===============================================================================
#       MAIN ENTRY
# ===============================================================================
if __name__ == "__main__":
    main()

# ===============================================================================
#       END OF FILE
# ===============================================================================