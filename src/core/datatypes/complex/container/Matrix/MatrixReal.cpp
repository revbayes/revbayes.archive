/**
 * @file
 * This file contains the implementation of MatrixReal, a complex type
 * used to hold matrices of reals (doubles).
 *
 * @brief Implementation of MatrixReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */


#include "ConstantNode.h"
#include "ContainerNode.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "RbNames.h"
#include "Real.h"
#include "Simplex.h"
#include "UserInterface.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorString.h"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>


/** Default constructor resulting in an empty real matrix */
MatrixReal::MatrixReal( void ) : Matrix( Real_name ) {
}


/** Construct matrix of specified dimensions (length), initialize it with double x (default 0.0) */
MatrixReal::MatrixReal( const size_t nRows, const size_t nCols, double x) : Matrix( Real_name ) {

    if ( nRows < 1 || nCols < 1 )
        throw RbException( "Nonpositive length(s) for " + Real_name + "[][]" );

    length[0] = nRows;
    length[1] = nCols;

    for ( size_t i = 0; i < length[0]; i++ )
        matrix.push_back( VectorReal( length[1], x ) );
}


/** Construct matrix from a two-dimensional set of STL vectors */
MatrixReal::MatrixReal( const std::vector<std::vector<double> >& x ) : Matrix( Real_name ) {

    size_t numRows = x.size();
    size_t numCols = x[0].size();
    for ( size_t i = 1; i < x.size(); i++ ) {
        if ( x[i].size() != numCols )
            throw RbException( "Invalid attempt to initialize a matrix container using a jagged matrix" );
    }

    length[0] = numRows;
    length[1] = numCols;

    for ( size_t i = 0; i < length[0]; i++ )
        matrix.push_back( VectorReal( x[i] ) );
}


/** Construct matrix from a length specification [ nRows, nCols ] and a single vector of values */
MatrixReal::MatrixReal( const std::vector<size_t>& len, const std::vector<double>& x ) : Matrix( Real_name ) {

    if ( len[0] < 1 || len[1] < 1 )
        throw RbException( "Nonpositive length(s) for " + Real_name + "[][]" );

    length = len;

    size_t index = 0;
    for ( size_t i = 0; i < len[0]; i++ ) {
        VectorReal y;
        for ( size_t j=0; j < len[1]; j++ ) {
            y.push_back( x[index++] );
        }
        matrix.push_back( y );
    }
}


/** Index operator (const) */
const VectorReal& MatrixReal::operator[]( const size_t i ) const {

    if ( i >= length[0] )
        throw RbException( "Index to " + Real_name + "[][] out of bounds" );

    return matrix[i];
}


/** Index operator */
VectorReal& MatrixReal::operator[]( const size_t i ) {

    if ( i >= length[0] )
        throw RbException( "Index to " + Real_name + "[][] out of bounds" );

    return matrix[i];
}


/** Overloaded container clear function */
void MatrixReal::clear( void ) {

    matrix.clear();
    length[0] = 0;
    length[1] = 0;
}


/** Clone function */
MatrixReal* MatrixReal::clone(void) const {

    return new MatrixReal(*this);
}


/** Get class vector describing type of object */
const VectorString& MatrixReal::getClass(void) const {

    static VectorString rbClass = VectorString(MatrixReal_name) + Matrix::getClass();
    return rbClass;
}


/** Get matrix content as an STL vector of doubles */
std::vector<double> MatrixReal::getContent( void ) const {

    std::vector<double> temp;

    for ( size_t i = 0; i < length[0]; i++ )
        for ( size_t j = 0; j < length[1]; j++ )
            temp.push_back( matrix[i][j] );

    return temp;
}


