############################################################
## @file
## @copyright (C) 2020
##    High Performance Computing Center Stuttgart (HLRS)
##    http://www.hlrs.de
##    All rights reserved.
##
## Use, modification, and distribution is subject to the HLRS License.
##
## @author Sergiy Gogolenko <gogolenko@hlrs.de>
##
############################################################

set(TEST_GEO_REGIONS "africa/south-sudan")# "europe/austria" "africa")

set(_EXTRACTMAP_DATA_TARGETS)

# Add targets to extract information from OSM files (and prepare OSRM and CSV files) for tests
foreach(test_geo_region_full ${TEST_GEO_REGIONS})
  string(REGEX MATCH "[^/]+$" test_geo_region "${test_geo_region_full}")
  set(_OSRM_TARGET ${test_geo_region}-osrm)
  set(_CITIES_CSV_TARGET ${test_geo_region}-csv)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}-download.cmake
    "file(DOWNLOAD \"https://download.geofabrik.de/${test_geo_region_full}-latest.osm.pbf\" ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osm.pbf)"
    )
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osm.pbf
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}-download.cmake
    COMMENT "Download OpenStreetMaps PBF-file from https://download.geofabrik.de/${test_geo_region_full}.html"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    VERBATIM
    )
  add_custom_target(${test_geo_region}-download
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osm.pbf)

  if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osrm")    
    # TODO: improve search, get rid of $ENV{OSRM_DIR}
    # Credit to: https://stackoverflow.com/questions/25687890/running-a-bash-command-via-cmake
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osrm
      COMMAND osrm-extract -p $ENV{OSRM_DIR}/share/osrm/profiles/car.lua ./${test_geo_region}.osm.pbf 2>&1
      COMMAND osrm-partition ./${test_geo_region}.osrm 2>&1
      COMMAND osrm-customize ./${test_geo_region}.osrm 2>&1
      WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
      COMMENT "Prepare OSRM file for test from OpenStreetMaps PBF file"
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osm.pbf
      # VERBATIM
      )

    add_custom_target(${_OSRM_TARGET}
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osrm)
    # add_custom_target(${test_geo_region}-osrm ALL
    #   COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #   ${CMAKE_SOURCE_DIR}/data/${test_geo_region}.osrm ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osrm
    #   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osrm)
    list(APPEND _EXTRACTMAP_DATA_TARGETS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osrm)
  endif()

  if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}-locations.csv)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}-locations.csv
      COMMAND osmosis
      --read-pbf ./${test_geo_region}.osm.pbf
      --tf accept-nodes place=city,town,village --tf reject-relations --tf reject-ways --lp
      --write-xml ./${test_geo_region}-locations.osm
      COMMAND xsltproc ${CMAKE_CURRENT_SOURCE_DIR}/scripts/osm-settlements-to-csv.xslt
      ./${test_geo_region}-locations.osm >
      ./${test_geo_region}-locations.csv
      WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
      COMMENT "Extracting information about settlements from OpenStreetMaps PBF file"
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}.osm.pbf
      # VERBATIM
      )
    add_custom_target(${_CITIES_CSV_TARGET}
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}-locations.csv)
    list(APPEND _EXTRACTMAP_DATA_TARGETS ${CMAKE_CURRENT_BINARY_DIR}/${test_geo_region}-locations.csv)
  endif()
    unset(_OSRM_TARGET)
    unset(_CITIES_CSV_TARGET)
endforeach()

add_custom_target(data
  DEPENDS ${_EXTRACTMAP_DATA_TARGETS})

# file(GLOB test_cases *.cpp)
# get_filename_component( case_name ${case_file} NAME_WE )
