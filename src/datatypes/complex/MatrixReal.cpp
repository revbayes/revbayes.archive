/**
 * @file
 * This file contains the implementation of MatrixReal, a complex type
 * used to hold matrices of reals (doubles).
 *
 * @brief Implementation of MatrixReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "MatrixReal.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbNames.h"
#include "Real.h"
#include "Simplex.h"
#include "UserInterface.h"
#include "VectorInteger.h"
#include "VectorReal.h"
#include "VectorString.h"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>


/** Construct matrix of specified dimensions, initialize it with double x (default 0.0) */
MatrixReal::MatrixReal(const size_t nRows, const size_t nCols, const double x) : RbComplex() {

    if (nRows < 0 || nCols < 0)
        throw RbException("Negative size of matrix");

    value.resize(nRows);
    for (size_t i=0; i<nRows; i++)
        value[i].resize(nCols);

    for (size_t i=0; i<nRows; i++)
        for (size_t j=0; j<nCols; j++)
            value[i][j] = x;
}


/** Construct matrix from a two-dimensional set of STL vectors */
MatrixReal::MatrixReal(const std::vector<std::vector<double> >& x) {

    value = x;
    nRows = value.size();
    nCols = value[0].size();
    for (size_t i=0; i<value.size(); i++)
        {
        if (value[i].size() != nCols)
            throw RbException("Attempted to initialize a matrix using a jagged matrix");
        }
}


/*!
 * This function performs addition of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * \brief operator+ (scalar)
 * \param A Matrix
 * \param b Scalar
 * \return A + b
 */
MatrixReal operator+(const MatrixReal& A, const double& b) {

	MatrixReal B;
    B = A;
	for (int i=0; i<B.getNumRows(); i++)
		for (int j=0; j<B.getNumCols(); j++)
			B[i][j] = A[i][j] + b;
	return B;
}

/*!
 * This function performs subtraction of a scalar from
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * \brief operator- (scalar)
 * \param A Matrix
 * \param b Scalar
 * \return A - b
 */
MatrixReal operator-(const MatrixReal& A, const double& b) {

	MatrixReal B;
    B = A;
	for (int i=0; i<B.getNumRows(); i++)
		for (int j=0; j<B.getNumCols(); j++)
			B[i][j] = A[i][j] - b;
	return B;
}

/*!
 * This function performs multiplication of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * \brief operator* (scalar)
 * \param A Matrix
 * \param b Scalar
 * \return A * b
 */
MatrixReal operator*(const MatrixReal& A, const double& b) {

	MatrixReal B;
    B = A;
	for (int i=0; i<B.getNumRows(); i++)
		for (int j=0; j<B.getNumCols(); j++)
			B[i][j] = A[i][j] * b;
	return B;
}

/*!
 * This function performs division with a scalar of
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * \brief operator/ (scalar)
 * \param A Matrix
 * \param b Scalar
 * \return A / b
 */
MatrixReal operator/(const MatrixReal& A, const double& b) {

	MatrixReal B;
    B = A;
	for (int i=0; i<B.getNumRows(); i++)
		for (int j=0; j<B.getNumCols(); j++)
			B[i][j] = A[i][j] / b;
	return B;
}

/*!
 * This function performs addition of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * \brief operator+ (scalar first)
 * \param a Scalar
 * \param B Matrix
 * \return a + B
 */
MatrixReal operator+(const double& a, const MatrixReal& B) {

	MatrixReal A;
    A = B;
	for (int i=0; i<A.getNumRows(); i++)
		for (int j=0; j<A.getNumCols(); j++)
			A[i][j] = a + B[i][j];
	return A;
}

/*!
 * This function subtracts each element of a
 * a matrix from a scalar and returns the
 * resulting matrix.
 *
 * \brief operator- (scalar first)
 * \param a Scalar
 * \param B Matrix
 * \return a - B
 */