/** Overloaded container method to get element or subcontainer for parser */
DAGNode* MatrixReal::getElement( const VectorInteger& index ) {

    if ( index.size() > 2 )
        throw RbException( "Too many indices for " + Real_name + "[][]" );

    // Disregard irrelevant negative value(s) at back of index
    size_t numIndices = index.size();
    for ( int i = static_cast<int>( index.size() ) - 1; i >= 0; i-- ) {
        if ( index[i] < 0 )
            numIndices--;
        else
            break;
    }

    if ( numIndices == 0 ) {

        return new ContainerNode( this->clone() );
    }

    else if ( numIndices == 1 ) {

        // Row submatrix, this is easy
        if ( index[0] > static_cast<int>( length[0] ) )
            throw RbException( "Index out of bound for " + Real_name + "[][]" );
        return new ContainerNode( matrix[index[0]].clone() );
    }
        
    else /* if ( numIndices == 2 ) */ {

        if ( index[0] < 0 ) {
            
            // We want a column submatrix, which is a little tricky
            if ( size_t( index[0] ) > length[0] )
                throw RbException( "Index out of bound for " + Real_name + "[][]" );
            
            VectorReal* temp = new VectorReal();
            for ( size_t j = 0; j < length[1]; j++ )
                temp->push_back( matrix[index[0]][j] );
            return new ContainerNode( temp );
        }
        else {
            
            // We want an element, this is easy
            if ( size_t( index[0] ) > length[0] || size_t( index[1] ) > length[1] )
                throw RbException( "Index out of bound for " + Real_name + "[][]" );
            
            return ( new Real(matrix[index[0]][index[1]]) )->wrapIntoVariable();
        }
    }
}


/** Get matrix value as an STL vector<vector> of doubles */
std::vector<std::vector<double> > MatrixReal::getValue( void ) const {

    std::vector<std::vector<double> > temp;

    for ( size_t i = 0; i < length[0]; i++ )
        temp.push_back( matrix[i].getValue() );

    return temp;
}


/** Calculates the number of digits to the left and right of the decimal point */
bool MatrixReal::numFmt(int& numToLft, int& numToRht, std::string s) const {

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
void MatrixReal::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    // find the maximum number of digits to the left and right of the decimal place
    int maxToLft = 0, maxToRht = 0;
    bool foundDecimalPoint = false;
    for (size_t i=0; i<length[0]; i++)
        {
        for (size_t j=0; j<length[1]; j++)
            {
            std::ostringstream v;
            v << matrix[i][j];
            int numToLft, numToRht;
            if (numFmt(numToLft, numToRht, v.str()) == true)
                foundDecimalPoint = true;
            if (numToLft > maxToLft)
                maxToLft = numToLft;
            if (numToRht > maxToRht)
                maxToRht = numToRht;
            }
        }

    // print the matrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i<length[0]; i++)
        {
        std::string lineStr = "";
        if (i == 0)
            lineStr += "[[ ";
        else 
            lineStr += pad  + " [ ";
        for (size_t j=0; j<length[1]; j++)
            {
            std::ostringstream v;
            v << matrix[i][j];
            int numToLft, numToRht;
            numFmt(numToLft, numToRht, v.str());
            for (int k=0; k<maxToLft-numToLft; k++)
                lineStr += " ";
            lineStr += v.str();
            if (numToRht == 0 && foundDecimalPoint == true)
                lineStr += ".";
            for (int k=0; k<maxToRht-numToRht; k++)
                lineStr += "0";
            if (j+1 < length[1])
                lineStr += ", ";
            }
        if (i == length[0]-1)
            lineStr += " ]]";
        else 
            lineStr += " ],\n";

        o << lineStr;
        //RBOUT(lineStr);
        }
    
    o.setf(previousFlags);
    o << o.precision(previousPrecision);
}


/** Overloaded container resize method */
void MatrixReal::resize( const std::vector<size_t>& len ) {

    if ( len.size() != 2 )
        throw RbException( "Invalid length specification in attempt to resize " + Real_name + "[][]" );

    if ( len[0] < length[0] || len[1] < length[1] )
        throw RbException( "Invalid attempt to shrink " + Real_name + "[][]" );

    // First add the new rows with the right number of columns
    for ( size_t i = length[0]; i < len[0]; i++ )
        matrix.push_back( VectorReal( len[1] ) );

    // Now add columns to the old rows
    for ( size_t i = 0; i < length[0]; i++ )
        matrix[i].resize( len[1] );

    // Set new length specification
    length = len;
}


