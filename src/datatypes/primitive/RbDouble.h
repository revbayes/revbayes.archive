/**
 * @file
 * This file contains the declaration of RbDouble, which is
 * a RevBayes wrapper around a regular double.
 *
 * @brief Declaration of RbDouble
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef RbDouble_H
#define RbDouble_H

#include "RbObject.h"

#include <ostream>
#include <string>

class StringVector;


class RbDouble : public RbObject {

    public:
	                        RbDouble(const double v);                   //!< Construct from double
	                        RbDouble(const int v);                      //!< Construct from int 
	                        RbDouble(const bool v);                     //!< Construct from bool
	                        ~RbDouble(void);                            //!< destructor

        // Basic utility functions
        RbObject*           clone(void) const;                          //!< Clone object
        bool                equals(const RbObject* obj) const;          //!< Equals comparison
        const StringVector& getClass(void) const;                       //!< Get class vector
        void                printValue(std::ostream& o) const;          //!< Print value (for user)
        std::string         toString(void) const;                       //!< Complete info about object

        // Type conversion
        RbObject*           convertTo(const std::string& type) const;       //!< Convert to type
                            operator double(void) const { return value; }   //!< Type conversion to double

        // Getters and setters
        void                setValue(double x) { value = x; }           //!< Set value
	    double              getValue(void) const { return value; }      //!< Get value

    private:
	    double              value;                                      //!< Value member
};

#endif

