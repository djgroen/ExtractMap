///////////////////////////////////////////////////////////////////////////////
// @file
// @copyright (C) 2020
//    All rights reserved.
//
// Use, modification, and distribution is subject to the license.
//
// @author Sergiy Gogolenko <sergiy.gogolenko@gmail.com>
//         Meizhu Wang      <mwang@know-center.at>
///////////////////////////////////////////////////////////////////////////////
// Computes the durations/distances of the fastest routes between all pairs of
// supplied coordinates.
///////////////////////////////////////////////////////////////////////////////

#include <extr_map/routing/osrm_table.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "csv.h"

#define DEBUG_MSG(text) \
  {}
// #define DEBUG_MSG(text) std::cerr << text

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr
        << "Usage: " << argv[0]
        << " input-route-network.osrm input-locations.csv > output-matrix.txtn";
    return EXIT_FAILURE;
  }
  // // no input-route-network.osrm
  // if(!std::filesystem::exists(argv[1]))
  //   return -2;

  // Read names from locations and their coordinates from CSV
  std::vector<std::string> location_names{};
  std::vector<std::pair<double, double>> latlons{};
  {
    std::string name;
    float lon;
    float lat;

    io::CSVReader<3> in{argv[2]};
    in.read_header(io::ignore_extra_column, "#name", "lat", "lon");
    DEBUG_MSG("Error file open" << std::endl);
    while (in.read_row(name, lat, lon)) {
      location_names.push_back(name);
      latlons.push_back({lat, lon});
      DEBUG_MSG(name << " at <" << lon << "," << lat << ">" << std::endl);
    }
  }

  extr_map::routing::Table table(argv[1]);
  if (!table.compute(latlons))  // Compute distance matrix from coordinates
    return EXIT_FAILURE;
  else {  // Write results to CSV
    DEBUG_MSG("Table service succeeded" << std::endl);


    // Print out distance matrix in CSV format to stdout
    unsigned i = 0;
    for (const auto& name : location_names) {
      std::cout << (i ? "," : "") << name;
      i++;
    }

    // const auto& distance_matrix = table.distances();
    // unsigned a = 0;
    // for (const auto& distance_array : distance_matrix) {
    //   unsigned b = 0;
    //   for (const auto& distance : distance_array) {
    //     std::cout << (b ? "," : "") << distance;
    //     DEBUG_MSG("d(" << a << "," << b << ")=" << distance << std::endl);
    //     b++;
    //   }
    //   std::cout << std::endl;
    //   a++;
    // }
    return EXIT_SUCCESS;
  }
}
