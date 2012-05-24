/**
 * @file
 * This file contains the declaration of RbMatrix,
 * a container type used to hold value matrices.
 *
 * @brief Declaration of RbMatrix
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

#ifndef RbMatrix_H
#define RbMatrix_H

#include "RlVector.h"
#include "Complex.h"

#include <iostream>
#include <string>
#include <vector>


/**
 * @brief Container class for matrices
 *
 * To allow convenient programmer access to
 * matrices, it may be best to implement them
 * as vector of vectors. A number of functions
 * that are inherited from Container assume that the content of the
 * container is stored in the single elements
 * vector. 
 */

class RbMatrix : public RlVector {

    public:
                                        RbMatrix(const TypeSpec &elemType);                                   //!< Default constructor
                                        RbMatrix(const TypeSpec &elemType, size_t n, size_t k);               //!< Constructor for RbMatrix with of dimension (n,k) filled with NULL pointers 
                                        RbMatrix(const TypeSpec &elemType, size_t n, size_t k, RbObject *x);  //!< Constructor for RbMatrix with of dimension (n,k) filled with copies of element x
        virtual                        ~RbMatrix(void) {}                                                     //!< Virtual destructor
    
        // Basic utility functions you have to override
        virtual RbMatrix*               clone(void) const;                                                  //!< Clone object
        static const std::string&       getClassName(void);                                                 //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                             //!< Get class type spec
        virtual void                    printValue(std::ostream& o) const;                                  //!< Print value for user
   
        // RbMatrix functions you have to override
        virtual const RbObject&         getElement(size_t row, size_t col) const;                           //!< Get element or subcontainer
        virtual RbObject&               getElement(size_t row, size_t col);                                 //!< Get element or subcontainer (non-const to return non-const element)
        virtual void                    setElement(size_t row, size_t col, RbObject* var);                  //!< set element
        virtual void                    resize(size_t rows, size_t cols);                                   //!< Resize to new length vector
        virtual void                    transpose(void);                                                    //!< Transpose RbMatrix
    
        // RbMatrix functions you do not have to override
        size_t                          getNumberOfColumns(void) const;                                     //!< Get the number of columns in the RbMatrix
        size_t                          getNumberOfRows(void) const;                                        //!< Get the number of rows in the RbMatrix


};



#endif
