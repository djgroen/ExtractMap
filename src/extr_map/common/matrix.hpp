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
#ifndef EXTRACTMAP_COMMON_MATRIX_HPP
#define EXTRACTMAP_COMMON_MATRIX_HPP

#include <utility>
#include <vector>
// TODO: add iterators
namespace extr_map {
namespace matrix {

// Lightweight dense matrix implementation
template <typename T> class DenseMatrix final {
public:
  typedef std::size_t size_type;
  typedef T value_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;

  DenseMatrix() : _rows{0}, _cols{0}, values{nullptr} {}
  DenseMatrix(size_type m, size_type n, size_type padding = 0);
  DenseMatrix(size_type n) : DenseMatrix{n, n, 0} {}
  ~DenseMatrix();

  void reserve(size_type m, size_type n, size_type padding = 0);
  void zeros();
  size_type rows() const { return _rows; }
  size_type cols() const { return _cols; }
  value_type *operator[](size_type i) noexcept { return values[i]; }
  const value_type *operator[](size_type i) const { return values[i]; }

private:
  void allocate(size_type row_size);

private:
  size_type _rows;
  size_type _cols;
  value_type **values;
};

// Lightweight dense matrix implementation
class UTriMatrix {
public:
  typedef std::size_t size_type;
  typedef double value_type;

  UTriMatrix(size_type n);
  ~UTriMatrix();

  int rows() const { return _rows; }
  int cols() const { return _cols; }
  value_type *operator[](size_type i) noexcept { return values[i]; }
  const value_type *operator[](size_type i) const { return values[i]; }

private:
  size_type _rows;
  size_type _cols;
  value_type **values;
};

} // namespace matrix
} // namespace extr_map

#include <extr_map/common/detail/matrix_impl.hpp>

#endif // EXTRACTMAP_COMMON_MATRIX_HPP
