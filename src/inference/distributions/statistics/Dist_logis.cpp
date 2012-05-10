/**
 * @file
 * This file contains the implementation of Dist_logis, which is used to hold
 * parameters and functions related to a Logistic distribution.
 *
 * @brief Implementation of Dist_logis
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
#include "Dist_logis.h"
#include "Move_msimplex.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "DistributionLogistic.h"
#include "Real.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>

#include "RbMathMatrix.h"


/** Default constructor for parser use */
Dist_logis::Dist_logis( void ) : DistributionContinuous(  ) {

}



double Dist_logis::cdf(double q) {
    
	return RbStatistics::Logistic::cdf(*location.value, *scale.value, q);
    
}


/** Clone this object */
Dist_logis* Dist_logis::clone( void ) const {

    return new Dist_logis( *this );
}


/**
 * This function calculates the probability density
 * for a Logistic-distributed random variable.
 *
 * @brief Logistic probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_logis::pdf( const RbLanguageObject& value ) const {
    
    double l = static_cast<const Real&     >( location->getValue() ).getValue();
    double s = static_cast<const RealPos&  >( scale->getValue()    ).getValue();
    double x = static_cast<const Real&     >( value               ).getValue();

	return RbStatistics::Logistic::pdf( l, s, x );

}



/**
 * This function calculates the natural log of the probability
 * density for a logistic-distributed random variable.
 *
 * @brief Natural log of logistic probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_logis::lnPdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Logistic::lnPdf(location.value[offset[0]], scale.value[offset[1]], randomVariable.value[offset[2]]);
}


/**
 * This function calculates the cumulative probability for
 * a Logistic-distributed random variable.
 *
 * @brief Logistic cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_logis::pdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Logistic::pdf(location.value[offset[0]], scale.value[offset[1]], randomVariable.value[offset[2]]);
}



/**
 * This function calculates the quantile for a
 * Logistic-distributed random variable.
 *
 * @brief Quantile of Logistic probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
double Dist_logis::quantile( double p ) {
    
    return RbStatistics::Logistic::quantile(*location.value, *scale.value, p);
}


/**
 * This function generates a Logistic-distributed
 * random variable.
 *
 * @brief Random draw from Logistic distribution
 *
 * @return      Random draw from Logistic distribution
 */
void Dist_logis::rvSingleValue( std::vector<size_t> &offset ) {
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    randomVariable.value[offset[2]] = RbStatistics::Logistic::rv(location.value[offset[0]], scale.value[offset[1]], *rng);
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_logis::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    location.value          = ( static_cast<double*>( p[0].value ) );
    location.lengths        = p[0].lengths;
    
    scale.value             = ( static_cast<double*>( p[1].value ) );
    scale.lengths           = p[1].lengths;
    
    randomVariable.value    = static_cast<double*>( p[2].value );
    randomVariable.lengths  = p[2].lengths;
    
}

