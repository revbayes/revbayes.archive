//
//  MatrixReal.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/17/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "EigenSystem.h"
#include "MatrixReal.h"
#include "RbException.h"

#include <cstring>
#include <iomanip>

using namespace RevBayesCore;


MatrixReal::MatrixReal( void ) : elements( std::vector<std::vector<double> >() ),
    nRows( 0 ),
    nCols( 0 ),
    eigensystem( NULL ),
    eigenNeedsUpdate( true )
{

}


MatrixReal::MatrixReal( size_t n ) : elements( std::vector<std::vector<double> >(n, std::vector<double>(n,0.0) ) ),
    nRows( n ),
    nCols( n ),
    eigensystem( NULL ),
    eigenNeedsUpdate( true )
{
    
}


MatrixReal::MatrixReal( size_t n, size_t k) : elements( std::vector<std::vector<double> >(n, std::vector<double>(k,0.0) ) ),
    nRows( n ),
    nCols( k ),
    eigensystem( NULL ),
    eigenNeedsUpdate( true )
{
    
}


MatrixReal::MatrixReal( size_t n, size_t k, double v) :
    elements( std::vector<std::vector<double> >(n, std::vector<double>(k,v) ) ),
    nRows( n ),
    nCols( k ),
    eigensystem( NULL ),
    eigenNeedsUpdate( true )
{

}

MatrixReal::MatrixReal( const MatrixReal &m ) :
    elements( m.elements ),
    nRows( m.nRows ),
    nCols( m.nCols ),
    eigensystem( NULL ),
    eigenNeedsUpdate( true )
{
    
}


MatrixReal::~MatrixReal( void )
{
    delete eigensystem;
}


MatrixReal& MatrixReal::operator=(const MatrixReal &m)
{
    
    if ( this != &m )
    {
        nCols = m.nCols;
        nRows = m.nRows;
        elements = m.elements;
        
        eigenNeedsUpdate = true;
    }
    
    return *this;
}


std::vector<double>& MatrixReal::operator[]( size_t index )
{
    // to be safe
    eigenNeedsUpdate = true;
    
    return elements[index];
}



const std::vector<double>& MatrixReal::operator[]( size_t index ) const
{
    return elements[index];
}


std::vector<std::vector<double> >::const_iterator MatrixReal::begin( void ) const
{
    // to be safe
    eigenNeedsUpdate = true;
    
    return elements.begin();
}


std::vector<std::vector<double> >::iterator MatrixReal::begin( void )
{
    // to be safe
    eigenNeedsUpdate = true;
    
    return elements.begin();
}


std::vector<std::vector<double> >::const_iterator MatrixReal::end( void ) const
{
    // to be safe
    eigenNeedsUpdate = true;
    
    return elements.end();
}


std::vector<std::vector<double> >::iterator MatrixReal::end( void )
{
    // to be safe
    eigenNeedsUpdate = true;
    
    return elements.end();
}


void MatrixReal::clear( void )
{
    // to be safe
    eigenNeedsUpdate = true;
    
    elements.clear();
}


MatrixReal MatrixReal::computeInverse( void ) const
{
 
    // update the eigensystem if necessary
    update();
    
    /*
    if (check)  
    {
        for (size_t i=0; i<getDim(); i++)   
        {
            if (std::fabs(eigenval[i] - bkeigenval[i]) > 1e-6)   
            {
                std::cerr << "error: diag flag up but eigen vals not correct\n";
                exit(1);
            }
        }
    }
    */
    
    
    const std::vector<double>& eigenval = eigensystem->getRealEigenvalues();
    
    MatrixReal tmp(nRows, nRows, 0);
    for (size_t i = 0; i < nRows; i++)
    {
        tmp[i][i] = 1.0 / eigenval[i];
    }
    
    tmp *= eigensystem->getInverseEigenvectors();
    MatrixReal inverse = eigensystem->getEigenvectors() * tmp;

    return inverse;

}

