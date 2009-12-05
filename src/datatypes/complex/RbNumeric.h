/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the RevBayes abstract base class for all objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbObject.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbNumeric_H
#define RbNumeric_H

#include "RbList.h"

class RbNumeric : public RbList {

    public:
        virtual ~RbNumeric(void) { }                                                           //! Virtual destructor because of virtual functions

        // Pointer-based comparison -- throw not supported error by default
        virtual bool                lessThan(const RbObject* o) const;                        //!< Less than

        // Pointer-based arithmetic -- throw not supported error by default
        virtual RbObject*           add(const RbObject* o) const = 0;                             //!< Addition
        virtual RbObject*           subtract(const RbObject* o) const = 0;                        //!< Subtraction
        virtual RbObject*           multiply(const RbObject* o) const = 0;                        //!< Multiplication
        virtual RbObject*           divide(const RbObject* o) const = 0;                          //!< Division
        virtual RbObject*           raiseTo(const RbObject* o) const = 0;                         //!< Power

    protected:
        RbNumeric(void) { }                                                                    //!< Make it impossible to create objects
};

#endif
