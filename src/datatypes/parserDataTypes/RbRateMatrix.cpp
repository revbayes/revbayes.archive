/*!
 * @file
 * This file contains the implementation of RbRateMatrix, which is
 * a REvBayes wrapper around an STL vector of RateMatrix.
 *
 * @brief Implementation of RbRateMatrix
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

#include "RbRateMatrix.h"
#include <iostream>
#include <vector>

using namespace std;

/** Copy constructor */
RbRateMatrix::RbRateMatrix(const RbRateMatrix& s)
    : RbAbstractDataType(s.getType()), vector<RateMatrix>(s) {
}

/** Return copy */
RbRateMatrix* RbRateMatrix::copy(void) const {

	RbRateMatrix *x = new RbRateMatrix( *this );
	return x;
}

/** Print content to c */
void RbRateMatrix::print(ostream &c) const {

	for (RbRateMatrix::const_iterator i=begin(); i!=end(); i++) {
        if ( i==begin() )
            c << "{";
        else
            c << ",";
        // @todo c << (*i);
    }
    c << "}" << endl;
}
