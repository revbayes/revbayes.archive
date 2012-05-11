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
// Matrix<double>
Matrix<double>                            operator+(const Matrix<double>& A);                                             //!< Unary operator + 
Matrix<double>                            operator-(const Matrix<double>& A);                                             //!< Unary operator - 
Matrix<double>                            operator+(const Matrix<double>& A, const Matrix<double>& B);                        //!< operator + 
Matrix<double>                            operator-(const Matrix<double>& A, const Matrix<double>& B);                        //!< operator - 
Matrix<double>                            operator*(const Matrix<double>& A, const Matrix<double>& B);                        //!< operator * (matrix multiplication) 
Matrix<double>                            operator/(const Matrix<double>& A, const Matrix<double>& B);                        //!< operator / for matrix / matrix 
Matrix<double>&                           operator+=(Matrix<double>& A, const Matrix<double>& B);                             //!< operator += 
Matrix<double>&                           operator-=(Matrix<double>& A, const Matrix<double>& B);                             //!< operator -= 
Matrix<double>&                           operator*=(Matrix<double>& A, const Matrix<double>& B);                             //!< operator *= (matrix multiplication)
Matrix<double>                            operator+(const double &a, const Matrix<double>& B);                            //!< operator + for scalar + matrix 
Matrix<double>                            operator-(const double &a, const Matrix<double>& B);                            //!< operator - for scalar - matrix 
Matrix<double>                            operator*(const double &a, const Matrix<double>& B);                            //!< operator * for scalar * matrix 
Matrix<double>                            operator/(const double &a, const Matrix<double>& B);                            //!< operator / for scalar / matrix 
Matrix<double>                            operator+(const Matrix<double>& A, const double& b);                            //!< operator + for matrix + scalar 
Matrix<double>                            operator-(const Matrix<double>& A, const double& b);                            //!< operator - for matrix - scalar 
Matrix<double>                            operator*(const Matrix<double>& A, const double& b);                            //!< operator * for matrix * scalar 
Matrix<double>                            operator/(const Matrix<double>& A, const double& b);                            //!< operator / for matrix / scalar 
Matrix<double>&                           operator+=(Matrix<double>& A, const double& b);                                 //!< operator += for scalar 
Matrix<double>&                           operator-=(Matrix<double>& A, const double& b);                                 //!< operator -= for scalar 
Matrix<double>&                           operator*=(Matrix<double>& A, const double& b);                                 //!< operator *= for scalar 
Matrix<double>&                           operator/=(Matrix<double>& A, const double& b);                                 //!< operator /= for scalar 

#endif

