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

#include "../RbDataType.h"

using namespace std;

class RbString : public RbDataType, public std::string {

public:
	    RbString(const string& v);      //!< Constructor from string
	    RbString(const RbString& s);    //!< Copy constructor

	RbObject*   clone(void);           //!< Copy
	void        print(ostream &c) const;    //!< Print the value to c

	void    dump(std::ostream& c);              //!< Dump to ostream c
	void    resurrect(const RbDumpState& x);  //!< Resurrect from dumped state

private:
	string  value;      //!< Value member
};

#endif