MatrixReal operator-(const double& a, const MatrixReal& B) {

	MatrixReal A;
    A = B;
	for (int i=0; i<A.getNumRows(); i++)
		for (int j=0; j<A.getNumCols(); j++)
			A[i][j] = a - B[i][j];
	return A;
}

/*!
 * This function performs multiplication of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * \brief operator* (scalar first)
 * \param a Scalar
 * \param B Matrix
 * \return a * B
 */
MatrixReal operator*(const double& a, const MatrixReal& B) {

	MatrixReal A;
    A = B;
	for (int i=0; i<A.getNumRows(); i++)
		for (int j=0; j<A.getNumCols(); j++)
			A[i][j] = a * B[i][j];
	return A;
}

/*!
 * This function performs division of a scalar by
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * \brief operator/ (scalar first)
 * \param a Scalar
 * \param B Matrix
 * \return a / B
 */
MatrixReal operator/(const double& a, const MatrixReal& B) {

	MatrixReal A;
    A = B;
	for (int i=0; i<A.getNumRows(); i++)
		for (int j=0; j<A.getNumCols(); j++)
			A[i][j] = a / B[i][j];
	return A;
}

/*!
 * This function performs addition of a scalar to
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * \brief operator+= (scalar)
 * \param A Matrix
 * \param b Scalar
 * \return A += b
 */
MatrixReal &operator+=(MatrixReal& A, const double& b) {

	for (int i=0; i<A.getNumRows(); i++)
		for (int j=0; j<A.getNumCols(); j++)
			A[i][j] += b;
	return A;
}

/*!
 * This function performs subtraction of a scalar from
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * \brief operator-= (scalar)
 * \param A Matrix
 * \param b Scalar
 * \return A -= b
 */
MatrixReal &operator-=(MatrixReal& A, const double& b) {

	for (int i=0; i<A.getNumRows(); i++)
		for (int j=0; j<A.getNumCols(); j++)
			A[i][j] -= b;
	return A;
}

/*!
 * This function performs multiplication of a scalar to
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * \brief operator*= (scalar)
 * \param A Matrix
 * \param b Scalar
 * \return A *= b
 */
MatrixReal &operator*=(MatrixReal& A, const double& b) {

	for (int i=0; i<A.getNumRows(); i++)
		for (int j=0; j<A.getNumCols(); j++)
			A[i][j] *= b;
	return A;
}

/*!
 * This function performs division with a scalar of
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * \brief operator/= (scalar)
 * \param A Matrix
 * \param b Scalar
 * \return A /= b
 */
MatrixReal &operator/=(MatrixReal& A, const double& b) {

	for (int i=0; i<A.getNumRows(); i++)
		for (int j=0; j<A.getNumCols(); j++)
			A[i][j] /= b;
	return A;
}

/*!
 * This function performs elementwise addition of two
 * matrices and returns the resulting matrix. If the
 * matrices are not conformant, a null matrix is returned.
 *
 * \brief operator+
 * \param A Matrix 1
 * \param B Matrix 2
 * \return A + B, null matrix on failure
 */
MatrixReal operator+(const MatrixReal& A, const MatrixReal& B) {

	int m = A.getNumRows();
	int n = A.getNumCols();
	if (B.getNumRows() != m || B.getNumCols() != n)
        throw RbException("Cannot add matrices A and B: the matrices are not of the same dimension");
	else 
        {
		MatrixReal C(m, n, 0.0);
		for (int i=0; i<m; i++) 
            {
			for (int j=0; j<n; j++)
				C[i][j] = A[i][j] + B[i][j];
            }
		return C;
        }
}

/*!
 * This function performs elementwise subtraction of two
 * matrices and returns the resulting matrix. If the
 * matrices are not conformant, a null matrix is returned.
 *
 * \brief operator-
 * \param A Matrix 1
 * \param B Matrix 2
 * \return A - B, null matrix on failure
 */
