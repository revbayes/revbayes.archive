/*!
 * \file
 * This file contains the declaration of RbBool, which is
 * a REvBayes wrapper around a regular bool.
 *
 * \brief Declaration of RbBool
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef RbBool_H
#define RbBool_H

#include "RbDataType.h"

class RbBool : public RbDataType {

public:
	    RbBool(const bool v);       //!< Constructor from bool
	    RbBool(const RbBool& s);    //!< Copy constructor

	RbBool* copy(void) const;       //!< Copy
	void    print(void) const;      //!< Print the value

	bool    store(void) const { return true; }  //!< What does this do?
	bool    restore(void) { return true; }      //!< What does this do?

private:
	bool    value;      //!< value member
};

#endif

