/*
 *  DistExponential.h
 *  REvBayes
 *
 *  Created by John Huelsenbeck on 8/18/09.
 *  Copyright 2009 University of California, Berkeley. All rights reserved.
 *
 */

#ifndef DISTEXPONENTIAL_H
#define DISTEXPONENTIAL_H

#include "Distribution.h"
#include "../datatypes/RbDataType.h"
#include "../datatypes/primary/RbDouble.h"

class DistExponential : Distribution{
public:
	DistExponential();
	virtual ~DistExponential();

//	RbDataType* execute(); //!< Call appropriate function based on functionType
	RbDataType getDataType(); //!< Get data type for type checking
	double lnPdf(RbDataType* value, std::vector<RbDataType*> arguments); //!< Ln probability density
	double pdf(RbDataType* value, std::vector<RbDataType*> arguments); //!< Probability density
	RbDataType* rv(); //!< Draw random variables
	double cdf(RbDataType* variable, std::vector<RbDataType*> arguments);
	double quantile(std::vector<RbDataType*> arguments);
	double quantile(double lambda, double p);

private:
};


#endif /* DISTEXPONENTIAL_H_ */

