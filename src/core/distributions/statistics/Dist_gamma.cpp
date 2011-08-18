/**
 * @file
 * This file contains the implementation of Dist_gamma, which is used to hold
 * parameters and functions related to an gamma distribution.
 *
 * @brief Implementation of Dist_gamma
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#include "DAGNode.h"
#include "Dist_gamma.h"
#include "Move_mscale.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbNames.h"
#include "Real.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>


/** Default constructor for parser use */
Dist_gamma::Dist_gamma( void ) : DistributionContinuous( getMemberRules() ) {

}


/** Constructor for internal use */
Dist_gamma::Dist_gamma( double rate ) : DistributionContinuous( getMemberRules() ) {

//    setMemberValue( "rate", new Real(rate) );
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
double Dist_gamma::cdf( const RbLanguageObject* value ) {

    const RealPos* lambda = static_cast<const RealPos*>( getMemberValue("rate") );
    const RealPos* q      = static_cast<const RealPos*>( value            );

    return 1.0 - std::exp( - (*lambda) * (*q) );
}


/** Clone this object */
Dist_gamma* Dist_gamma::clone( void ) const {

    return new Dist_gamma( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_gamma::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_gamma_name ) + DistributionContinuous::getClass();
    return rbClass;
}

/** Get member variable rules */
const MemberRules& Dist_gamma::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ValueRule( "rate", RealPos_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get random variable type */
const TypeSpec Dist_gamma::getVariableType( void ) const {

    return TypeSpec( RealPos_name );
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
double Dist_gamma::lnPdf( const RbLanguageObject* value ) {

    double lambda = static_cast<const RealPos*>( getMemberValue( "rate" ) )->getValue();
    double x      = static_cast<const RealPos*>( value              )->getValue();

    return std::log(lambda) -lambda * x;
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
double Dist_gamma::pdf( const RbLanguageObject* value ) {

    double lambda = static_cast<const RealPos*>( getMemberValue( "rate" ) )->getValue();
    double x      = static_cast<const RealPos*>( value              )->getValue();

    return lambda * std::exp( -lambda * x );
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
RealPos* Dist_gamma::quantile(const double p) {

    const RealPos* lambda = static_cast<const RealPos*>( getMemberValue( "rate" ) );

    return new RealPos( -( 1.0 / (*lambda) ) * std::log( 1.0 - p ) );
}


/**
 * This function generates an exponentially-distributed
 * random variable.
 *
 * @brief Random draw from exponential distribution
 *
 * @return      Random draw from exponential distribution
 */
RealPos* Dist_gamma::rv( void ) {

    const RealPos*         lambda = static_cast<const RealPos*>( getMemberValue("rate") );
    RandomNumberGenerator* rng    = GLOBAL_RNG;

    double u = rng->uniform01();

    return new RealPos( -( 1.0 / (*lambda) ) * std::log( u ) );
}

