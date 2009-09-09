/*!
 * @file
 * This file contains the implementation of RbLogic, which is
 * a REvBayes wrapper around an stl vector of bools.
 *
 * @brief Implementation of RbLogic
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbLogic.h"
#include <iostream>
#include <vector>

using namespace std;

/** Copy constructor */
RbLogic::RbLogic(const RbLogic& s)
    : RbAbstractDataType(s.getType()), vector<bool>(s) {
}

/** Return copy */
RbLogic* RbLogic::copy(void) const {

	RbLogic *x = new RbLogic( *this );
	return x;
}

/** Print content to c */
void RbLogic::print(ostream &c) const {

	for (RbLogic::const_iterator i=begin(); i!=end(); i++) {
        if ( i==begin() )
            c << "{";
        else
            c << ",";
        if ( *i )
            c << "TRUE";
        else
            c << "FALSE";
    }
    c << "}" << endl;
}
