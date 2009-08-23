/*!
 * \file
 * This file contains the declaration of Scalar, which is
 * a REvBayes wrapper around a regular double.
 *
 * \brief Declaration of Scalar
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef Scalar_H
#define Scalar_H

#include "RbDataType.h"

class Scalar : public RbDataType {

public:
	    Scalar(const double v);     //!< Constructor from double
	    Scalar(const Scalar& s);    //!< Copy constructor

	Scalar* copy(void) const;       //!< Copy
	void    print(void) const;      //!< Print the value

	bool    store(void) const { return true; }  //!< What does this do?
	bool    restore(void) { return true; }      //!< What does this do?

private:
	double value;
};

#endif

