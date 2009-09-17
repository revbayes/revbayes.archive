/*!
 * \file
 * This file contains the declaration of RbInt, which is
 * a REvBayes wrapper around a regular int.
 *
 * \brief Declaration of RbInt
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef RbInt_H
#define RbInt_H

#include "RbDataType.h"

class RbInt : public RbDataType {

public:
	    RbInt(const int v);       //!< Constructor from int
	    RbInt(const RbInt& s);    //!< Copy constructor

	RbInt*  copy(void) const;     //!< Copy
	void    print(void) const;    //!< Print the value to cout
	void    print(ostream &c) const;    //!< Print the value to ostream c
	int     getValue() {return value;} //!< gets the actual value

	bool    store(void) const { return true; }  //!< What does this do?
	bool    restore(void) { return true; }      //!< What does this do?

private:
	int     value;      //!< value member
};

#endif
