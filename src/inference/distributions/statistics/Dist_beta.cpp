/**
 * @file
 * This file contains the implementation of Dist_beta, which is used to hold
 * parameters and functions related to an beta distribution.
 *
 * @brief Implementation of Dist_beta
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

#include "Dist_beta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbUtil.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "DistributionBeta.h"
#include "RbStatisticsHelper.h"

#include <cmath>
#include <sstream>


/** Default constructor for parser use */
Dist_beta::Dist_beta( void ) : DistributionContinuous( ) {
    // Do nothing
}


/**
 * This function calculates the cumulative probability for
 * an beta-distributed random variable.
 *
 * @brief beta cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_beta::cdf(double q) {
    
	return RbStatistics::Beta::cdf(*alpha.value, *beta.value, q);
    
}


/** Clone this object */
Dist_beta* Dist_beta::clone( void ) const {

    return new Dist_beta( *this );
}


/**
 * This function calculates the natural log of the probability
 * density for an beta-distributed random variable.
 *
 * @brief Natural log of beta probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_beta::lnPdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Beta::lnPdf(alpha.value[offset[0]], beta.value[offset[1]], randomVariable.value[offset[2]]);
}


/**
 * This function calculates the probability density
 * for an beta-distributed random variable.
 *
 * @brief beta probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_beta::pdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Beta::pdf(alpha.value[offset[0]], beta.value[offset[1]], randomVariable.value[offset[2]]);
}


/**
 * This function calculates the quantile for a
 * beta-distributed random variable.
 *
 * @brief Quantile of beta probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
double Dist_beta::quantile( double p ) {
    
    return RbStatistics::Beta::quantile(*alpha.value, *beta.value, p);
}


/**
 * This function generates a beta-distributed
 * random variable.
 *
 * @brief Random draw from beta distribution
 *
 * @return      Random draw from beta distribution
 */
void Dist_beta::rvSingleValue( std::vector<size_t> &offset ) {
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    randomVariable.value[offset[2]] = RbStatistics::Beta::rv(alpha.value[offset[0]], beta.value[offset[1]], *rng);
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_beta::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    alpha.value             = ( static_cast<double*>( p[0].value ) );
    alpha.lengths           = p[0].lengths;
    
    beta.value              = ( static_cast<double*>( p[1].value ) );
    beta.lengths            = p[1].lengths;
    
    randomVariable.value    = static_cast<double*>( p[2].value );
    randomVariable.lengths  = p[2].lengths;
    
}




