/**
 * @file
 * This file contains the abstract class for distributions implementing the distribution interface with the common functions.
 *
 * @brief Implementation of the abstract class Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface Distribution
 * @extends RbObject
 *
 * $Id$
 */

#include "Distribution.h"
#include "../datatypes/RbDataType.h"
#include "../datatypes/primary/RbDouble.h"


/**
 * @brief Executes the in functionType specified function to the distribution
 *
 * This function checks the functionType and then calls the corresponding function, e.g the pdf, cdf, quantile or rv
 *
 * @param value      the value used as a parameter for the corresponding function of the distribution
 *
 * @returns The return value of the corresponding function of the distribution
 */
RbDataType* Distribution::execute(RbDataType* value) {
	RbDataType* dt;

	switch (functionType) {
		case Distribution::D:
		case Distribution::P:
			dt = new RbDouble(pdf(value));
			break;
		case Distribution::Q:
			dt = new RbDouble(quantile(value));
			break;
		case Distribution::R:
			dt = rv();
			break;
		default:
			;
			// TODO throw some error, but actually should never even be possible
	}

	return dt;
}
