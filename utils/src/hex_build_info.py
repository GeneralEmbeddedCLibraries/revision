# ===============================================================================
# @file:    hex_build_info.py
# @note:    This sript generated build info and stores it into HEX file 
# @author:  Ziga Miklosic
# @date:    01.06.2022
# @brief:      
# ===============================================================================

# ===============================================================================
#       IMPORTS  
# ===============================================================================
from distutils.command.build import build
import sys
import os
import subprocess

import argparse
from intelhex import IntelHex

# ===============================================================================
#       SCRIPT VERSIONING
# ===============================================================================
SCRIPT_VER = "V0.0.1"

# HEX file endiannes
HEX_FILE_LITTLE_ENDIAN = True

# ===============================================================================
#       BUILD INFO SETTINGS
# ===============================================================================

# End string termination
STRING_TERMINATION = "\x00"

# New line
NEWLINE = "\n\r"




# Git commands
GIT_USER_NAME_CMD       = "git config user.name"
GIT_USER_EMAIL_CMD      = "git config user.email"
GIT_CURRENT_BRANCH_CMD  = "git rev-parse --abbrev-ref HEAD"
GIT_WORKTREE_STATUS_CMD = "git status --porcelain"
GIT_COMMIT_SHA_CMD      = "git rev-parse --short HEAD"


# Tool description
TOOL_DESCRIPTION = \
"hex_build_info.py %s" % SCRIPT_VER


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
    parser.add_argument("-v",   help="get version",                     action="store_true" ,                   required=False )
    parser.add_argument("-f",   help="HEX file",                        metavar="hex_file",         type=str,   required=False )
    parser.add_argument("-ba",  help="build info HEX file location",    metavar="build_info_addr",  type=str,   required=False )
    parser.add_argument("-n",   help="software project name",           metavar="name",             type=str,   required=False )
    parser.add_argument("-c",   help="used build configuration",        metavar="build_cfg",        type=str,   required=False )
    parser.add_argument("-pc",  help="computer name",                   metavar="pc_name",          type=str,   required=False )
    parser.add_argument("-os",  help="host OS",                         metavar="host_os",          type=str,   required=False )

    # Get args
    args = parser.parse_args()

    # Convert namespace to dict
    args = vars(args)

    # Get arguments
    file_name       = args["f"]
    ver_flag        = args["v"]
    build_info_addr = args["ba"]
    sw_proj_name    = args["n"]
    build_cfg       = args["c"]
    pc_name         = args["pc"]
    host_os         = args["os"]

    return file_name, ver_flag, build_info_addr, sw_proj_name, build_cfg, pc_name, host_os

# ===============================================================================
# @brief: Get tool version
#
# @return:       void
# ===============================================================================
def print_script_version():
    print("Script version: %s" % SCRIPT_VER)

# ===============================================================================
# @brief: Create build info
#
# @param[in]:   sw_proj_name    - SW project name
# @param[in]:   build_cfg       - Build configuration
# @param[in]:   pc_name         - Name of PC where SW is being build
# @param[in]:   host_os         - Host PC OS name
# @return:      void
# ===============================================================================
def create_build_info(sw_proj_name, build_cfg, pc_name, host_os):

    git_name        = subprocess.check_output( GIT_USER_NAME_CMD ).decode("utf-8")[:-1] 
    git_email       = subprocess.check_output( GIT_USER_EMAIL_CMD ).decode("utf-8")[:-1] 
    git_branch      = subprocess.check_output( GIT_CURRENT_BRANCH_CMD ).decode("utf-8")[:-1] 
    git_worktree    = subprocess.check_output( GIT_WORKTREE_STATUS_CMD ).decode("utf-8")[:-1] 
    commit_sha      = subprocess.check_output( GIT_COMMIT_SHA_CMD ).decode("utf-8")[:-1] 

    build_info_str =                                      NEWLINE +\
    "-------------------------------------------"       + NEWLINE +\
    "\tBuild Informations"                              + NEWLINE + \
    "-------------------------------------------"       + NEWLINE +\
    "Project name: \t"      + str(sw_proj_name)         + NEWLINE +\
    "Build config: \t"      + str(build_cfg)            + NEWLINE +\
    "PC name: \t"           + str(pc_name)              + NEWLINE +\
    "Host OS: \t"           + str(host_os)              + NEWLINE +\
    "Name: \t\t"            + str(git_name)             + NEWLINE +\
    "Email: \t\t"           + str(git_email)            + NEWLINE +\
    "Git branch: \t"        + str(git_branch)           + NEWLINE +\
    "Commit SHA: \t"        + str(commit_sha)           + NEWLINE +\
    "Git status: \t"        + str(git_worktree)         + NEWLINE +\
    STRING_TERMINATION;

    return build_info_str

def write_build_info(file, addr, build_info):

    print("File: %s" % file)
    print("Addr: 0x%08X" % addr)

    # Create and load hex file
    ih = IntelHex(file)

    # Store build info
    for offset, char in enumerate(build_info):       
        
        # Convert string to ascii character
        ih[addr+offset] = ord(char)

    # Write to file
    ih.tofile(file, format="hex")


# ===============================================================================
# @brief:   Main entry
#
# @return:       void
# ===============================================================================
def main():
    
    # Get invocation arguments
    file_name, ver_flag, build_info_addr, sw_proj_name, build_cfg, pc_name, host_os = arg_parser()

    # Version infor
    if ver_flag:
        print_script_version()

    # Hex file info
    else:
        
        # Create build info
        if build_info_addr is not None:

            # Convert to hex
            build_info_addr = int( build_info_addr, 16 )

            # Create
            build_info_str = create_build_info(sw_proj_name, build_cfg, pc_name, host_os)

            print( "Build info: %s" % build_info_str )

            # Write to hex
            write_build_info( file_name, build_info_addr, build_info_str )

    
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