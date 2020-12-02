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

find_package(PythonInterp 3)

file(GLOB BENCHMARK_TESTS "*.bench.cpp")
foreach(test_path ${BENCHMARK_TESTS})
  get_filename_component(test_file "${test_path}" NAME)
  string(REPLACE ".bench.cpp" "" test_name "${test_file}")
  set(test_name "${test_name}")
  if (NOT DEFINED ${test_name}_REPORTED)
    message(STATUS "Adding Benchmark: ${test_file}")
    # Only report the adding of the benchmark once.
    set(${test_name}_REPORTED ON CACHE INTERNAL "")
  endif()
  add_executable(bench-${test_name} ${test_file})
  target_link_libraries(bench-${test_name} PUBLIC extr_map::core libfast_csv_parser)
  target_compile_features(bench-${test_name} PUBLIC cxx_variable_templates)
  if(PYTHONINTERP_FOUND)
    configure_file(${test_name}.bench.py tmp/${test_name}.bench.py @ONLY)
    file(
      COPY ${CMAKE_CURRENT_BINARY_DIR}/tmp/${test_name}.bench.py
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
      FILE_PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      )
  endif()
endforeach()

configure_file(pruning.bench.all.sh tmp/pruning.bench.all.sh @ONLY)
file(
  COPY ${CMAKE_CURRENT_BINARY_DIR}/tmp/pruning.bench.all.sh
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/pruning.bench.all.sh
  FILE_PERMISSIONS
  OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  )
