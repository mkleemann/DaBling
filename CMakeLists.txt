##################################################################################
# CMakeLists.txt for the DaBling project
##################################################################################

cmake_minimum_required(VERSION 2.8)

project(DaBling)

##################################################################################
# tools to be used for programming the AVR
##################################################################################
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER avrispmkII)
set(AVR_UPLOADTOOL_PORT usb)
# AVR and fuses needs to be set
# atmega8 with 1MHz internal clock; standard configuration
set(AVR_MCU atmega8)
set(AVR_H_FUSE 0xD9)
set(AVR_L_FUSE 0xE1)

##################################################################################
# status messages
##################################################################################
message(STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}")
message(STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}")
message(STATUS "Current MCU is set to: ${AVR_MCU}")
message(STATUS "Current H_FUSE is set to: ${AVR_H_FUSE}")
message(STATUS "Current L_FUSE is set to: ${AVR_L_FUSE}")

##################################################################################
# set build type
##################################################################################
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

##################################################################################
# needs to be defined for AVR toolchain
##################################################################################
set(MCU_SPEED "1000000UL")

##################################################################################
# some cmake cross-compile necessities
##################################################################################
if(CMAKE_HOST_SYSTEM MATCHES "Windows")
    if(DEFINED ENV{AVR_FIND_ROOT_PATH})
        set(CMAKE_FIND_ROOT_PATH $ENV{AVR_FIND_ROOT_PATH})
    else(DEFINED ENV{AVR_FIND_ROOT_PATH})
        message(FATAL_ERROR "Please set AVR_FIND_ROOT_PATH in your environment.")
    endif(DEFINED ENV{AVR_FIND_ROOT_PATH})
else(CMAKE_HOST_SYSTEM MATCHES "Windows")
    set(CMAKE_FIND_ROOT_PATH "/usr/avr")
endif(CMAKE_HOST_SYSTEM MATCHES "Windows")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

##################################################################################
# status messages for generating
##################################################################################
message(STATUS "Set CMAKE_FIND_ROOT_PATH to ${CMAKE_FIND_ROOT_PATH}")
message(STATUS "Set CMAKE_SYSTEM_INCLUDE_PATH to ${CMAKE_SYSTEM_INCLUDE_PATH}")
message(STATUS "Set CMAKE_SYSTEM_LIBRARY_PATH to ${CMAKE_SYSTEM_LIBRARY_PATH}")

##################################################################################
# set compiler options for build types
##################################################################################
if(CMAKE_BUILD_TYPE MATCHES Release)
   set(CMAKE_C_FLAGS_RELEASE "-Os")
   set(CMAKE_CXX_FLAGS_RELEASE "-Os")
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
   set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
   set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

if(CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_C_FLAGS_DEBUG "-O0 -save-temps -g -gdwarf-3 -gstrict-dwarf")
   set(CMAKE_CXX_FLAGS_DEBUG "-O0 -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

##################################################################################
# compiler options for all build types
##################################################################################
add_definitions("-DF_CPU=${MCU_SPEED}")
add_definitions("-fpack-struct")
add_definitions("-fshort-enums")
add_definitions("-Wall")
add_definitions("-Werror")
add_definitions("-pedantic")
add_definitions("-pedantic-errors")
add_definitions("-funsigned-char")
add_definitions("-funsigned-bitfields")
add_definitions("-ffunction-sections")
add_definitions("-c")
add_definitions("-std=gnu99")

##################################################################################
# set include path for submodules
##################################################################################
include_directories(modules)

##################################################################################
# down to the submodules
##################################################################################
add_subdirectory(modules/adc)
#add_subdirectory(modules/config)
add_subdirectory(modules/timer)
add_subdirectory(modules/leds)

##################################################################################
# down to the application itself
##################################################################################
add_subdirectory(app)

##################################################################################
# use default documentation module
##################################################################################
include("${PROJECT_SOURCE_DIR}/cmake/Modules/defaultDocuTarget.cmake")

