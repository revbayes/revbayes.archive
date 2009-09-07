/*
 * DistBeta.cpp
 *
 *  Created on: 7 sep 2009
 *      Author: Sebastian
 */

#include <vector>

#include "DistBeta.h"

DistBeta::DistBeta() {
	// TODO Auto-generated constructor stub

}

DistBeta::~DistBeta() {
	// TODO Auto-generated destructor stub
}

/*!
 * This function returns the probability density for a
 * Beta-distributed random variable.
 *
 * \brief Beta probability density.
 * \param a parameter of the Beta.
 * \param b parameter of the Beta.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
RbDataType* DistBeta::pdf(RbDataType value, std::vector<RbDataDyte> arguments) {

	double pdf;
	if ( x < 0.0 || 1.0 < x )
		pdf = 0.0;
	else
		pdf = std::pow(x, (a - 1.0)) * std::pow((1.0 - x), (b - 1.0)) / RbMath::beta(a, b);
	return pdf;
}
