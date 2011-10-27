/**
 * @file
 * This file contains the implementation of MatrixComplex, a complex type
 * used to hold matrices of complex values.
 *
 * @brief Implementation of MatrixComplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Complex.h"
#include "ConstantNode.h"
#include "MatrixComplex.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "RbUtil.h"
#include "UserInterface.h"
#include "VectorComplex.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>


// Definition of the static type spec member
const TypeSpec MatrixComplex::typeSpec(MatrixComplex_name);


/** Default constructor resulting in an empty Complex matrix */
MatrixComplex::MatrixComplex(void) : Matrix(Complex_name) {

}


/** Construct matrix of specified dimensions (length), initialize it with double x (default 0.0) */
MatrixComplex::MatrixComplex(const size_t nRows, const size_t nCols, std::complex<double> x) : Matrix(Complex_name) {

    for ( size_t i = 0; i < nRows; i++ )
        elements.push_back( RbPtr<RbLanguageObject>( new VectorComplex( nCols, x ) ) );
}


/** Construct matrix from a two-dimensional set of STL vectors */
MatrixComplex::MatrixComplex(const std::vector<std::vector<std::complex<double> > >& x) : Matrix(Complex_name) {

    size_t numRows = x.size();
    size_t numCols = x[0].size();
    for (size_t i = 1; i < x.size(); i++) 
        {
        if ( x[i].size() != numCols )
            throw RbException( "Invalid attempt to initialize a matrix container using a jagged matrix" );
        }

    for ( size_t i = 0; i < numRows; i++ )
        elements.push_back( RbPtr<RbLanguageObject>( new VectorComplex( x[i] ) ) );
}


/** Index operator (const) */
const RbPtr<VectorComplex> MatrixComplex::operator[]( const size_t i ) const {

    if ( i >= size() )
        throw RbException( "Index to " + Complex_name + "[][] out of bounds" );

    return RbPtr<VectorComplex>( static_cast<VectorComplex*>(elements[i].get()) );
}


/** Index operator */
RbPtr<VectorComplex> MatrixComplex::operator[]( const size_t i ) {

    if ( i >= size() )
        throw RbException( "Index to " + Complex_name + "[][] out of bounds" );
    
    return RbPtr<VectorComplex>( static_cast<VectorComplex*>(elements[i].get()) );
}

/** Clone function */
MatrixComplex* MatrixComplex::clone(void) const {

    return new MatrixComplex(*this);
}


/** Get class vector describing type of object */
const VectorString& MatrixComplex::getClass(void) const {

    static VectorString rbClass = VectorString(MatrixComplex_name) + Matrix::getClass();
    return rbClass;
}


/** Overloaded container method to get element or subcontainer for parser */
RbPtr<RbObject> MatrixComplex::getElement( size_t row , size_t col) const {
    
    RbPtr<VectorComplex> tmp = operator[](row);
    return tmp->getElement(col);
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& MatrixComplex::getTypeSpec(void) const {
    return typeSpec;
}


/** Get matrix value as an STL vector<vector> of doubles */
std::vector<std::vector<std::complex<double> > > MatrixComplex::getValue( void ) const {

    std::vector<std::vector<std::complex<double> > > temp;
    for ( size_t i = 0; i < size(); i++ )
        temp.push_back( operator[](i)->getValue() );
    return temp;
}


/** Calculates the number of digits to the left and right of the decimal point */
bool MatrixComplex::numFmt(int& numToLft, int& numToRht, std::string s) const {

    int ba = 0;
    int n[2] = { 0, 0 };
    bool foundDecimalPoint = false;
    for (size_t i=0; i<s.size(); i++)
        {
        if ( isdigit(s[i]) != 0 )
            n[ba]++;
        else if (s[i] == '.')
            {
            ba = 1;
            foundDecimalPoint = true;
            }
        }
    numToLft = n[0];
    numToRht = n[1];
    return foundDecimalPoint;
}


/** Print value for user */
void MatrixComplex::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
//    // find the maximum number of digits to the left and right of the decimal place
//    int maxToLft = 0, maxToRht = 0;
//    bool foundDecimalPoint = false;
//    for (size_t i=0; i<size(); i++)
//        {
//        for (size_t j=0; j<cols; j++)
//            {
//            std::ostringstream v;
//            v << matrix[i][j];
//            int numToLft, numToRht;
//            if (numFmt(numToLft, numToRht, v.str()) == true)
//                foundDecimalPoint = true;
//            if (numToLft > maxToLft)
//                maxToLft = numToLft;
//            if (numToRht > maxToRht)
//                maxToRht = numToRht;
//            }
//        }
        
    // print the matrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i<size(); i++) {
        std::string lineStr = "";
        if (i == 0)
            lineStr += "[ ";
        else 
            lineStr += pad  + "  ";
        
        
        const RbPtr<VectorComplex> vec = operator[](i);
        lineStr += vec->briefInfo();
        if (i == size()-1)
            lineStr += " ]";
        else 
            lineStr += " ,\n";
        
        o << lineStr;
        //RBOUT(lineStr);
        }
    
    o.setf(previousFlags);
    o << o.precision(previousPrecision);
}


