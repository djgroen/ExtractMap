//////////////////////////////////////////////////////////////////////////////
// @file
// @copyright (C) 2020
//    All rights reserved.
//
// Use, modification, and distribution is subject to the license.
//
// @author Sergiy Gogolenko <sergiy.gogolenko@gmail.com>
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OSRM_TABLE_HPP
#define OSRM_TABLE_HPP

#include <string>
#include <utility>
#include <vector>

#include <extr_map/common/matrix.hpp>

namespace extr_map {
namespace routing {
class Table {
public:
  static const int inf_distance = 4e8;

public:
  Table(const std::string &routes_file = "world.osrm")
      : routes_file{routes_file} {}

  bool compute(const std::vector<std::pair<double, double>> &latlon,
               const char *table_type = "distances");

  const matrix::DenseMatrix<double> &distances() const { return m_distances; }
  matrix::DenseMatrix<double> &distances() { return m_distances; }

private:
  std::string routes_file;
  matrix::DenseMatrix<double> m_distances;
};
} // namespace routing
} // namespace extr_map

#endif // OSRM_TABLE_HPP
