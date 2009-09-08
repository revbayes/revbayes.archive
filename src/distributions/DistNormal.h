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
	DistNormal();
	virtual ~DistNormal();

//	RbDataType* execute(); //!< Call appropriate function based on functionType
	RbDataType getDataType(); //!< Get data type for type checking
	double lnPdf(RbDataType* value, std::vector<RbDataType*> arguments); //!< Ln probability density
	double pdf(RbDataType* value, std::vector<RbDataType*> arguments); //!< Probability density
	RbDataType* rv(); //!< Draw random variables
	double cdf(RbDataType* variable, std::vector<RbDataType*> arguments);
	double quantile(std::vector<RbDataType*> arguments);
	double quantile(double p);
	double quantile(double mu, double sigma, double p);

private:
};

#endif /* DISTNORMAL_H_ */