MatrixReal operator-(const MatrixReal& A, const MatrixReal& B) {

	int m = A.getNumRows();
	int n = A.getNumCols();
	if (B.getNumRows() != m || B.getNumCols() != n)
        throw RbException("Cannot subtract matrices A and B: the matrices are not of the same dimension");
	else 
        {
		MatrixReal C(m, n, 0.0);
		for (int i=0; i<m; i++) 
            {
			for (int j=0; j<n; j++)
				C[i][j] = A[i][j] - B[i][j];
            }
		return C;
        }
}

/*!
 * Compute C = A*B, where C[i][j] is the dot-product of 
 * row i of A and column j of B. Note that this operator
 * does not perform elementwise multiplication. If the 
 * matrices do not have the right dimensions for matrix
 * multiplication (that is, if the number of columns of A
 * is different from the number of rows of B), the function
 * returns a null matrix.
 *
 * \brief Matrix multiplication
 * \param A An (m X n) matrix
 * \param B An (n X k) matrix
 * \return A * B, an (m X k) matrix, or null matrix on failure
 */
MatrixReal operator*(const MatrixReal& A, const MatrixReal& B) {

	if ( A.getNumCols() != B.getNumRows() )
        throw RbException("Cannot multiply matrices A and B: the number of columns of A does not equal the number of rows in B");
	int M = A.getNumRows();
	int N = A.getNumCols();
	int K = B.getNumCols();
	MatrixReal C(M, K, 0.0);
	for (int i=0; i<M; i++) {
		for (int j=0; j<K; j++) {
			double sum = 0.0;
			for (int k=0; k<N; k++)
				sum += A[i][k] * B [k][j];
			C[i][j] = sum;
		}
	}
	return C;
}

/*!
 * This function performs elementwise addition on two
 * matrices and puts the result in the first matrix.
 * If the two matrices are nonconformant, the first
 * matrix is left intact.
 *
 * \brief operator+=
 * \param A Matrix 1
 * \param B Matrix 2
 * \return A += B, A unmodified on failure
 */
MatrixReal&  operator+=(MatrixReal& A, const MatrixReal& B) {

	int m = A.getNumRows();
	int n = A.getNumCols();
	if (B.getNumRows() == m && B.getNumCols() == n) 
        {
		for (int i=0; i<m; i++) 
            {
			for (int j=0; j<n; j++)
				A[i][j] += B[i][j];
            }
        }
	return A;
}

/*!
 * This function performs elementwise subtraction on two
 * matrices and puts the result in the first matrix.
 * If the two matrices are nonconformant, the first
 * matrix is left intact.
 *
 * \brief operator-=
 * \param A Matrix 1
 * \param B Matrix 2
 * \return A -= B, A unmodified on failure
 */
MatrixReal&  operator-=(MatrixReal& A, const MatrixReal& B) {

	int m = A.getNumRows();
	int n = A.getNumCols();
	if (B.getNumRows() == m && B.getNumCols() == n) 
        {
		for (int i=0; i<m; i++) 
            {
			for (int j=0; j<n; j++)
				A[i][j] -= B[i][j];
            }
        }
	return A;
}

