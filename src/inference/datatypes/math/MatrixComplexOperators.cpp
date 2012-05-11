//
//  MatrixComplexOperators.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 5/11/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MatrixComplexOperators.h"
#include "RbException.h"
#include "RbMathMatrix.h"


/**
 * This function performs addition of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator+ (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A + b
 */
Matrix<std::complex<double> > operator+(const Matrix<std::complex<double> >& A, const double& b) {
    
	Matrix<std::complex<double> > B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			B[i][j] = A[i][j] - b;
	return B;
}


/**
 * This function performs subtraction of a scalar from
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator- (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A - b
 */
Matrix<std::complex<double> > operator-(const Matrix<std::complex<double> >& A, const double& b) {
    
	Matrix<std::complex<double> > B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			B[i][j] = A[i][j] - b;
	return B;
}

/**
 * This function performs multiplication of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator* (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A * b
 */
Matrix<std::complex<double> > operator*(const Matrix<std::complex<double> >& A, const double& b) {
    
	Matrix<std::complex<double> > B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			B[i][j] = A[i][j] * b;
	return B;
}

/**
 * This function performs division with a scalar of
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator/ (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A / b
 */
Matrix<std::complex<double> > operator/(const Matrix<std::complex<double> >& A, const double& b) {
    
	Matrix<std::complex<double> > B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			B[i][j] = A[i][j] / b;
	return B;
}

/**
 * This function performs addition of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator+ (scalar first)
 * @param a Scalar
 * @param B Matrix
 * @return a + B
 */
Matrix<std::complex<double> > operator+(const double& a, const Matrix<std::complex<double> >& B) {
    
	Matrix<std::complex<double> > A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			A[i][j] = a + B[i][j];
	return A;
}

/**
 * This function subtracts each element of a
 * a matrix from a scalar and returns the
 * resulting matrix.
 *
 * @brief operator- (scalar first)
 * @param a Scalar
 * @param B Matrix
 * @return a - B
 */
Matrix<std::complex<double> > operator-(const double& a, const Matrix<std::complex<double> >& B) {
    
	Matrix<std::complex<double> > A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			A[i][j] = a - B[i][j];
	return A;
}

/**
 * This function performs multiplication of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator* (scalar first)
 * @param a Scalar
 * @param B Matrix
 * @return a * B
 */
Matrix<std::complex<double> > operator*(const double& a, const Matrix<std::complex<double> >& B) {
    
	Matrix<std::complex<double> > A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			A[i][j] = a * B[i][j];
	return A;
}

/**
 * This function performs division of a scalar by
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator/ (scalar first)
 * @param a Scalar
 * @param B Matrix
 * @return a / B
 */
Matrix<std::complex<double> > operator/(const double& a, const Matrix<std::complex<double> >& B) {
    
	Matrix<std::complex<double> > A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			A[i][j] = a / B[i][j];
	return A;
}

/**
 * This function performs division of a scalar by
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator/ (scalar first)
 * @param A Matrix
 * @param B Matrix
 * @return A / B (actually, A * B^(-1))
 */
Matrix<std::complex<double> > operator/(const Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B) {
    
    if ( A.getNumberOfRows() != A.getNumberOfColumns() )
        throw RbException("Cannot divide non-square matrices");
	if ( A.getNumberOfColumns() != B.getNumberOfColumns() )
        throw RbException("Cannot divide matrices of differing dimension");
    
	size_t N = A.getNumberOfColumns();
	Matrix<std::complex<double> > invB(N, N, std::complex<double>(0.0));
    RbMath::matrixInverse(B, invB);
    
	Matrix<std::complex<double> > C(N, N, std::complex<double>(0.0));
	for (size_t i=0; i<N; i++) 
    {
		for (size_t j=0; j<N; j++) 
        {
			std::complex<double> sum = std::complex<double>(0.0,0.0);
			for (size_t k=0; k<N; k++)
				sum += (A[i][k] * B[k][j]);
			C[i][j] = sum;
        }
    }
	return C;    
}

/**
 * This function performs addition of a scalar to
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * @brief operator+= (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A += b
 */
Matrix<std::complex<double> > &operator+=(Matrix<std::complex<double> >& A, const double& b) {
    
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			A[i][j] += b;
	return A;
}

/**
 * This function performs subtraction of a scalar from
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * @brief operator-= (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A -= b
 */
Matrix<std::complex<double> > &operator-=(Matrix<std::complex<double> >& A, const double& b) {
    
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			A[i][j] -= b;
	return A;
}

/**
 * This function performs multiplication of a scalar to
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * @brief operator*= (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A *= b
 */
Matrix<std::complex<double> > &operator*=(Matrix<std::complex<double> >& A, const double& b) {
    
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			A[i][j] *= b;
	return A;
}

/**
 * This function performs division with a scalar of
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * @brief operator/= (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A /= b
 */
Matrix<std::complex<double> > &operator/=(Matrix<std::complex<double> >& A, const double& b) {
    
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			A[i][j] /= b;
	return A;
}

/**
 * This function performs elementwise addition of two
 * matrices and returns the resulting matrix. If the
 * matrices are not conformant, a null matrix is returned.
 *
 * @brief operator+
 * @param A Matrix 1
 * @param B Matrix 2
 * @return A + B, null matrix on failure
 */
