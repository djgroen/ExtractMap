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
project(pruning)

add_executable(pruning pruning.cpp)
target_link_libraries(pruning PUBLIC extr_map::core libfast_csv_parser)
target_compile_features(pruning PUBLIC cxx_variable_templates)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibOSRM_CXXFLAGS}")

add_test(NAME pruning-south-sudan
  COMMAND $<TARGET_FILE:pruning>
  ${CMAKE_BINARY_DIR}/data/south-sudan.osrm
  ${CMAKE_SOURCE_DIR}/data/south-sudan-locations.csv
  > ${CMAKE_BINARY_DIR}/south-sudan-routing.csv
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_property(TEST pruning-south-sudan PROPERTY
  REQUIRED_FILES ${CMAKE_BINARY_DIR}/data/south-sudan.osrm)