/** Complete info about object */
std::string MatrixReal::richInfo(void) const {

    // TODO: Replace with something that makes sense
    std::ostringstream o;
    o <<  "MatrixReal: value = " ;
    printValue(o);

    return o.str();
}


/** Set matrix content from single STL vector of doubles */
void MatrixReal::setContent( const std::vector<double>& x ) {
    
    if ( x.size() != length[0]*length[1] )
        throw RbException( "Incorrect number of elements in setting " + Real_name + "[][]" );
    
    matrix.clear();
    
    size_t index = 0;
    for ( size_t i = 0; i < length[0]; i++ ) {
        VectorReal y;
        for ( size_t j = 0; j < length[1]; j++ ) {
            y.push_back( x[index++] );
        }
        matrix.push_back( y );
    }
}


/** Set matrix value from an STL vector<vector> of doubles */
void MatrixReal::setValue( const std::vector<std::vector<double> >& x ) {

    if ( x.size() != length[0] )
        throw RbException( "Wrong number of rows in setting value of " + Real_name + "[][]" );

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i].size() != length[1] )
            throw RbException( "Wrong number of columns in at least one row in setting value of " + Real_name + "[][]" );
    }

    matrix.clear();
    for ( size_t i = 0; i < length[0]; i++ )
        matrix.push_back( VectorReal( x[i] ) );
}


/** Overloaded container setElement method */
void MatrixReal::setElement( const VectorNatural& index, DAGNode* var ) {

    if ( index.size() != 2 )
        throw RbException ( "Wrong number of indices for " + Real_name + "[][]" );

    if ( index[0] >= length[0] || index[1] >= length[1] )
        throw RbException( "Index out of bounds for " + Real_name + "[][]" );

    RbObject* value = var->getValue()->clone();
    if ( value == NULL )
        throw RbException( "Cannot set " + Real_name + "[][] element to NULL" );

    if ( value->isType( Real_name ) )
        matrix[index[0]][index[1]] = static_cast<Real*>( value )->getValue();
    else {
        // We rely on convertTo to throw an error with a meaningful message
        matrix[index[0]][index[1]] = static_cast<Real*>( value->convertTo( Real_name ) )->getValue();
    }
}


/** Overloaded container setLength method */
void MatrixReal::setLength( const std::vector<size_t>& len) {

    if ( len[0] * len[1] != size() )
        throw RbException( "New length for " + Real_name + "[][] does not have the right number of elements" );

    std::vector<double> temp = getContent();
    setContent( temp );
}


/** Overloaded container size method */
size_t MatrixReal::size( void ) const {

    return length[0] * length[1];
}


