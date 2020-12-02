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
#ifndef EXTRACTMAP_PRUNING_GEO_PRUNING_CPP
#define EXTRACTMAP_PRUNING_GEO_PRUNING_CPP

#include <utility>
#include <vector>

namespace extr_map {
namespace pruning {
class GeoPruning {
 public:
  GeoPruning() {}
  bool compute(const std::vector<std::pair<double, double>>& latlon,
               const double knnroute = 2, const double alpha = .9);
};
}
}

#endif  // EXTRACTMAP_PRUNING_GEO_PRUNING_CPP
