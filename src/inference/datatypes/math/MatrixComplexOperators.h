/**
 * @file
 * This file contains the declaration and implementation of operators
 * on a matrix of real numbers.
 *
 * @brief Operators on matrix of real numbers
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 *
 * $Id: $
 */

#ifndef MatrixComplexOperators_H
#define MatrixComplexOperators_H

#include "Matrix.h"

#include <complex>


Matrix<std::complex<double> >                         operator+(const Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B);                        //!< operator + 
Matrix<std::complex<double> >                         operator-(const Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B);                        //!< operator - 
Matrix<std::complex<double> >                         operator*(const Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B);                        //!< operator * (matrix multiplication) 
Matrix<std::complex<double> >&                        operator+=(Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B);                             //!< operator += 
Matrix<std::complex<double> >&                        operator-=(Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B);                             //!< operator -= 
Matrix<std::complex<double> >&                        operator*=(Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B);                             //!< operator *= (matrix multiplication)
Matrix<std::complex<double> >                         operator+(const double &a, const Matrix<std::complex<double> >& B);                                               //!< operator + for scalar + matrix 
Matrix<std::complex<double> >                         operator-(const double &a, const Matrix<std::complex<double> >& B);                                               //!< operator - for scalar - matrix 
Matrix<std::complex<double> >                         operator*(const double &a, const Matrix<std::complex<double> >& B);                                               //!< operator * for scalar * matrix 
Matrix<std::complex<double> >                         operator/(const double &a, const Matrix<std::complex<double> >& B);                                               //!< operator / for scalar / matrix 
Matrix<std::complex<double> >                         operator+(const Matrix<std::complex<double> >& A, const double& b);                                               //!< operator + for matrix + scalar 
Matrix<std::complex<double> >                         operator-(const Matrix<std::complex<double> >& A, const double& b);                                               //!< operator - for matrix - scalar 
Matrix<std::complex<double> >                         operator*(const Matrix<std::complex<double> >& A, const double& b);                                               //!< operator * for matrix * scalar 
Matrix<std::complex<double> >                         operator/(const Matrix<std::complex<double> >& A, const double& b);                                               //!< operator / for matrix / scalar 
Matrix<std::complex<double> >                         operator/(const Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B);                        //!< operator / for matrix / matrix 
Matrix<std::complex<double> >&                        operator+=(Matrix<std::complex<double> >& A, const double& b);                                                    //!< operator += for scalar 
Matrix<std::complex<double> >&                        operator-=(Matrix<std::complex<double> >& A, const double& b);                                                    //!< operator -= for scalar 
Matrix<std::complex<double> >&                        operator*=(Matrix<std::complex<double> >& A, const double& b);                                                    //!< operator *= for scalar 
Matrix<std::complex<double> >&                        operator/=(Matrix<std::complex<double> >& A, const double& b);                                                    //!< operator /= for scalar 



#endif