/** Transpose the matrix */
void MatrixReal::transpose( void ) {

    MatrixReal temp(length[1], length[0]);
    for ( size_t i = 0; i < length[0]; i++ )
        for ( size_t j = 0; j < length[1]; j++ )
            temp[j][i] = matrix[i][j];
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
MatrixReal operator+(const MatrixReal& A, const double& b) {

	MatrixReal B;
    B = A;
	for (size_t i=0; i<B.getNumRows(); i++)
		for (size_t j=0; j<B.getNumCols(); j++)
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
MatrixReal operator-(const MatrixReal& A, const double& b) {

	MatrixReal B;
    B = A;
	for (size_t i=0; i<B.getNumRows(); i++)
		for (size_t j=0; j<B.getNumCols(); j++)
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
MatrixReal operator*(const MatrixReal& A, const double& b) {

	MatrixReal B;
    B = A;
	for (size_t i=0; i<B.getNumRows(); i++)
		for (size_t j=0; j<B.getNumCols(); j++)
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
MatrixReal operator/(const MatrixReal& A, const double& b) {

	MatrixReal B;
    B = A;
	for (size_t i=0; i<B.getNumRows(); i++)
		for (size_t j=0; j<B.getNumCols(); j++)
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
MatrixReal operator+(const double& a, const MatrixReal& B) {

	MatrixReal A;
    A = B;
	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
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
MatrixReal operator-(const double& a, const MatrixReal& B) {

	MatrixReal A;
    A = B;
	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
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
MatrixReal operator*(const double& a, const MatrixReal& B) {

	MatrixReal A;
    A = B;
	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
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
MatrixReal operator/(const double& a, const MatrixReal& B) {

	MatrixReal A;
    A = B;
	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
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
MatrixReal operator/(const MatrixReal& A, const MatrixReal& B) {

    if ( A.getNumRows() != A.getNumCols() )
        throw RbException("Cannot divide non-square matrices");
	if ( A.getNumCols() != B.getNumCols() )
        throw RbException("Cannot divide matrices of differing dimension");
        
	size_t N = A.getNumCols();
	MatrixReal invB(N, N, 0.0);
    RbMath::matrixInverse(B, invB);

	MatrixReal C(N, N, 0.0);
	for (size_t i=0; i<N; i++) 
        {
		for (size_t j=0; j<N; j++) 
            {
			double sum = 0.0;
			for (size_t k=0; k<N; k++)
				sum += A[i][k] * B [k][j];
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
MatrixReal &operator+=(MatrixReal& A, const double& b) {

	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
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
MatrixReal &operator-=(MatrixReal& A, const double& b) {

	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
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
MatrixReal &operator*=(MatrixReal& A, const double& b) {

	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
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
MatrixReal &operator/=(MatrixReal& A, const double& b) {

	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
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
MatrixReal operator+(const MatrixReal& A, const MatrixReal& B) {

	size_t m = A.getNumRows();
	size_t n = A.getNumCols();
	if (B.getNumRows() != m || B.getNumCols() != n)
        throw RbException("Cannot add matrices A and B: the matrices are not of the same dimension");
	else 
        {
		MatrixReal C(m, n, 0.0);
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
MatrixReal operator-(const MatrixReal& A, const MatrixReal& B) {

	size_t m = A.getNumRows();
	size_t n = A.getNumCols();
	if (B.getNumRows() != m || B.getNumCols() != n)
        throw RbException("Cannot subtract matrices A and B: the matrices are not of the same dimension");
	else 
        {
		MatrixReal C(m, n, 0.0);
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
MatrixReal operator*(const MatrixReal& A, const MatrixReal& B) {

	if ( A.getNumCols() != B.getNumRows() )
        throw RbException("Cannot multiply matrices A and B: the number of columns of A does not equal the number of rows in B");
	size_t M = A.getNumRows();
	size_t N = A.getNumCols();
	size_t K = B.getNumCols();
	MatrixReal C(M, K, 0.0);
	for (size_t i=0; i<M; i++) {
		for (size_t j=0; j<K; j++) {
			double sum = 0.0;
			for (size_t k=0; k<N; k++)
				sum += A[i][k] * B [k][j];
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
MatrixReal&  operator+=(MatrixReal& A, const MatrixReal& B) {

	size_t m = A.getNumRows();
	size_t n = A.getNumCols();
	if (B.getNumRows() == m && B.getNumCols() == n) 
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
MatrixReal&  operator-=(MatrixReal& A, const MatrixReal& B) {

	size_t m = A.getNumRows();
	size_t n = A.getNumCols();
	if (B.getNumRows() == m && B.getNumCols() == n) 
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
MatrixReal &operator*=(MatrixReal& A, const MatrixReal& B) {

	if ( A.getNumRows() == A.getNumCols() && B.getNumRows() == B.getNumCols() && A.getNumRows() == B.getNumRows() ) 
        {
		size_t N = A.getNumRows();
		MatrixReal C(N, N, 0.0);
		for (size_t i=0; i<N; i++) 
            {
			for (size_t j=0; j<N; j++) 
                {
				double sum = 0.0;
				for (size_t k=0; k<N; k++)
					sum += A[i][k] * B [k][j];
				C[i][j] = sum;
                }
            }
		A = C;
        }
	return A;
}

