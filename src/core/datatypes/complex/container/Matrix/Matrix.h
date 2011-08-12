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

#include "Container.h"

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
class Matrix : public Container {

    public:
        virtual                        ~Matrix(void) {}                                                     //!< Virtual destructor

        // Basic utility functions you have to override
        virtual Matrix*                 clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class
        virtual void                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object

        // Container functions you have to override to allow vector of vectors representation
        virtual void                    clear(void) = 0;                                                    //!< Clear
        virtual Container*              getElement(size_t index) = 0;                                       //!< Get element or subcontainer
        virtual RbLanguageObject*       getElement(size_t row, size_t col) = 0;                             //!< Get element or subcontainer
        virtual void                    setElement(size_t index, RbLanguageObject* var) = 0;                //!< set element
        virtual void                    setElement(size_t row, size_t col, RbLanguageObject* var) = 0;      //!< set element
        virtual void                    resize(size_t rows) = 0;                                            //!< Resize to new length vector
        virtual void                    resize(size_t rows, size_t cols) = 0;                               //!< Resize to new length vector
        virtual size_t                  size(void) const = 0;                                               //!< Get number of rows
        virtual void                    transpose(void) = 0;                                                //!< Transpose matrix
    
        // Matrix functions you do not have to override
        size_t                          getNumberOfColumns(void) const { return cols; }                     //!< Get the number of columns in the matrix
        size_t                          getNumberOfRows(void) const { return rows; }                        //!< Get the number of rows in the matrix

    protected:
                                        Matrix(const std::string& elemType);                                //!< Set type of elements
                                        Matrix(const std::string& elemType, const MemberRules& memberRules);//!< Set type of elements

    
        size_t                          rows;
        size_t                          cols;
};

#endif
