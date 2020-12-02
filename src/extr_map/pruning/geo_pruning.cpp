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

#include <extr_map/pruning/geo_pruning.hpp>

// #include <string>
// #include <ostream>
// #include <utility>

// #define DEBUG_MSG(text) {}
// #define DEBUG_MSG(text) std::cerr << text

#include <geodesic.h>
#include <algorithm>

namespace distance {
namespace ellipsoid {
struct ellipsoid {
  double a;  // the equatorial radius
  double f;  // the flattening
};
static const struct ellipsoid WGS84 { 6378137, 1. / 298.257223563 };
}

double geodesic(const double lat1, const double lon1, const double lat2,
                const double lon2, ellipsoid::ellipsoid geom) {
  double s12;
  struct geod_geodesic g;
  geod_init(&g, geom.a, geom.f);
  geod_inverse(&g, lat1, lon1, lat2, lon2, &s12, nullptr, nullptr);
  return s12;
}

class Geodesic {
 public:
  inline Geodesic(ellipsoid::ellipsoid geom = ellipsoid::WGS84) {
    geod_init(&g, geom.a, geom.f);
  }
  inline double operator()(const double lat1, const double lon1,
                           const double lat2, const double lon2) const {
    double s12;
    geod_inverse(&g, lat1, lon1, lat2, lon2, &s12, nullptr, nullptr);
    return s12;
  }

  inline double operator()(const std::pair<double, double> pt1,
                           const std::pair<double, double> pt2) const {
    double s12;
    geod_inverse(&g, pt1.first, pt1.second, pt2.first, pt2.second, &s12,
                 nullptr, nullptr);
    return s12;
  }

  inline double radius() const { return g.a; }
  inline double flattening() const { return g.f; }

 private:
  struct geod_geodesic g;
};

// vincenty() {}
}

// TODO exploit symmetry
bool extr_map::pruning::GeoPruning::compute(
    const std::vector<std::pair<double, double>>& latlons,
    const double knnroute, const double alpha) {
  if (knnroute >= latlons.size() || knnroute < 1) return false;
  distance::Geodesic geodesic{};
  std::vector<double> distances;
  std::vector<double> psorted_distances;
  for (std::size_t i = 0; i < latlons.size() - 1; i++) {
    for (std::size_t j     = 0; j < latlons.size(); j++)  // j=i+1...
      psorted_distances[j] = distances[j] = geodesic(latlons[i], latlons[j]);
    std::nth_element(psorted_distances.begin(),
                     psorted_distances.begin() + knnroute - 1,
                     psorted_distances.end());
    double kth_dist = psorted_distances[knnroute - 1] * alpha;
    // identify
    for (std::size_t j = i + 1; j < latlons.size(); j++)
      if (distances[j] > kth_dist) distances[j] = 0;
  }
  return true;
}
