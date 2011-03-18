/**
 * @file DistributionGeometric
 * This file contains the functions of the geometric distribution.
 *
 * @brief Implementation of the geometric distribution.
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

#include "DistributionBeta.h"
#include "DistributionBinomial.h"
#include "DistributionGeometric.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"

/*!
 * This function calculates the probability density 
 * for a Geometricly-distributed random variable.
 *
 * \brief Geometric probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Geometric::cdf(int n, double p)
{
    
    if(p <= 0 || p > 1) {
        std::ostringstream s;
        s << "Cannot compute cdf of the Geometric distribution because n = " << n << " is not an integer";
        throw (RbException(s));
    }
    
    if (n < 0.) return 0.0;
    if (!RbMath::isFinite(n)) return 1.0;
    
    if(p == 1.) { /* we cannot assume IEEE */
        return n;
    }
    n = log1p(-p) * (n + 1);
        
    return -expm1(n);
}

/*!
 * This function calculates the probability density 
 * for a Geometricly-distributed random variable.
 *
 * \brief Geometric probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Geometric::lnPdf(int n, double p) {
    return pdf(n,p,true);
}

/*!
 * This function calculates the probability density 
 * for a Geometricly-distributed random variable.
 *
 * \brief Geometric probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Geometric::pdf(int n, double p) {
    return pdf(n,p,false);
}

/*!
 * This function calculates the probability density 
 * for a Geometricly-distributed random variable.
 *
 * From R:
 *
 *     (1) pdf() has both p and q arguments, when one may be represented
 *         more accurately than the other (in particular, in df()).
 *     (2) pdf() does NOT check that inputs x and n are integers. This
 *         should be done in the calling function, where necessary.
 *         -- but is not the case at all when called e.g., from df() or dbeta() !
 *     (3) Also does not check for 0 <= p <= 1 and 0 <= q <= 1 or NaN's.
 *         Do this in the calling function.
 *
 * \brief Geometric probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Geometric::pdf(int n, double p, bool asLog) {
    
    double prob;
    
    if (p <= 0 || p > 1) {
        std::ostringstream s;
        s << "Cannot compute pdf of the Geometric distribution because n = " << n << " is not an integer";
        throw (RbException(s));
    }
    
    if (n < 0 || !RbMath::isFinite(n) || p == 0) return 0.0;
    
    /* prob = (1-p)^x, stable for small p */
    prob = RbStatistics::Binomial::pdf(0.,n, p,1-p, asLog);
    
    return((asLog) ? log(p) + prob : p*prob);
}

/*!
 * This function calculates the quantile 
 * for a Geometricly-distributed random variable.
 *
 * \brief Geometric probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Geometric::quantile(double q, double p) {
    if (p <= 0 || p > 1) {
        std::ostringstream s;
        s << "Cannot compute pdf of the Geometric distribution because q = " << q << " is not an integer";
        throw (RbException(s));
    }

    if (p == 1) return(0);
    
    /* add a fuzz to ensure left continuity */
    return ceil(log(q) / log1p(- p) - 1 - 1e-7);
}

