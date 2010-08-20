/**
 * @file
 * This file contains the declaration of MatrixReal, a complex type
 * used to hold double matrices.
 *
 * @brief Declaration of MatrixReal
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

#ifndef MatrixReal_H
#define MatrixReal_H

#include "RbComplex.h"
#include "VectorInteger.h"

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Real matrix (vector of vectors)
 *
 * This real matrix class is implemented as a vector of vectors.
 *
 */
class MatrixReal : public RbComplex {

    public:
                                                   MatrixReal(void) : RbComplex(), nRows(0), nCols(0) {}                       //!< Default constructor (empty matrix)
                                                   MatrixReal(const size_t nRows, const size_t nCols, const double x = 0.0);   //!< Construct matrix containing double x
                                                   MatrixReal(const std::vector<std::vector<double> >& x);                     //!< Construct matrix from a two-dimensional set of STL vectors

        // Basic utility functions
        virtual MatrixReal*                        clone(void) const;                                                          //!< Clone object
        virtual bool                               equals(const RbObject* obj) const;                                          //!< Equals comparison
        virtual const VectorString&                getClass(void) const;                                                       //!< Get class vector
        void                                       printValue(std::ostream& o) const;                                          //!< Print value for user
        virtual std::string                        toString(void) const;                                                       //!< Complete info about object

        // Element access functions for parser
	    int                                        getDim(void) const { return 1; }                                            //!< Get subscript dimensions
        virtual const std::string&                 getElementType(void) const;                                                 //!< Get element type
        const RbObject*                            getElement(const VectorInteger& index) const;                               //!< Get element (read-only)
        const VectorInteger&                       getLength(void) const;                                                      //!< Get length in each dim
        virtual void                               resize(const VectorInteger& len);                                           //!< Resize
        virtual void                               setElement(const VectorInteger& index, RbObject* val);                      //!< Set element
        void                                       setLength(const VectorInteger& len);                                        //!< Set length in each dim

        // Overloaded operators and built-in functions
        std::vector<double>&                       operator[](size_t i) { return value[i]; }                                   //!< Index op allowing change
        const std::vector<double>&                 operator[](size_t i) const { return value[i]; }                             //!< Const index op
        
        // Regular functions
        int                                        getNumRows(void) const { return nRows; }                                    //!< Get the number of rows in the matrix
        int                                        getNumCols(void) const { return nCols; }                                    //!< Get the number of columns in the matrix
        const std::vector<std::vector<double> >&   getValue(void) const { return value; }                                      //!< Get value directly
        void                                       matrixMult(const MatrixReal& a, const MatrixReal& b);
        virtual void                               setValue(const std::vector<std::vector<double> >& x);                       //!< Set value directly

	protected:
        virtual RbObject*                          convertTo(const std::string& type) const;                                   //!< Convert to type
        virtual bool                               isConvertibleTo(const std::string& type) const;                             //!< Is convertible to type and dim?

    private:
        // helper functions for this class
        bool                                       numFmt(int& numToLft, int& numToRht, std::string s) const;                  //!< Calculates the number of digits to the left and right of the decimal
        std::vector<std::vector<double> >          value;                                                                      //!< Vector of vector of values
        size_t                                     nRows;                                                                      //!< Number of rows
        size_t                                     nCols;                                                                      //!< Number of columns
};

        // operators defined outside of the class
        MatrixReal                                 operator+(const MatrixReal &A, const MatrixReal &B);                        //!< operator + 
        MatrixReal                                 operator-(const MatrixReal &A, const MatrixReal &B);                        //!< operator - 
        MatrixReal                                 operator*(const MatrixReal &A, const MatrixReal &B);                        //!< operator * (matrix multiplication) 
        MatrixReal&                                operator+=(const MatrixReal &A, const MatrixReal &B);                       //!< operator += 
        MatrixReal&                                operator-=(const MatrixReal &A, const MatrixReal &B);                       //!< operator -= 
        MatrixReal&                                operator*=(const MatrixReal &A, const MatrixReal &B);                       //!< operator *= (matrix multiplication)
        MatrixReal                                 operator+(double &a, const MatrixReal &B);                                  //!< operator + for scalar + matrix 
        MatrixReal                                 operator-(double &a, const MatrixReal &B);                                  //!< operator - for scalar - matrix 
        MatrixReal                                 operator*(double &a, const MatrixReal &B);                                  //!< operator * for scalar * matrix 
        MatrixReal                                 operator/(double &a, const MatrixReal &B);                                  //!< operator / for scalar / matrix 
        MatrixReal                                 operator+(const MatrixReal &A, double &b);                                  //!< operator + for matrix + scalar 
        MatrixReal                                 operator-(const MatrixReal &A, double &b);                                  //!< operator - for matrix - scalar 
        MatrixReal                                 operator*(const MatrixReal &A, double &b);                                  //!< operator * for matrix * scalar 
        MatrixReal                                 operator/(const MatrixReal &A, double &b);                                  //!< operator / for matrix / scalar 
        MatrixReal&                                operator+=(const MatrixReal &A, double &b);                                 //!< operator += for scalar 
        MatrixReal&                                operator-=(const MatrixReal &A, double &b);                                 //!< operator -= for scalar 
        MatrixReal&                                operator*=(const MatrixReal &A, double &b);                                 //!< operator *= for scalar 
        MatrixReal&                                operator/=(MatrixReal& A, double& b);                                       //!< operator /= for scalar 

#endif
