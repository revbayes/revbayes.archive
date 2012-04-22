/**
 * @file
 * This file contains the implementation of Dist_unif, which is used to hold
 * parameters and functions related to a uniform distribution.
 *
 * @brief Implementation of Dist_unif
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

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Dist_unif.h"
#include "MemberFunction.h"
#include "RealPos.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>


/** Default constructor for parser use */
Dist_unif::Dist_unif(void) : InferenceDistributionContinuous() {

}


/**
 * This function calculates the cumulative probability for
 * a uniformly-distributed random variable.
 *
 * @brief Exponential cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_unif::cdf(double q) {

    double l   = *min.value;
    double u   = *max.value;

    if ( q < l )
        return 0.0;
    else if ( q > u )
        return 1.0;

    return ( q - l ) / ( u - l );
}


/** Clone this object */
Dist_unif* Dist_unif::clone(void) const {

    return new Dist_unif(*this);
}


/** Get max value of distribution */
double Dist_unif::getMax( void ) const {

    if (max.value == NULL) {
        return 1.0;
    }
    else {
        return *max.value;
    }
}


/** Get min value of distribution */
double Dist_unif::getMin( void ) const {
    
    if (min.value == NULL) {
        return 0.0;
    }
    else {
        return *min.value;
    }
}


/**
 * This function calculates the natural log of the probability
 * density for a uniformly-distributed random variable.
 *
 * @brief Natural log of uniform probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_unif::lnPdfSingleValue( std::vector<size_t> &offset ) const {
    
    double l   = min.value[offset[0]];
    double u   = max.value[offset[1]];
    double x   = randomVariable.value[offset[2]];

    if ( x < l || x > u )
        return RbConstants::Double::neginf;

    return -std::log( u - l );
}


/**
 * This function calculates the probability density
 * for a uniformly-distributed random variable.
 *
 * @brief Exponential probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_unif::pdfSingleValue( std::vector<size_t> &offset ) const {
    
    double l   = min.value[offset[0]];
    double u   = max.value[offset[1]];
    double x   = randomVariable.value[offset[2]];

    if ( x < l || x > u )
        return 0.0;

    return 1.0 / ( u - l );
}


/**
 * This function calculates the quantile for an
 * uniformly-distributed random variable.
 *
 * @brief Quantile of uniform probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
double Dist_unif::quantile(double p) {
    
    double l   = *min.value;
    double u   = *max.value;

    double q = l + ( u - l ) * p ;
    return q;
}


/**
 * This function generates a uniformly-distributed
 * random variable.
 *
 * @brief Random draw from uniform distribution
 *
 * @return      Random draw from uniform distribution
 */
void Dist_unif::rvSingleValue( std::vector<size_t> &offset ) {

    double l = min.value[offset[0]];
    double u = max.value[offset[1]];
    RandomNumberGenerator* rng = GLOBAL_RNG;

    double d = rng->uniform01();
    randomVariable.value[offset[2]] = l + ( u - l ) * d;

}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_unif::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    min.value               = ( static_cast<double*>( p[0].value ) );
    min.lengths             = p[0].lengths;
    
    max.value               = ( static_cast<double*>( p[1].value ) );
    max.lengths             = p[1].lengths;
    
    randomVariable.value    = static_cast<double*>( p[2].value );
    randomVariable.lengths  = p[2].lengths;
    
}


