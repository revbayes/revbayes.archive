/**
 * @file
 * This file contains the implementation of Dist_gamma, which is used to hold
 * parameters and functions related to an gamma distribution.
 *
 * @brief Implementation of Dist_gamma
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

#include "DAGNode.h"
#include "Dist_gamma.h"
#include "MemberFunction.h"
#include "RealPos.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbUtil.h"
#include "Real.h"
#include "ValueRule.h"
#include "Workspace.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "DistributionGamma.h"
#include "RbStatisticsHelper.h"

#include <cmath>
#include <sstream>


/** Default constructor for parser use */
Dist_gamma::Dist_gamma( void ) : DistributionContinuous(  ) {
    // Do nothing
}


/**
 * This function calculates the cumulative probability for
 * an gamma-distributed random variable.
 *
 * @brief Gamma cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_gamma::cdf(double q) {
    
	return RbStatistics::Gamma::cdf(*shape.value, *rate.value, q);
    
}


/** Clone this object */
Dist_gamma* Dist_gamma::clone( void ) const {

    return new Dist_gamma( *this );
}


/**
 * This function calculates the natural log of the probability
 * density for an gamma-distributed random variable.
 *
 * @brief Natural log of gamma probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_gamma::lnPdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Gamma::lnPdf(shape.value[offset[0]], rate.value[offset[1]], randomVariable.value[offset[2]]);
}


/**
 * This function calculates the probability density
 * for an gamma-distributed random variable.
 *
 * @brief Gamma probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_gamma::pdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Gamma::pdf(shape.value[offset[0]], rate.value[offset[1]], randomVariable.value[offset[2]]);
}


/**
 * This function calculates the quantile for a
 * gamma-distributed random variable.
 *
 * @brief Quantile of gamma probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
double Dist_gamma::quantile( double p ) {
    
    return RbStatistics::Gamma::quantile(*shape.value, *rate.value, p);
}


/**
 * This function generates a gamma-distributed
 * random variable.
 *
 * @brief Random draw from gamma distribution
 *
 * @return      Random draw from gamma distribution
 */
void Dist_gamma::rvSingleValue( std::vector<size_t> &offset ) {
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    randomVariable.value[offset[2]] = RbStatistics::Gamma::rv(shape.value[offset[0]], rate.value[offset[1]], *rng);
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_gamma::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    shape.value             = static_cast<double*>( p[0].value );
    shape.lengths           = p[0].lengths;
    
    rate.value              = static_cast<double*>( p[1].value );
    rate.lengths            = p[1].lengths;
    
    randomVariable.value    = static_cast<double*>( p[2].value );
    randomVariable.lengths  = p[2].lengths;
    
}



