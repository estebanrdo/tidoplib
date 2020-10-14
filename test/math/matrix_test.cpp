#define BOOST_TEST_MODULE Tidop matrix test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/matrix.h>

using namespace tl::math;

BOOST_AUTO_TEST_SUITE(MatrixTestSuite)

struct MatrixTest 
{
  MatrixTest() 
    : _mat_4x4_d(new Matrix<4, 4>())
  {}
  ~MatrixTest()
  { 
    if (_mat_4x4_d) delete _mat_4x4_d, _mat_4x4_d = nullptr;
  }
  
  void setup()
  {
    _mat_3x3_d.at(0, 0) = 1.5;
    _mat_3x3_d.at(0, 1) = 0.0;
    _mat_3x3_d.at(0, 2) = 2.5;
    _mat_3x3_d.at(1, 0) = 1.0;
    _mat_3x3_d.at(1, 1) = 1.0;
    _mat_3x3_d.at(1, 2) = 1.2;
    _mat_3x3_d.at(2, 0) = 1.3;
    _mat_3x3_d.at(2, 1) = 2.6;
    _mat_3x3_d.at(2, 2) = 0.3;

    
    _mat_4x4_d->at(0, 0) = 4.5;    _mat_4x4_d->at(0, 1) = 2.7;  _mat_4x4_d->at(0, 2) = 5.5;  _mat_4x4_d->at(0, 3) = 4.98;
    _mat_4x4_d->at(1, 0) = 1.36;   _mat_4x4_d->at(1, 1) = 7.62;	_mat_4x4_d->at(1, 2) = 78.3; _mat_4x4_d->at(1, 3) = 45.5;
    _mat_4x4_d->at(2, 0) = 14.3;   _mat_4x4_d->at(2, 1) = 45.3;	_mat_4x4_d->at(2, 2) = 5.;   _mat_4x4_d->at(2, 3) = 45.;
    _mat_4x4_d->at(3, 0) = 12.374; _mat_4x4_d->at(3, 1) = 41.6;	_mat_4x4_d->at(3, 2) = 1.3;  _mat_4x4_d->at(3, 3) = 12.7;

    mat_zero = Matrix<3, 3>::zero();
    mat_ones = Matrix<3, 3>::ones();
    mat_identity = Matrix<3, 3>::identity();

    _mat_5x5_d.at(0, 0) = 6;
    _mat_5x5_d.at(0, 1) = 8;
    _mat_5x5_d.at(0, 2) = 6;
    _mat_5x5_d.at(0, 3) = 7;
    _mat_5x5_d.at(0, 4) = 3;
    _mat_5x5_d.at(1, 0) = 9;
    _mat_5x5_d.at(1, 1) = 6;
    _mat_5x5_d.at(1, 2) = 2;
    _mat_5x5_d.at(1, 3) = 3;
    _mat_5x5_d.at(1, 4) = 3;
    _mat_5x5_d.at(2, 0) = 8;
    _mat_5x5_d.at(2, 1) = 3;
    _mat_5x5_d.at(2, 2) = 2;
    _mat_5x5_d.at(2, 3) = 3;
    _mat_5x5_d.at(2, 4) = 3;
    _mat_5x5_d.at(3, 0) = 5;
    _mat_5x5_d.at(3, 1) = 3;
    _mat_5x5_d.at(3, 2) = 3;
    _mat_5x5_d.at(3, 3) = 7;
    _mat_5x5_d.at(3, 4) = 6;
    _mat_5x5_d.at(4, 0) = 5;
    _mat_5x5_d.at(4, 1) = 5;
    _mat_5x5_d.at(4, 2) = 7;
    _mat_5x5_d.at(4, 3) = 4;
    _mat_5x5_d.at(4, 4) = 7;


    _mat_2x3_i.at(0, 0) = 6;
    _mat_2x3_i.at(0, 1) = 8;
    _mat_2x3_i.at(0, 2) = 6;
    _mat_2x3_i.at(1, 0) = 9;
    _mat_2x3_i.at(1, 1) = 6;
    _mat_2x3_i.at(1, 2) = 2;

    
    _cofactor_matrix.at(0, 0) = 1;
    _cofactor_matrix.at(0, 1) = 2;
    _cofactor_matrix.at(0, 2) = 3;
    _cofactor_matrix.at(1, 0) = 0;
    _cofactor_matrix.at(1, 1) = 4;
    _cofactor_matrix.at(1, 2) = 5;
    _cofactor_matrix.at(2, 0) = 1;
    _cofactor_matrix.at(2, 1) = 0;
    _cofactor_matrix.at(2, 2) = 6;
  }
 
  void teardown()
  {

  }

  Matrix<2, 2> _mat_2x2_d;
  Matrix<3, 3> _mat_3x3_d;
  Matrix<4, 4> *_mat_4x4_d;
  Matrix<2, 2, float> _mat_2x2_f;
  Matrix<3, 3, float> _mat_3x3_f;
  Matrix<4, 4, float> _mat_4x4_f;
  Matrix<5, 5> _mat_5x5_d;
  Matrix<2, 3, int> _mat_2x3_i;
  Matrix3x3i _cofactor_matrix;
  Matrix<3, 3> mat_zero;
  Matrix<3, 3> mat_ones;
  Matrix<3, 3> mat_identity;

};


BOOST_FIXTURE_TEST_CASE(default_constructor, MatrixTest)
{
  constexpr double ini_value = -std::numeric_limits<double>().max();

  for (size_t r = 0; r < _mat_2x2_d.rows; r++){
    for (size_t c = 0; c < _mat_2x2_d.cols; c++){
      BOOST_CHECK_EQUAL(ini_value, _mat_2x2_d.at(r, c));
    }
  }

}

BOOST_FIXTURE_TEST_CASE(iterator_list_constructor, MatrixTest)
{

  Matrix<2, 2, int> it_list2 {{
    {
      {0, 1},
      {2, 3}
    }
  }};

  BOOST_CHECK_EQUAL(0, it_list2.at(0, 0));
  BOOST_CHECK_EQUAL(1, it_list2.at(0, 1));
  BOOST_CHECK_EQUAL(2, it_list2.at(1, 0));
  BOOST_CHECK_EQUAL(3, it_list2.at(1, 1));

  Matrix<2, 2, int> it_list3 = {
    {
      { 0, 1 },
      {2, 3}
    }
  };

  BOOST_CHECK_EQUAL(0, it_list3.at(0, 0));
  BOOST_CHECK_EQUAL(1, it_list3.at(0, 1));
  BOOST_CHECK_EQUAL(2, it_list3.at(1, 0));
  BOOST_CHECK_EQUAL(3, it_list3.at(1, 1));

  Matrix<2, 3, int> it_list2x3 = {
    {
      {0, 1, 2},
      {3, 4, 5}
    }
  };

  BOOST_CHECK_EQUAL(0, it_list2x3.at(0, 0));
  BOOST_CHECK_EQUAL(1, it_list2x3.at(0, 1));
  BOOST_CHECK_EQUAL(2, it_list2x3.at(0, 2));
  BOOST_CHECK_EQUAL(3, it_list2x3.at(1, 0));
  BOOST_CHECK_EQUAL(4, it_list2x3.at(1, 1));
  BOOST_CHECK_EQUAL(5, it_list2x3.at(1, 2));

  //Matrix<3, 3, int> it_list3x3 = {
  //  {
  //    {0, 1, 2},
  //    {3, 4, 5},
  //    {6, 7, 8}
  //  }
  //};

  //BOOST_CHECK_EQUAL(0, it_list3x3.at(0, 0));
  //BOOST_CHECK_EQUAL(1, it_list3x3.at(0, 1));
  //BOOST_CHECK_EQUAL(2, it_list3x3.at(0, 2));

  //Matrix<5, 5, int> it_list5 = {
  //  {
  //    {6, 8,6,7,3},
  //    {9,6,2,3,3},
  //    {8,3,2,3,3},
  //    {5,3,3,7,6},
  //    {5,5,7,4,7}
  //  }
  //};
}

BOOST_FIXTURE_TEST_CASE(at, MatrixTest)
{
  BOOST_CHECK_EQUAL(1.5, _mat_3x3_d.at(0, 0));
  BOOST_CHECK_EQUAL(0.0, _mat_3x3_d.at(0, 1));
  BOOST_CHECK_EQUAL(2.5, _mat_3x3_d.at(0, 2));
  BOOST_CHECK_EQUAL(1.0, _mat_3x3_d.at(1, 0));
  BOOST_CHECK_EQUAL(1.0, _mat_3x3_d.at(1, 1));
  BOOST_CHECK_EQUAL(1.2, _mat_3x3_d.at(1, 2));
  BOOST_CHECK_EQUAL(1.3, _mat_3x3_d.at(2, 0));
  BOOST_CHECK_EQUAL(2.6, _mat_3x3_d.at(2, 1));
  BOOST_CHECK_EQUAL(0.3, _mat_3x3_d.at(2, 2));
}

BOOST_FIXTURE_TEST_CASE(rows, MatrixTest)
{
  BOOST_CHECK_EQUAL(2, _mat_2x2_d.rows);
  BOOST_CHECK_EQUAL(3, _mat_3x3_d.rows);
  BOOST_CHECK_EQUAL(3, _mat_3x3_f.rows);
}

BOOST_FIXTURE_TEST_CASE(cols, MatrixTest)
{
  BOOST_CHECK_EQUAL(2, _mat_2x2_d.cols);
  BOOST_CHECK_EQUAL(3, _mat_3x3_d.cols);
  BOOST_CHECK_EQUAL(3, _mat_3x3_f.cols);
}

