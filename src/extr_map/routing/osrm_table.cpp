///////////////////////////////////////////////////////////////////////////////
// @file
// @copyright (C) 2020
//    All rights reserved.
//
// Use, modification, and distribution is subject to the license.
//
// @author Sergiy Gogolenko <sergiy.gogolenko@gmail.com>
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <extr_map/routing/osrm_table.hpp>

#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"
#include "osrm/table_parameters.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include <ostream>
#include <string>
#include <utility>

#define DEBUG_MSG(text)                                                        \
  {}
// #define DEBUG_MSG(text) std::cerr << text

// This function translates C-string to character.
// If input is a const-string, the transformation is performed in compile-time.
// Credit to:
// https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c
constexpr unsigned int cstr_to_int(const char *str, int h = 0) {
  return !str[h] ? 5381 : (cstr_to_int(str, h + 1) * 33) ^ str[h];
}

bool extr_map::routing::Table::compute(
    const std::vector<std::pair<double, double>> &latlons,
    const char *table_type) {
  // table_type: "distances" corresponds to distances matrix,
  // it can be changed to "durations"

  // Configure based on a .osrm base path, and no datasets in shared mem from
  // osrm-datastore
  osrm::EngineConfig config;
  config.storage_config = {routes_file};
  config.use_shared_memory = false;

  // Select routing speed up techniques:
  // - MLD: Multi-Level Dijkstra (requires extract+partition+customize
  // pre-processing)
  // - CH: Contraction Hierarchies(requires extract+contract pre-processing)
  config.algorithm = osrm::EngineConfig::Algorithm::CH;

  // Routing machine with several services (such as Route, Table, Nearest,
  // Trip, Match)
  const osrm::OSRM osrm{config};

  osrm::TableParameters params;
  for (const auto &latlon : latlons)
    params.coordinates.push_back({osrm::util::FloatLongitude{latlon.second},
                                  osrm::util::FloatLatitude{latlon.first}});

  // define annotations type (Duration, Distance, or All)
  switch (cstr_to_int(table_type)) {
  case cstr_to_int("distances"):
    params.annotations = osrm::TableParameters::AnnotationsType::Distance;
    break;
  case cstr_to_int("durations"):
    params.annotations = osrm::TableParameters::AnnotationsType::Duration;
    break;
  default:
    return false;
  }

  // Response is in JSON format
  osrm::engine::api::ResultT result = osrm::json::Object();

  // Execute routing request, this does the heavy lifting
  const auto status = osrm.Table(params, result);
  auto &json_result = result.get<osrm::json::Object>();

  if (status == osrm::Status::Ok) {
    DEBUG_MSG("Table service worked fine probably\n");
    const auto &distance_matrix =
        json_result.values.at(table_type).get<osrm::json::Array>().values;

    // Copy results to m_distances
    // m_distances.clear();
    // m_distances.resize(distance_matrix.size());
    m_distances.reserve(distance_matrix.size(), distance_matrix.size());
    unsigned a = 0;
    for (const auto &distance_array : distance_matrix) {
      unsigned b = 0;
      // m_distances[a].resize(distance_array.get<osrm::json::Array>().values.size());
      for (const auto &distance_entry :
           distance_array.get<osrm::json::Array>().values) {
        // If connection between two coordinates is missing, the corresponding
        // matrix element is `Null`.
        // So whenever this happens, we use unreasonably large distance.
        // If the entry is neither Null nor Number, the code throws exception.
        double distance = distance_entry.is<osrm::json::Null>()
                              ? inf_distance
                              : distance_entry.get<osrm::json::Number>().value;
        m_distances[a][b] = distance;
        b++;
      }
      a++;
    }
  } else if (status == osrm::Status::Error) {
    DEBUG_MSG("Code:    "
              << json_result.values["code"].get<osrm::json::String>().value
              << "\n");
    DEBUG_MSG("Message: "
              << json_result.values["message"].get<osrm::json::String>().value
              << "\n");
    return false;
  }
  return true;
}
