/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_MATH_LU_DECOMPOSITION_H
#define TL_MATH_LU_DECOMPOSITION_H

#include "config_tl.h"

#include <algorithm>

#ifdef HAVE_OPENBLAS
#include <lapacke.h>
#endif // HAVE_OPENBLAS

#include "tidop/math/math.h"
#include "tidop/core/messages.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"



namespace tl
{

namespace math
{

//template<typename T, size_t _rows, size_t _cols>
//class Matrix;

/*! \addtogroup Math
 *  \{
 */

/*! \addtogroup Algebra
 *  \{
 */


//#ifdef HAVE_OPENBLAS
//
//template<typename T> inline
//typename std::enable_if<
//    std::is_same<float, typename std::remove_cv<T>::type>::value, int>::type
//lapackeGETRF(lapack_int rows, lapack_int cols, T *a, lapack_int lda, lapack_int *ipiv)
//{
//  lapack_int info = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, ipiv);
//  return info;
//}
//
//template<typename T> inline
//typename std::enable_if<
//    std::is_same<double, typename std::remove_cv<T>::type>::value, int>::type
//lapackeGETRF(lapack_int rows, lapack_int cols, T *a, lapack_int lda, lapack_int *ipiv)
//{
//  lapack_int info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, ipiv);
//  return info;
//}
//
//template<typename T> inline
//typename std::enable_if<
//    std::is_same<float, typename std::remove_cv<T>::type>::value, int>::type
//lapackeGETRS(lapack_int rows, lapack_int nrhs, T *a, lapack_int lda, lapack_int *ipiv, T *b, lapack_int ldb)
//{
//  lapack_int info = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', rows, nrhs, a, lda, ipiv, b, ldb);
//  return info;
//}
//
//template<typename T> inline
//typename std::enable_if<
//    std::is_same<double, typename std::remove_cv<T>::type>::value, int>::type
//lapackeGETRS(lapack_int rows, lapack_int nrhs, T *a, lapack_int lda, lapack_int *ipiv, T *b, lapack_int ldb)
//{
//  lapack_int info = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', rows, nrhs, a, lda, ipiv, b, ldb);
//  return info;
//}
//
//#endif // HAVE_OPENBLAS



/*!
 * \brief Factorización o descomposición LU
 *
 * Sea A una matriz no singular (si lo fuera, entonces la descomposición podría no ser única)
 *
 * \f[ A=LU \f]
 *
 * donde L y U son matrices inferiores y superiores triangulares respectivamente.
 *
 */
template<typename T>
class LuDecomposition;


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
class LuDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

  LuDecomposition(const Matrix_t<T, _rows, _cols> &a);

  Vector<T, _rows> solve(const Vector<T, _rows> &b);
  Matrix<T> solve(const Matrix<T> &b);
  Matrix<T, _rows, _cols> lu() const;

  T determinant() const;
  Matrix<T, _rows, _cols> inverse();

private:

  void decompose();
  tl::math::Vector<T, _rows> findMaxElementsByRows();
//#ifdef HAVE_OPENBLAS
//  void lapackeDecompose();
//#endif // HAVE_OPENBLAS

private:

