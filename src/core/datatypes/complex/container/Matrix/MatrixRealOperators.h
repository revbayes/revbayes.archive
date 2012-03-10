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

#ifndef MatrixRealOperators_H
#define MatrixRealOperators_H

#include "Matrix.h"



// operators defined outside of the class
// Matrix<Real>
Matrix<Real>                            operator+(const Matrix<Real>& A);                                             //!< Unary operator + 
Matrix<Real>                            operator-(const Matrix<Real>& A);                                             //!< Unary operator - 
Matrix<Real>                            operator+(const Matrix<Real>& A, const Matrix<Real>& B);                        //!< operator + 
Matrix<Real>                            operator-(const Matrix<Real>& A, const Matrix<Real>& B);                        //!< operator - 
Matrix<Real>                            operator*(const Matrix<Real>& A, const Matrix<Real>& B);                        //!< operator * (matrix multiplication) 
Matrix<Real>                            operator/(const Matrix<Real>& A, const Matrix<Real>& B);                        //!< operator / for matrix / matrix 
Matrix<Real>&                           operator+=(Matrix<Real>& A, const Matrix<Real>& B);                             //!< operator += 
Matrix<Real>&                           operator-=(Matrix<Real>& A, const Matrix<Real>& B);                             //!< operator -= 
Matrix<Real>&                           operator*=(Matrix<Real>& A, const Matrix<Real>& B);                             //!< operator *= (matrix multiplication)
Matrix<Real>                            operator+(const double &a, const Matrix<Real>& B);                            //!< operator + for scalar + matrix 
Matrix<Real>                            operator-(const double &a, const Matrix<Real>& B);                            //!< operator - for scalar - matrix 
Matrix<Real>                            operator*(const double &a, const Matrix<Real>& B);                            //!< operator * for scalar * matrix 
Matrix<Real>                            operator/(const double &a, const Matrix<Real>& B);                            //!< operator / for scalar / matrix 
Matrix<Real>                            operator+(const Matrix<Real>& A, const double& b);                            //!< operator + for matrix + scalar 
Matrix<Real>                            operator-(const Matrix<Real>& A, const double& b);                            //!< operator - for matrix - scalar 
Matrix<Real>                            operator*(const Matrix<Real>& A, const double& b);                            //!< operator * for matrix * scalar 
Matrix<Real>                            operator/(const Matrix<Real>& A, const double& b);                            //!< operator / for matrix / scalar 
Matrix<Real>&                           operator+=(Matrix<Real>& A, const double& b);                                 //!< operator += for scalar 
Matrix<Real>&                           operator-=(Matrix<Real>& A, const double& b);                                 //!< operator -= for scalar 
Matrix<Real>&                           operator*=(Matrix<Real>& A, const double& b);                                 //!< operator *= for scalar 
Matrix<Real>&                           operator/=(Matrix<Real>& A, const double& b);                                 //!< operator /= for scalar 

#endif


