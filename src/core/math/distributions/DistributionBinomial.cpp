/**
 * @file DistributionBinomial
 * This file contains the functions of the binomial distribution.
 *
 * @brief Implementation of the binomial distribution.
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
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"

/*!
 * This function calculates the probability density 
 * for a binomially-distributed random variable.
 *
 * \brief Binomial probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Binomial::cdf(double n, double p, double x) {

    if(RbMath::isInt(n)) 
        {
        std::ostringstream s;
        s << "Cannot compute cdf of the binomial distribution because n = " << n << " is not an interger";
        throw (RbException(s));
        }
    n = int(n);
    /* PR#8560: n=0 is a valid value */
    if(n < 0 || p < 0 || p > 1) 
        {
        std::ostringstream s;
        s << "Cannot compute cdf of the binomial distribution for n = " << n << " and p = " << p;
        throw (RbException(s));
        }
    
    if (x < 0) 
        return 0.0;
    
    x = floor(x + 1e-7);
    if (n <= x) 
        return 1.0;
    
    return RbStatistics::Beta::cdf(p, x + 1, n - x);
}

/*!
 * This function calculates the probability density 
 * for a binomially-distributed random variable.
 *
 * \brief Binomial probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Binomial::lnPdf(double n, double p, double x) {

    double q = 1.0 - p;
    return pdf(n, p, q, x, true);
}

/*!
 * This function calculates the probability density 
 * for a binomially-distributed random variable.
 *
 * \brief Binomial probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Binomial::pdf(double n, double p, double x) {

    double q = 1.0 - p;
    return pdf(n,p,q,x,false);
}

/*!
 * This function calculates the probability density 
 * for a binomially-distributed random variable.
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
 * \brief Binomial probability density.
 * \param n is the number of trials. 
 * \param p is the success probability. 
 * \param x is the number of successes. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Binomial::pdf(double n, double p, double q, double x, bool asLog) {

    double lf, lc;
    
    if (p == 0) 
        return((x == 0) ? 1.0 : 0.0);
    if (q == 0) 
        return((x == n) ? 1.0 : 0.0);
    
    if (x == 0) 
        {
        if(n == 0) 
            return 1.0;
        lc = (p < 0.1) ? -RbMath::binomialDeviance(n,n*q) - n*p : n*log(q);
        return( (asLog ? lc : exp(lc)) );
        }
    if (x == n) 
        {
        lc = (q < 0.1) ? -RbMath::binomialDeviance(n,n*p) - n*q : n*log(p);
        return( (asLog ? lc : exp(lc) ) );
        }
    if (x < 0 || x > n) 
        return( 0.0 );
    
    /* n*p or n*q can underflow to zero if n and p or q are small.  This
     used to occur in dbeta, and gives NaN as from R 2.3.0.  */
    lc = RbMath::stirlerr(n) - RbMath::stirlerr(x) - RbMath::stirlerr(n-x) - RbMath::binomialDeviance(x,n*p) - RbMath::binomialDeviance(n-x,n*q);
    
    /* f = (M_2PI*x*(n-x))/n; could overflow or underflow */
    /* Upto R 2.7.1:
     * lf = log(M_2PI) + log(x) + log(n-x) - log(n);
     * -- following is much better for  x << n : */
    lf = log(RbConstants::TwoPI) + log(x) + RbMath::log1p(- x/n);
    
    return (asLog ? (lc - 0.5*lf) : exp(lc - 0.5*lf));
}


