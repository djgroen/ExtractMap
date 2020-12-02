///////////////////////////////////////////////////////////////////////////////
// @file
// @copyright (C) 2020
//    All rights reserved.
//
// Use, modification, and distribution is subject to the license.
//
// @author Sergiy Gogolenko <sergiy.gogolenko@gmail.com>
///////////////////////////////////////////////////////////////////////////////
// Computes location graph by pruning of the APSP routes table.
///////////////////////////////////////////////////////////////////////////////

// #include <cstdlib>
// #include <exception>
// #include <iostream>

#include <extr_map/pruning/geo_pruning.hpp>
#include <extr_map/pruning/triangle_pruning.hpp>
#include <extr_map/routing/osrm_table.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "csv.h"

// #define DEBUG_MSG(text)
#define DEBUG_MSG(text) std::cerr << text

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr
        << "Usage: " << argv[0]
        << " input-route-network.osrm input-locations.csv > output-matrix.txt";
    return EXIT_FAILURE;
  }
  // // no input-route-network.osrm
  // if(!std::filesystem::exists(argv[1]))
  //   return -2;
  // // no input-locations.csv
  // if(!std::filesystem::exists(argv[2]))
  //   return -2;

  // Read locations from CSV
  std::vector<std::pair<double, double>> latlons{};
  {
    std::string name;
    float lon;
    float lat;

    io::CSVReader<3, io::trim_chars<' '>, io::double_quote_escape<',', '\"'> > in{argv[2]};
    in.read_header(io::ignore_extra_column, "#name", "lat", "lon");
    while (in.read_row(name, lat, lon)) {
      latlons.push_back({lat, lon});
      DEBUG_MSG(name << " at <" << lon << "," << lat << ">" << std::endl);
    }
  }

  // Compute distance matrix from coordinates
  extr_map::routing::Table table(argv[1]);
  if (!table.compute(latlons)) return EXIT_FAILURE;
  DEBUG_MSG("Table service succeeded" << std::endl);

  // Prune the distance matrix
  auto& distance_matrix = table.distances();
  extr_map::pruning::TrianglePruning pruner;
  pruner.compute(distance_matrix);

  // // Write results to CSV
  // unsigned a = 0;
  // for (const auto& distance_array : distance_matrix) {
  //   unsigned b = 0;
  //   for (const auto& distance : distance_array) {
  //     if (!extr_map::pruning::is_inf(distance)) {
  //       std::cout << (b ? "," : "") << distance;
  //       DEBUG_MSG("d(" << a << "," << b << ")=" << distance << std::endl);
  //       b++;
  //     }
  //   }
  //   std::cout << std::endl;
  //   a++;
  // }
  return EXIT_SUCCESS;
}
