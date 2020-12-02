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

#include <extr_map/pruning/triangle_pruning.hpp>

// #include <omp.h>
#include <stdlib.h>

// #define DEBUG_MSG(text) {}
// #define DEBUG_MSG(text) std::cerr << text

extr_map::matrix::UTriMatrix::UTriMatrix(size_type n) : _rows{n}, _cols{n} {
  // Allocate a buffer big enough to hold the matrix.
  values = (value_type **)malloc(sizeof(value_type *) * _rows);
  value_type *mem_chunk =
      (value_type *)malloc(((n * (n - 1)) >> 1) * sizeof(value_type));
  for (extr_map::matrix::UTriMatrix::UTriMatrix::size_type i = 0; i < _rows; i++) {
    values[i] = mem_chunk - 1; // do not include diagonal
    mem_chunk += _cols - i;
  }
}

extr_map::matrix::UTriMatrix::~UTriMatrix() {
  free(values[0] + 1); // take into account diagonal
  free(values);
}

bool extr_map::pruning::is_inf(const double r) { return inf_distance - r < 0.1; }

// bool extr_map::pruning::TrianglePruning::compute(
//     std::vector<std::vector<double>>& routes, double triangle_factor) {
//   int size = routes.size();
//   for (int k = 2; k < size; k++)
//     for (int j = 1; j < k; j++)
//       for (int i = 0; i < j; i++) {
//         double a = routes[i][j], b = routes[i][k], c = routes[j][k];
//         double d_max;  //= std::max(std::max(a,b),c); //std::max({a,b,c});
//         double s = a + b + c;
//         double* d;
//         if (a < b) {
//           if (b < c) {
//             d     = &routes[j][k];
//             d_max = c;
//             s     = a + b;
//           } else {
//             d     = &routes[i][k];
//             d_max = b;
//             s     = a + c;
//           }
//         } else {
//           if (a < c) {
//             d = &routes[j][k], d_max = c, s = a + b;
//           } else {
//             d     = &routes[i][j];
//             d_max = a;
//             s     = b + c;
//           }
//         }
//         if (d_max > triangle_factor * s) *d = inf_distance;
//       }
//   return true;
// }

double pass(double *x) { return *x + 1; }

bool extr_map::pruning::TrianglePruning::compute(
    extr_map::matrix::DenseMatrix<double> &routes, double triangle_factor) {
  // int size = routes.size();
  int size = routes.rows();
  graph.reserve(routes.cols(), routes.rows());
  graph.zeros();
#pragma omp parallel for schedule(static, 1)
  for (int i = 1; i < size - 2; i++)
    for (int j = i; j < size - 1; j++)
      for (int k = j; k < size; k++) {
        double a = routes[i][j], b = routes[i][k], c = routes[j][k];
        double d_max; //= std::max(std::max(a,b),c); //std::max({a,b,c});
        double s;
        int x, y;
        if (a < b) {
          if (b < c) {
            x = j;
            y = k;
            d_max = c;
            s = a + b;
          } else {
            x = i;
            y = k;
            d_max = b;
            s = a + c;
          }
        } else {
          if (a < c) {
            x = j;
            y = k;
            d_max = c;
            s = a + b;
          } else {
            x = i;
            y = j;
            d_max = a;
            s = b + c;
          }
        }
        if (d_max > triangle_factor * s)
          graph[x][y] = true;
      }
  return true;
}