/*!
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
		int N = A.getNumRows();
		MatrixReal C(N, N, 0.0);
		for (int i=0; i<N; i++) 
            {
			for (int j=0; j<N; j++) 
                {
				double sum = 0.0;
				for (int k=0; k<N; k++)
					sum += A[i][k] * B [k][j];
				C[i][j] = sum;
                }
            }
		A = C;
        }
	return A;
}

/** Clone function */
MatrixReal* MatrixReal::clone(void) const {

    return new MatrixReal(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* MatrixReal::convertTo(const std::string& type) const {

    throw RbException("Cannot convert MatrixReal to " + type + ".");
	return NULL;
}


/** Pointer-based equals comparison */
bool MatrixReal::equals(const RbObject* obj) const {

    // Check type first
    if ( !obj->isType( MatrixReal_name ) )
        return false;

    // Now go through all elements
    const MatrixReal& temp = *((MatrixReal*)(obj));

    if ( temp.nRows != nRows || temp.nCols != nCols )
        return false;

    for ( size_t i=0; i<nRows; i++)
        for ( size_t j=0; j<nCols; j++ )
            if ( RbMath::isEqualTo(temp[i][j], value[i][j], 0.0000001) == false )
                return false;
            
    return true;
}


/** Get class vector describing type of object */
const VectorString& MatrixReal::getClass(void) const {

    static VectorString rbClass = VectorString(MatrixReal_name) + RbComplex::getClass();
    return rbClass;
}


/**
 * Get element for parser (read-only). Since MatrixReal is implemented as a vector
 * of vectors, the object returned should be a VectorReal.
 */
const RbObject* MatrixReal::getElement(const VectorInteger& index) const {

    static VectorReal x;
    static Real y;

    // The parser might want to access a row vector or a matrix element
    if ( index.size() == 1 ) {
        if ( index[0] < 0 || index[0] >= (int)nRows )
            throw RbException("Row index out of bounds");
        x = value[index[0]];
        return &x;
    }
    else if ( index.size() == 2 ) {
        if ( index[0] < 0 || index[0] >= (int)nRows )
            throw RbException("Row index out of bounds");
        if ( index[1] < 0 || index[1] >= (int)nCols )
            throw RbException("Column index out of bounds");
        y = value[index[0]][index[1]];
        return &y;
    }
    else
        throw (RbException("Index error in " + MatrixReal_name));

    return NULL;
}


/** Get element type for parser */
const std::string& MatrixReal::getElementType(void) const {

    static std::string rbType = VectorReal_name;
    return rbType;
}


/** Get element length for parser */
const VectorInteger& MatrixReal::getLength(void) const {

    static VectorInteger length = VectorInteger(0);

    length[0] = int(value.size());
    return length;
}


/** Convert to object of another class. The caller manages the object. */
bool MatrixReal::isConvertibleTo(const std::string& type) const {

    return false;
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


/** Allow the parser to resize the matrix */
void MatrixReal::resize(const VectorInteger& len) {

    // Catch errors
    if (len.size() != 1 && len.size() != 2)
        throw (RbException("Length specification error"));
    for (size_t i=0; i<len.size(); i++)
        if (len[i] < 0)
            throw (RbException("Negative length specification"));

    // Record old size
    size_t oldNRows = nRows;
    size_t oldNCols = nCols;

    // Resize vectors
    value.resize(len[0]);
    if ( len.size() == 2 ) {
        for ( size_t i=0; i<nCols; i++ )
            value[i].resize(len[1]);
    }
    
    // Record new size
    nRows = len[0];
    nCols = len[1];

    // Fill new cells with NaN
    for (size_t i=0; i<oldNRows; i++)
        for (size_t j=oldNCols; j<nCols; j++)
            value[i][j] = std::numeric_limits<double>::quiet_NaN();
    for (size_t i=oldNRows; i<nRows; i++)
        for (size_t j=0; j<nCols; j++)
            value[i][j] = std::numeric_limits<double>::quiet_NaN();
}


/** Allow parser to set an element (any type conversion is done by parser) */
void MatrixReal::setElement(const VectorInteger& index, RbObject* val) {

    // Catch errors
    if ( index.size() != 1 && index.size() != 2 )
        throw (RbException("Index error"));
    if ( index.size() == 1 && !val->isType(VectorReal_name) )
        throw (RbException("Type mismatch"));
    if ( index.size() == 2 && !val->isType(Real_name) )
        throw (RbException("Type mismatch"));

    // Resize if necessary
    if ( index[0] >= (int)nRows || (index.size() == 2 && index[1] >= (int)nCols) ) {

        VectorInteger newLen;
        if (index[0] >= (int)nRows)
            newLen.push_back(index[0] + 1);
        else
            newLen.push_back(nRows);

        if (index.size() == 2 && index[1] >= (int)nCols)
            newLen.push_back(index[1] + 1);
        else
            newLen.push_back(nCols);

        resize(newLen);
    }

    // Parser may want to set a row vector or an element
    if ( index.size() == 1 ) {
        VectorReal* vec = dynamic_cast<VectorReal*>(val);
        
        if ( vec->size() != nCols )
            throw RbException("Row vector has wrong number of elements");

        for (size_t j=0; j<nCols; j++)
            value[index[0]][j] = (*vec)[j];

        delete val;
    }
    else if ( index.size() == 2 ) {
        value[index[0]][index[1]] = ((Real*)(val))->getValue();
        delete val;
    }
}


/** Allow parser to rearrange the matrix */
void MatrixReal::setLength(const VectorInteger& len) {

    // If just one dimension, we don\t do anything
    if ( len.size() == 1 && len[0] != nRows )
        throw (RbException("Length specification error"));

    // If two dimensions, we rearrange the matrix
    if ( len.size() == 2 && (len[0] != nRows || len[1] != nCols) ) {

        if ( len[0] * len[1] != nRows * nCols )
            throw RbException("New length specification does not match number of elements");

        // Create temp vector
        std::vector<std::vector<double> >   temp;
        temp.resize(len[0]);
        for (int i=0; i<len[0]; i++)
            temp[i].resize(len[1]);

        // Set the temp vector
        size_t index = 0;
        for (int i=0; i<len[0]; i++) {
            for (int j=0; j<len[1]; j++) {
                temp[i][j] = value[index/nCols][index%nCols];
                ++index;
            }
        }
        // Reset the value
        nRows = len[0];
        nCols = len[1];
        value = temp;
    }
}


void MatrixReal::setValue(const std::vector<std::vector<double> >& x) { 

    value = x; 
    nRows = value.size();
    nCols = value[0].size();
    for (size_t i=0; i<value.size(); i++)
        {
        if (value[i].size() != nCols)
            throw RbException("Attempted to initialize a matrix using a jagged matrix");
        }
}


/** Print value for user */
void MatrixReal::printValue(std::ostream& o) const {

    int previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    // find the maximum number of digits to the left and right of the decimal place
    int maxToLft = 0, maxToRht = 0;
    bool foundDecimalPoint = false;
    for (size_t i=0; i<value.size(); i++)
        {
        for (size_t j=0; j<value[i].size(); j++)
            {
            std::ostringstream v;
            v << value[i][j];
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
    for (size_t i=0; i<value.size(); i++)
        {
        std::string lineStr = "";
        if (i == 0)
            lineStr += "[[ ";
        else 
            lineStr += pad  + " [ ";
        for (size_t j=0; j<value[i].size(); j++)
            {
            std::ostringstream v;
            v << value[i][j];
            int numToLft, numToRht;
            numFmt(numToLft, numToRht, v.str());
            for (int k=0; k<maxToLft-numToLft; k++)
                lineStr += " ";
            lineStr += v.str();
            if (numToRht == 0 && foundDecimalPoint == true)
                lineStr += ".";
            for (int k=0; k<maxToRht-numToRht; k++)
                lineStr += "0";
            if (j+1 < value[i].size())
                lineStr += ", ";
            }
        if (i == value.size()-1)
            lineStr += " ]]";
        else 
            lineStr += " ],\n";

        o << lineStr;
        //RBOUT(lineStr);
        }
    
    o.setf(previousFlags);
    o << std::setprecision(previousPrecision);
}


/** Complete info about object */
std::string MatrixReal::toString(void) const {

    // TODO: Replace with something that makes sense
    std::ostringstream o;
    o <<  "MatrixReal: value = " ;
    printValue(o);

    return o.str();
}

