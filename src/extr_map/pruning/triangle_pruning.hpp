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
#ifndef EXTRACTMAP_PRUNING_TRIANGLE_PRUNING_CPP
#define EXTRACTMAP_PRUNING_TRIANGLE_PRUNING_CPP

#include <extr_map/common/matrix.hpp>
#include <utility>

namespace extr_map {
namespace pruning {

constexpr double inf_distance = 4e6;
bool is_inf(const double r);

class TrianglePruning {
public:
  TrianglePruning() {}
  bool compute(matrix::DenseMatrix<double> &routes,
               double triangle_factor = .9);
  const extr_map::matrix::DenseMatrix<bool> &compute() const { return graph; }

private:
  extr_map::matrix::DenseMatrix<bool> graph;
};
} // namespace pruning
} // namespace extr_map

#endif // EXTRACTMAP_PRUNING_TRIANGLE_PRUNING_CPP
