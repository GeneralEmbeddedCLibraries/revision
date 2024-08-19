# Changelog
All notable changes to this project/module will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project/module adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---
## V2.0.0 - xx.xx.2024

### Changes
 - Application header structure completely changed (therefore increased major version)


---
## V1.4.0 - 15.02.2024

### Changes
 - Increased application header size to 512 bytes (0x200). That was issue with STM32G4 family, cousing interrupt misfireing
 - Increased application header version 

---
## V1.3.0 - 24.08.2023

### Changes
 - Structure of application header:
    - Removed signature
    - Added header version
 - Exposing application header definition
    - For bootloader use in order to ease parsing

### Fixed
 - Compiler warnings

### Todo
 - Release for hex_analyzer tool
 - Instructions how to use hex_analyzer tool

---
## V1.2.1 - 13.12.2022

### Fixed
 - Replace old version notes with changelog in markdown
 - Removed end termination characters from SW and HW description strings

### Todo
 - Release for hex_analyzer tool
 - Instructions how to use hex_analyzer tool

---
## V1.2.0 - 15.06.2022

### Added
 - Project Information Generation Tool proj_info.exe V0.1.0
 - Added API function to get project informations
 - Added detailed instructions how to use project information generation tool

### Todo
 - Release for hex_analyzer tool
 - Instructions how to use hex_analyzer tool

---
## V1.1.0 - 30.05.2022

### Added
 - Added missing test revision number inside application header

### Fixed
 - Fixing bug with version string creation 

### Todo
 - Post-build script
 - Automate to call as post-build process inside Makefile

---
## V1.0.0 - 30.05.2022

### Added
 - SW & HW versioning via configuration
 - Initial application header
 - Description of setting up linker to put app header into specified memory location

### Todo
 - Post-build script
 - Automate to call as post-build process inside Makefile

---