/**
 * @file
 * This file contains the declaration of MatrixComplex, a complex type
 * used to hold double matrices.
 *
 * MatrixComplex is implemented as a container in
 * RevBayes (like matrices in R). This means that
 * we cannot give subscript operator access to
 * the elements. Instead, use the getValue and
 * setValue functions to translate to and from
 * STL vectors of vectors, for fast calculations.
 * If necessary, one could also add C-style pointer
 * arrays to the interface.
 *
 * @brief Declaration of MatrixComplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 * @extends RbComplex
 *
 * $Id:$
 */

#ifndef MatrixComplex_H
#define MatrixComplex_H

#include "Matrix.h"
#include "VectorComplex.h"
#include <complex>
#include <iostream>
#include <string>
#include <vector>

class VectorNatural;



class MatrixComplex : public Matrix {

    public:
                                                           MatrixComplex(void);                                                                           //!< Default constructor (empty matrix)
                                                           MatrixComplex(const size_t nRows, const size_t nCols, std::complex<double> x = std::complex<double>(0.0,0.0));                 //!< Construct matrix containing complex x
                                                           MatrixComplex(const std::vector<std::vector<std::complex<double> > >& x);                      //!< Construct matrix from a two-dimensional set of STL vectors
                                                           MatrixComplex(const std::vector<size_t>& length, const std::vector<std::complex<double> >& x); //!< Construct matrix from length specification and vector of content

        // Overoaded operators
        VectorComplex&                                     operator[](size_t i);                                                                          //!< Subscript operator
        const VectorComplex&                               operator[](size_t i) const;                                                                    //!< Subscript operator (const)

        // Basic utility functions
        MatrixComplex*                                     clone(void) const;                                                                             //!< Clone object
        const VectorString&                                getClass(void) const;                                                                          //!< Get class vector
        void                                               printValue(std::ostream& o) const;                                                             //!< Print value for user
        std::string                                        richInfo(void) const;                                                                          //!< Complete info about object

        // Container functions
        void                                               clear(void);                                                                                   //!< Clear
        void                                               resize(const std::vector<size_t>& len);                                                        //!< Resize to new length vector
        void                                               setLength(const std::vector<size_t>& len);                                                     //!< Reorganize container
        size_t                                             size(void) const;                                                                              //!< Get total number of elements

        // Matrix functions
        std::vector<std::complex<double> >                 getContent(void) const;                                                                        //!< Get content (all elements) in an STL vector
        std::vector<std::vector<std::complex<double> > >   getValue(void) const;                                                                          //!< Get value as STL vector<vector> of complex values
        void                                               setContent(const std::vector<std::complex<double> >& x);                                       //!< Set content using STL vector of complex values
        void                                               setValue(const std::vector<std::vector<std::complex<double> > >& x);                           //!< Set value using STL vector<vector> of complex values
        void                                               transpose(void);                                                                               //!< Transpose the matrix

    private:
        RbObject*                                          getDefaultElement(void) { return new Complex(); }                                              //!< Get default element for empty slots
        bool                                               numFmt(int& numToLft, int& numToRht, std::string s) const;                                     //!< Calculates the number of digits to the left and right of the decimal
        
        // Parser help functions
        DAGNode*                                           getElement(const VectorInteger& index);                                                        //!< Get element or subcontainer for parser
        void                                               setElement(const VectorNatural& index, DAGNode* var);                                          //!< Allow parser to set element

        std::vector<VectorComplex>                         matrix;                                                                                        //!< We use vector of vectors instead of container internally
};

        // operators defined outside of the class
        MatrixComplex                                      operator+(const MatrixComplex& A, const MatrixComplex& B);                                     //!< operator + 
        MatrixComplex                                      operator-(const MatrixComplex& A, const MatrixComplex& B);                                     //!< operator - 
        MatrixComplex                                      operator*(const MatrixComplex& A, const MatrixComplex& B);                                     //!< operator * (matrix multiplication) 
        MatrixComplex&                                     operator+=(MatrixComplex& A, const MatrixComplex& B);                                          //!< operator += 
        MatrixComplex&                                     operator-=(MatrixComplex& A, const MatrixComplex& B);                                          //!< operator -= 
        MatrixComplex&                                     operator*=(MatrixComplex& A, const MatrixComplex& B);                                          //!< operator *= (matrix multiplication)
        MatrixComplex                                      operator+(const double &a, const MatrixComplex& B);                                            //!< operator + for scalar + matrix 
        MatrixComplex                                      operator-(const double &a, const MatrixComplex& B);                                            //!< operator - for scalar - matrix 
        MatrixComplex                                      operator*(const double &a, const MatrixComplex& B);                                            //!< operator * for scalar * matrix 
        MatrixComplex                                      operator/(const double &a, const MatrixComplex& B);                                            //!< operator / for scalar / matrix 
        MatrixComplex                                      operator+(const MatrixComplex& A, const double& b);                                            //!< operator + for matrix + scalar 
        MatrixComplex                                      operator-(const MatrixComplex& A, const double& b);                                            //!< operator - for matrix - scalar 
        MatrixComplex                                      operator*(const MatrixComplex& A, const double& b);                                            //!< operator * for matrix * scalar 
        MatrixComplex                                      operator/(const MatrixComplex& A, const double& b);                                            //!< operator / for matrix / scalar 
        MatrixComplex                                      operator/(const MatrixComplex& A, const MatrixComplex& B);                                     //!< operator / for matrix / matrix 
        MatrixComplex&                                     operator+=(MatrixComplex& A, const double& b);                                                 //!< operator += for scalar 
        MatrixComplex&                                     operator-=(MatrixComplex& A, const double& b);                                                 //!< operator -= for scalar 
        MatrixComplex&                                     operator*=(MatrixComplex& A, const double& b);                                                 //!< operator *= for scalar 
        MatrixComplex&                                     operator/=(MatrixComplex& A, const double& b);                                                 //!< operator /= for scalar 

#endif