///** Push back a row vector */
//void MatrixComplex::push_back( const VectorComplex& x ) {
//    
//    if ( size() > 0 && x.size() != getNumberOfColumns() )
//        throw RbException( "Cannot make matrix with rows of unequal size" );
//    
//    elements.push_back( RbPtr<RbLanguageObject>( x.clone() ) );
//}


/** Overloaded container resize method */
void MatrixComplex::resize( size_t rows, size_t cols ) {
    
    //    if ( len.size() != 2 )
    //        throw RbException( "Invalid length specification in attempt to resize " + Complex_name + "[][]" );
    //
    //    if ( len[0] < rows || len[1] < cols )
    //        throw RbException( "Invalid attempt to shrink " + Complex_name + "[][]" );
    //
    //    // First add the new rows with the right number of columns
    //    for ( size_t i = rows; i < len[0]; i++ )
    //        matrix.push_back( VectorComplex( len[1] ) );
    //
    //    // Now add columns to the old rows
    //    for ( size_t i = 0; i < rows; i++ )
    //        matrix[i].resize( len[1] );
    //
    //    // Set new length specification
    //    length = len;
    
    throw RbException("Not implemented method MatrixComplex::resize()");
}



/** Complete info about object */
std::string MatrixComplex::richInfo(void) const {

    // TODO: Replace with something that makes sense
    std::ostringstream o;
    printValue(o);

    return o.str();
}


/** Set matrix value from an STL vector<vector> of complex values */
void MatrixComplex::setValue( const std::vector<std::vector<std::complex<double> > >& x ) {

    if ( x.size() != size() )
        throw RbException( "Wrong number of rows in setting value of " + Real_name + "[][]" );
    
    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i].size() != getNumberOfColumns() )
            throw RbException( "Wrong number of columns in at least one row in setting value of " + Real_name + "[][]" );
    }
    
    elements.clear();
    for ( size_t i = 0; i < size(); i++ )
        elements.push_back( RbPtr<RbLanguageObject>( new VectorComplex( x[i] ) ) );
}


/** Overloaded container setElement method */
void MatrixComplex::setElement( size_t row, size_t col, RbPtr<RbLanguageObject> value ) {
    
    if ( row >= getNumberOfRows() || col >= getNumberOfColumns() )
        throw RbException( "Index out of bounds for " + Real_name + "[][]" );
    
    if ( value == NULL )
        throw RbException( "Cannot set " + Real_name + "[][] element to NULL" );
    
    // We rely on the setElement of VectorReal for type cast and to throw an error with a meaningful message
    operator[](row)->setElement(col,value);
    
}


/** Transpose the matrix */
void MatrixComplex::transpose( void ) {

}


