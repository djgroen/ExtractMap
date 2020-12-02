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

project(examples)

# TODO set up export of fast-cpp-csv-parser
set(LibFastCSV_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/third-party/fast-cpp-csv-parser")
# include(ExternalProject)
# ExternalProject_Add(
#   fast-cpp-csv-parser
#   GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser.git
#   GIT_TAG master
#   GIT_PROGRESS TRUE
#   PREFIX ${CMAKE_CURRENT_BINARY_DIR}/fast-cpp-csv-parser
#   INSTALL_COMMAND ""
#   )
# ExternalProject_Get_Property(fast-cpp-csv-parser source_dir binary_dir)
add_library(libfast_csv_parser INTERFACE)
# add_dependencies(libfast_csv_parser fast-cpp-csv-parser)
target_include_directories(libfast_csv_parser INTERFACE "${LibFastCSV_INCLUDE_DIRS}")
# target_include_directories(libfast_csv_parser INTERFACE "${source_dir}")
unset(LibFastCSV_INCLUDE_DIRS)

subdirs(pruning)
subdirs(routes_table)
