# ===============================================================================
# @file:    hex_crc_calc.py
# @note:    This sript calculated CRC on HEX file
# @author:  Ziga Miklosic
# @date:    01.06.2022
# @brief:   
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



# Tool description
TOOL_DESCRIPTION = \
"hex_crc_calc.py %s" % SCRIPT_VER


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
    parser.add_argument("-i",   help="get HEX info from application hader", metavar="app_header_addr", type=str,    required=False )
    parser.add_argument("-crc",	help="calculate and add CRC to app header", nargs=3, metavar=("start_addr", "stop_addr", "app_header_addr"),  required=False )

    # Get args
    args = parser.parse_args()

    # Convert namespace to dict
    args = vars(args)

    return args["f"], args["i"], args["crc"], args["v"]


# ===============================================================================
# @brief:   Main entry
#
# @return:       void
# ===============================================================================
def main():
    pass
    
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