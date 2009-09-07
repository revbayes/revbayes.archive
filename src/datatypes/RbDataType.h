/*!
 * \file
 * This file contains the declaration of RbDataType, which is
 * the REvBayes base class for basic data types.
 *
 * \brief Declaration of RbDataType
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef RbDataType_H
#define RbDataType_H

#include <string>
#include "../main/RbObject.h"

using namespace std;

class RbDataType {

public:
	RbDataType(const char* name) : typeName(name) {}            //!< Constructor from type name (C style)
	RbDataType(const string& name) : typeName(name) {}          //!< Constructor from type name (string)

	virtual RbDataType*     copy(void) {};               //!< Copy
	string                  getType(void) { return typeName; }  //!< Get type name
    virtual void            print(void) {};              //!< Print to cout
    virtual void            print(ostream &c) const {}          //!< Print to specified ostream

protected:
	const string    typeName;                                   //!< Name of the data type (for type checking or debugging)
};

#endif
