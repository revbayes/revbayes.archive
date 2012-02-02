/**
 * @file
 * This file contains the declaration of Matrix,
 * a container type used to hold value matrices.
 *
 * @brief Declaration of Matrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 *
 * $Id$
 */

#ifndef Matrix_H
#define Matrix_H

#include "Vector.h"

#include <iostream>
#include <string>
#include <vector>

class VectorInteger;


/**
 * @brief Container class for matrices
 *
 * To allow convenient programmer access to
 * matrices, it may be best to implement them
 * as vector of vectors. A number of functions
 * that are inherited from Container and
 * Valuecontainer assume that the content of the
 * container is stored in the single elements
 * vector. To force programmers deriving from
 * the Matrix class to override these functions
 * if they decide to use vector of vectors to
 * represent the values internally, these
 * functions are declared as abstract here.
 */
const std::string Matrix_name = "Matrix";

class Matrix : public Vector {

    public:
        virtual                        ~Matrix(void) {}                                                     //!< Virtual destructor

        // Basic utility functions you have to override
        virtual Matrix*                 clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class
        virtual void                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object
   
        // Matrix functions you have to override
        virtual const RbObject*         getElement(size_t row, size_t col) const = 0;                       //!< Get element or subcontainer
        virtual RbObject*               getElement(size_t row, size_t col) = 0;                             //!< Get element or subcontainer (non-const to return non-const element)
        virtual void                    setElement(size_t row, size_t col, RbLanguageObject* var) = 0;      //!< set element
        virtual void                    resize(size_t rows, size_t cols) = 0;                               //!< Resize to new length vector
        virtual void                    transpose(void) = 0;                                                //!< Transpose matrix
    
        // Matrix functions you do not have to override
        size_t                          getNumberOfColumns(void) const;                                     //!< Get the number of columns in the matrix
        size_t                          getNumberOfRows(void) const;                                        //!< Get the number of rows in the matrix

    protected:
                                        Matrix(const std::string& elemType);                                //!< Set type of elements
                                        Matrix(const std::string& elemType, RbPtr<const MemberRules> memberRules);//!< Set type of elements
                                        Matrix(const Matrix& m);                                            //!< Copy constructor

};

#endif