MatrixReal* MatrixReal::clone(void) const
{
     return new MatrixReal( *this );
}


size_t MatrixReal::getDim( void ) const
{
    // we assume that this is a square matrix
    return nRows;
}


EigenSystem& MatrixReal::getEigenSystem( void )
{
    // update the eigensystem if necessary
    update();
    
    return *eigensystem;
}


const EigenSystem& MatrixReal::getEigenSystem( void ) const
{
    // update the eigensystem if necessary
    update();
    
    return *eigensystem;
}


double MatrixReal::getLogDet() const
{
    // update the eigensystem if necessary
    update();

    const std::vector<double>& eigenval = eigensystem->getRealEigenvalues();

    double tot = 0;
    for (size_t i=0; i<nRows; i++)
    {
        tot += log(eigenval[i]);
    }
    if (std::isnan(tot))
    {
        std::cerr << "in MatrixReal::getLogDet(): nan\n";
        std::cerr << "eigen values:\n";
        for (size_t i=0; i<nRows; i++)
        {
            std::cerr << eigenval[i] << '\n';
        }
        RbException("Problem when computing log-determinant.");
    }
    
    return tot;
}


size_t MatrixReal::getNumberOfColumns( void ) const
{
    return nCols;
}



size_t MatrixReal::getNumberOfRows( void ) const
{
    return nRows;
}

//double MatrixReal::getCovariance(size_t k, size_t l)  const
//{
//
//    if (k>getDim())
//    {
//        throw RbException("Index out of range in symmetric real matrix.");
//    }
//
//    if (l>getDim())
//    {
//        throw RbException("Index out of range in symmetric real matrix.");
//    }
//
//    return (*this)[k][l];
//}
//
//double MatrixReal::getPrecision(size_t k, size_t l)  const
//{
//
//    if (k>getDim())
//    {
//        throw RbException("Index out of range in symmetric real matrix.");
//    }
//
//    if (l>getDim())
//    {
//        throw RbException("Index out of range in symmetric real matrix.");
//    }
//
//    update();
//
//    return inverse[k][l];
//}
//
//double MatrixReal::getCorrel(size_t k, size_t l)  const
//{
//
//    if (k>getDim())
//    {
//        throw RbException("Index out of range in symmetric real matrix.");
//    }
//
//    if (l>getDim())
//    {
//        throw RbException("Index out of range in symmetric real matrix.");
//    }
//
//    return (*this)[k][l] / sqrt((*this)[k][k] * (*this)[l][l]);
//}
//
//double MatrixReal::getPartialCorrel(size_t k, size_t l)  const
//{
//
//    if (k>getDim())
//    {
//        throw RbException("Index out of range in symmetric real matrix.");
//    }
//
//    if (l>getDim())
//    {
//        throw RbException("Index out of range in symmetric real matrix.");
//    }
//
//    update();
//
//    return - inverse[k][l] / sqrt(inverse[k][k] * inverse[l][l]);
//}


bool MatrixReal::isPositive()  const
{

    update();

    const std::vector<double>& eigenval = eigensystem->getRealEigenvalues();

    bool pos = true;
    for (size_t i=0; i<nRows; i++)
    {
        pos &= (eigenval[i] > 0);
    }

    return pos;
}


void MatrixReal::resize(size_t r, size_t c)
{
    
    elements = std::vector<std::vector<double> >(r, std::vector<double>(c,0.0) );
    
    nRows = r;
    nCols = c;
    
    eigenNeedsUpdate = true;
}


size_t MatrixReal::size( void ) const
{
    return nRows*nCols;
}


void MatrixReal::update( void ) const
{
    
    if ( eigensystem == NULL )
    {
        eigensystem = new EigenSystem(this);
    }

    if ( eigenNeedsUpdate == true )
    {

        try
        {

            eigensystem->update();

            eigenNeedsUpdate = true;

        }

        catch(...)
        {
            throw RbException("MatrixReal: eigen update failed");
        }

    }
}



