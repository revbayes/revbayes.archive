/**
 * @file
 * This file contains the implementation of Dist_norm, which is used to hold
 * parameters and functions related to a normal distribution.
 *
 * @brief Implementation of Dist_norm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "Dist_norm.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "DistributionNormal.h"

#include <cmath>


/** Constructor for parser use */
Dist_norm::Dist_norm( void ) : InferenceDistributionContinuous() {

}


/**
 * This function calculates the cumulative probability for a
 * normally-distributed random variable.
 *
 * @brief Normal cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 * @see Adams, A. G. 1969. Areas under the normal curve. Computer J. 12:197-198.
 */
double Dist_norm::cdf(double q) {

	return RbStatistics::Normal::cdf(*mu.value, *sigma.value, q);

}


/** Clone this object */
Dist_norm* Dist_norm::clone( void ) const {

    return new Dist_norm( *this );
}



/**
 * This function calculates the natural log of the probability
 * density for a normally-distributed random variable.
 *
 * @brief Natural log of normal probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_norm::lnPdf( void ) const {

    return RbStatistics::Normal::lnPdf(*mu.value, *sigma.value, *randomVariable.value);
}


/**
 * This function calculates the probability density
 * for a normally-distributed random variable.
 *
 * @brief Normal probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_norm::pdf( void ) const {
    
    return RbStatistics::Normal::pdf(*mu.value, *sigma.value, *randomVariable.value);
}


/**
 * This function calculates the quantile for a
 * normally-distributed random variable.
 *
 * @brief Quantile of normal probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 * @see Odeh, R. E. and J. O. Evans. 1974. The percentage points of the normal
 *      distribution. Applied Statistics, 22:96-97.
 * @see Wichura, M. J.  1988. Algorithm AS 241: The percentage points of the
 *      normal distribution. 37:477-484.
 * @see Beasley, JD & S. G. Springer. 1977. Algorithm AS 111: The percentage
 *      points of the normal distribution. 26:118-121.
 */
double Dist_norm::quantile( double p ) {
    
    return RbStatistics::Normal::quantile(*mu.value, *sigma.value, p);
}


/**
 * This function generates a normally-distributed
 * random variable.
 *
 * @todo What algorithm is this? It is not Box-Muller, not Ziggurat. It does
 *       not resemble the R code, which uses Ahrens & Dieter and Kinderman & Ramage.
 *       Code for extra normal rv based on guesswork using comments in RbStatistics.cpp.
 *
 * @brief Random draw from normal distribution
 *
 * @return      Random draw
 */
void Dist_norm::rv(void) {

    static bool   availableNormalRv = false;
    static double extraNormalRv;

    if ( availableNormalRv ) {
        availableNormalRv = false;
        *randomVariable.value = extraNormalRv;
    }
    else {

        RandomNumberGenerator* rng = GLOBAL_RNG;
        *randomVariable.value = RbStatistics::Normal::rv(*mu.value, *sigma.value, *rng);
    }

}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_norm::setParameters(const std::vector<RbValue<void *> > &p) {
    
    mu.value                = ( static_cast<double*>( p[0].value ) );
    mu.lengths              = p[0].lengths;
    
    sigma.value             = ( static_cast<double*>( p[1].value ) );
    sigma.lengths           = p[1].lengths;
    
    randomVariable.value    = static_cast<double*>( p[2].value );
    randomVariable.lengths  = p[2].lengths;
    
}

