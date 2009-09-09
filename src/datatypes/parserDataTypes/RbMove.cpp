/*!
 * @file
 * This file contains the implementation of RbMove, which is
 * a REvBayes wrapper around an STL vector of Move.
 *
 * @brief Implementation of RbMove
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbMove.h"
#include <iostream>
#include <vector>

/** Copy constructor */
RbMove::RbMove(const RbMove& s)
    : RbDataType(s.getType()), std::vector<Move>(s) {
}

/** Return copy */
RbMove* RbMove::copy(void) const {

	RbMove *x = new RbMove( *this );
	return x;
}

/** Print content to c */
void RbMove::print(ostream &c) const {

	for (RbMove::const_iterator i=begin(); i!=end(); i++) {
        if ( i==begin() )
            c << "{";
        else
            c << ",";
        i->print(c);
        c << std::endl;
    }
    c << "}" << std::endl;
}
