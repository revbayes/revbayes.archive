/**
 * @file
 * This file contains the implementation of Dist_exp, which is used to hold
 * parameters and functions related to an exponential distribution.
 *
 * @brief Implementation of Dist_exp
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
#include "Dist_exp.h"
#include "DistributionExponential.h"
#include "MemberFunction.h"
#include "RealPos.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbUtil.h"
#include "Real.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>


/** Default constructor for parser use */
Dist_exp::Dist_exp( void ) : InferenceDistributionContinuous() {

}


/**
 * This function calculates the cumulative probability for
 * an exponentially-distributed random variable.
 *
 * @brief Exponential cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_exp::cdf(double q) {
    
	return RbStatistics::Exponential::cdf(*lambda.value, q);
    
}


/** Clone this object */
Dist_exp* Dist_exp::clone( void ) const {

    return new Dist_exp( *this );
}


/**
 * This function calculates the natural log of the probability
 * density for an exponentially-distributed random variable.
 *
 * @brief Natural log of exponential probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_exp::lnPdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Exponential::lnPdf(lambda.value[offset[0]], randomVariable.value[offset[1]]);
}


/**
 * This function calculates the probability density
 * for an exponentially-distributed random variable.
 *
 * @brief Exponential probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_exp::pdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Exponential::pdf(lambda.value[offset[0]], randomVariable.value[offset[1]]);
}


/**
 * This function calculates the quantile for an
 * exponentially-distributed random variable.
 *
 * @brief Quantile of exponential probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
double Dist_exp::quantile(const double p) {

    double quant = -( 1.0 / *lambda.value ) * std::log( 1.0 - p );
    return quant;
}


/**
 * This function generates an exponentially-distributed
 * random variable.
 *
 * @brief Random draw from exponential distribution
 *
 * @return      Random draw from exponential distribution
 */
void Dist_exp::rvSingleValue( std::vector<size_t> &offset ) {
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    randomVariable.value[offset[1]] = RbStatistics::Exponential::rv(lambda.value[offset[0]], *rng);
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_exp::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    lambda.value            = ( static_cast<double*>( p[0].value ) );
    lambda.lengths          = p[0].lengths;
    
    randomVariable.value    = static_cast<double*>( p[1].value );
    randomVariable.lengths  = p[1].lengths;
    
}