////////////////////////////////// Global Matrix operators ///////////////////////////////////


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
MatrixComplex operator+(const MatrixComplex& A, const double& b) {

	MatrixComplex B;
    B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			(*B[i])[j] = (*A[i])[j] - b;
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
MatrixComplex operator-(const MatrixComplex& A, const double& b) {

	MatrixComplex B;
    B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			(*B[i])[j] = (*A[i])[j] - b;
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
MatrixComplex operator*(const MatrixComplex& A, const double& b) {

	MatrixComplex B;
    B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			(*B[i])[j] = (*A[i])[j] * b;
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
MatrixComplex operator/(const MatrixComplex& A, const double& b) {

	MatrixComplex B;
    B = A;
	for (size_t i=0; i<B.getNumberOfRows(); i++)
		for (size_t j=0; j<B.getNumberOfColumns(); j++)
			(*B[i])[j] = (*A[i])[j] / b;
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
MatrixComplex operator+(const double& a, const MatrixComplex& B) {

	MatrixComplex A;
    A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			(*A[i])[j] = a + (*B[i])[j];
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
MatrixComplex operator-(const double& a, const MatrixComplex& B) {

	MatrixComplex A;
    A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			(*A[i])[j] = a - (*B[i])[j];
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
MatrixComplex operator*(const double& a, const MatrixComplex& B) {

	MatrixComplex A;
    A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			(*A[i])[j] = a * (*B[i])[j];
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
MatrixComplex operator/(const double& a, const MatrixComplex& B) {

	MatrixComplex A;
    A = B;
	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			(*A[i])[j] = a / (*B[i])[j];
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
MatrixComplex operator/(const MatrixComplex& A, const MatrixComplex& B) {

    if ( A.getNumberOfRows() != A.getNumberOfColumns() )
        throw RbException("Cannot divide non-square matrices");
	if ( A.getNumberOfColumns() != B.getNumberOfColumns() )
        throw RbException("Cannot divide matrices of differing dimension");
        
	size_t N = A.getNumberOfColumns();
	MatrixComplex invB(N, N, 0.0);
    RbMath::matrixInverse(B, invB);

	MatrixComplex C(N, N, 0.0);
	for (size_t i=0; i<N; i++) 
        {
		for (size_t j=0; j<N; j++) 
            {
			std::complex<double> sum = std::complex<double>(0.0,0.0);
			for (size_t k=0; k<N; k++)
				sum += (*A[i])[k] * (*B[k])[j];
			(*C[i])[j] = sum;
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
MatrixComplex &operator+=(MatrixComplex& A, const double& b) {

	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			(*A[i])[j] += b;
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
MatrixComplex &operator-=(MatrixComplex& A, const double& b) {

	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			(*A[i])[j] -= b;
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
MatrixComplex &operator*=(MatrixComplex& A, const double& b) {

	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			(*A[i])[j] *= b;
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
MatrixComplex &operator/=(MatrixComplex& A, const double& b) {

	for (size_t i=0; i<A.getNumberOfRows(); i++)
		for (size_t j=0; j<A.getNumberOfColumns(); j++)
			(*A[i])[j] /= b;
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
MatrixComplex operator+(const MatrixComplex& A, const MatrixComplex& B) {

	size_t m = A.getNumberOfRows();
	size_t n = A.getNumberOfColumns();
	if (B.getNumberOfRows() != m || B.getNumberOfColumns() != n)
        throw RbException("Cannot add matrices A and B: the matrices are not of the same dimension");
	else 
        {
		MatrixComplex C(m, n, 0.0);
		for (size_t i=0; i<m; i++) 
            {
			for (size_t j=0; j<n; j++)
				(*C[i])[j] = (*A[i])[j] + (*B[i])[j];
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
MatrixComplex operator-(const MatrixComplex& A, const MatrixComplex& B) {

	size_t m = A.getNumberOfRows();
	size_t n = A.getNumberOfColumns();
	if (B.getNumberOfRows() != m || B.getNumberOfColumns() != n)
        throw RbException("Cannot subtract matrices A and B: the matrices are not of the same dimension");
	else 
        {
		MatrixComplex C(m, n, 0.0);
		for (size_t i=0; i<m; i++) 
            {
			for (size_t j=0; j<n; j++)
				(*C[i])[j] = (*A[i])[j] - (*B[i])[j];
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
MatrixComplex operator*(const MatrixComplex& A, const MatrixComplex& B) {

	if ( A.getNumberOfColumns() != B.getNumberOfRows() )
        throw RbException("Cannot multiply matrices A and B: the number of columns of A does not equal the number of rows in B");
	size_t M = A.getNumberOfRows();
	size_t N = A.getNumberOfColumns();
	size_t K = B.getNumberOfColumns();
	MatrixComplex C(M, K, 0.0);
	for (size_t i=0; i<M; i++) 
        {
		for (size_t j=0; j<K; j++) 
            {
			std::complex<double> sum = std::complex<double>(0.0,0.0);
			for (size_t k=0; k<N; k++)
				sum += (*A[i])[k] * (*B[k])[j];
			(*C[i])[j] = sum;
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
MatrixComplex&  operator+=(MatrixComplex& A, const MatrixComplex& B) {

	size_t m = A.getNumberOfRows();
	size_t n = A.getNumberOfColumns();
	if (B.getNumberOfRows() == m && B.getNumberOfColumns() == n) 
        {
		for (size_t i=0; i<m; i++) 
            {
			for (size_t j=0; j<n; j++)
				(*A[i])[j] += (*B[i])[j];
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
MatrixComplex&  operator-=(MatrixComplex& A, const MatrixComplex& B) {

	size_t m = A.getNumberOfRows();
	size_t n = A.getNumberOfColumns();
	if (B.getNumberOfRows() == m && B.getNumberOfColumns() == n) 
        {
		for (size_t i=0; i<m; i++) 
            {
			for (size_t j=0; j<n; j++)
				(*A[i])[j] -= (*B[i])[j];
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
MatrixComplex &operator*=(MatrixComplex& A, const MatrixComplex& B) {

	if ( A.getNumberOfRows() == A.getNumberOfColumns() && B.getNumberOfRows() == B.getNumberOfColumns() && A.getNumberOfRows() == B.getNumberOfRows() ) 
        {
		size_t N = A.getNumberOfRows();
		MatrixComplex C(N, N, 0.0);
		for (size_t i=0; i<N; i++) 
            {
			for (size_t j=0; j<N; j++) 
                {
                std::complex<double> sum = std::complex<double>(0.0,0.0);
				for (size_t k=0; k<N; k++)
					sum += (*A[i])[k] * (*B[k])[j];
				(*C[i])[j] = sum;
                }
            }
		A = C;
        }
	return A;
}

