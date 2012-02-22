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
 * @extends Matrix
 *
 * $Id$
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

    // Overoaded operators
    VectorComplex&                                      operator[](size_t i);                                                                           //!< Subscript operator
    const VectorComplex&                                operator[](size_t i) const;                                                                     //!< Subscript operator (const)

    // Basic utility functions
    MatrixComplex*                                      clone(void) const;                                                                              //!< Clone object
    static const std::string&                           getClassName(void);                                                                             //!< Get class name
    static const TypeSpec&                              getClassTypeSpec(void);                                                                         //!< Get class type spec
    const TypeSpec&                                     getTypeSpec(void) const;                                                                        //!< Get language type of the object
    void                                                printValue(std::ostream& o) const;                                                              //!< Print value for user
        
    // Matrix functions
    const RbObject&                                     getElement(size_t row, size_t col) const;                                                       //!< Get element or subcontainer
    RbObject&                                           getElement(size_t row, size_t col);                                                             //!< Get element or subcontainer (non-const to return non-const element)
    void                                                setElement(size_t row, size_t col, RbLanguageObject* var);                                      //!< set element
    void                                                resize(size_t nRows, size_t nCols);                                                             //!< Resize to new length vector
    void                                                transpose(void);                                                                                //!< Transpose the matrix

    // Matrix functions
    std::vector<std::vector<std::complex<double> > >    getValue(void) const;                                                                           //!< Get value as STL vector<vector> of complex values
//    void                                                push_back(const VectorComplex& x);                                                              //!< Push back a row vector
    void                                                setValue(const std::vector<std::vector<std::complex<double> > >& x);                            //!< Set value using STL vector<vector> of complex values

private:
    bool                                                numFmt(int& numToLft, int& numToRht, std::string s) const;                                      //!< Calculates the number of digits to the left and right of the decimal
        
    
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