  Matrix<T, _rows, _cols> LU;
  Vector<size_t, _rows> mIndx;
  T d;
  size_t mRows;
};


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
LuDecomposition<Matrix_t<T, _rows, _cols>>::LuDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : LU(a),
    mIndx(a.rows()),
    mRows(a.rows())
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");

//#ifdef HAVE_OPENBLAS
//  this->lapackeDecompose();
//#else
  this->decompose();
//#endif // HAVE_OPENBLAS

}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _rows> LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b)
{
  TL_ASSERT(b.size() == mRows, "LuDecomposition::solve bad sizes")

  Vector<T, _rows> x(b);

//#ifdef HAVE_OPENBLAS    
//  lapack_int nrhs = 1; ///¿Porque es 1?
//  lapack_int lda = mRows;
//  lapack_int *ipiv = new lapack_int[mRows];
//  lapack_int ldb = x.size();
//
//  lapack_int info = lapackeGETRS(mRows, nrhs, LU.data(), lda, ipiv, x.data(), ldb);
//
//  delete[] ipiv;
//
//#else

  T sum;
  size_t ii = 0;
  size_t j;
  size_t ip;
  for (size_t i = 0; i < mRows; i++) {
    ip = mIndx[i];
    sum = x[ip];
    x[ip] = x[i];
    if (ii != 0) {
      for (j = ii - 1; j < i; j++) {
        sum -= LU[i][j] * x[j]; 
      }
    } else if (sum != math::consts::zero<T>) {
      ii = i + 1;
    }
      
    x[i] = sum;
  }

  for (int i = static_cast<int>(mRows) - 1; i >= 0; i--) {
    sum = x[i];
    for (j = i + 1; j < mRows; j++) sum -= LU[i][j] * x[j];
    x[i] = sum / LU[i][i];
  }

//#endif

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Matrix<T> LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Matrix<T> &b) ///Por ahora solo funciona con matrizes dinamicas
{
  TL_ASSERT(b.rows() == mRows, "LuDecomposition::solve bad sizes")
  
  Matrix<T> x(b);

//#ifdef HAVE_OPENBLAS    
//  lapack_int info;
//  lapack_int nrhs = 1; ///¿Porque es 1?
//  lapack_int lda = mRows;
//  lapack_int *ipiv = new lapack_int[mRows];
//  lapack_int ldb = mRows;
//
//  info = lapackeGETRS(mRows, nrhs, LU.data(), lda, ipiv, x.data(), ldb);
//
//  delete[] ipiv;
//
//#else

  Vector<T, _rows> xx(mRows);


  for (size_t j = 0; j < mRows; j++) {

    for (size_t i = 0; i < mRows; i++) {
      xx[i] = b[i][j];
    }

    xx = this->solve(xx);

    for (size_t i = 0; i < mRows; i++) { 
      x[i][j] = xx[i]; 
    }
  }

//#endif

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void LuDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{

  T big;
  T temp;

  this->d = consts::one<T>;

  Vector<T, _rows> max_elements = findMaxElementsByRows();

  size_t pivot_row = 0;

  for (size_t k = 0; k < mRows; k++) {

    big = consts::zero<T>;
    pivot_row = k;

    for (size_t i = k; i < mRows; i++) {
      temp = abs(LU[i][k]) / max_elements[i];
      if (temp > big) {
        big = temp;
        pivot_row = i;
      }
    }

    if (k != pivot_row) {
      LU.swapRows(pivot_row, k);
      this->d = -this->d;
      max_elements[pivot_row] = max_elements[k];
    }

    mIndx[k] = pivot_row;

    if (LU[k][k] == consts::zero<T>) 
      LU[k][k] = std::numeric_limits<T>().min();
    
    for (size_t i = k + 1; i < mRows; i++) {
    
      temp = LU[i][k] /= LU[k][k];
      
      for (size_t j = k + 1; j < mRows; j++)
        LU[i][j] -= temp * LU[k][j];
    }

    //std::cout << LU << std::endl;
  }
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
tl::math::Vector<T, _rows> LuDecomposition<Matrix_t<T, _rows, _cols>>::findMaxElementsByRows()
{
  Vector<T, _rows> max_elements(mRows, 0);
  T element;
  T max;

  for (size_t r = 0; r < mRows; r++) {

    max = consts::zero<T>;

    for (size_t c = 0; c < mRows; c++) {
      if ((element = abs(LU[r][c])) > max) {
        max = element;
      }
    }

    TL_ASSERT(max != consts::zero<T>, "Singular matrix")

    max_elements[r] = max;
  }

  return max_elements;
}

//#ifdef HAVE_OPENBLAS
//
//template<
//  template<typename, size_t, size_t> 
//  class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//inline void LuDecomposition<Matrix_t<T, _rows, _cols>>::lapackeDecompose()
//{
//  lapack_int info;
//  lapack_int lda = mRows;
//  lapack_int *ipiv = new lapack_int[mRows];
//  
//  info = lapackeGETRF(mRows, mRows, LU.data(), lda, ipiv);
//
//  delete[] ipiv;
//
//  TL_ASSERT(info == 0, "The algorithm computing LU failed to converge.")
//}
//
//#endif // HAVE_OPENBLAS

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline Matrix<T, _rows, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::lu() const
{
  return LU;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
T LuDecomposition<Matrix_t<T, _rows, _cols>>::determinant() const
{
  T det = this->d;

	for (size_t i = 0; i < mRows; i++) 
    det *= LU[i][i];

	return det;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
Matrix<T, _rows, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::inverse()
{
  Matrix<T, _rows, _cols> matrix = Matrix<T, _rows, _cols>::identity(mRows, mRows);

  Matrix<T, _rows, _cols> inv = this->solve(matrix);
  return inv;
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_LU_DECOMPOSITION_H
