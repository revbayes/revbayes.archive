/**
 * @file DistributionCauchy
 * This file contains the functions of the chauchy distribution.
 *
 * @brief Implementation of the chauchy distribution.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-03-17, version 1.0
 *
 * $Id$
 */


#include <cmath>

#include "DistributionCauchy.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"

using namespace RevBayesCore;

/*!
 * This function calculates the probability density 
 * for a Cauchy-distributed random variable.
 *
 * \brief Geometric probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Cauchy::lnPdf(double location, double scale, double x) {

    return pdf(location, scale, x, true);
}

/*!
 * This function calculates the probability density 
 * for a Cauchy-distributed random variable.
 *
 * \brief Geometric probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Cauchy::pdf(double location, double scale, double x)
{

    return pdf(location, scale, x, false);
}


double RbStatistics::Cauchy::pdf(double location, double scale, double x, bool give_log)
{
    
    if (scale <= 0) 
    {
        std::ostringstream s;
        s << "Cannot compute pdf of the Cauchy distribution because scale = " << scale << " is negativ.";
        throw RbException(s.str());
    }
    
    double y = (x - location) / scale;
    return give_log ? - log(RbConstants::PI * scale * (1.0 + y * y)) : 1.0 / (RbConstants::PI * scale * (1.0 + y * y));
}

