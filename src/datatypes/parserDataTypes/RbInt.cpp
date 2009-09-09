/*!
 * @file
 * This file contains the implementation of RbInt, which is
 * a REvBayes wrapper around an STL vector of int.
 *
 * @brief Implementation of RbInt
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbInt.h"
#include <iostream>
#include <string>
#include <vector>


/** The data type */
const string RbInt::dataType = std::string("RbInt");


/** Copy constructor */
RbInt::RbInt(const RbInt& s)
    : RbAbstractDataType(s.getType()), std::vector<int>(s) {
}

/** Return copy */
RbInt* RbInt::copy(void) const {

	return new RbInt( *this );
}

/** Print content to c */
void RbInt::print(std::ostream& c) const {

	for (RbInt::const_iterator i=begin(); i!=end(); i++) {
        if ( i==begin() )
            c << "{";
        else
            c << ",";
        c << *i;
    }
    c << "}" << endl;
}
