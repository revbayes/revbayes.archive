/**
 * @file
 * This file contains the implementation of RbString, which is
 * a REvBayes wrapper around a regular string.
 *
 * @brief Implementation of RbString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id$
 */

#include "RbString.h"
#include <iostream>
#include <vector>

using namespace std;

/** Copy constructor */
RbString::RbString(const RbString& s)
    : RbAbstractDataType(s.getType()), vector<string>((vector<string>) s) {
}

/** Return copy */
RbString* RbString::copy(void) const {

	RbString *x = new RbString( *this );
	return x;
}

/** Print content to c */
void RbString::print(ostream &c) const {

    c << size();
	for (RbString::const_iterator i=begin(); i!=end(); i++)
        c << " " << *i;
    c << endl;
}
