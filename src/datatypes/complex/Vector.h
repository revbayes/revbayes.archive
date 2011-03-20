/**
 * @file
 * This file contains the declaration of Vector, a complex type
 * that acts as a base class for all vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef Vector_H
#define Vector_H

#include "RbComplex.h"

#include <iostream>
#include <vector>

class Vector : public RbComplex {

    public:
        // destructor
                                      Vector(void);
		virtual                      ~Vector(void) {}                                              //!< Virtual destructor 

        // Basic utility functions, most of which are carried through as pure virtual from RbComplex
        virtual Vector*               clone(void) const = 0;                                       //!< Clone object
        virtual bool                  equals(const RbObject* obj) const = 0;                       //!< Equals comparison
		virtual const VectorString&   getClass(void) const;                                        //!< Get class vector
		virtual void                  printValue(std::ostream& o) const = 0;                       //!< Print value for user
		virtual std::string           richInfo(void) const = 0;                                    //!< Complete info 

        // Convenience functions, with the pure virtual ones to be implemented in the derived VectorXxx classes
        virtual void                 clear(void) = 0;                                              //!< Clear
        bool                         getIsRowVector(void) const { return isRowVector; }            //!< Is the vector a row vector
        virtual void                 pop_back(void) = 0;                                           //!< Drop element
        virtual void                 resize(size_t n) = 0;                                         //!< Resize
        virtual size_t               size(void) const = 0;                                         //!< Get size
        void                         transpose(void);                                              //!< Transpose vector

        // Element access functions for parser to be implemented in the derived VectorXxx classes
        int                          getDim(void) const { return 1; }                              //!< Get subscript dimensions
        virtual const std::string&   getElementType(void) const = 0;                               //!< Get element type
        virtual const RbObject*      getElement(const VectorInteger& index) const = 0;             //!< Get element (read-only)
        virtual const VectorInteger& getLength(void) const = 0;                                    //!< Get length in each dim
        virtual void                 resize(const VectorInteger& len) = 0;                         //!< Resize
        virtual void                 setElement(const VectorInteger& index, RbObject* val) = 0;    //!< Set element
        virtual void                 setLength(const VectorInteger& len) = 0;                      //!< Set length in each dim

    private:
        bool                         isRowVector;                                                  //!< Indicates whether the vector is a row vector (true) or a column vector (false)
};

#endif


