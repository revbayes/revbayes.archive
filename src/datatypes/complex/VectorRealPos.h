/**
 * @file
 * This file contains the declaration of VectorRealPos, a complex type
 * used to hold double vectors.
 *
 * @brief Declaration of VectorRealPos
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef VectorRealPos_H
#define VectorRealPos_H

#include "VectorInteger.h"
#include "VectorReal.h"

#include <iostream>
#include <string>
#include <vector>

class VectorRealPos : public VectorReal {

    public:
                                    VectorRealPos(void) : VectorReal() {}                   //!< Default constructor (empty vector)
                                    VectorRealPos(const double x);                          //!< Construct vector with one double x
	                                VectorRealPos(const size_t n, const double x);          //!< Construct vector with n doubles x
	                                VectorRealPos(const std::vector<double>& x);            //!< Constructor from double vector

        // Basic utility functions
        virtual VectorRealPos*      clone(void) const;                                      //!< Clone object
        virtual bool                equals(const RbObject* obj) const;                      //!< Equals comparison
        virtual const VectorString& getClass(void) const;                                   //!< Get class vector
        virtual std::string         toString(void) const;                                   //!< Complete info about object

        // Overloaded operators and built-in functions
        virtual void                push_back(double x);                                    //!< Append element to end
        virtual void                push_front(double x);                                   //!< Add element in front

        // Element access functions for parser
        const std::string&          getElementType(void) const;                             //!< Get element type
        virtual void                setElement(const VectorInteger& index, RbObject* val);  //!< Set element

        // Regular functions
        virtual void                setValue(const std::vector<double>& x);                 //!< Set value directly after checking

	protected:
        RbObject*                   convertTo(const std::string& type) const;               //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;         //!< Is convertible to type and dim?
};

#endif