Matrix<std::complex<double> > operator+(const Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B) {
    
	size_t m = A.getNumberOfRows();
	size_t n = A.getNumberOfColumns();
	if (B.getNumberOfRows() != m || B.getNumberOfColumns() != n)
        throw RbException("Cannot add matrices A and B: the matrices are not of the same dimension");
	else 
    {
		Matrix<std::complex<double> > C(m, n, std::complex<double>( 0.0 ) );
		for (size_t i=0; i<m; i++) 
        {
			for (size_t j=0; j<n; j++)
				C[i][j] = A[i][j] + B[i][j];
        }
		return C;
    }
}

/**
 * This function performs elementwise subtraction of two
 * matrices and returns the resulting matrix. If the
 * matrices are not conformant, a null matrix is returned.
 *
 * @brief operator-
 * @param A Matrix 1
 * @param B Matrix 2
 * @return A - B, null matrix on failure
 */
Matrix<std::complex<double> > operator-(const Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B) {
    
	size_t m = A.getNumberOfRows();
	size_t n = A.getNumberOfColumns();
	if (B.getNumberOfRows() != m || B.getNumberOfColumns() != n)
        throw RbException("Cannot subtract matrices A and B: the matrices are not of the same dimension");
	else 
    {
		Matrix<std::complex<double> > C(m, n, std::complex<double> ( 0.0 ) );
		for (size_t i=0; i<m; i++) 
        {
			for (size_t j=0; j<n; j++)
				C[i][j] = A[i][j] - B[i][j];
        }
		return C;
    }
}

/**
 * Compute C = A*B, where C[i][j] is the dot-product of 
 * row i of A and column j of B. Note that this operator
 * does not perform elementwise multiplication. If the 
 * matrices do not have the right dimensions for matrix
 * multiplication (that is, if the number of columns of A
 * is different from the number of rows of B), the function
 * returns a null matrix.
 *
 * @brief Matrix multiplication
 * @param A An (m X n) matrix
 * @param B An (n X k) matrix
 * @return A * B, an (m X k) matrix, or null matrix on failure
 */
Matrix<std::complex<double> > operator*(const Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B) {
    
	if ( A.getNumberOfColumns() != B.getNumberOfRows() )
        throw RbException("Cannot multiply matrices A and B: the number of columns of A does not equal the number of rows in B");
	size_t M = A.getNumberOfRows();
	size_t N = A.getNumberOfColumns();
	size_t K = B.getNumberOfColumns();
	Matrix<std::complex<double> > C(M, K, std::complex<double>( 0.0 ) );
	for (size_t i=0; i<M; i++) 
    {
		for (size_t j=0; j<K; j++) 
        {
			std::complex<double> sum = std::complex<double>(0.0,0.0);
			for (size_t k=0; k<N; k++)
				sum += (A[i][k] * B[k][j]);
			C[i][j] = sum;
        }
    }
	return C;
}

/**
 * This function performs elementwise addition on two
 * matrices and puts the result in the first matrix.
 * If the two matrices are nonconformant, the first
 * matrix is left intact.
 *
 * @brief operator+=
 * @param A Matrix 1
 * @param B Matrix 2
 * @return A += B, A unmodified on failure
 */
Matrix<std::complex<double> >&  operator+=(Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B) {
    
	size_t m = A.getNumberOfRows();
	size_t n = A.getNumberOfColumns();
	if (B.getNumberOfRows() == m && B.getNumberOfColumns() == n) 
    {
		for (size_t i=0; i<m; i++) 
        {
			for (size_t j=0; j<n; j++)
				A[i][j] += B[i][j];
        }
    }
	return A;
}

/**
 * This function performs elementwise subtraction on two
 * matrices and puts the result in the first matrix.
 * If the two matrices are nonconformant, the first
 * matrix is left intact.
 *
 * @brief operator-=
 * @param A Matrix 1
 * @param B Matrix 2
 * @return A -= B, A unmodified on failure
 */
Matrix<std::complex<double> >&  operator-=(Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B) {
    
	size_t m = A.getNumberOfRows();
	size_t n = A.getNumberOfColumns();
	if (B.getNumberOfRows() == m && B.getNumberOfColumns() == n) 
    {
		for (size_t i=0; i<m; i++) 
        {
			for (size_t j=0; j<n; j++)
				A[i][j] -= B[i][j];
        }
    }
	return A;
}

/**
 * Compute C = A*B, where C[i][j] is the dot-product of 
 * row i of A and column j of B. Then assign the result to
 * A. Note that this operator does not perform elementwise
 * multiplication. If the matrices are not both square of the
 * same dimension, then the operation is not possible to
 * perform and we return an unomidified A.
 *
 * \brief Matrix multiplication with assignment (operator *=)
 * \param A An (n X n) matrix
 * \param B An (n X n) matrix
 * \return A = A * B, an (n X n) matrix, or unmodified A on failure
 */
Matrix<std::complex<double> > &operator*=(Matrix<std::complex<double> >& A, const Matrix<std::complex<double> >& B) {
    
	if ( A.getNumberOfRows() == A.getNumberOfColumns() && B.getNumberOfRows() == B.getNumberOfColumns() && A.getNumberOfRows() == B.getNumberOfRows() ) 
    {
		size_t N = A.getNumberOfRows();
		Matrix<std::complex<double> > C(N, N, std::complex<double> (0.0) );
		for (size_t i=0; i<N; i++) 
        {
			for (size_t j=0; j<N; j++) 
            {
                std::complex<double> sum = std::complex<double>(0.0,0.0);
				for (size_t k=0; k<N; k++)
					sum += (A[i][k] * B[k][j]);
				C[i][j] = sum;
            }
        }
		A = C;
    }
	return A;
}
