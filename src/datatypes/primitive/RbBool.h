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

#include <iostream>
#include "RbDataType.h"
#include "RbAbstractDataType.h"

class RbBool : public RbAbstractDataType {

public:

	RbBool(const bool v);                                            //!< Constructor from double
	RbBool(const bool v, std::string& name);                         //!< Constructor from double
	RbBool(const RbBool& s);                                         //!< Copy constructor
	virtual ~RbBool();

    // implemented abstract/virtual functions from base classes
	RbObject*  clone(void);                                        //!< clone this object
	void       print(std::ostream &c) const;                            //!< Print the value to ostream c
	void       dump(std::ostream& c);                              //!< Dump to ostream c
	void       resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
	const std::string&  getType(void) const;                       //!< Get type name

	//overloaded operators
	bool       operator==(RbObject& o) const;                      //!< Comparison

	// memeber functions
	void       setValue(bool v);
	bool        getValue(void) const;

private:
	bool        value;      //!< value member
};

#endif

