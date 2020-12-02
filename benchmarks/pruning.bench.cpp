///////////////////////////////////////////////////////////////////////////////
// @file
// @copyright (C) 2020
//    All rights reserved.
//
// Use, modification, and distribution is subject to the license.
//
// @author Sergiy Gogolenko <sergiy.gogolenko@gmail.com>
///////////////////////////////////////////////////////////////////////////////
// Benchmarks location graph pruning
///////////////////////////////////////////////////////////////////////////////

// TODO: switch to https://github.com/google/benchmark
#include "benchmark.h"
#include <stdlib.h>

#include <extr_map/pruning/triangle_pruning.hpp>
#include <extr_map/routing/osrm_table.hpp>

#include <iostream>
#include <string>
#include <utility>

#include "csv.h"

#define DEBUG_MSG(text) std::cerr << "DBG: " << text << std::endl

int test(const std::string &locations_file, const std::string &osrm_file,
         int repeats) {
  // Read locations from CSV
  std::vector<std::pair<double, double>> latlons{};
  {
    float lon;
    float lat;

    io::CSVReader<2, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>>
        in{locations_file};
    in.read_header(io::ignore_extra_column, "lat", "lon");
    while (in.read_row(lat, lon)) {
      latlons.push_back({lat, lon});
    }
    DEBUG_MSG(latlons.size() << ":\"Read locations\"");
  }

  // Compute distance matrix from coordinates
  extr_map::routing::Table table(osrm_file);
  BENCH_CALL_TIMING(if (!table.compute(latlons)) return EXIT_FAILURE);
  DEBUG_MSG("True:\"Table service succeeded\"");

  auto &distance_matrix = table.distances();
  for (int i = 0; i < repeats; i++) {
    extr_map::pruning::TrianglePruning pruner;
    BENCH_CALL_TIMING(pruner.compute(distance_matrix));
  }
  return EXIT_SUCCESS;
}

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    std::cerr
        << "Usage: " << argv[0]
        << " input-route-network.osrm input-locations.csv > output-matrix.txt"
        << std::endl;
    return EXIT_FAILURE;
  }
  return test(argv[2], argv[1], argc == 3 ? 1 : std::atoi(argv[3]));

  // if(argc == 3)
  //     return test(argv[2], argv[1]);
  // else
  //   return test(argv[2], argv[1], std::atoi(argv[3]));
    
}
