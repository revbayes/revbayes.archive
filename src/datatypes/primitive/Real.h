/**
 * @file
 * This file contains the declaration of Real, which is
 * a RevBayes wrapper around a regular double.
 *
 * @brief Declaration of Real
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef Real_H
#define Real_H

#include "RbObject.h"

#include <ostream>
#include <string>

class VectorString;

/**
 * Real is the class used to hold a real value. Internally, the real is represented by
 * a double.
 *
 * @note Some functions are virtual because RealPos is derived from double
 */
class Real : public RbObject {

    public:
                                    Real(void);                                      //!< Default constructor (0.0)
                                    Real(const double v);                            //!< Construct from double
                                    Real(const int v);                               //!< Construct from int 
                                    Real(const bool v);                              //!< Construct from bool

        // Basic utility functions
        virtual RbObject*           clone(void) const;                               //!< Clone object
        bool                        equals(const RbObject* obj) const;               //!< Equals comparison
        virtual const VectorString& getClass(void) const;                            //!< Get class vector
        void                        printValue(std::ostream& o) const;               //!< Print value (for user)
        virtual std::string         toString(void) const;                            //!< Complete info about object

        // Type conversion
        virtual RbObject*           convertTo(const std::string& type, int dim=0) const;                        //!< Convert to type and dim
        virtual bool                isConvertibleTo(const std::string& type, int dim=0, bool once=false) const; //!< Is convertible to type and dim?
                                    operator double(void) const { return value; }    //!< Type conversion to double for convenience

        // Getters and setters
        virtual void                setValue(double x) { value = x; }                //!< Set value
        double                      getValue(void) const { return value; }           //!< Get value

	protected:
 		double                      value;                                           //!< Value member
};

#endif

