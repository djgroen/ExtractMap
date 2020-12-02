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

#include <cstring>
#include <stdlib.h>

template <typename T>
void extr_map::matrix::DenseMatrix<T>::allocate(size_type row_size) {
  // Allocate a buffer big enough to hold the matrix with the given number of
  // bytes per row
  if (row_size >= rows()) {
    values = (value_type **)malloc(sizeof(value_type *) * rows());
    value_type *mem_chunk =
        (value_type *)malloc(sizeof(value_type) * row_size * rows());
    for (size_type i = 0; i < rows(); i++) {
      values[i] = mem_chunk;
      mem_chunk += row_size;
    }
  } else
    throw;
}

template <typename T>
extr_map::matrix::DenseMatrix<T>::DenseMatrix(size_type rows, size_type cols,
                                           size_type padding)
    : _rows{rows}, _cols{cols} {
  allocate(cols + padding);
}

template <typename T>
void extr_map::matrix::DenseMatrix<T>::reserve(size_type rows, size_type cols,
                                            size_type padding) {
  _rows = rows;
  _cols = cols;
  if (values != nullptr) {
    free(values[0]);
    free(values);
  }
  allocate(cols + padding);
}

template <typename T> void extr_map::matrix::DenseMatrix<T>::zeros() {
  std::memset(values[0], 0,
              sizeof(T) *
                  (cols() > 1 ? rows() * (values[1] - values[0]) : rows()));
}

template <typename T> extr_map::matrix::DenseMatrix<T>::~DenseMatrix() {
  free(values[0]);
  free(values);
}
