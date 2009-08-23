/*!
 * \file
 * This file contains the declaration of RbStringVector, which
 * holds a vector of strings.
 *
 * \brief Declaration of RbStringVector
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef RbStringVector_H
#define RbStringVector_H

#include "RbDataType.h"
#include <vector>

using namespace std;

class RbStringVector : public RbDataType {

public:
	    RbStringVector(const string& v);            //!< Constructor from string
	    RbStringVector(vector<string> v);           //!< Constructor from vector
	    RbStringVector(const RbStringVector& s);    //!< Copy constructor

	RbStringVector* copy(void) const;           //!< Copy
	void            print(void) const;          //!< Print the value
	void            print(ostream &c) const;    //!< Print the value

	bool            store(void) const { return true; }  //!< What does this do?
	bool            restore(void) { return true; }      //!< What does this do?

private:
	vector<string>  value;      //!< Value member
};

#endif

