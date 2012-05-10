/**
 * @file
 * This file contains the implementation of Dist_lnorm, which is used to hold
 * parameters and functions related to a lognormal distribution.
 *
 * @brief Implementation of Dist_lnorm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-08-17, version 1.0
 *
 * $Id$
 */

#include "DAGNode.h"
#include "Dist_lnorm.h"
#include "DistributionLognormal.h"
#include "MemberFunction.h"
#include "RealPos.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser use */
Dist_lnorm::Dist_lnorm( void ) : DistributionContinuous(  ) {
	
}


/**
 * This function calculates the cumulative probability for a
 * lognormally-distributed random variable.
 *
 * @brief Lognormal cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_lnorm::cdf(double q) {
    
	return RbStatistics::Lognormal::cdf(*mean.value, *sd.value, q);
    
}


/** Clone this object */
Dist_lnorm* Dist_lnorm::clone( void ) const {
	
    return new Dist_lnorm( *this );
}


/**
 * This function calculates the natural log of the probability
 * density for a lognormally-distributed random variable.
 *
 * @brief Natural log of lognormal probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_lnorm::lnPdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Lognormal::lnPdf(mean.value[offset[0]], sd.value[offset[1]], randomVariable.value[offset[2]]);
}


/**
 * This function calculates the probability density
 * for a lognormally-distributed random variable.
 *
 * @brief Lognormal probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_lnorm::pdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Lognormal::pdf(mean.value[offset[0]], sd.value[offset[1]], randomVariable.value[offset[2]]);
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
 */
double Dist_lnorm::quantile( double p ) {
    
    return RbStatistics::Lognormal::quantile(*mean.value, *sd.value, p);
}


/**
 * This function generates a lognormally-distributed
 * random variable.
 *
 *
 * @brief Random draw from lognormal distribution
 *
 * @return      Random draw
 */
void Dist_lnorm::rvSingleValue( std::vector<size_t> &offset ) {
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    randomVariable.value[offset[2]] = RbStatistics::Lognormal::rv(mean.value[offset[0]], sd.value[offset[1]], *rng);
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_lnorm::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    mean.value              = static_cast<double*>( p[0].value );
    mean.lengths            = p[0].lengths;
    
    sd.value                = static_cast<double*>( p[1].value );
    sd.lengths              = p[1].lengths;
    
    randomVariable.value    = static_cast<double*>( p[2].value );
    randomVariable.lengths  = p[2].lengths;
    
}



