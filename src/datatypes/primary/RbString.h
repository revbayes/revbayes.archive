/*!
 * \file
 * This file contains the declaration of RbString, which is
 * a REvBayes wrapper around a regular string.
 *
 * \brief Declaration of RbString
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef RbString_H
#define RbString_H

#include "RbDataType.h"

using namespace std;

class RbString : public RbDataType {

public:
	    RbString(const string& v);      //!< Constructor from string
	    RbString(const RbString& s);    //!< Copy constructor

	RbString*   copy(void) const;           //!< Copy
	void        print(void) const;          //!< Print the value to cout
	void        print(ostream &c) const;    //!< Print the value to c

	bool        store(void) const { return true; }  //!< What does this do?
	bool        restore(void) { return true; }      //!< What does this do?

private:
	string  value;      //!< Value member
};

#endif