#include "RbMathMatrix.h"
#include "RbException.h"

MatrixReal operator+(const MatrixReal& A);
MatrixReal operator-(const MatrixReal& A);
MatrixReal operator*(const MatrixReal& A, double b);
MatrixReal operator/(const MatrixReal& A, double b);
MatrixReal operator+(double a, const MatrixReal& B);
MatrixReal operator-(double a, const MatrixReal& B);
MatrixReal operator*(double a, const MatrixReal& B);
MatrixReal operator/(double a, const MatrixReal& B);
MatrixReal operator/(const MatrixReal& A, const MatrixReal& B);
MatrixReal &operator/=(MatrixReal& A, double b);
std::vector<double> operator*(const MatrixReal& A, const std::vector<double> &b);


/**
 * This function performs unary plus on a matrix,
 * which simply returns a copy of the matrix.
 *
 * @param  A The matrix operand
 * @return A copy of the operand
 */
MatrixReal operator+(const MatrixReal& A) {
    
	MatrixReal B = A;
    
    return B;
}


/**
 * This function performs unary minus on a matrix.
 *
 * @param A The matrix operand
 * @return -A (the negative of the operand)
 */
MatrixReal operator-(const MatrixReal& A) {
    
	MatrixReal B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			B[i][j] = -B[i][j];
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
MatrixReal MatrixReal::operator+(double b) const {
    
	MatrixReal B = MatrixReal(*this);
    B += b;
    
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
MatrixReal MatrixReal::operator-(double b) const {
    
	MatrixReal B = *this;
    B -= b;
    
	return B;
}


/**
 * This function performs subtraction of a scalar from
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator* (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A * b
 */
MatrixReal MatrixReal::operator*(double b) const
{
    
	MatrixReal B = *this;
    B *= b;
    
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
MatrixReal operator*(const MatrixReal& A, double b) {
    
	MatrixReal B = A;
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
MatrixReal operator/(const MatrixReal& A, double b) {
    
	MatrixReal B = A;
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
MatrixReal operator+(double a, const MatrixReal& B) {
    
	MatrixReal A = B;
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
MatrixReal operator-(double a, const MatrixReal& B)
{
    
	MatrixReal A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
    {
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
        {
			A[i][j] = a - B[i][j];
        }
    }
    
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
MatrixReal operator*(double a, const MatrixReal& B)
{
    
	MatrixReal A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
    {
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
        {
			A[i][j] = a * B[i][j];
        }
    }
    
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
MatrixReal operator/(double a, const MatrixReal& B)
{
    
	MatrixReal A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
    {
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
        {
			A[i][j] = a / B[i][j];
        }
    }
    
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
MatrixReal operator/(const MatrixReal& A, const MatrixReal& B)
{
    
    if ( A.getNumberOfRows() != A.getNumberOfColumns() )
    {
        throw RbException("Cannot divide non-square matrices");
    }
	if ( A.getNumberOfColumns() != B.getNumberOfColumns() )
    {
        throw RbException("Cannot divide matrices of differing dimension");
    }
    
	size_t N = A.getNumberOfColumns();
	MatrixReal invB(N, N, double( 0.0 ) );
    RbMath::matrixInverse(B, invB);
    
	MatrixReal C(N, N, double( 0.0 ) );
	for (size_t i=0; i<N; i++) 
    {
		for (size_t j=0; j<N; j++) 
        {
			double sum = 0.0;
			for (size_t k=0; k<N; k++)
            {
				sum += A[i][k] * B[k][j];
            }
            
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
MatrixReal& MatrixReal::operator+=(double b)
{
    
	for (size_t i=0; i<nRows; i++)
    {
		for (size_t j=0; j<nCols; j++)
        {
			elements[i][j] += b;
        }
    }
    
	return *this;
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
MatrixReal& MatrixReal::operator-=(double b)
{
    
	for (size_t i=0; i<nRows; i++)
    {
		for (size_t j=0; j<nCols; j++)
        {
			elements[i][j] -= b;
        }
    }
    
	return *this;
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
MatrixReal& MatrixReal::operator*=(double b)
{
    
	for (size_t i=0; i<nRows; i++)
    {
		for (size_t j=0; j<nCols; j++)
        {
			elements[i][j] *= b;
        }
    }
    
	return *this;
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
MatrixReal& operator/=(MatrixReal& A, double b) {
    
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
MatrixReal MatrixReal::operator+(const MatrixReal& B) const {
    
    MatrixReal A = MatrixReal(*this);
    A += B;
    
    return A;
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
MatrixReal MatrixReal::operator-(const MatrixReal& B) const {
    
	MatrixReal A = *this;
    A -= B;
    
    return A;
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
MatrixReal MatrixReal::operator*(const MatrixReal& B) const {
    
	MatrixReal C = MatrixReal(*this);
    C *= B;
    
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
MatrixReal&  MatrixReal::operator+=(const MatrixReal& B) {
    
	if (B.getNumberOfRows() == nRows && B.getNumberOfColumns() == nCols) 
    {
		for (size_t i=0; i<nRows; i++) 
        {
			for (size_t j=0; j<nCols; j++)
				elements[i][j] += B[i][j];
        }
    }
    else
    {
        throw RbException("Cannot multiply matrices A and B: the number of columns of A does not equal the number of rows in B");
    }
    
	return *this;
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
MatrixReal& MatrixReal::operator-=(const MatrixReal& B) {
    
	if (B.getNumberOfRows() == nRows && B.getNumberOfColumns() == nCols) 
    {
		for (size_t i=0; i<nRows; i++) 
        {
			for (size_t j=0; j<nCols; j++)
				elements[i][j] -= B[i][j];
        }
    }
    else
    {
        throw RbException("Cannot multiply matrices A and B: the number of columns of A does not equal the number of rows in B");
    }
    
	return *this;
}

/**
 * Compute C = A*B, where C[i][j] is the dot-product of 
 * row i of A and column j of B. Then assign the result to
 * A. Note that this operator does not perform elementwise
 * multiplication. 
 *
 * \brief Matrix multiplication with assignment (operator *=)
 * \param A An (n X m) matrix
 * \param B An (m X p) matrix
 * \return A = A * B, an (n X p) matrix, or unmodified A on failure
 */
MatrixReal& MatrixReal::operator*=(const MatrixReal& B) {
    
    size_t bRows = B.getNumberOfRows();
    size_t bCols = B.getNumberOfColumns();
	if ( nCols == bRows ) 
    {
		MatrixReal C(nRows, bCols, 0.0 );
		for (size_t i=0; i<nRows; i++) 
        {
			for (size_t j=0; j<bCols; j++) 
            {
				double sum = 0.0;
				for (size_t k=0; k<nCols; k++)
					sum += elements[i][k] * B[k][j];
				C[i][j] = sum;
            }
        }
        
        nCols = C.nCols;
        nRows = C.nRows;
        elements = C.elements;
    }
    else
    {
        throw RbException("Cannot multiply matrices A and B: the number of columns of A does not equal the number of rows in B");
    }
    
	return *this;
}


std::vector<double> MatrixReal::operator*(const std::vector<double> &V) const
{
    std::vector<double> E(nCols, 0.0);
    
    for (unsigned int i = 0; i < nCols; i++)
    {
        for (unsigned int j = 0; j < V.size(); j++)
        {
            E[i] = E[i] + elements[j][i] * V[j];
        }
    }
    
    return E;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const MatrixReal& x) {
    
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.getNumberOfRows(); i++) 
    {
        if (i == 0)
            o << "[ ";
        else 
            o << "  ";
        
        for (size_t j = 0; j < x.getNumberOfColumns(); ++j) 
        {
            if (j != 0)
                o << ", ";
            o << x[i][j];
        }
        o <<  " ]";
        
        if (i == x.size()-1)
            o << " ]";
        else 
            o << " ,\n";
        
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}


