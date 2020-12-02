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

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/fast-cpp-csv-parser/csv.h")
  file(DOWNLOAD
    "https://github.com/ben-strasser/fast-cpp-csv-parser/archive/master.zip"
    "${CMAKE_CURRENT_BINARY_DIR}/fast-cpp-csv-parser.zip")
  execute_process(
    COMMAND unzip e ./fast-cpp-csv-parser.zip
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    TIMEOUT 60)
  file(RENAME
    "${CMAKE_CURRENT_BINARY_DIR}/fast-cpp-csv-parser-master"
    "${CMAKE_CURRENT_BINARY_DIR}/fast-cpp-csv-parser")
  file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/fast-cpp-csv-parser.zip")
endif()
