# **Revision Utility Tools**

## **HEX Analyzer**

File: ***hex_analyzer.exe***

Tool for analyzing HEX file in order to acquire hidden informations inside builded HEX file, such as application header and build informations.



## **HEX CRC Calculator**

File: ***hex_crc_calc.exe***

Tool for calculation of application CRC and size. TODO:...



## **Project Information Generation Tool**

File: ***proj_info.exe***

Tool for creating detailed application informations. Shall be called inside pre-build process inside Software Development Environment. How to setup IDE to be able to automate project informations generation look at README.md of the root repository.

Help command:
```
>>>proj_info.exe --help
usage: proj_info.exe [-h] [-f proj_info_file] [-n name] [-c build_cfg] [-pc pc_name] [-os host_os]

Project Information Generation Tool V0.1.0

optional arguments:
  -h, --help         show this help message and exit
  -f proj_info_file  proj info file
  -n name            software project name
  -c build_cfg       used build configuration
  -pc pc_name        computer name
  -os host_os        host OS

Enjoy the program!

>>>
```