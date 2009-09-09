/*
 * DistNormal.h
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#ifndef DISTNORMAL_H
#define DISTNORMAL_H

#include "Distribution.h"
#include "../datatypes/RbDataType.h"
#include "../datatypes/primary/RbDouble.h"

class DistNormal : Distribution{
public:
	DistNormal(double* s, double* m);
	virtual ~DistNormal();

//	RbDataType* execute(); //!< Call appropriate function based on functionType
	RbDataType getDataType(); //!< Get data type for type checking
	double lnPdf(RbDataType* value); //!< Ln probability density
	double pdf(RbDataType* value); //!< Probability density
	RbDataType* rv(); //!< Draw random variables
	double cdf(RbDataType* variable);
	double quantile(RbDataType* variable);

private:
	double* sigma;
	double* mu;
};

#endif /* DISTNORMAL_H_ */
