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

#include "ValueContainer.h"

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
class Matrix : public ValueContainer {

    public:
        virtual                        ~Matrix(void) {}                                                     //!< Virtual destructor

        // Basic utility functions you have to override
        virtual Matrix*                 clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class
        virtual void                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object

        // Container functions you have to override to allow vector of vectors representation
        virtual void                    clear(void) = 0;                                                    //!< Clear
        virtual void                    resize(const std::vector<size_t>& len) = 0;                         //!< Resize to new length vector (generic container version, can be mapped to function taking nrows and ncols)
        virtual void                    setLength(const std::vector<size_t>& len) = 0;                      //!< Reorganize container (generic container version, can be mapped to function taking nrows and ncols)
        virtual size_t                  size(void) const = 0;                                               //!< Get total number of elements
        
        // Matrix functions you do not have to override
        size_t                          getNumCols(void) const { return length[1]; }                        //!< Get the number of columns in the matrix
        size_t                          getNumRows(void) const { return length[0]; }                        //!< Get the number of rows in the matrix

        // Matrix function you have to override
        virtual void                    transpose(void) = 0;                                                //!< Transpose matrix

    protected:
                                        Matrix(const std::string& elemType);                                //!< Set type of elements

        // Parser help functions you have to override
        virtual DAGNode*                getElement(const VectorInteger& index) = 0;                         //!< Get element or subcontainer for parser (generic container version, can be mapped to take row and col indices)
        virtual void                    setElement(const VectorNatural& index, DAGNode* var) = 0;           //!< Allow parser to set element (generic container version, can be mapped to take row and col indices)
};

#endif
