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
project(ExtractMap)

# subdirs(routing)

find_package(LibOSRM REQUIRED)
find_package(Proj4 QUIET)

find_package(OpenMP)
if(NOT TARGET OpenMP::OpenMP_CXX)
  find_package(Threads REQUIRED)
  add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
  set_property(TARGET OpenMP::OpenMP_CXX
    PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
  # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
  set_property(TARGET OpenMP::OpenMP_CXX
    PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)
endif()

# Build the ExtractMap library

set(GEO_PRUNING_SOURCES
  pruning/vincenty_distance.cpp
  )
if(Proj4_FOUND)
  list(APPEND GEO_PRUNING_SOURCES pruning/geo_pruning.cpp)
  message(STATUS "Exclude GeoPruning with Proj4 coordinate transformation")
endif()

add_library( ${ExtractMap_LIBRARY} STATIC
  routing/osrm_table.cpp

  pruning/triangle_pruning.cpp
  ${GEO_PRUNING_SOURCES}
  )

if(OPENMP_FOUND)
    target_link_libraries(${ExtractMap_LIBRARY} PUBLIC OpenMP::OpenMP_CXX)
endif()

# Use headers from core ExtractMap library
target_include_directories( ${ExtractMap_LIBRARY} PUBLIC ${ExtractMap_INCLUDE_DIR} )

# Request modern C++ features support
target_compile_features( ${ExtractMap_LIBRARY}
  PUBLIC
  cxx_auto_type
  cxx_decltype

  cxx_constexpr
  cxx_final
  cxx_noexcept

  cxx_lambdas
  cxx_trailing_return_types

  cxx_alias_templates
  cxx_template_template_parameters
  cxx_variable_templates
  cxx_variadic_templates

  cxx_variadic_macros

  cxx_long_long_type
  cxx_nullptr
  cxx_rvalue_references

  cxx_range_for
  cxx_generalized_initializers )

# Mark dependencies on third-party libraries
if(LibOSRM_FOUND)
  link_directories(${LibOSRM_LIBRARY_DIRS})
  target_link_libraries(${ExtractMap_LIBRARY}
    PUBLIC ${LibOSRM_LIBRARIES} ${LibOSRM_DEPENDENT_LIBRARIES})
  target_include_directories(${ExtractMap_LIBRARY}
    PRIVATE ${LibOSRM_INCLUDE_DIR})
  set_target_properties(${ExtractMap_LIBRARY}
    PROPERTIES COMPILE_FLAGS "${LibOSRM_CXXFLAGS}")
  # target_include_directories(${ExtractMap_LIBRARY} PUBLIC ${ExtractMap_INCLUDE_DIR})
  # set_property(TARGET ${ExtractMap_LIBRARY} PROPERTY CXX_STANDARD 17)
endif()

# Alias library as ExtractMap::core library
add_library(extr_map::core ALIAS ${ExtractMap_LIBRARY})

install(TARGETS ${ExtractMap_LIBRARY}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  COMPONENT core
  )

# find_package(PROJ4)
# target_link_libraries(MyApp ${PROJ_LIBRARIES})
# include_directories(${PROJ_INCLUDE_DIRS})

# find_package(PROJ4)
# target_link_libraries(MyApp ${PROJ4_LIBRARIES})
# include_directories(${PROJ4_INCLUDE_DIRS})

# https://github.com/tarantool/gis
