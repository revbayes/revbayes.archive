/**
 * @file
 * This file contains the interface for distributions.
 * A distribution is taken in the statistical sense.
 * This interface specifies the pdf, cdf, quantile and rv function with its parameters which need to be provided by any distribution.
 *
 * @brief Declaration of Interface Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Distribution
 * @extends RbObject
 * @package distributions
 *
 * $Id$
 */

#ifndef Distribution_H
#define Distribution_H

#include <vector>
#include <string>
#include <list>
#include "RbObject.h"
#include "StringVector.h"

class RandomNumberGenerator;

class Distribution: public RbObject {

public:
    static const StringVector   rbClass;            //!< Static class attribute

	virtual ~Distribution() { }                        //!< Destructor does nothing

    std::string getReturnType() const { return returnType; }    //!< Get return type
	virtual double lnPdf(RbObject* o) = 0;       //!< Ln probability density function
	virtual double pdf(RbObject* o) = 0;         //!< Probability density function
	virtual RbObject* rv(RandomNumberGenerator* r)  = 0;
protected:
	Distribution() :
		RbObject() {
	}

	std::string returnType;
};

#endif
