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
project(routes_table)

# Regions to get OSM data from Geofabrik
set(TEST_GEO_REGIONS "africa/south-sudan") # "europe/austria"

add_executable(routes_table routes_table.cpp)
target_link_libraries(routes_table PUBLIC extr_map::core libfast_csv_parser)
target_compile_features(routes_table PUBLIC cxx_variable_templates)
# target_compile_features(routes_table PUBLIC cxx_std_17)
# set_property(TARGET routes_table PROPERTY CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibOSRM_CXXFLAGS}")

# TODO: test fixtures and deps https://cmake.org/cmake/help/v3.8/prop_test/FIXTURES_REQUIRED.html
add_test(NAME routes-table-south-sudan
  COMMAND $<TARGET_FILE:routes_table>
  ${CMAKE_BINARY_DIR}/data/south-sudan.osrm
  ${CMAKE_SOURCE_DIR}/data/south-sudan-locations.csv
  > ${CMAKE_BINARY_DIR}/south-sudan-routing.csv
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_property(TEST routes-table-south-sudan PROPERTY
  REQUIRED_FILES ${CMAKE_BINARY_DIR}/data/south-sudan.osrm)
set_property(TEST routes-table-south-sudan PROPERTY SKIP_RETURN_CODE -2)
