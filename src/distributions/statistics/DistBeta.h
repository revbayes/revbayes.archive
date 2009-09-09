/*
 * DistBeta.h
 *
 *  Created on: 7 sep 2009
 *      Author: Sebastian
 */

#ifndef DISTBETA_H_
#define DISTBETA_H_

#include "Distribution.h"
#include "RbDataType.h"

class DistBeta : Distribution{
public:
	DistBeta();
	virtual ~DistBeta();

	RbDataType* execute(); //!< Call appropriate function based on functionType
	RbDataType getDataType(); //!< Get data type for type checking
	RbDataType* lnPdf(RbDataType value, std::vector<RbDataDyte> arguments); //!< Ln probability density
	RbDataType* pdf(RbDataType value, std::vector<RbDataDyte> arguments); //!< Probability density
	RbDataType* rv(); //!< Draw random variables
};

#endif /* DISTBETA_H_ */
