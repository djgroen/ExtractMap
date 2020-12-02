############################################################
## @file
## @copyright (C) 2020
##    All rights reserved.
##
## Use, modification, and distribution is subject to the license.
##
## @author Sergiy Gogolenko <sergiy.gogolenko@gmail.com>
##
############################################################

project(ExtractMap CXX C)

cmake_minimum_required(VERSION 3.5)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR AND NOT MSVC_IDE)
  message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'. Please delete them.")
endif()

# Look for CMake packages in local CMake folder as well
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
  message(STATUS "Building on a 64 bit system")
else()
  message(STATUS "Building on a 32 bit system")
endif()

if(WIN32 AND MSVC_VERSION LESS 1900)
  message(FATAL_ERROR "Building with Microsoft compiler needs Latest Visual Studio 2015 (Community or better)")
endif()

###############################################################################
# Set up paths
###############################################################################
# set( ExtractMap_VERSION      000001   )
# set( ExtractMap_VERSION_MAJOR 0 )
# set( ExtractMap_VERSION_MINOR 1 )
# set( ExtractMap_VERSION_PATCH 0 )
# set( ExtractMap_VERSION "${ExtractMap_VERSION_MAJOR}.${ExtractMap_VERSION_MINOR}.${ExtractMap_VERSION_PATCH}" )
# set( ExtractMap_USE_CCACHE   NO ) # turn this option On if compilation time is too long

set( ExtractMap_NAMESPACE   "extr_map" )
set( ExtractMap_LIBRARY     "${ExtractMap_NAMESPACE}" )
set( ExtractMap_INCLUDE_DIR  ${ExtractMap_BINARY_DIR}/src/ ${ExtractMap_SOURCE_DIR}/src/ )

###############################################################################
# Extra options for early detection of errors
###############################################################################
add_compile_options(
    -pedantic
    -Wall
)

############################################################
# Set subdirectories
############################################################
add_subdirectory(third-party)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(benchmarks)
enable_testing()
add_subdirectory(examples)

############################################################
# Print out status and usage information
############################################################
message(STATUS "ExtractMap version is ${ExtractMap_VERSION}")
message(STATUS "ExtractMap deployment directory is ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Compile ExtractMap with ${CMAKE_CXX_COMPILER}")
message(STATUS "ExtractMap build type is ${CMAKE_BUILD_TYPE}")

message(STATUS "")
string(TOLOWER "${CMAKE_GENERATOR}" cmake_generator_tolower)
if(cmake_generator_tolower MATCHES "makefile")
    message(STATUS "Some useful commands:")
    message(STATUS "-------------------+---------------------------------------------------------")
    message(STATUS "Command            |   Description")
    message(STATUS "-------------------+---------------------------------------------------------")
    message(STATUS "make install       | Install to ${CMAKE_INSTALL_PREFIX}. To change that:")
    message(STATUS "                   |     cmake . -DCMAKE_INSTALL_PREFIX=/your/path/to/install")
    message(STATUS "                   |   To install EXTRACTMAP headers to a separate location, do:")
    message(STATUS "                   |     cmake . -DExtractMap_INCLUDE_INSTALL_DIR=/your/path/to/install")
    message(STATUS "make test          | Build and run the examples.")
    message(STATUS "                   |   Note: these are not unit-tests.")
    message(STATUS "                   |     The latter must change in further releases.")
    message(STATUS "make package_source| Pack TGZ tarball with sources.")
    message(STATUS "make package       | Build STGZ package installer.")
    message(STATUS "-------------------+---------------------------------------------------------")
else()
    message(STATUS "For information about installing/packaging, read this text:")
    message(STATUS "  `doc/build-instructions/linux.md'")
endif()
message(STATUS "")
