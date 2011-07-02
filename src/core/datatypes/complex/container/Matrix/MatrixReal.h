/**
 * @file
 * This file contains the declaration of MatrixReal, a complex type
 * used to hold double matrices.
 *
 * @brief Declaration of MatrixReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef MatrixReal_H
#define MatrixReal_H

#include "Matrix.h"
#include "VectorReal.h"
#include <iostream>
#include <string>
#include <vector>

class VectorNatural;


/**
 * @brief Real matrix
 *
 * MatrixReal is implemented as a container in
 * RevBayes (like matrices in R). This means that
 * we cannot give subscript operator access to
 * the elements. Instead, use the getValue and
 * setValue functions to translate to and from
 * STL vectors of vectors, for fast calculations.
 * If necessary, one could also add C-style pointer
 * arrays to the interface.
 */
class MatrixReal : public Matrix {

    public:
                                            MatrixReal(void);                                                           //!< Default constructor (empty matrix)
                                            MatrixReal(const size_t nRows, const size_t nCols, double x = 0.0);         //!< Construct matrix containing double x
                                            MatrixReal(const std::vector<std::vector<double> >& x);                     //!< Construct matrix from a two-dimensional set of STL vectors
                                            MatrixReal(const std::vector<size_t>& length, const std::vector<double>& x);//!< Construct matrix from length specification and vector of content

        // Overoaded operators
        VectorReal&                         operator[](size_t i);                                                       //!< Subscript operator
        const VectorReal&                   operator[](size_t i) const;                                                 //!< Subscript operator (const)

        // Basic utility functions
        MatrixReal*                         clone(void) const;                                                          //!< Clone object
        const VectorString&                 getClass(void) const;                                                       //!< Get class vector
        void                                printValue(std::ostream& o) const;                                          //!< Print value for user
        std::string                         richInfo(void) const;                                                       //!< Complete info about object

        // Container functions
        void                                clear(void);                                                                //!< Clear
        void                                resize(const std::vector<size_t>& len);                                     //!< Resize to new length vector
        void                                setLength(const std::vector<size_t>& len);                                  //!< Reorganize container
        size_t                              size(void) const;                                                           //!< Get total number of elements

        // Matrix functions
        std::vector<double>                 getContent(void) const;                                                     //!< Get content (all elements) in an STL vector
        std::vector<std::vector<double> >   getValue(void) const;                                                       //!< Get value as STL vector<vector> of doubles
        void                                push_back(const VectorReal& x);                                             //!< Push back a row vector
        void                                setContent(const std::vector<double>& x);                                   //!< Set content using STL vector of doubles
        void                                setValue(const std::vector<std::vector<double> >& x);                       //!< Set value using STL vector<vector> of doubles
        void                                transpose(void);                                                            //!< Transpose the matrix

    private:
        RbObject*                           getDefaultElement(void) { return new Real(); }                              //!< Get default element for empty slots
        bool                                numFmt(int& numToLft, int& numToRht, std::string s) const;                  //!< Calculates the number of digits to the left and right of the decimal
        
        // Parser help functions
        DAGNode*                            getElement(const VectorInteger& index);                                     //!< Get element or subcontainer for parser
        void                                setElement(const VectorNatural& index, DAGNode* var);                       //!< Allow parser to set element

        std::vector<VectorReal>             matrix;                                                                     //!< We use vector of vectors instead of container internally @Fredrik: The class hierarchy is not very consistent if this class does not use the member values from the parent class. It also might result into the overhead of additional memory allocations and hence slower performance. (Sebastian)
};

        // operators defined outside of the class
        MatrixReal                          operator+(const MatrixReal& A, const MatrixReal& B);                        //!< operator + 
        MatrixReal                          operator-(const MatrixReal& A, const MatrixReal& B);                        //!< operator - 
        MatrixReal                          operator*(const MatrixReal& A, const MatrixReal& B);                        //!< operator * (matrix multiplication) 
        MatrixReal                          operator/(const MatrixReal& A, const MatrixReal& B);                        //!< operator / for matrix / matrix 
        MatrixReal&                         operator+=(MatrixReal& A, const MatrixReal& B);                             //!< operator += 
        MatrixReal&                         operator-=(MatrixReal& A, const MatrixReal& B);                             //!< operator -= 
        MatrixReal&                         operator*=(MatrixReal& A, const MatrixReal& B);                             //!< operator *= (matrix multiplication)
        MatrixReal                          operator+(const double &a, const MatrixReal& B);                            //!< operator + for scalar + matrix 
        MatrixReal                          operator-(const double &a, const MatrixReal& B);                            //!< operator - for scalar - matrix 
        MatrixReal                          operator*(const double &a, const MatrixReal& B);                            //!< operator * for scalar * matrix 
        MatrixReal                          operator/(const double &a, const MatrixReal& B);                            //!< operator / for scalar / matrix 
        MatrixReal                          operator+(const MatrixReal& A, const double& b);                            //!< operator + for matrix + scalar 
        MatrixReal                          operator-(const MatrixReal& A, const double& b);                            //!< operator - for matrix - scalar 
        MatrixReal                          operator*(const MatrixReal& A, const double& b);                            //!< operator * for matrix * scalar 
        MatrixReal                          operator/(const MatrixReal& A, const double& b);                            //!< operator / for matrix / scalar 
        MatrixReal&                         operator+=(MatrixReal& A, const double& b);                                 //!< operator += for scalar 
        MatrixReal&                         operator-=(MatrixReal& A, const double& b);                                 //!< operator -= for scalar 
        MatrixReal&                         operator*=(MatrixReal& A, const double& b);                                 //!< operator *= for scalar 
        MatrixReal&                         operator/=(MatrixReal& A, const double& b);                                 //!< operator /= for scalar 

#endif
