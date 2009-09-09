/*!
 * @file
 * This file contains the implementation of RbSimplex, which is
 * a REvBayes wrapper around an stl vector of Simplex.
 *
 * @brief Implementation of RbSimplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @package datatypes
 * @since Version 1.0, 2009-09-09
 *
 * $Id$
 */

#include "RbSimplex.h"
#include <iostream>
#include <vector>

using namespace std;

/** Copy constructor */
RbSimplex::RbSimplex(const RbSimplex& s)
    : RbAbstractDataType(s.getType()), vector<Simplex>(s) {
}

/** Return copy */
RbSimplex* RbSimplex::copy(void) const {

	RbSimplex *x = new RbSimplex( *this );
	return x;
}

/** Print content to c */
void RbSimplex::print(ostream &c) const {

	for (RbSimplex::const_iterator i=begin(); i!=end(); i++) {
        if ( i == begin() )
            c << "{";
        else
            c << ",";
        i->print(c);
    }
    c << "}" << endl;
}
