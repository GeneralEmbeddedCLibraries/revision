# ===============================================================================
# @file:    build_info.py
# @note:    This sript generated build info
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

# Build info max. string size
# Unit: byte
BUILD_INFO_STRING_SIZE = 2048

# End string termination
STRING_TERMINATION = "\x00"     # TODO: Check how this looks in outputed HEX file!!!!

# New line
NEWLINE = str("\\r\\n\\\n")

# Git commands
GIT_USER_NAME_CMD       = "git config user.name"
GIT_USER_EMAIL_CMD      = "git config user.email"
GIT_CURRENT_BRANCH_CMD  = "git rev-parse --abbrev-ref HEAD"
GIT_WORKTREE_STATUS_CMD = "git status --porcelain"
GIT_COMMIT_SHA_CMD      = "git rev-parse --short HEAD"

# Tool description
TOOL_DESCRIPTION = \
"build_info.py %s" % SCRIPT_VER


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
    parser.add_argument("-f",   help="build info file",                 metavar="build_info_file",  type=str,   required=False )
    parser.add_argument("-n",   help="software project name",           metavar="name",             type=str,   required=False )
    parser.add_argument("-c",   help="used build configuration",        metavar="build_cfg",        type=str,   required=False )
    parser.add_argument("-pc",  help="computer name",                   metavar="pc_name",          type=str,   required=False )
    parser.add_argument("-os",  help="host OS",                         metavar="host_os",          type=str,   required=False )

    # Get args
    args = parser.parse_args()

    # Convert namespace to dict
    args = vars(args)

    # Get arguments
    ver_flag        = args["v"]
    file            = args["f"]
    sw_proj_name    = args["n"]
    build_cfg       = args["c"]
    pc_name         = args["pc"]
    host_os         = args["os"]

    return ver_flag, file, sw_proj_name, build_cfg, pc_name, host_os

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

    build_info_str = \
    "=============================================================" + NEWLINE +\
    "\tBUILD INFORMATIONS" + NEWLINE +\
    "  generated by <build_info.py>  " + str(SCRIPT_VER) + str(")")  + NEWLINE +\
    "=============================================================" + NEWLINE +\
    "{:>15}".format("Project name: ")   + str(sw_proj_name)         + NEWLINE +\
    "{:>15}".format("Build config: ")   + str(build_cfg)            + NEWLINE +\
    "{:>15}".format("PC name: ")        + str(pc_name)              + NEWLINE +\
    "{:>15}".format("Host OS: ")        + str(host_os)              + NEWLINE +\
    "{:>15}".format("Author: ")         + str(git_name)             + NEWLINE +\
    "{:>15}".format("Email: ")          + str(git_email)            + NEWLINE +\
    NEWLINE +\
    "{:>20}".format("Git  informations ")                           + NEWLINE +\
    "{:>15}".format("Branch: ")         + str(git_branch)           + NEWLINE +\
    "{:>15}".format("Commit SHA: ")     + str(commit_sha)           + NEWLINE +\
    STRING_TERMINATION;

    #"{:>20}".format("Status: ")         + str(git_worktree)           + NEWLINE +\     # TODO: Fix this
    #STRING_TERMINATION;

    return build_info_str



def write_c_file_header(file):
    file.write("// Copyright (c) 2022 Ziga Miklosic\n")
    file.write("// All Rights Reserved\n")
    file.write("// This software is under MIT licence (https://opensource.org/licenses/MIT)\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("/*!\n")
    file.write("*@file      build_info.c\n")
    file.write("*@brief     Build informations\n")
    file.write("*@author    Ziga Miklosic\n")
    file.write("*@date      xx.xx.xxxx\n")
    file.write("*/\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("/*!\n")
    file.write(" * @addtogroup BUILD_INFO\n")
    file.write(" * @{ <!-- BEGIN GROUP -->\n")
    file.write(" */\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("// Includes\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("#include <stdint.h>\n")
    file.write("#include <stdlib.h>\n")
    file.write("\n")
    file.write("#include <build_info.h>\n")
    file.write("\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("// Definitions\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("\n")
    file.write("/**\n")
    file.write(" *   Total size of build information string\n")
    file.write(" */\n")
    file.write("#define BUILD_INFO_STRING_SIZE      ( %s )\n" % BUILD_INFO_STRING_SIZE)
    file.write("\n")
    file.write("\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("// Variables\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("\n")
    file.write("/**\n")
    file.write(" *   Build information string\n")
    file.write(" */\n")
    file.write("static volatile const char __attribute__ (( section( "".build_info"" ))) gs_build_info[BUILD_INFO_STRING_SIZE] = \"\\\n")


def write_c_file_footer(file):
    file.write("\";\n")
    file.write("\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("// Functions\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("/**\n")
    file.write(" * 	@brief		Get build info string\n")
    file.write(" *\n")
    file.write(" * @return 		gs_build_info - Build information string\n")
    file.write(" */\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("const char* build_info_get_str(void)\n")
    file.write("{\n")
    file.write("    return (const char*) &gs_build_info;\n")
    file.write("}\n")
    file.write("\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("/*!\n")
    file.write(" * @} <!-- END GROUP -->\n")
    file.write(" */\n")
    file.write("////////////////////////////////////////////////////////////////////////////////\n")
    file.write("\n")

def create_c_file(file, build_info):
    write_c_file_header(file)
    file.write(build_info)
    write_c_file_footer(file)

def create_h_file(file):
    pass


def write_build_info(file, build_info):

    # Create/Open Source file
    build_info_c = open(file, "w")

    # Create/Open Header file
    h_file = file[:-1] + "h"
    build_info_h = open(h_file, "w")

    # Create source file
    create_c_file(build_info_c, build_info)

    # Create header file
    create_h_file(build_info_h)

    # Close file
    build_info_c.close()
    build_info_h.close()






# ===============================================================================
# @brief:   Main entry
#
# @return:       void
# ===============================================================================
def main():
    
    # Get invocation arguments
    ver_flag, file, sw_proj_name, build_cfg, pc_name, host_os = arg_parser()

    # Version infor
    if ver_flag:
        print_script_version()

    # Hex file info
    else:
        
        # Create
        build_info_str = create_build_info(sw_proj_name, build_cfg, pc_name, host_os)

        # Write to file
        write_build_info(file, build_info_str)

        print("")
        print("====================================================================")
        print("     HEX BUILD INFO TOOL %s" % SCRIPT_VER )
        print("====================================================================")
        print("Build information successfully generated!")

    
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