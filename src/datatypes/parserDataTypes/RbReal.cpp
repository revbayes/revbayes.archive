/*!
 * @file
 * This file contains the implementation of RbReal, which is
 * a REvBayes wrapper around an STL vector of doubles.
 *
 * @brief Implementation of RbReal
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbReal.h"
#include <iostream>
#include <vector>

/** The data type */
const string RbReal::dataType = std::string("RbReal");

/** Copy constructor */
RbReal::RbReal(const RbReal& s)
    : RbAbstractDataType(s.getType()), std::vector<double>(s) {
}

/** Return copy */
RbReal* RbReal::copy(void) const {

	return new RbReal( *this );
}

/** Print content to c */
void RbReal::print(std::ostream& c) const {

	for (RbReal::const_iterator i=begin(); i!=end(); i++) {
        if ( i==begin() )
            c << "{";
        else
            c << ",";
        c << *i;
    }
    c << "}" << endl;
}