BOOST_FIXTURE_TEST_CASE(inverse2x2, MatrixTest)
{
  Matrix<2, 2> _mat_2x2;
  _mat_2x2.at(0, 0) = 2;
  _mat_2x2.at(0, 1) = 3;
  _mat_2x2.at(1, 0) = 1;
  _mat_2x2.at(1, 1) = 4;
  bool invertible;
  Matrix<2, 2> inv_mat = _mat_2x2.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE(0.8, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.6, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.2, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.4, inv_mat.at(1, 1), 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse3x3, MatrixTest)
{
  bool invertible;
  Matrix<3, 3> inv_mat = _mat_3x3_d.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE( 2.877551, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-6.632653, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE( 2.551020, inv_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-1.285714, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE( 2.857143, inv_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.714286, inv_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(-1.326531, inv_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE( 3.979592, inv_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-1.530612, inv_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse4x4, MatrixTest)
{
  bool invertible;
  Matrix<4, 4> inv_mat = _mat_4x4_d->inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE( 0.268435, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.018133, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.010673, inv_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.002479, inv_mat.at(0, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.077647, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE( 0.005298, inv_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.006780, inv_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE( 0.035491, inv_mat.at(1, 3), 0.1);
  BOOST_CHECK_CLOSE( 0.007528, inv_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE( 0.013172, inv_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.019248, inv_mat.at(2, 2), 0.1);
  BOOST_CHECK_CLOSE( 0.018059, inv_mat.at(2, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.007974, inv_mat.at(3, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.001035, inv_mat.at(3, 1), 0.1);
  BOOST_CHECK_CLOSE( 0.034578, inv_mat.at(3, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.036946, inv_mat.at(3, 3), 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse, MatrixTest)
{
  bool invertible;
  Matrix<5, 5> inv_mat = _mat_5x5_d.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE(-0.00208479, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.0799166, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0.273801, inv_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.066713, inv_mat.at(0, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.0250174, inv_mat.at(0, 4), 0.1);
  BOOST_CHECK_CLOSE(0.000694945, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.359972, inv_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.4246, inv_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0.0222376, inv_mat.at(1, 3), 0.1);
  BOOST_CHECK_CLOSE(0.00833914, inv_mat.at(1, 4), 0.1);
  BOOST_CHECK_CLOSE(0.134468, inv_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.345379, inv_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(0.339819, inv_mat.at(2, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.197012, inv_mat.at(2, 3), 0.1);
  BOOST_CHECK_CLOSE(0.113621, inv_mat.at(2, 4), 0.1);
  BOOST_CHECK_CLOSE(0.113621, inv_mat.at(3, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.144545, inv_mat.at(3, 1), 0.1);
  BOOST_CHECK_CLOSE(0.0778318, inv_mat.at(3, 2), 0.1);
  BOOST_CHECK_CLOSE(0.135858, inv_mat.at(3, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.136553, inv_mat.at(3, 4), 0.1);                 
  BOOST_CHECK_CLOSE(-0.198402, inv_mat.at(4, 0), 0.1);
  BOOST_CHECK_CLOSE(0.227936, inv_mat.at(4, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.276581, inv_mat.at(4, 2), 0.1);
  BOOST_CHECK_CLOSE(0.151147, inv_mat.at(4, 3), 0.1);
  BOOST_CHECK_CLOSE(0.11918, inv_mat.at(4, 4), 0.1);                 
                        
}

BOOST_FIXTURE_TEST_CASE(transpose_squared, MatrixTest)
{
  Matrix<3, 3> transp_mat = _mat_3x3_d.transpose();

  BOOST_CHECK_EQUAL(1.5, transp_mat.at(0, 0));
  BOOST_CHECK_EQUAL(1, transp_mat.at(0, 1));
  BOOST_CHECK_EQUAL(1.3, transp_mat.at(0, 2));
  BOOST_CHECK_EQUAL(0, transp_mat.at(1, 0));
  BOOST_CHECK_EQUAL(1, transp_mat.at(1, 1));
  BOOST_CHECK_EQUAL(2.6, transp_mat.at(1, 2));
  BOOST_CHECK_EQUAL(2.5, transp_mat.at(2, 0));
  BOOST_CHECK_EQUAL(1.2, transp_mat.at(2, 1));
  BOOST_CHECK_EQUAL(0.3, transp_mat.at(2, 2));
}


/// Transpuesta de matriz no cuadrada
BOOST_FIXTURE_TEST_CASE(transpose, MatrixTest)
{
  Matrix<3, 2, int> transp_mat = _mat_2x3_i.transpose();

  BOOST_CHECK_EQUAL(6, transp_mat.at(0, 0));
  BOOST_CHECK_EQUAL(9, transp_mat.at(0, 1));
  BOOST_CHECK_EQUAL(8, transp_mat.at(1, 0));
  BOOST_CHECK_EQUAL(6, transp_mat.at(1, 1));
  BOOST_CHECK_EQUAL(6, transp_mat.at(2, 0));
  BOOST_CHECK_EQUAL(2, transp_mat.at(2, 1));
}

BOOST_FIXTURE_TEST_CASE(determinant2x2, MatrixTest)
{
  Matrix<2, 2> _mat_2x2;
  _mat_2x2.at(0, 0) = 2;
  _mat_2x2.at(0, 1) = 3;
  _mat_2x2.at(1, 0) = 1;
  _mat_2x2.at(1, 1) = 4;
  BOOST_CHECK_EQUAL(5, _mat_2x2.determinant());
}

BOOST_FIXTURE_TEST_CASE(determinant3x3, MatrixTest)
{
  BOOST_CHECK_CLOSE(-0.98, _mat_3x3_d.determinant(),0.1);
}

BOOST_FIXTURE_TEST_CASE(determinant4x4, MatrixTest)
{
  BOOST_CHECK_CLOSE(353100.53, _mat_4x4_d->determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinantnxn, MatrixTest)
{
  BOOST_CHECK_CLOSE(-2877.99, _mat_5x5_d.determinant(), 0.1);
}

/// http://www.mathwords.com/c/cofactor_matrix.htm
BOOST_FIXTURE_TEST_CASE(cofactor_matrix, MatrixTest)
{
  Matrix3x3i cofactor_matrix = _cofactor_matrix.cofactorMatrix();

  BOOST_CHECK_EQUAL( 24, cofactor_matrix.at(0, 0));
  BOOST_CHECK_EQUAL(  5, cofactor_matrix.at(0, 1));
  BOOST_CHECK_EQUAL( -4, cofactor_matrix.at(0, 2));
  BOOST_CHECK_EQUAL(-12, cofactor_matrix.at(1, 0));
  BOOST_CHECK_EQUAL(  3, cofactor_matrix.at(1, 1));
  BOOST_CHECK_EQUAL(  2, cofactor_matrix.at(1, 2));
  BOOST_CHECK_EQUAL( -2, cofactor_matrix.at(2, 0));
  BOOST_CHECK_EQUAL( -5, cofactor_matrix.at(2, 1));
  BOOST_CHECK_EQUAL(  4, cofactor_matrix.at(2, 2));
}

BOOST_FIXTURE_TEST_CASE(first_minor, MatrixTest)
{
  int first_minor = _cofactor_matrix.firstMinor(1, 1);

  BOOST_CHECK_EQUAL(3, first_minor);

  first_minor = _cofactor_matrix.firstMinor(0, 1);

  BOOST_CHECK_EQUAL(-5, first_minor);
}

BOOST_FIXTURE_TEST_CASE(cofactor, MatrixTest)
{
  int cofactor = _cofactor_matrix.cofactor(1,1);

  BOOST_CHECK_EQUAL(3, cofactor);

  cofactor = _cofactor_matrix.cofactor(0, 1);

  BOOST_CHECK_EQUAL(5, cofactor);
}

BOOST_FIXTURE_TEST_CASE(zero, MatrixTest)
{
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      BOOST_CHECK_EQUAL(0, mat_zero.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(ones, MatrixTest)
{
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      BOOST_CHECK_EQUAL(1, mat_ones.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(identity, MatrixTest)
{
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      if (r == c) {
        BOOST_CHECK_EQUAL(1, mat_identity.at(r, c));
      } else {
        BOOST_CHECK_EQUAL(0, mat_identity.at(r, c));
      }
    }
  }
}

/* Operaciones unarias */

BOOST_FIXTURE_TEST_CASE(plus, MatrixTest)
{
  Matrix3x3d mat = +_mat_3x3_d;
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      BOOST_CHECK_EQUAL(mat.at(r,c), _mat_3x3_d.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(minus, MatrixTest)
{
  Matrix3x3d mat = -_mat_3x3_d;
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      BOOST_CHECK_EQUAL(-mat.at(r, c), _mat_3x3_d.at(r, c));
    }
  }
}

/* Operaciones binarias entre matrices */

/// Suma o adición de matrices

BOOST_FIXTURE_TEST_CASE(addition, MatrixTest)
{
  Matrix3x3d mat = mat_ones + _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat.at(0, 0));
  BOOST_CHECK_EQUAL(1.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(3.5, mat.at(0, 2));
  BOOST_CHECK_EQUAL(2.0, mat.at(1, 0));
  BOOST_CHECK_EQUAL(2.0, mat.at(1, 1));
  BOOST_CHECK_EQUAL(2.2, mat.at(1, 2));
  BOOST_CHECK_EQUAL(2.3, mat.at(2, 0));
  BOOST_CHECK_EQUAL(3.6, mat.at(2, 1));
  BOOST_CHECK_EQUAL(1.3, mat.at(2, 2));

  mat_ones += _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat_ones.at(0, 0));
  BOOST_CHECK_EQUAL(1.0, mat_ones.at(0, 1));
  BOOST_CHECK_EQUAL(3.5, mat_ones.at(0, 2));
  BOOST_CHECK_EQUAL(2.0, mat_ones.at(1, 0));
  BOOST_CHECK_EQUAL(2.0, mat_ones.at(1, 1));
  BOOST_CHECK_EQUAL(2.2, mat_ones.at(1, 2));
  BOOST_CHECK_EQUAL(2.3, mat_ones.at(2, 0));
  BOOST_CHECK_EQUAL(3.6, mat_ones.at(2, 1));
  BOOST_CHECK_EQUAL(1.3, mat_ones.at(2, 2));
}

/// Resta de matrices

BOOST_FIXTURE_TEST_CASE(subtraction, MatrixTest)
{
  Matrix3x3d mat = mat_identity - _mat_3x3_d;

  BOOST_CHECK_EQUAL(-0.5, mat.at(0, 0));
  BOOST_CHECK_EQUAL( 0.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(-2.5, mat.at(0, 2));
  BOOST_CHECK_EQUAL(-1.0, mat.at(1, 0));
  BOOST_CHECK_EQUAL( 0.0, mat.at(1, 1));
  BOOST_CHECK_EQUAL(-1.2, mat.at(1, 2));
  BOOST_CHECK_EQUAL(-1.3, mat.at(2, 0));
  BOOST_CHECK_EQUAL(-2.6, mat.at(2, 1));
  BOOST_CHECK_EQUAL( 0.7, mat.at(2, 2));

  mat_identity -= _mat_3x3_d;

  BOOST_CHECK_EQUAL(-0.5, mat_identity.at(0, 0));
  BOOST_CHECK_EQUAL( 0.0, mat_identity.at(0, 1));
  BOOST_CHECK_EQUAL(-2.5, mat_identity.at(0, 2));
  BOOST_CHECK_EQUAL(-1.0, mat_identity.at(1, 0));
  BOOST_CHECK_EQUAL( 0.0, mat_identity.at(1, 1));
  BOOST_CHECK_EQUAL(-1.2, mat_identity.at(1, 2));
  BOOST_CHECK_EQUAL(-1.3, mat_identity.at(2, 0));
  BOOST_CHECK_EQUAL(-2.6, mat_identity.at(2, 1));
  BOOST_CHECK_EQUAL( 0.7, mat_identity.at(2, 2));
}

/// Multiplicación de matrices

BOOST_FIXTURE_TEST_CASE(multiplication, MatrixTest)
{
  Matrix<2,3,int> mat = _mat_2x3_i * _cofactor_matrix;

  BOOST_CHECK_EQUAL(12, mat.at(0, 0));
  BOOST_CHECK_EQUAL(44, mat.at(0, 1));
  BOOST_CHECK_EQUAL(94, mat.at(0, 2));
  BOOST_CHECK_EQUAL(11, mat.at(1, 0));
  BOOST_CHECK_EQUAL(42, mat.at(1, 1));
  BOOST_CHECK_EQUAL(69, mat.at(1, 2));

}

/// Multiplicación de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(MatrixScalar, MatrixTest)
{
  Matrix<2,3,int> mat = _mat_2x3_i * 10;

  BOOST_CHECK_EQUAL(60, mat.at(0, 0));
  BOOST_CHECK_EQUAL(80, mat.at(0, 1));
  BOOST_CHECK_EQUAL(60, mat.at(0, 2));
  BOOST_CHECK_EQUAL(90, mat.at(1, 0));
  BOOST_CHECK_EQUAL(60, mat.at(1, 1));
  BOOST_CHECK_EQUAL(20, mat.at(1, 2));

  _mat_2x3_i *= 10;

  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(0, 0));
  BOOST_CHECK_EQUAL(80, _mat_2x3_i.at(0, 1));
  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(0, 2));
  BOOST_CHECK_EQUAL(90, _mat_2x3_i.at(1, 0));
  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(1, 1));
  BOOST_CHECK_EQUAL(20, _mat_2x3_i.at(1, 2));
}

/// Multiplicación de un escalar por una matriz

BOOST_FIXTURE_TEST_CASE(scalar_matrix, MatrixTest)
{
  Matrix<2,3,int> mat = 10 * _mat_2x3_i;

  BOOST_CHECK_EQUAL(60, mat.at(0, 0));
  BOOST_CHECK_EQUAL(80, mat.at(0, 1));
  BOOST_CHECK_EQUAL(60, mat.at(0, 2));
  BOOST_CHECK_EQUAL(90, mat.at(1, 0));
  BOOST_CHECK_EQUAL(60, mat.at(1, 1));
  BOOST_CHECK_EQUAL(20, mat.at(1, 2));
}

// División de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(div_matrix_scalar, MatrixTest)
{
  Matrix<3,3,double> mat = _mat_3x3_d / 10.;

  _mat_3x3_d.at(0, 0) = 1.5;
  _mat_3x3_d.at(0, 1) = 0.0;
  _mat_3x3_d.at(0, 2) = 2.5;
  _mat_3x3_d.at(1, 0) = 1.0;
  _mat_3x3_d.at(1, 1) = 1.0;
  _mat_3x3_d.at(1, 2) = 1.2;
  _mat_3x3_d.at(2, 0) = 1.3;
  _mat_3x3_d.at(2, 1) = 2.6;
  _mat_3x3_d.at(2, 2) = 0.3;

  BOOST_CHECK_EQUAL(.15, mat.at(0, 0));
  BOOST_CHECK_EQUAL(.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(.25, mat.at(0, 2));
  BOOST_CHECK_EQUAL(.10, mat.at(1, 0));
  BOOST_CHECK_EQUAL(.10, mat.at(1, 1));
  BOOST_CHECK_EQUAL(.12, mat.at(1, 2));
  BOOST_CHECK_EQUAL(.13, mat.at(2, 0));
  BOOST_CHECK_EQUAL(.26, mat.at(2, 1));
  BOOST_CHECK_EQUAL(.03, mat.at(2, 2));
}

BOOST_AUTO_TEST_SUITE_END()




//
//
//BOOST_AUTO_TEST_SUITE(MatrixDynTestSuite)
//
//struct MatrixDynTest 
//{
//  MatrixDynTest() 
//    : _mat_2x2_d(new MatrixDyn<double>(2, 2)),
//      _mat_3x3_d(new MatrixDyn<double>(3, 3)),
//      _mat_3x3_f(new MatrixDyn<float>(3, 3)),
//      _mat_4x4_d(new MatrixDyn<double>(4, 4)),
//      _mat_5x5_d(new MatrixDyn<double>(5, 5)),
//      _mat_2x3_i(new MatrixDyn<int>(2, 3))
//  {}
//  ~MatrixDynTest()
//  { 
//    if (_mat_2x2_d) delete _mat_2x2_d, _mat_2x2_d = nullptr;
//    if (_mat_3x3_d) delete _mat_3x3_d, _mat_3x3_d = nullptr;
//    if (_mat_3x3_f) delete _mat_3x3_f, _mat_3x3_f = nullptr;
//    if (_mat_4x4_d) delete _mat_4x4_d, _mat_4x4_d = nullptr;
//    if (_mat_5x5_d) delete _mat_5x5_d, _mat_5x5_d = nullptr;
//    if (_mat_2x3_i) delete _mat_2x3_i, _mat_2x3_i = nullptr;
//  }
//  
//  void setup()
//  {
//
//    _mat_3x3_d->at(0, 0) = 1.5;
//    _mat_3x3_d->at(0, 1) = 0.0;
//    _mat_3x3_d->at(0, 2) = 2.5;
//    _mat_3x3_d->at(1, 0) = 1.0;
//    _mat_3x3_d->at(1, 1) = 1.0;
//    _mat_3x3_d->at(1, 2) = 1.2;
//    _mat_3x3_d->at(2, 0) = 1.3;
//    _mat_3x3_d->at(2, 1) = 2.6;
//    _mat_3x3_d->at(2, 2) = 0.3;
//
//    _mat_4x4_d->at(0, 0) = 4.5;    _mat_4x4_d->at(0, 1) = 2.7;  _mat_4x4_d->at(0, 2) = 5.5;  _mat_4x4_d->at(0, 3) = 4.98;
//    _mat_4x4_d->at(1, 0) = 1.36;   _mat_4x4_d->at(1, 1) = 7.62;	_mat_4x4_d->at(1, 2) = 78.3; _mat_4x4_d->at(1, 3) = 45.5;
//    _mat_4x4_d->at(2, 0) = 14.3;   _mat_4x4_d->at(2, 1) = 45.3;	_mat_4x4_d->at(2, 2) = 5.;   _mat_4x4_d->at(2, 3) = 45.;
//    _mat_4x4_d->at(3, 0) = 12.374; _mat_4x4_d->at(3, 1) = 41.6;	_mat_4x4_d->at(3, 2) = 1.3;  _mat_4x4_d->at(3, 3) = 12.7;
//
//    mat_zero = MatrixDyn<int>::zero(3, 3);
//    mat_ones = MatrixDyn<int>::ones(3, 3);
//    mat_identity = MatrixDyn<int>::identity(3, 3);
//
//    _mat_5x5_d->at(0, 0) = 6;
//    _mat_5x5_d->at(0, 1) = 8;
//    _mat_5x5_d->at(0, 2) = 6;
//    _mat_5x5_d->at(0, 3) = 7;
//    _mat_5x5_d->at(0, 4) = 3;
//    _mat_5x5_d->at(1, 0) = 9;
//    _mat_5x5_d->at(1, 1) = 6;
//    _mat_5x5_d->at(1, 2) = 2;
//    _mat_5x5_d->at(1, 3) = 3;
//    _mat_5x5_d->at(1, 4) = 3;
//    _mat_5x5_d->at(2, 0) = 8;
//    _mat_5x5_d->at(2, 1) = 3;
//    _mat_5x5_d->at(2, 2) = 2;
//    _mat_5x5_d->at(2, 3) = 3;
//    _mat_5x5_d->at(2, 4) = 3;
//    _mat_5x5_d->at(3, 0) = 5;
//    _mat_5x5_d->at(3, 1) = 3;
//    _mat_5x5_d->at(3, 2) = 3;
//    _mat_5x5_d->at(3, 3) = 7;
//    _mat_5x5_d->at(3, 4) = 6;
//    _mat_5x5_d->at(4, 0) = 5;
//    _mat_5x5_d->at(4, 1) = 5;
//    _mat_5x5_d->at(4, 2) = 7;
//    _mat_5x5_d->at(4, 3) = 4;
//    _mat_5x5_d->at(4, 4) = 7;
//
//
//    _mat_2x3_i->at(0, 0) = 6;
//    _mat_2x3_i->at(0, 1) = 8;
//    _mat_2x3_i->at(0, 2) = 6;
//    _mat_2x3_i->at(1, 0) = 9;
//    _mat_2x3_i->at(1, 1) = 6;
//    _mat_2x3_i->at(1, 2) = 2;
//
//    _cofactor_matrix.resize(3,3);
//    _cofactor_matrix.at(0, 0) = 1;
//    _cofactor_matrix.at(0, 1) = 2;
//    _cofactor_matrix.at(0, 2) = 3;
//    _cofactor_matrix.at(1, 0) = 0;
//    _cofactor_matrix.at(1, 1) = 4;
//    _cofactor_matrix.at(1, 2) = 5;
//    _cofactor_matrix.at(2, 0) = 1;
//    _cofactor_matrix.at(2, 1) = 0;
//    _cofactor_matrix.at(2, 2) = 6;
//  }
// 
//  void teardown()
//  {
//
//  }
//
//  MatrixDyn<double> *_mat_2x2_d;
//  MatrixDyn<double> *_mat_3x3_d;
//  MatrixDyn<double> *_mat_4x4_d;
//  MatrixDyn<float> *_mat_2x2_f;
//  MatrixDyn<float> *_mat_3x3_f;
//  MatrixDyn<float> *_mat_4x4_f;
//  MatrixDyn<double> *_mat_5x5_d;
//  MatrixDyn<int> *_mat_2x3_i;
//  MatrixDyn<int> _cofactor_matrix;
//  MatrixDyn<int> mat_zero;
//  MatrixDyn<int> mat_ones;
//  MatrixDyn<int> mat_identity;
//
//};
//
//
//BOOST_FIXTURE_TEST_CASE(default_constructor, MatrixDynTest)
//{
//  double ini_value = -std::numeric_limits<double>().max();
//
//  for (size_t r = 0; r < _mat_2x2_d->rows(); r++){
//    for (size_t c = 0; c < _mat_2x2_d->cols(); c++){
//      BOOST_CHECK_EQUAL(ini_value, _mat_2x2_d->at(r, c));
//    }
//  }
//
//}
//
////BOOST_FIXTURE_TEST_CASE(iterator_list_constructor, MatrixDynTest)
////{
////
////  Matrix<2, 2, int> it_list2 {{
////    {
////      {0, 1},
////      {2, 3}
////    }
////  }};
////
////  BOOST_CHECK_EQUAL(0, it_list2.at(0, 0));
////  BOOST_CHECK_EQUAL(1, it_list2.at(0, 1));
////  BOOST_CHECK_EQUAL(2, it_list2.at(1, 0));
////  BOOST_CHECK_EQUAL(3, it_list2.at(1, 1));
////
////  Matrix<2, 2, int> it_list3 = {
////    {
////      { 0, 1 },
////      {2, 3}
////    }
////  };
////
////  BOOST_CHECK_EQUAL(0, it_list3.at(0, 0));
////  BOOST_CHECK_EQUAL(1, it_list3.at(0, 1));
////  BOOST_CHECK_EQUAL(2, it_list3.at(1, 0));
////  BOOST_CHECK_EQUAL(3, it_list3.at(1, 1));
////
////  Matrix<2, 3, int> it_list2x3 = {
////    {
////      {0, 1, 2},
////      {3, 4, 5}
////    }
////  };
////
////  BOOST_CHECK_EQUAL(0, it_list2x3.at(0, 0));
////  BOOST_CHECK_EQUAL(1, it_list2x3.at(0, 1));
////  BOOST_CHECK_EQUAL(2, it_list2x3.at(0, 2));
////  BOOST_CHECK_EQUAL(3, it_list2x3.at(1, 0));
////  BOOST_CHECK_EQUAL(4, it_list2x3.at(1, 1));
////  BOOST_CHECK_EQUAL(5, it_list2x3.at(1, 2));
////
////}
//
//BOOST_FIXTURE_TEST_CASE(at, MatrixDynTest)
//{
//  BOOST_CHECK_EQUAL(1.5, _mat_3x3_d->at(0, 0));
//  BOOST_CHECK_EQUAL(0.0, _mat_3x3_d->at(0, 1));
//  BOOST_CHECK_EQUAL(2.5, _mat_3x3_d->at(0, 2));
//  BOOST_CHECK_EQUAL(1.0, _mat_3x3_d->at(1, 0));
//  BOOST_CHECK_EQUAL(1.0, _mat_3x3_d->at(1, 1));
//  BOOST_CHECK_EQUAL(1.2, _mat_3x3_d->at(1, 2));
//  BOOST_CHECK_EQUAL(1.3, _mat_3x3_d->at(2, 0));
//  BOOST_CHECK_EQUAL(2.6, _mat_3x3_d->at(2, 1));
//  BOOST_CHECK_EQUAL(0.3, _mat_3x3_d->at(2, 2));
//}
//
//BOOST_FIXTURE_TEST_CASE(rows, MatrixDynTest)
//{
//  BOOST_CHECK_EQUAL(2, _mat_2x2_d->rows());
//  BOOST_CHECK_EQUAL(3, _mat_3x3_d->rows());
//  BOOST_CHECK_EQUAL(3, _mat_3x3_f->rows());
//  BOOST_CHECK_EQUAL(4, _mat_4x4_d->rows());
//  BOOST_CHECK_EQUAL(5, _mat_5x5_d->rows());
//}
//
//BOOST_FIXTURE_TEST_CASE(cols, MatrixDynTest)
//{
//  BOOST_CHECK_EQUAL(2, _mat_2x2_d->cols());
//  BOOST_CHECK_EQUAL(3, _mat_3x3_d->cols());
//  BOOST_CHECK_EQUAL(3, _mat_3x3_f->cols());
//  BOOST_CHECK_EQUAL(4, _mat_4x4_d->cols());
//  BOOST_CHECK_EQUAL(5, _mat_5x5_d->cols());
//}
//
//BOOST_FIXTURE_TEST_CASE(inverse2x2, MatrixDynTest)
//{
//  MatrixDyn<double> _mat_2x2(2, 2);
//  _mat_2x2.at(0, 0) = 2;
//  _mat_2x2.at(0, 1) = 3;
//  _mat_2x2.at(1, 0) = 1;
//  _mat_2x2.at(1, 1) = 4;
//  bool invertible;
//  MatrixDyn<double> inv_mat = _mat_2x2.inverse(&invertible);
//  BOOST_CHECK(invertible);
//  BOOST_CHECK_CLOSE(0.8, inv_mat.at(0, 0), 0.1);
//  BOOST_CHECK_CLOSE(-0.6, inv_mat.at(0, 1), 0.1);
//  BOOST_CHECK_CLOSE(-0.2, inv_mat.at(1, 0), 0.1);
//  BOOST_CHECK_CLOSE(0.4, inv_mat.at(1, 1), 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(inverse3x3, MatrixDynTest)
//{
//  bool invertible;
//  MatrixDyn<double> inv_mat = _mat_3x3_d->inverse(&invertible);
//  BOOST_CHECK(invertible);
//  BOOST_CHECK_CLOSE( 2.877551, inv_mat.at(0, 0), 0.1);
//  BOOST_CHECK_CLOSE(-6.632653, inv_mat.at(0, 1), 0.1);
//  BOOST_CHECK_CLOSE( 2.551020, inv_mat.at(0, 2), 0.1);
//  BOOST_CHECK_CLOSE(-1.285714, inv_mat.at(1, 0), 0.1);
//  BOOST_CHECK_CLOSE( 2.857143, inv_mat.at(1, 1), 0.1);
//  BOOST_CHECK_CLOSE(-0.714286, inv_mat.at(1, 2), 0.1);
//  BOOST_CHECK_CLOSE(-1.326531, inv_mat.at(2, 0), 0.1);
//  BOOST_CHECK_CLOSE( 3.979592, inv_mat.at(2, 1), 0.1);
//  BOOST_CHECK_CLOSE(-1.530612, inv_mat.at(2, 2), 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(inverse4x4, MatrixDynTest)
//{
//  bool invertible;
//  MatrixDyn<double> inv_mat = _mat_4x4_d->inverse(&invertible);
//  BOOST_CHECK(invertible);
//  BOOST_CHECK_CLOSE( 0.268435, inv_mat.at(0, 0), 0.1);
//  BOOST_CHECK_CLOSE(-0.018133, inv_mat.at(0, 1), 0.1);
//  BOOST_CHECK_CLOSE(-0.010673, inv_mat.at(0, 2), 0.1);
//  BOOST_CHECK_CLOSE(-0.002479, inv_mat.at(0, 3), 0.1);
//  BOOST_CHECK_CLOSE(-0.077647, inv_mat.at(1, 0), 0.1);
//  BOOST_CHECK_CLOSE( 0.005298, inv_mat.at(1, 1), 0.1);
//  BOOST_CHECK_CLOSE(-0.006780, inv_mat.at(1, 2), 0.1);
//  BOOST_CHECK_CLOSE( 0.035491, inv_mat.at(1, 3), 0.1);
//  BOOST_CHECK_CLOSE( 0.007528, inv_mat.at(2, 0), 0.1);
//  BOOST_CHECK_CLOSE( 0.013172, inv_mat.at(2, 1), 0.1);
//  BOOST_CHECK_CLOSE(-0.019248, inv_mat.at(2, 2), 0.1);
//  BOOST_CHECK_CLOSE( 0.018059, inv_mat.at(2, 3), 0.1);
//  BOOST_CHECK_CLOSE(-0.007974, inv_mat.at(3, 0), 0.1);
//  BOOST_CHECK_CLOSE(-0.001035, inv_mat.at(3, 1), 0.1);
//  BOOST_CHECK_CLOSE( 0.034578, inv_mat.at(3, 2), 0.1);
//  BOOST_CHECK_CLOSE(-0.036946, inv_mat.at(3, 3), 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(inverse, MatrixDynTest)
//{
//  bool invertible;
//  MatrixDyn<double> inv_mat = _mat_5x5_d->inverse(&invertible);
//  BOOST_CHECK(invertible);
//  BOOST_CHECK_CLOSE(-0.00208479, inv_mat.at(0, 0), 0.1);
//  BOOST_CHECK_CLOSE(-0.0799166, inv_mat.at(0, 1), 0.1);
//  BOOST_CHECK_CLOSE(0.273801, inv_mat.at(0, 2), 0.1);
//  BOOST_CHECK_CLOSE(-0.066713, inv_mat.at(0, 3), 0.1);
//  BOOST_CHECK_CLOSE(-0.0250174, inv_mat.at(0, 4), 0.1);
//  BOOST_CHECK_CLOSE(0.000694945, inv_mat.at(1, 0), 0.1);
//  BOOST_CHECK_CLOSE(0.359972, inv_mat.at(1, 1), 0.1);
//  BOOST_CHECK_CLOSE(-0.4246, inv_mat.at(1, 2), 0.1);
//  BOOST_CHECK_CLOSE(0.0222376, inv_mat.at(1, 3), 0.1);
//  BOOST_CHECK_CLOSE(0.00833914, inv_mat.at(1, 4), 0.1);
//  BOOST_CHECK_CLOSE(0.134468, inv_mat.at(2, 0), 0.1);
//  BOOST_CHECK_CLOSE(-0.345379, inv_mat.at(2, 1), 0.1);
//  BOOST_CHECK_CLOSE(0.339819, inv_mat.at(2, 2), 0.1);
//  BOOST_CHECK_CLOSE(-0.197012, inv_mat.at(2, 3), 0.1);
//  BOOST_CHECK_CLOSE(0.113621, inv_mat.at(2, 4), 0.1);
//  BOOST_CHECK_CLOSE(0.113621, inv_mat.at(3, 0), 0.1);
//  BOOST_CHECK_CLOSE(-0.144545, inv_mat.at(3, 1), 0.1);
//  BOOST_CHECK_CLOSE(0.0778318, inv_mat.at(3, 2), 0.1);
//  BOOST_CHECK_CLOSE(0.135858, inv_mat.at(3, 3), 0.1);
//  BOOST_CHECK_CLOSE(-0.136553, inv_mat.at(3, 4), 0.1);                 
//  BOOST_CHECK_CLOSE(-0.198402, inv_mat.at(4, 0), 0.1);
//  BOOST_CHECK_CLOSE(0.227936, inv_mat.at(4, 1), 0.1);
//  BOOST_CHECK_CLOSE(-0.276581, inv_mat.at(4, 2), 0.1);
//  BOOST_CHECK_CLOSE(0.151147, inv_mat.at(4, 3), 0.1);
//  BOOST_CHECK_CLOSE(0.11918, inv_mat.at(4, 4), 0.1);                 
//                        
//}
//
//BOOST_FIXTURE_TEST_CASE(transpose_squared, MatrixDynTest)
//{
//  MatrixDyn<double> transp_mat = _mat_3x3_d->transpose();
//
//  BOOST_CHECK_EQUAL(1.5, transp_mat.at(0, 0));
//  BOOST_CHECK_EQUAL(1, transp_mat.at(0, 1));
//  BOOST_CHECK_EQUAL(1.3, transp_mat.at(0, 2));
//  BOOST_CHECK_EQUAL(0, transp_mat.at(1, 0));
//  BOOST_CHECK_EQUAL(1, transp_mat.at(1, 1));
//  BOOST_CHECK_EQUAL(2.6, transp_mat.at(1, 2));
//  BOOST_CHECK_EQUAL(2.5, transp_mat.at(2, 0));
//  BOOST_CHECK_EQUAL(1.2, transp_mat.at(2, 1));
//  BOOST_CHECK_EQUAL(0.3, transp_mat.at(2, 2));
//}
//
//
///// Transpuesta de matriz no cuadrada
//BOOST_FIXTURE_TEST_CASE(transpose, MatrixDynTest)
//{
//  MatrixDyn<int> transp_mat = _mat_2x3_i->transpose();
//
//  BOOST_CHECK_EQUAL(6, transp_mat.at(0, 0));
//  BOOST_CHECK_EQUAL(9, transp_mat.at(0, 1));
//  BOOST_CHECK_EQUAL(8, transp_mat.at(1, 0));
//  BOOST_CHECK_EQUAL(6, transp_mat.at(1, 1));
//  BOOST_CHECK_EQUAL(6, transp_mat.at(2, 0));
//  BOOST_CHECK_EQUAL(2, transp_mat.at(2, 1));
//}
//
//BOOST_FIXTURE_TEST_CASE(determinant2x2, MatrixDynTest)
//{
//  MatrixDyn<double> _mat_2x2(2, 2);
//  _mat_2x2.at(0, 0) = 2;
//  _mat_2x2.at(0, 1) = 3;
//  _mat_2x2.at(1, 0) = 1;
//  _mat_2x2.at(1, 1) = 4;
//  BOOST_CHECK_EQUAL(5, _mat_2x2.determinant());
//}
//
//BOOST_FIXTURE_TEST_CASE(determinant3x3, MatrixDynTest)
//{
//  BOOST_CHECK_CLOSE(-0.98, _mat_3x3_d->determinant(), 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(determinant4x4, MatrixDynTest)
//{
//  BOOST_CHECK_CLOSE(353100.53, _mat_4x4_d->determinant(), 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(determinantnxn, MatrixDynTest)
//{
//  BOOST_CHECK_CLOSE(-2877.99, _mat_5x5_d->determinant(), 0.1);
//}
//
///// http://www.mathwords.com/c/cofactor_matrix.htm
//BOOST_FIXTURE_TEST_CASE(cofactor_matrix, MatrixDynTest)
//{
//  MatrixDyn<int> cofactor_matrix = _cofactor_matrix.cofactorMatrix();
//
//  BOOST_CHECK_EQUAL( 24, cofactor_matrix.at(0, 0));
//  BOOST_CHECK_EQUAL(  5, cofactor_matrix.at(0, 1));
//  BOOST_CHECK_EQUAL( -4, cofactor_matrix.at(0, 2));
//  BOOST_CHECK_EQUAL(-12, cofactor_matrix.at(1, 0));
//  BOOST_CHECK_EQUAL(  3, cofactor_matrix.at(1, 1));
//  BOOST_CHECK_EQUAL(  2, cofactor_matrix.at(1, 2));
//  BOOST_CHECK_EQUAL( -2, cofactor_matrix.at(2, 0));
//  BOOST_CHECK_EQUAL( -5, cofactor_matrix.at(2, 1));
//  BOOST_CHECK_EQUAL(  4, cofactor_matrix.at(2, 2));
//}
//
//BOOST_FIXTURE_TEST_CASE(first_minor, MatrixDynTest)
//{
//  int first_minor = _cofactor_matrix.firstMinor(1, 1);
//
//  BOOST_CHECK_EQUAL(3, first_minor);
//
//  first_minor = _cofactor_matrix.firstMinor(0, 1);
//
//  BOOST_CHECK_EQUAL(-5, first_minor);
//}
//
//BOOST_FIXTURE_TEST_CASE(cofactor, MatrixDynTest)
//{
//  int cofactor = _cofactor_matrix.cofactor(1,1);
//
//  BOOST_CHECK_EQUAL(3, cofactor);
//
//  cofactor = _cofactor_matrix.cofactor(0, 1);
//
//  BOOST_CHECK_EQUAL(5, cofactor);
//}
//
//BOOST_FIXTURE_TEST_CASE(zero, MatrixDynTest)
//{
//  for (size_t r = 0; r < mat_identity.rows(); r++){
//    for (size_t c = 0; c < mat_identity.cols(); c++){
//      BOOST_CHECK_EQUAL(0, mat_zero.at(r, c));
//    }
//  }
//}
//
//BOOST_FIXTURE_TEST_CASE(ones, MatrixDynTest)
//{
//  for (size_t r = 0; r < mat_identity.rows(); r++){
//    for (size_t c = 0; c < mat_identity.cols(); c++){
//      BOOST_CHECK_EQUAL(1, mat_ones.at(r, c));
//    }
//  }
//}
//
//BOOST_FIXTURE_TEST_CASE(identity, MatrixDynTest)
//{
//  for (size_t r = 0; r < mat_identity.rows(); r++){
//    for (size_t c = 0; c < mat_identity.cols(); c++){
//      if (r == c) {
//        BOOST_CHECK_EQUAL(1, mat_identity.at(r, c));
//      } else {
//        BOOST_CHECK_EQUAL(0, mat_identity.at(r, c));
//      }
//    }
//  }
//}
//
///* Operaciones unarias */
//
//BOOST_FIXTURE_TEST_CASE(plus, MatrixDynTest)
//{
//  MatrixDyn<double> mat = +(*_mat_3x3_d);
//  for (size_t r = 0; r < mat.rows(); r++){
//    for (size_t c = 0; c < mat.cols(); c++){
//      BOOST_CHECK_EQUAL(mat.at(r,c), _mat_3x3_d->at(r, c));
//    }
//  }
//}
//
//BOOST_FIXTURE_TEST_CASE(minus, MatrixDynTest)
//{
//  MatrixDyn<double> mat = -(*_mat_3x3_d);
//  for (size_t r = 0; r < mat.rows(); r++){
//    for (size_t c = 0; c < mat.cols(); c++){
//      BOOST_CHECK_EQUAL(-mat.at(r, c), _mat_3x3_d->at(r, c));
//    }
//  }
//}
//
///* Operaciones binarias entre matrices */
//
///// Suma o adición de matrices
//
//BOOST_FIXTURE_TEST_CASE(addition, MatrixDynTest)
//{
//  MatrixDyn<double> mat_ones_double = MatrixDyn<double>::ones(3, 3);
//
//  MatrixDyn<double> mat = mat_ones_double + (*_mat_3x3_d);
//
//  BOOST_CHECK_EQUAL(2.5, mat.at(0, 0));
//  BOOST_CHECK_EQUAL(1.0, mat.at(0, 1));
//  BOOST_CHECK_EQUAL(3.5, mat.at(0, 2));
//  BOOST_CHECK_EQUAL(2.0, mat.at(1, 0));
//  BOOST_CHECK_EQUAL(2.0, mat.at(1, 1));
//  BOOST_CHECK_EQUAL(2.2, mat.at(1, 2));
//  BOOST_CHECK_EQUAL(2.3, mat.at(2, 0));
//  BOOST_CHECK_EQUAL(3.6, mat.at(2, 1));
//  BOOST_CHECK_EQUAL(1.3, mat.at(2, 2));
//
//  mat_ones_double += (*_mat_3x3_d);
//
//  BOOST_CHECK_EQUAL(2.5, mat_ones_double.at(0, 0));
//  BOOST_CHECK_EQUAL(1.0, mat_ones_double.at(0, 1));
//  BOOST_CHECK_EQUAL(3.5, mat_ones_double.at(0, 2));
//  BOOST_CHECK_EQUAL(2.0, mat_ones_double.at(1, 0));
//  BOOST_CHECK_EQUAL(2.0, mat_ones_double.at(1, 1));
//  BOOST_CHECK_EQUAL(2.2, mat_ones_double.at(1, 2));
//  BOOST_CHECK_EQUAL(2.3, mat_ones_double.at(2, 0));
//  BOOST_CHECK_EQUAL(3.6, mat_ones_double.at(2, 1));
//  BOOST_CHECK_EQUAL(1.3, mat_ones_double.at(2, 2));
//}
//
///// Resta de matrices
//
//BOOST_FIXTURE_TEST_CASE(subtraction, MatrixDynTest)
//{
//  MatrixDyn<double> mat_identity_double = MatrixDyn<double>::identity(3, 3);
//
//  MatrixDyn<double> mat = mat_identity_double - (*_mat_3x3_d);
//
//  BOOST_CHECK_EQUAL(-0.5, mat.at(0, 0));
//  BOOST_CHECK_EQUAL( 0.0, mat.at(0, 1));
//  BOOST_CHECK_EQUAL(-2.5, mat.at(0, 2));
//  BOOST_CHECK_EQUAL(-1.0, mat.at(1, 0));
//  BOOST_CHECK_EQUAL( 0.0, mat.at(1, 1));
//  BOOST_CHECK_EQUAL(-1.2, mat.at(1, 2));
//  BOOST_CHECK_EQUAL(-1.3, mat.at(2, 0));
//  BOOST_CHECK_EQUAL(-2.6, mat.at(2, 1));
//  BOOST_CHECK_EQUAL( 0.7, mat.at(2, 2));
//
//  mat_identity_double -=  (*_mat_3x3_d);
//
//  BOOST_CHECK_EQUAL(-0.5, mat_identity_double.at(0, 0));
//  BOOST_CHECK_EQUAL( 0.0, mat_identity_double.at(0, 1));
//  BOOST_CHECK_EQUAL(-2.5, mat_identity_double.at(0, 2));
//  BOOST_CHECK_EQUAL(-1.0, mat_identity_double.at(1, 0));
//  BOOST_CHECK_EQUAL( 0.0, mat_identity_double.at(1, 1));
//  BOOST_CHECK_EQUAL(-1.2, mat_identity_double.at(1, 2));
//  BOOST_CHECK_EQUAL(-1.3, mat_identity_double.at(2, 0));
//  BOOST_CHECK_EQUAL(-2.6, mat_identity_double.at(2, 1));
//  BOOST_CHECK_EQUAL( 0.7, mat_identity_double.at(2, 2));
//}
//
///// Multiplicación de matrices
//
//BOOST_FIXTURE_TEST_CASE(multiplication, MatrixDynTest)
//{
//  MatrixDyn<int> mat = (*_mat_2x3_i) * _cofactor_matrix;
//
//  BOOST_CHECK_EQUAL(12, mat.at(0, 0));
//  BOOST_CHECK_EQUAL(44, mat.at(0, 1));
//  BOOST_CHECK_EQUAL(94, mat.at(0, 2));
//  BOOST_CHECK_EQUAL(11, mat.at(1, 0));
//  BOOST_CHECK_EQUAL(42, mat.at(1, 1));
//  BOOST_CHECK_EQUAL(69, mat.at(1, 2));
//}
//
///// Multiplicación de una matriz por un escalar
//
//BOOST_FIXTURE_TEST_CASE(MatrixScalar, MatrixDynTest)
//{
//  MatrixDyn<int> mat = (*_mat_2x3_i) * 10;
//
//  BOOST_CHECK_EQUAL(60, mat.at(0, 0));
//  BOOST_CHECK_EQUAL(80, mat.at(0, 1));
//  BOOST_CHECK_EQUAL(60, mat.at(0, 2));
//  BOOST_CHECK_EQUAL(90, mat.at(1, 0));
//  BOOST_CHECK_EQUAL(60, mat.at(1, 1));
//  BOOST_CHECK_EQUAL(20, mat.at(1, 2));
//
//  (*_mat_2x3_i) *= 10;
//
//  BOOST_CHECK_EQUAL(60, _mat_2x3_i->at(0, 0));
//  BOOST_CHECK_EQUAL(80, _mat_2x3_i->at(0, 1));
//  BOOST_CHECK_EQUAL(60, _mat_2x3_i->at(0, 2));
//  BOOST_CHECK_EQUAL(90, _mat_2x3_i->at(1, 0));
//  BOOST_CHECK_EQUAL(60, _mat_2x3_i->at(1, 1));
//  BOOST_CHECK_EQUAL(20, _mat_2x3_i->at(1, 2));
//}
//
///// Multiplicación de un escalar por una matriz
//
//BOOST_FIXTURE_TEST_CASE(scalar_matrix, MatrixDynTest)
//{
//  MatrixDyn<int> mat = 10 * (*_mat_2x3_i);
//
//  BOOST_CHECK_EQUAL(60, mat.at(0, 0));
//  BOOST_CHECK_EQUAL(80, mat.at(0, 1));
//  BOOST_CHECK_EQUAL(60, mat.at(0, 2));
//  BOOST_CHECK_EQUAL(90, mat.at(1, 0));
//  BOOST_CHECK_EQUAL(60, mat.at(1, 1));
//  BOOST_CHECK_EQUAL(20, mat.at(1, 2));
//}
//
//// División de una matriz por un escalar
//
//BOOST_FIXTURE_TEST_CASE(div_matrix_scalar, MatrixDynTest)
//{
//  MatrixDyn<double> mat = (*_mat_3x3_d) / 10.;
//
//  BOOST_CHECK_EQUAL(.15, mat.at(0, 0));
//  BOOST_CHECK_EQUAL(.0, mat.at(0, 1));
//  BOOST_CHECK_EQUAL(.25, mat.at(0, 2));
//  BOOST_CHECK_EQUAL(.10, mat.at(1, 0));
//  BOOST_CHECK_EQUAL(.10, mat.at(1, 1));
//  BOOST_CHECK_EQUAL(.12, mat.at(1, 2));
//  BOOST_CHECK_EQUAL(.13, mat.at(2, 0));
//  BOOST_CHECK_EQUAL(.26, mat.at(2, 1));
//  BOOST_CHECK_EQUAL(.03, mat.at(2, 2));
//}
//
//BOOST_AUTO_TEST_SUITE_END()











BOOST_AUTO_TEST_SUITE(Matrix2TestSuite)

struct Matrix2Test 
{
  Matrix2Test()
    : _mat_dyn_default_constructor(new Matrix2<double>()),
      _mat_dyn_2x2_constructor(new Matrix2<double>(2, 2)),
      _mat_dyn_2x2(new Matrix2<double>(2, 2)),
      _mat_dyn_3x3_d(new Matrix2<double>(3, 3)),
      _mat_dyn_4x4_d(new Matrix2<double>(4, 4)),
      _mat_dyn_5x5_d(new Matrix2<double>(5, 5)),
      _mat_dyn_2x3_i(new Matrix2<int>(2, 3)),
      _cofactor_matrix_dyn(new Matrix2<int>(3, 3))
  {}
  ~Matrix2Test()
  { 
    if (_mat_dyn_default_constructor) delete _mat_dyn_default_constructor, _mat_dyn_default_constructor = nullptr;
    if (_mat_dyn_2x2_constructor) delete _mat_dyn_2x2_constructor, _mat_dyn_2x2_constructor = nullptr;
    if (_mat_dyn_2x2) delete _mat_dyn_2x2, _mat_dyn_2x2 = nullptr;
    if (_mat_dyn_3x3_d) delete _mat_dyn_3x3_d, _mat_dyn_3x3_d = nullptr;
    if (_mat_dyn_4x4_d) delete _mat_dyn_4x4_d, _mat_dyn_4x4_d = nullptr;
    if (_mat_dyn_2x3_i) delete _mat_dyn_2x3_i, _mat_dyn_2x3_i = nullptr;
    if (_cofactor_matrix_dyn) delete _cofactor_matrix_dyn, _cofactor_matrix_dyn = nullptr;
  }
  
  void setup()
  {
    
    _mat_2x2.at(0, 0) = 2.;
    _mat_2x2.at(0, 1) = 3.;
    _mat_2x2.at(1, 0) = 1.;
    _mat_2x2.at(1, 1) = 4.;

    _mat_dyn_2x2->at(0, 0) = 2.;
    _mat_dyn_2x2->at(0, 1) = 3.;
    _mat_dyn_2x2->at(1, 0) = 1.;
    _mat_dyn_2x2->at(1, 1) = 4.;

    _mat_3x3_d.at(0, 0) = 1.5;
    _mat_3x3_d.at(0, 1) = 0.0;
    _mat_3x3_d.at(0, 2) = 2.5;
    _mat_3x3_d.at(1, 0) = 1.0;
    _mat_3x3_d.at(1, 1) = 1.0;
    _mat_3x3_d.at(1, 2) = 1.2;
    _mat_3x3_d.at(2, 0) = 1.3;
    _mat_3x3_d.at(2, 1) = 2.6;
    _mat_3x3_d.at(2, 2) = 0.3;

    _mat_dyn_3x3_d->at(0, 0) = 1.5;
    _mat_dyn_3x3_d->at(0, 1) = 0.0;
    _mat_dyn_3x3_d->at(0, 2) = 2.5;
    _mat_dyn_3x3_d->at(1, 0) = 1.0;
    _mat_dyn_3x3_d->at(1, 1) = 1.0;
    _mat_dyn_3x3_d->at(1, 2) = 1.2;
    _mat_dyn_3x3_d->at(2, 0) = 1.3;
    _mat_dyn_3x3_d->at(2, 1) = 2.6;
    _mat_dyn_3x3_d->at(2, 2) = 0.3;

    _mat_4x4_d.at(0, 0) = 4.5;    _mat_4x4_d.at(0, 1) = 2.7;  _mat_4x4_d.at(0, 2) = 5.5;  _mat_4x4_d.at(0, 3) = 4.98;
    _mat_4x4_d.at(1, 0) = 1.36;   _mat_4x4_d.at(1, 1) = 7.62;	_mat_4x4_d.at(1, 2) = 78.3; _mat_4x4_d.at(1, 3) = 45.5;
    _mat_4x4_d.at(2, 0) = 14.3;   _mat_4x4_d.at(2, 1) = 45.3;	_mat_4x4_d.at(2, 2) = 5.;   _mat_4x4_d.at(2, 3) = 45.;
    _mat_4x4_d.at(3, 0) = 12.374; _mat_4x4_d.at(3, 1) = 41.6;	_mat_4x4_d.at(3, 2) = 1.3;  _mat_4x4_d.at(3, 3) = 12.7;
        
    _mat_dyn_4x4_d->at(0, 0) = 4.5;    _mat_dyn_4x4_d->at(0, 1) = 2.7;  _mat_dyn_4x4_d->at(0, 2) = 5.5;  _mat_dyn_4x4_d->at(0, 3) = 4.98;
    _mat_dyn_4x4_d->at(1, 0) = 1.36;   _mat_dyn_4x4_d->at(1, 1) = 7.62;	_mat_dyn_4x4_d->at(1, 2) = 78.3; _mat_dyn_4x4_d->at(1, 3) = 45.5;
    _mat_dyn_4x4_d->at(2, 0) = 14.3;   _mat_dyn_4x4_d->at(2, 1) = 45.3;	_mat_dyn_4x4_d->at(2, 2) = 5.;   _mat_dyn_4x4_d->at(2, 3) = 45.;
    _mat_dyn_4x4_d->at(3, 0) = 12.374; _mat_dyn_4x4_d->at(3, 1) = 41.6;	_mat_dyn_4x4_d->at(3, 2) = 1.3;  _mat_dyn_4x4_d->at(3, 3) = 12.7;

    mat_zero = Matrix2<double, 3, 3>::zero();
    mat_dyn_zero = Matrix2<double>::zero(3, 3);
    mat_ones = Matrix2<double, 3, 3>::ones();
    mat_dyn_ones = Matrix2<double>::ones(3, 3);
    mat_identity = Matrix2<double, 3, 3>::identity();
    mat_dyn_identity = Matrix2<double>::identity(3, 3);

    _mat_5x5_d.at(0, 0) = 6;
    _mat_5x5_d.at(0, 1) = 8;
    _mat_5x5_d.at(0, 2) = 6;
    _mat_5x5_d.at(0, 3) = 7;
    _mat_5x5_d.at(0, 4) = 3;
    _mat_5x5_d.at(1, 0) = 9;
    _mat_5x5_d.at(1, 1) = 6;
    _mat_5x5_d.at(1, 2) = 2;
    _mat_5x5_d.at(1, 3) = 3;
    _mat_5x5_d.at(1, 4) = 3;
    _mat_5x5_d.at(2, 0) = 8;
    _mat_5x5_d.at(2, 1) = 3;
    _mat_5x5_d.at(2, 2) = 2;
    _mat_5x5_d.at(2, 3) = 3;
    _mat_5x5_d.at(2, 4) = 3;
    _mat_5x5_d.at(3, 0) = 5;
    _mat_5x5_d.at(3, 1) = 3;
    _mat_5x5_d.at(3, 2) = 3;
    _mat_5x5_d.at(3, 3) = 7;
    _mat_5x5_d.at(3, 4) = 6;
    _mat_5x5_d.at(4, 0) = 5;
    _mat_5x5_d.at(4, 1) = 5;
    _mat_5x5_d.at(4, 2) = 7;
    _mat_5x5_d.at(4, 3) = 4;
    _mat_5x5_d.at(4, 4) = 7;

    _mat_dyn_5x5_d->at(0, 0) = 6;
    _mat_dyn_5x5_d->at(0, 1) = 8;
    _mat_dyn_5x5_d->at(0, 2) = 6;
    _mat_dyn_5x5_d->at(0, 3) = 7;
    _mat_dyn_5x5_d->at(0, 4) = 3;
    _mat_dyn_5x5_d->at(1, 0) = 9;
    _mat_dyn_5x5_d->at(1, 1) = 6;
    _mat_dyn_5x5_d->at(1, 2) = 2;
    _mat_dyn_5x5_d->at(1, 3) = 3;
    _mat_dyn_5x5_d->at(1, 4) = 3;
    _mat_dyn_5x5_d->at(2, 0) = 8;
    _mat_dyn_5x5_d->at(2, 1) = 3;
    _mat_dyn_5x5_d->at(2, 2) = 2;
    _mat_dyn_5x5_d->at(2, 3) = 3;
    _mat_dyn_5x5_d->at(2, 4) = 3;
    _mat_dyn_5x5_d->at(3, 0) = 5;
    _mat_dyn_5x5_d->at(3, 1) = 3;
    _mat_dyn_5x5_d->at(3, 2) = 3;
    _mat_dyn_5x5_d->at(3, 3) = 7;
    _mat_dyn_5x5_d->at(3, 4) = 6;
    _mat_dyn_5x5_d->at(4, 0) = 5;
    _mat_dyn_5x5_d->at(4, 1) = 5;
    _mat_dyn_5x5_d->at(4, 2) = 7;
    _mat_dyn_5x5_d->at(4, 3) = 4;
    _mat_dyn_5x5_d->at(4, 4) = 7;

    _mat_2x3_i.at(0, 0) = 6;
    _mat_2x3_i.at(0, 1) = 8;
    _mat_2x3_i.at(0, 2) = 6;
    _mat_2x3_i.at(1, 0) = 9;
    _mat_2x3_i.at(1, 1) = 6;
    _mat_2x3_i.at(1, 2) = 2;
    
    _mat_dyn_2x3_i->at(0, 0) = 6;
    _mat_dyn_2x3_i->at(0, 1) = 8;
    _mat_dyn_2x3_i->at(0, 2) = 6;
    _mat_dyn_2x3_i->at(1, 0) = 9;
    _mat_dyn_2x3_i->at(1, 1) = 6;
    _mat_dyn_2x3_i->at(1, 2) = 2;

    
    _cofactor_matrix.at(0, 0) = 1;
    _cofactor_matrix.at(0, 1) = 2;
    _cofactor_matrix.at(0, 2) = 3;
    _cofactor_matrix.at(1, 0) = 0;
    _cofactor_matrix.at(1, 1) = 4;
    _cofactor_matrix.at(1, 2) = 5;
    _cofactor_matrix.at(2, 0) = 1;
    _cofactor_matrix.at(2, 1) = 0;
    _cofactor_matrix.at(2, 2) = 6;

    _cofactor_matrix_dyn->at(0, 0) = 1;
    _cofactor_matrix_dyn->at(0, 1) = 2;
    _cofactor_matrix_dyn->at(0, 2) = 3;
    _cofactor_matrix_dyn->at(1, 0) = 0;
    _cofactor_matrix_dyn->at(1, 1) = 4;
    _cofactor_matrix_dyn->at(1, 2) = 5;
    _cofactor_matrix_dyn->at(2, 0) = 1;
    _cofactor_matrix_dyn->at(2, 1) = 0;
    _cofactor_matrix_dyn->at(2, 2) = 6;

    mat_singular.at(0, 0) = 1;
    mat_singular.at(0, 1) = 2;
    mat_singular.at(0, 2) = 3;
    mat_singular.at(1, 0) = 4;
    mat_singular.at(1, 1) = 5;
    mat_singular.at(1, 2) = 6;
    mat_singular.at(2, 0) = 7;
    mat_singular.at(2, 1) = 8;
    mat_singular.at(2, 2) = 9;
  }
 
  void teardown()
  {

  }

  Matrix2<double> *_mat_dyn_default_constructor;
  Matrix2<double, 2, 2> _mat_2x2_default_constructor;
  Matrix2<double> *_mat_dyn_2x2_constructor;

  Matrix2<double, 2, 2> _mat_2x2;
  Matrix2<double> *_mat_dyn_2x2;

  Matrix2<double, 3, 3> _mat_3x3_d;
  Matrix2<double> *_mat_dyn_3x3_d;

  Matrix2<double, 4, 4> _mat_4x4_d;
  Matrix2<double> *_mat_dyn_4x4_d;

  Matrix2<double, 5, 5> _mat_5x5_d;
  Matrix2<double> *_mat_dyn_5x5_d;

  //Matrix<2, 2, float> _mat_2x2_f;
  //Matrix<3, 3, float> _mat_3x3_f;
  //Matrix<4, 4, float> _mat_4x4_f;
  
  Matrix2<int, 2, 3> _mat_2x3_i;
  Matrix2<int> *_mat_dyn_2x3_i;
  Matrix2<int, 3, 3> _cofactor_matrix;
  Matrix2<int> *_cofactor_matrix_dyn;
  Matrix2<double, 3, 3> mat_zero;
  Matrix2<double> mat_dyn_zero;
  Matrix2<double, 3, 3> mat_ones;
  Matrix2<double> mat_dyn_ones;
  Matrix2<double, 3, 3> mat_identity;
  Matrix2<double> mat_dyn_identity;

  Matrix2<int,3,3> mat_singular;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, Matrix2Test)
{
  BOOST_CHECK_EQUAL(0, _mat_dyn_default_constructor->rows());
  BOOST_CHECK_EQUAL(0, _mat_dyn_default_constructor->cols());

  constexpr double ini_value = -std::numeric_limits<double>().max();

  for (size_t r = 0; r < _mat_2x2_default_constructor.rows(); r++){
    for (size_t c = 0; c < _mat_2x2_default_constructor.cols(); c++){
      BOOST_CHECK_EQUAL(static_cast<double>(ini_value), _mat_2x2_default_constructor.at(r, c));
    }
  }
  BOOST_CHECK_EQUAL(2, _mat_2x2_default_constructor.rows());
  BOOST_CHECK_EQUAL(2, _mat_2x2_default_constructor.cols());
}

BOOST_FIXTURE_TEST_CASE(rows_cols_constructor, Matrix2Test)
{
  BOOST_CHECK_EQUAL(2, _mat_dyn_2x2_constructor->rows());
  BOOST_CHECK_EQUAL(2, _mat_dyn_2x2_constructor->cols());

  constexpr double ini_value = -std::numeric_limits<double>().max();

  for (size_t r = 0; r < _mat_dyn_2x2_constructor->rows(); r++){
    for (size_t c = 0; c < _mat_dyn_2x2_constructor->cols(); c++){
      BOOST_CHECK_EQUAL(ini_value, _mat_dyn_2x2_constructor->at(r, c));
    }
  }
}

//BOOST_FIXTURE_TEST_CASE(iterator_list_constructor, Matrix2Test)
//{
//
//  Matrix<2, 2, int> it_list2 {{
//    {
//      {0, 1},
//      {2, 3}
//    }
//  }};
//
//  BOOST_CHECK_EQUAL(0, it_list2.at(0, 0));
//  BOOST_CHECK_EQUAL(1, it_list2.at(0, 1));
//  BOOST_CHECK_EQUAL(2, it_list2.at(1, 0));
//  BOOST_CHECK_EQUAL(3, it_list2.at(1, 1));
//
//  Matrix<2, 2, int> it_list3 = {
//    {
//      { 0, 1 },
//      {2, 3}
//    }
//  };
//
//  BOOST_CHECK_EQUAL(0, it_list3.at(0, 0));
//  BOOST_CHECK_EQUAL(1, it_list3.at(0, 1));
//  BOOST_CHECK_EQUAL(2, it_list3.at(1, 0));
//  BOOST_CHECK_EQUAL(3, it_list3.at(1, 1));
//
//  Matrix<2, 3, int> it_list2x3 = {
//    {
//      {0, 1, 2},
//      {3, 4, 5}
//    }
//  };
//
//  BOOST_CHECK_EQUAL(0, it_list2x3.at(0, 0));
//  BOOST_CHECK_EQUAL(1, it_list2x3.at(0, 1));
//  BOOST_CHECK_EQUAL(2, it_list2x3.at(0, 2));
//  BOOST_CHECK_EQUAL(3, it_list2x3.at(1, 0));
//  BOOST_CHECK_EQUAL(4, it_list2x3.at(1, 1));
//  BOOST_CHECK_EQUAL(5, it_list2x3.at(1, 2));
//
//  //Matrix<3, 3, int> it_list3x3 = {
//  //  {
//  //    {0, 1, 2},
//  //    {3, 4, 5},
//  //    {6, 7, 8}
//  //  }
//  //};
//
//  //BOOST_CHECK_EQUAL(0, it_list3x3.at(0, 0));
//  //BOOST_CHECK_EQUAL(1, it_list3x3.at(0, 1));
//  //BOOST_CHECK_EQUAL(2, it_list3x3.at(0, 2));
//
//  //Matrix<5, 5, int> it_list5 = {
//  //  {
//  //    {6, 8,6,7,3},
//  //    {9,6,2,3,3},
//  //    {8,3,2,3,3},
//  //    {5,3,3,7,6},
//  //    {5,5,7,4,7}
//  //  }
//  //};
//}

BOOST_FIXTURE_TEST_CASE(at, Matrix2Test)
{
  BOOST_CHECK_EQUAL(1.5, _mat_3x3_d.at(0, 0));
  BOOST_CHECK_EQUAL(0.0, _mat_3x3_d.at(0, 1));
  BOOST_CHECK_EQUAL(2.5, _mat_3x3_d.at(0, 2));
  BOOST_CHECK_EQUAL(1.0, _mat_3x3_d.at(1, 0));
  BOOST_CHECK_EQUAL(1.0, _mat_3x3_d.at(1, 1));
  BOOST_CHECK_EQUAL(1.2, _mat_3x3_d.at(1, 2));
  BOOST_CHECK_EQUAL(1.3, _mat_3x3_d.at(2, 0));
  BOOST_CHECK_EQUAL(2.6, _mat_3x3_d.at(2, 1));
  BOOST_CHECK_EQUAL(0.3, _mat_3x3_d.at(2, 2));

  BOOST_CHECK_EQUAL(1.5, _mat_dyn_3x3_d->at(0, 0));
  BOOST_CHECK_EQUAL(0.0, _mat_dyn_3x3_d->at(0, 1));
  BOOST_CHECK_EQUAL(2.5, _mat_dyn_3x3_d->at(0, 2));
  BOOST_CHECK_EQUAL(1.0, _mat_dyn_3x3_d->at(1, 0));
  BOOST_CHECK_EQUAL(1.0, _mat_dyn_3x3_d->at(1, 1));
  BOOST_CHECK_EQUAL(1.2, _mat_dyn_3x3_d->at(1, 2));
  BOOST_CHECK_EQUAL(1.3, _mat_dyn_3x3_d->at(2, 0));
  BOOST_CHECK_EQUAL(2.6, _mat_dyn_3x3_d->at(2, 1));
  BOOST_CHECK_EQUAL(0.3, _mat_dyn_3x3_d->at(2, 2));
}

BOOST_FIXTURE_TEST_CASE(rows, Matrix2Test)
{
  BOOST_CHECK_EQUAL(0, _mat_dyn_default_constructor->rows());
  BOOST_CHECK_EQUAL(2, _mat_2x2_default_constructor.rows());
  BOOST_CHECK_EQUAL(2, _mat_dyn_2x2_constructor->rows());
  BOOST_CHECK_EQUAL(2, _mat_dyn_2x2->rows());
  BOOST_CHECK_EQUAL(3, _mat_3x3_d.rows());
  BOOST_CHECK_EQUAL(3, _mat_dyn_3x3_d->rows());
}

BOOST_FIXTURE_TEST_CASE(cols, Matrix2Test)
{
  BOOST_CHECK_EQUAL(0, _mat_dyn_default_constructor->cols());
  BOOST_CHECK_EQUAL(2, _mat_2x2_default_constructor.cols());
  BOOST_CHECK_EQUAL(2, _mat_dyn_2x2_constructor->cols());
  BOOST_CHECK_EQUAL(2, _mat_dyn_2x2->cols());
  BOOST_CHECK_EQUAL(3, _mat_3x3_d.cols());
  BOOST_CHECK_EQUAL(3, _mat_dyn_3x3_d->cols());
}


BOOST_FIXTURE_TEST_CASE(determinant2x2, Matrix2Test)
{
  BOOST_CHECK_EQUAL(5., _mat_2x2.determinant());
  BOOST_CHECK_EQUAL(5., _mat_dyn_2x2->determinant());
}

BOOST_FIXTURE_TEST_CASE(determinant3x3, Matrix2Test)
{
  BOOST_CHECK_CLOSE(-0.98, _mat_3x3_d.determinant(), 0.1);
  BOOST_CHECK_CLOSE(-0.98, _mat_dyn_3x3_d->determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinant4x4, Matrix2Test)
{
  BOOST_CHECK_CLOSE(353100.53, _mat_4x4_d.determinant(), 0.1);
  BOOST_CHECK_CLOSE(353100.53, _mat_dyn_4x4_d->determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinantnxn, Matrix2Test)
{
  BOOST_CHECK_CLOSE(-2877.99, _mat_5x5_d.determinant(), 0.1);
  BOOST_CHECK_CLOSE(-2877.99, _mat_dyn_5x5_d->determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinant_singular, Matrix2Test)
{
  BOOST_CHECK_EQUAL(0, mat_singular.determinant());
}

BOOST_FIXTURE_TEST_CASE(inverse2x2, Matrix2Test)
{
  bool invertible;
  Matrix2<double, 2, 2> inv_mat = _mat_2x2.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE(0.8, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.6, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.2, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.4, inv_mat.at(1, 1), 0.1);

  Matrix2<double> inv_mat_dyn = _mat_dyn_2x2->inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE(0.8, inv_mat_dyn.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.6, inv_mat_dyn.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.2, inv_mat_dyn.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.4, inv_mat_dyn.at(1, 1), 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse3x3, Matrix2Test)
{
  bool invertible;
  Matrix2<double, 3, 3> inv_mat = _mat_3x3_d.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE( 2.877551, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-6.632653, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE( 2.551020, inv_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-1.285714, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE( 2.857143, inv_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.714286, inv_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(-1.326531, inv_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE( 3.979592, inv_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-1.530612, inv_mat.at(2, 2), 0.1);


  Matrix2<double> inv_mat2 = _mat_dyn_3x3_d->inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE( 2.877551, inv_mat2.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-6.632653, inv_mat2.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE( 2.551020, inv_mat2.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-1.285714, inv_mat2.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE( 2.857143, inv_mat2.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.714286, inv_mat2.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(-1.326531, inv_mat2.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE( 3.979592, inv_mat2.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-1.530612, inv_mat2.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse4x4, Matrix2Test)
{
  bool invertible;
  Matrix2<double, 4, 4> inv_mat = _mat_4x4_d.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE( 0.268435, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.018133, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.010673, inv_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.002479, inv_mat.at(0, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.077647, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE( 0.005298, inv_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.006780, inv_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE( 0.035491, inv_mat.at(1, 3), 0.1);
  BOOST_CHECK_CLOSE( 0.007528, inv_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE( 0.013172, inv_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.019248, inv_mat.at(2, 2), 0.1);
  BOOST_CHECK_CLOSE( 0.018059, inv_mat.at(2, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.007974, inv_mat.at(3, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.001035, inv_mat.at(3, 1), 0.1);
  BOOST_CHECK_CLOSE( 0.034578, inv_mat.at(3, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.036946, inv_mat.at(3, 3), 0.1);

  Matrix2<double> inv_mat2 = _mat_dyn_4x4_d->inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE( 0.268435, inv_mat2.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.018133, inv_mat2.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.010673, inv_mat2.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.002479, inv_mat2.at(0, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.077647, inv_mat2.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE( 0.005298, inv_mat2.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.006780, inv_mat2.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE( 0.035491, inv_mat2.at(1, 3), 0.1);
  BOOST_CHECK_CLOSE( 0.007528, inv_mat2.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE( 0.013172, inv_mat2.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.019248, inv_mat2.at(2, 2), 0.1);
  BOOST_CHECK_CLOSE( 0.018059, inv_mat2.at(2, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.007974, inv_mat2.at(3, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.001035, inv_mat2.at(3, 1), 0.1);
  BOOST_CHECK_CLOSE( 0.034578, inv_mat2.at(3, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.036946, inv_mat2.at(3, 3), 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse, Matrix2Test)
{
  bool invertible;
  Matrix2<double, 5, 5> inv_mat = _mat_5x5_d.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE(-0.00208479, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.0799166, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0.273801, inv_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.066713, inv_mat.at(0, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.0250174, inv_mat.at(0, 4), 0.1);
  BOOST_CHECK_CLOSE(0.000694945, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.359972, inv_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.4246, inv_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0.0222376, inv_mat.at(1, 3), 0.1);
  BOOST_CHECK_CLOSE(0.00833914, inv_mat.at(1, 4), 0.1);
  BOOST_CHECK_CLOSE(0.134468, inv_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.345379, inv_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(0.339819, inv_mat.at(2, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.197012, inv_mat.at(2, 3), 0.1);
  BOOST_CHECK_CLOSE(0.113621, inv_mat.at(2, 4), 0.1);
  BOOST_CHECK_CLOSE(0.113621, inv_mat.at(3, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.144545, inv_mat.at(3, 1), 0.1);
  BOOST_CHECK_CLOSE(0.0778318, inv_mat.at(3, 2), 0.1);
  BOOST_CHECK_CLOSE(0.135858, inv_mat.at(3, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.136553, inv_mat.at(3, 4), 0.1);                 
  BOOST_CHECK_CLOSE(-0.198402, inv_mat.at(4, 0), 0.1);
  BOOST_CHECK_CLOSE(0.227936, inv_mat.at(4, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.276581, inv_mat.at(4, 2), 0.1);
  BOOST_CHECK_CLOSE(0.151147, inv_mat.at(4, 3), 0.1);
  BOOST_CHECK_CLOSE(0.11918, inv_mat.at(4, 4), 0.1);                 
 
  
  Matrix2<double> inv_mat2 = _mat_dyn_5x5_d->inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE(-0.00208479, inv_mat2.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.0799166, inv_mat2.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0.273801, inv_mat2.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.066713, inv_mat2.at(0, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.0250174, inv_mat2.at(0, 4), 0.1);
  BOOST_CHECK_CLOSE(0.000694945, inv_mat2.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.359972, inv_mat2.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.4246, inv_mat2.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0.0222376, inv_mat2.at(1, 3), 0.1);
  BOOST_CHECK_CLOSE(0.00833914, inv_mat2.at(1, 4), 0.1);
  BOOST_CHECK_CLOSE(0.134468, inv_mat2.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.345379, inv_mat2.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(0.339819, inv_mat2.at(2, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.197012, inv_mat2.at(2, 3), 0.1);
  BOOST_CHECK_CLOSE(0.113621, inv_mat2.at(2, 4), 0.1);
  BOOST_CHECK_CLOSE(0.113621, inv_mat2.at(3, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.144545, inv_mat2.at(3, 1), 0.1);
  BOOST_CHECK_CLOSE(0.0778318, inv_mat2.at(3, 2), 0.1);
  BOOST_CHECK_CLOSE(0.135858, inv_mat2.at(3, 3), 0.1);
  BOOST_CHECK_CLOSE(-0.136553, inv_mat2.at(3, 4), 0.1);                 
  BOOST_CHECK_CLOSE(-0.198402, inv_mat2.at(4, 0), 0.1);
  BOOST_CHECK_CLOSE(0.227936, inv_mat2.at(4, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.276581, inv_mat2.at(4, 2), 0.1);
  BOOST_CHECK_CLOSE(0.151147, inv_mat2.at(4, 3), 0.1);
  BOOST_CHECK_CLOSE(0.11918, inv_mat2.at(4, 4), 0.1);
}

BOOST_FIXTURE_TEST_CASE(transpose_squared, Matrix2Test)
{
  Matrix2<double, 3, 3> transp_mat = _mat_3x3_d.transpose();

  BOOST_CHECK_EQUAL(1.5, transp_mat.at(0, 0));
  BOOST_CHECK_EQUAL(1, transp_mat.at(0, 1));
  BOOST_CHECK_EQUAL(1.3, transp_mat.at(0, 2));
  BOOST_CHECK_EQUAL(0, transp_mat.at(1, 0));
  BOOST_CHECK_EQUAL(1, transp_mat.at(1, 1));
  BOOST_CHECK_EQUAL(2.6, transp_mat.at(1, 2));
  BOOST_CHECK_EQUAL(2.5, transp_mat.at(2, 0));
  BOOST_CHECK_EQUAL(1.2, transp_mat.at(2, 1));
  BOOST_CHECK_EQUAL(0.3, transp_mat.at(2, 2));

  Matrix2<double> transp_mat2 = _mat_dyn_3x3_d->transpose();
  BOOST_CHECK_EQUAL(1.5, transp_mat2.at(0, 0));
  BOOST_CHECK_EQUAL(1, transp_mat2.at(0, 1));
  BOOST_CHECK_EQUAL(1.3, transp_mat2.at(0, 2));
  BOOST_CHECK_EQUAL(0, transp_mat2.at(1, 0));
  BOOST_CHECK_EQUAL(1, transp_mat2.at(1, 1));
  BOOST_CHECK_EQUAL(2.6, transp_mat2.at(1, 2));
  BOOST_CHECK_EQUAL(2.5, transp_mat2.at(2, 0));
  BOOST_CHECK_EQUAL(1.2, transp_mat2.at(2, 1));
  BOOST_CHECK_EQUAL(0.3, transp_mat2.at(2, 2));
}


/// Transpuesta de matriz no cuadrada
BOOST_FIXTURE_TEST_CASE(transpose, Matrix2Test)
{
  Matrix2<int, 3, 2> transp_mat = _mat_2x3_i.transpose();

  BOOST_CHECK_EQUAL(6, transp_mat.at(0, 0));
  BOOST_CHECK_EQUAL(9, transp_mat.at(0, 1));
  BOOST_CHECK_EQUAL(8, transp_mat.at(1, 0));
  BOOST_CHECK_EQUAL(6, transp_mat.at(1, 1));
  BOOST_CHECK_EQUAL(6, transp_mat.at(2, 0));
  BOOST_CHECK_EQUAL(2, transp_mat.at(2, 1));

  Matrix2<int> transp_mat2 = _mat_dyn_2x3_i->transpose();

  BOOST_CHECK_EQUAL(6, transp_mat2.at(0, 0));
  BOOST_CHECK_EQUAL(9, transp_mat2.at(0, 1));
  BOOST_CHECK_EQUAL(8, transp_mat2.at(1, 0));
  BOOST_CHECK_EQUAL(6, transp_mat2.at(1, 1));
  BOOST_CHECK_EQUAL(6, transp_mat2.at(2, 0));
  BOOST_CHECK_EQUAL(2, transp_mat2.at(2, 1));
}

/// http://www.mathwords.com/c/cofactor_matrix.htm
BOOST_FIXTURE_TEST_CASE(cofactor_matrix, Matrix2Test)
{
  Matrix2<int,3,3> cofactor_matrix = _cofactor_matrix.cofactorMatrix();

  BOOST_CHECK_EQUAL( 24, cofactor_matrix.at(0, 0));
  BOOST_CHECK_EQUAL(  5, cofactor_matrix.at(0, 1));
  BOOST_CHECK_EQUAL( -4, cofactor_matrix.at(0, 2));
  BOOST_CHECK_EQUAL(-12, cofactor_matrix.at(1, 0));
  BOOST_CHECK_EQUAL(  3, cofactor_matrix.at(1, 1));
  BOOST_CHECK_EQUAL(  2, cofactor_matrix.at(1, 2));
  BOOST_CHECK_EQUAL( -2, cofactor_matrix.at(2, 0));
  BOOST_CHECK_EQUAL( -5, cofactor_matrix.at(2, 1));
  BOOST_CHECK_EQUAL(  4, cofactor_matrix.at(2, 2));

  Matrix2<int> cofactor_matrix_2 = _cofactor_matrix_dyn->cofactorMatrix();

  BOOST_CHECK_EQUAL( 24, cofactor_matrix_2.at(0, 0));
  BOOST_CHECK_EQUAL(  5, cofactor_matrix_2.at(0, 1));
  BOOST_CHECK_EQUAL( -4, cofactor_matrix_2.at(0, 2));
  BOOST_CHECK_EQUAL(-12, cofactor_matrix_2.at(1, 0));
  BOOST_CHECK_EQUAL(  3, cofactor_matrix_2.at(1, 1));
  BOOST_CHECK_EQUAL(  2, cofactor_matrix_2.at(1, 2));
  BOOST_CHECK_EQUAL( -2, cofactor_matrix_2.at(2, 0));
  BOOST_CHECK_EQUAL( -5, cofactor_matrix_2.at(2, 1));
  BOOST_CHECK_EQUAL(  4, cofactor_matrix_2.at(2, 2));
}

BOOST_FIXTURE_TEST_CASE(first_minor, Matrix2Test)
{
  int first_minor = _cofactor_matrix.firstMinor(1, 1);
  BOOST_CHECK_EQUAL(3, first_minor);

  first_minor = _cofactor_matrix.firstMinor(0, 1);
  BOOST_CHECK_EQUAL(-5, first_minor);

  first_minor = _cofactor_matrix_dyn->firstMinor(1, 1);
  BOOST_CHECK_EQUAL(3, first_minor);

  first_minor = _cofactor_matrix_dyn->firstMinor(0, 1);
  BOOST_CHECK_EQUAL(-5, first_minor);
}

BOOST_FIXTURE_TEST_CASE(cofactor, Matrix2Test)
{
  int cofactor = _cofactor_matrix.cofactor(1,1);
  BOOST_CHECK_EQUAL(3, cofactor);

  cofactor = _cofactor_matrix.cofactor(0, 1);
  BOOST_CHECK_EQUAL(5, cofactor);

  cofactor = _cofactor_matrix_dyn->cofactor(1,1);
  BOOST_CHECK_EQUAL(3, cofactor);

  cofactor = _cofactor_matrix_dyn->cofactor(0, 1);
  BOOST_CHECK_EQUAL(5, cofactor);
}

BOOST_FIXTURE_TEST_CASE(zero, Matrix2Test)
{
  for (size_t r = 0; r < mat_zero.rows(); r++){
    for (size_t c = 0; c < mat_zero.cols(); c++){
      BOOST_CHECK_EQUAL(0, mat_zero.at(r, c));
    }
  }

  for (size_t r = 0; r < mat_dyn_zero.rows(); r++){
    for (size_t c = 0; c < mat_dyn_zero.cols(); c++){
      BOOST_CHECK_EQUAL(0, mat_dyn_zero.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(ones, Matrix2Test)
{
  for (size_t r = 0; r < mat_ones.rows(); r++){
    for (size_t c = 0; c < mat_ones.cols(); c++){
      BOOST_CHECK_EQUAL(1, mat_ones.at(r, c));
    }
  }

  for (size_t r = 0; r < mat_dyn_ones.rows(); r++){
    for (size_t c = 0; c < mat_dyn_ones.cols(); c++){
      BOOST_CHECK_EQUAL(1, mat_dyn_ones.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(identity, Matrix2Test)
{
  for (size_t r = 0; r < mat_identity.rows(); r++){
    for (size_t c = 0; c < mat_identity.cols(); c++){
      if (r == c) {
        BOOST_CHECK_EQUAL(1, mat_identity.at(r, c));
      } else {
        BOOST_CHECK_EQUAL(0, mat_identity.at(r, c));
      }
    }
  }

  for (size_t r = 0; r < mat_dyn_identity.rows(); r++){
    for (size_t c = 0; c < mat_dyn_identity.cols(); c++){
      if (r == c) {
        BOOST_CHECK_EQUAL(1, mat_dyn_identity.at(r, c));
      } else {
        BOOST_CHECK_EQUAL(0, mat_dyn_identity.at(r, c));
      }
    }
  }
}

BOOST_FIXTURE_TEST_CASE(rowEchelonForm, Matrix2Test)
{
}

BOOST_FIXTURE_TEST_CASE(trace, Matrix2Test)
{
  BOOST_CHECK_EQUAL(6, _mat_2x2.trace());
  BOOST_CHECK_CLOSE(2.7999, _mat_3x3_d.trace(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(invertible, Matrix2Test)
{
  BOOST_CHECK(_mat_2x2.invertible());
}

BOOST_FIXTURE_TEST_CASE(singular, Matrix2Test)
{
  BOOST_CHECK(mat_singular.singular());
}


/* Operaciones unarias */

BOOST_FIXTURE_TEST_CASE(plus, Matrix2Test)
{
  Matrix2<double,3,3> mat = +_mat_3x3_d;
  for (size_t r = 0; r < mat.rows(); r++){
    for (size_t c = 0; c < mat.cols(); c++){
      BOOST_CHECK_EQUAL(mat.at(r,c), _mat_3x3_d.at(r, c));
    }
  }

  Matrix2<double> mat2 = +(*_mat_dyn_3x3_d);
  for (size_t r = 0; r < mat2.rows(); r++){
    for (size_t c = 0; c < mat2.cols(); c++){
      BOOST_CHECK_EQUAL(mat2.at(r,c), _mat_dyn_3x3_d->at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(minus, Matrix2Test)
{
  Matrix2<double,3,3> mat = -_mat_3x3_d;
  for (size_t r = 0; r < mat.rows(); r++){
    for (size_t c = 0; c < mat.cols(); c++){
      BOOST_CHECK_EQUAL(-mat.at(r, c), _mat_3x3_d.at(r, c));
    }
  }

  Matrix2<double> mat2 = -(*_mat_dyn_3x3_d);
  for (size_t r = 0; r < mat2.rows(); r++){
    for (size_t c = 0; c < mat2.cols(); c++){
      BOOST_CHECK_EQUAL(-mat2.at(r,c), _mat_dyn_3x3_d->at(r, c));
    }
  }
}

/* Operaciones binarias entre matrices */

/// Suma o adición de matrices

BOOST_FIXTURE_TEST_CASE(addition, Matrix2Test)
{
  Matrix2<double,3,3> mat = mat_ones + _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat.at(0, 0));
  BOOST_CHECK_EQUAL(1.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(3.5, mat.at(0, 2));
  BOOST_CHECK_EQUAL(2.0, mat.at(1, 0));
  BOOST_CHECK_EQUAL(2.0, mat.at(1, 1));
  BOOST_CHECK_EQUAL(2.2, mat.at(1, 2));
  BOOST_CHECK_EQUAL(2.3, mat.at(2, 0));
  BOOST_CHECK_EQUAL(3.6, mat.at(2, 1));
  BOOST_CHECK_EQUAL(1.3, mat.at(2, 2));

  mat_ones += _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat_ones.at(0, 0));
  BOOST_CHECK_EQUAL(1.0, mat_ones.at(0, 1));
  BOOST_CHECK_EQUAL(3.5, mat_ones.at(0, 2));
  BOOST_CHECK_EQUAL(2.0, mat_ones.at(1, 0));
  BOOST_CHECK_EQUAL(2.0, mat_ones.at(1, 1));
  BOOST_CHECK_EQUAL(2.2, mat_ones.at(1, 2));
  BOOST_CHECK_EQUAL(2.3, mat_ones.at(2, 0));
  BOOST_CHECK_EQUAL(3.6, mat_ones.at(2, 1));
  BOOST_CHECK_EQUAL(1.3, mat_ones.at(2, 2));

  Matrix2<double> mat2 = mat_dyn_ones + (*_mat_dyn_3x3_d);

  BOOST_CHECK_EQUAL(2.5, mat2.at(0, 0));
  BOOST_CHECK_EQUAL(1.0, mat2.at(0, 1));
  BOOST_CHECK_EQUAL(3.5, mat2.at(0, 2));
  BOOST_CHECK_EQUAL(2.0, mat2.at(1, 0));
  BOOST_CHECK_EQUAL(2.0, mat2.at(1, 1));
  BOOST_CHECK_EQUAL(2.2, mat2.at(1, 2));
  BOOST_CHECK_EQUAL(2.3, mat2.at(2, 0));
  BOOST_CHECK_EQUAL(3.6, mat2.at(2, 1));
  BOOST_CHECK_EQUAL(1.3, mat2.at(2, 2));

  mat_dyn_ones += (*_mat_dyn_3x3_d);

  BOOST_CHECK_EQUAL(2.5, mat_dyn_ones.at(0, 0));
  BOOST_CHECK_EQUAL(1.0, mat_dyn_ones.at(0, 1));
  BOOST_CHECK_EQUAL(3.5, mat_dyn_ones.at(0, 2));
  BOOST_CHECK_EQUAL(2.0, mat_dyn_ones.at(1, 0));
  BOOST_CHECK_EQUAL(2.0, mat_dyn_ones.at(1, 1));
  BOOST_CHECK_EQUAL(2.2, mat_dyn_ones.at(1, 2));
  BOOST_CHECK_EQUAL(2.3, mat_dyn_ones.at(2, 0));
  BOOST_CHECK_EQUAL(3.6, mat_dyn_ones.at(2, 1));
  BOOST_CHECK_EQUAL(1.3, mat_dyn_ones.at(2, 2));
}

/// Resta de matrices

BOOST_FIXTURE_TEST_CASE(subtraction, Matrix2Test)
{
  Matrix2<double,3,3> mat = mat_identity - _mat_3x3_d;

  BOOST_CHECK_EQUAL(-0.5, mat.at(0, 0));
  BOOST_CHECK_EQUAL( 0.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(-2.5, mat.at(0, 2));
  BOOST_CHECK_EQUAL(-1.0, mat.at(1, 0));
  BOOST_CHECK_EQUAL( 0.0, mat.at(1, 1));
  BOOST_CHECK_EQUAL(-1.2, mat.at(1, 2));
  BOOST_CHECK_EQUAL(-1.3, mat.at(2, 0));
  BOOST_CHECK_EQUAL(-2.6, mat.at(2, 1));
  BOOST_CHECK_EQUAL( 0.7, mat.at(2, 2));

  mat_identity -= _mat_3x3_d;

  BOOST_CHECK_EQUAL(-0.5, mat_identity.at(0, 0));
  BOOST_CHECK_EQUAL( 0.0, mat_identity.at(0, 1));
  BOOST_CHECK_EQUAL(-2.5, mat_identity.at(0, 2));
  BOOST_CHECK_EQUAL(-1.0, mat_identity.at(1, 0));
  BOOST_CHECK_EQUAL( 0.0, mat_identity.at(1, 1));
  BOOST_CHECK_EQUAL(-1.2, mat_identity.at(1, 2));
  BOOST_CHECK_EQUAL(-1.3, mat_identity.at(2, 0));
  BOOST_CHECK_EQUAL(-2.6, mat_identity.at(2, 1));
  BOOST_CHECK_EQUAL( 0.7, mat_identity.at(2, 2));


  Matrix2<double> mat2 = mat_dyn_identity - (*_mat_dyn_3x3_d);

  BOOST_CHECK_EQUAL(-0.5, mat2.at(0, 0));
  BOOST_CHECK_EQUAL( 0.0, mat2.at(0, 1));
  BOOST_CHECK_EQUAL(-2.5, mat2.at(0, 2));
  BOOST_CHECK_EQUAL(-1.0, mat2.at(1, 0));
  BOOST_CHECK_EQUAL( 0.0, mat2.at(1, 1));
  BOOST_CHECK_EQUAL(-1.2, mat2.at(1, 2));
  BOOST_CHECK_EQUAL(-1.3, mat2.at(2, 0));
  BOOST_CHECK_EQUAL(-2.6, mat2.at(2, 1));
  BOOST_CHECK_EQUAL( 0.7, mat2.at(2, 2));

  mat_dyn_identity -= (*_mat_dyn_3x3_d);

  BOOST_CHECK_EQUAL(-0.5, mat_dyn_identity.at(0, 0));
  BOOST_CHECK_EQUAL( 0.0, mat_dyn_identity.at(0, 1));
  BOOST_CHECK_EQUAL(-2.5, mat_dyn_identity.at(0, 2));
  BOOST_CHECK_EQUAL(-1.0, mat_dyn_identity.at(1, 0));
  BOOST_CHECK_EQUAL( 0.0, mat_dyn_identity.at(1, 1));
  BOOST_CHECK_EQUAL(-1.2, mat_dyn_identity.at(1, 2));
  BOOST_CHECK_EQUAL(-1.3, mat_dyn_identity.at(2, 0));
  BOOST_CHECK_EQUAL(-2.6, mat_dyn_identity.at(2, 1));
  BOOST_CHECK_EQUAL( 0.7, mat_dyn_identity.at(2, 2));
}

/// Multiplicación de matrices

BOOST_FIXTURE_TEST_CASE(multiplication, Matrix2Test)
{
  Matrix2<int, 2, 3> mat = _mat_2x3_i * _cofactor_matrix;

  BOOST_CHECK_EQUAL(12, mat.at(0, 0));
  BOOST_CHECK_EQUAL(44, mat.at(0, 1));
  BOOST_CHECK_EQUAL(94, mat.at(0, 2));
  BOOST_CHECK_EQUAL(11, mat.at(1, 0));
  BOOST_CHECK_EQUAL(42, mat.at(1, 1));
  BOOST_CHECK_EQUAL(69, mat.at(1, 2));

  Matrix2<int> mat2 = (*_mat_dyn_2x3_i) * (*_cofactor_matrix_dyn);

  BOOST_CHECK_EQUAL(12, mat2.at(0, 0));
  BOOST_CHECK_EQUAL(44, mat2.at(0, 1));
  BOOST_CHECK_EQUAL(94, mat2.at(0, 2));
  BOOST_CHECK_EQUAL(11, mat2.at(1, 0));
  BOOST_CHECK_EQUAL(42, mat2.at(1, 1));
  BOOST_CHECK_EQUAL(69, mat2.at(1, 2));

}

/// Multiplicación de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(MatrixScalar, Matrix2Test)
{
  Matrix2<int,2,3> mat = _mat_2x3_i * 10;

  BOOST_CHECK_EQUAL(60, mat.at(0, 0));
  BOOST_CHECK_EQUAL(80, mat.at(0, 1));
  BOOST_CHECK_EQUAL(60, mat.at(0, 2));
  BOOST_CHECK_EQUAL(90, mat.at(1, 0));
  BOOST_CHECK_EQUAL(60, mat.at(1, 1));
  BOOST_CHECK_EQUAL(20, mat.at(1, 2));

  _mat_2x3_i *= 10;

  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(0, 0));
  BOOST_CHECK_EQUAL(80, _mat_2x3_i.at(0, 1));
  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(0, 2));
  BOOST_CHECK_EQUAL(90, _mat_2x3_i.at(1, 0));
  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(1, 1));
  BOOST_CHECK_EQUAL(20, _mat_2x3_i.at(1, 2));

  Matrix2<int> mat2 = (*_mat_dyn_2x3_i) * 10;
  
  BOOST_CHECK_EQUAL(60, mat2.at(0, 0));
  BOOST_CHECK_EQUAL(80, mat2.at(0, 1));
  BOOST_CHECK_EQUAL(60, mat2.at(0, 2));
  BOOST_CHECK_EQUAL(90, mat2.at(1, 0));
  BOOST_CHECK_EQUAL(60, mat2.at(1, 1));
  BOOST_CHECK_EQUAL(20, mat2.at(1, 2));

  (*_mat_dyn_2x3_i) *= 10;

  BOOST_CHECK_EQUAL(60, _mat_dyn_2x3_i->at(0, 0));
  BOOST_CHECK_EQUAL(80, _mat_dyn_2x3_i->at(0, 1));
  BOOST_CHECK_EQUAL(60, _mat_dyn_2x3_i->at(0, 2));
  BOOST_CHECK_EQUAL(90, _mat_dyn_2x3_i->at(1, 0));
  BOOST_CHECK_EQUAL(60, _mat_dyn_2x3_i->at(1, 1));
  BOOST_CHECK_EQUAL(20, _mat_dyn_2x3_i->at(1, 2));

}

/// Multiplicación de un escalar por una matriz

BOOST_FIXTURE_TEST_CASE(scalar_matrix, Matrix2Test)
{
  Matrix2<int,2,3> mat = 10 * _mat_2x3_i;

  BOOST_CHECK_EQUAL(60, mat.at(0, 0));
  BOOST_CHECK_EQUAL(80, mat.at(0, 1));
  BOOST_CHECK_EQUAL(60, mat.at(0, 2));
  BOOST_CHECK_EQUAL(90, mat.at(1, 0));
  BOOST_CHECK_EQUAL(60, mat.at(1, 1));
  BOOST_CHECK_EQUAL(20, mat.at(1, 2));

  Matrix2<int> mat2 = 10 * (*_mat_dyn_2x3_i);

  BOOST_CHECK_EQUAL(60, mat2.at(0, 0));
  BOOST_CHECK_EQUAL(80, mat2.at(0, 1));
  BOOST_CHECK_EQUAL(60, mat2.at(0, 2));
  BOOST_CHECK_EQUAL(90, mat2.at(1, 0));
  BOOST_CHECK_EQUAL(60, mat2.at(1, 1));
  BOOST_CHECK_EQUAL(20, mat2.at(1, 2));
}

// División de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(div_matrix_scalar, Matrix2Test)
{
  Matrix2<double,3,3> mat = _mat_3x3_d / 10.;

  BOOST_CHECK_EQUAL(.15, mat.at(0, 0));
  BOOST_CHECK_EQUAL(.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(.25, mat.at(0, 2));
  BOOST_CHECK_EQUAL(.10, mat.at(1, 0));
  BOOST_CHECK_EQUAL(.10, mat.at(1, 1));
  BOOST_CHECK_EQUAL(.12, mat.at(1, 2));
  BOOST_CHECK_EQUAL(.13, mat.at(2, 0));
  BOOST_CHECK_EQUAL(.26, mat.at(2, 1));
  BOOST_CHECK_EQUAL(.03, mat.at(2, 2));

  Matrix2<double> mat2 = (*_mat_dyn_3x3_d) / 10.;

  BOOST_CHECK_EQUAL(.15, mat2.at(0, 0));
  BOOST_CHECK_EQUAL(.0, mat2.at(0, 1));
  BOOST_CHECK_EQUAL(.25, mat2.at(0, 2));
  BOOST_CHECK_EQUAL(.10, mat2.at(1, 0));
  BOOST_CHECK_EQUAL(.10, mat2.at(1, 1));
  BOOST_CHECK_EQUAL(.12, mat2.at(1, 2));
  BOOST_CHECK_EQUAL(.13, mat2.at(2, 0));
  BOOST_CHECK_EQUAL(.26, mat2.at(2, 1));
  BOOST_CHECK_EQUAL(.03, mat2.at(2, 2));
}

BOOST_AUTO_TEST_SUITE_END()
