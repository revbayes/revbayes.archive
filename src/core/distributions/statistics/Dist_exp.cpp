/**
 * @file
 * This file contains the implementation of Dist_exp, which is used to hold
 * parameters and functions related to an exponential distribution.
 *
 * @brief Implementation of Dist_exp
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
#include "Dist_exp.h"
#include "Move_mscale.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbUtil.h"
#include "Real.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>


/** Default constructor for parser use */
Dist_exp::Dist_exp( void ) : DistributionContinuous( getMemberRules() ) {

}


/** Constructor for internal use */
Dist_exp::Dist_exp( double rate ) : DistributionContinuous( getMemberRules() ) {

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
double Dist_exp::cdf( const RbLanguageObject* value ) {

    const RealPos* lambda = static_cast<const RealPos*>( getMemberValue("rate") );
    const RealPos* q      = static_cast<const RealPos*>( value            );

    return 1.0 - std::exp( - (*lambda) * (*q) );
}


/** Clone this object */
Dist_exp* Dist_exp::clone( void ) const {

    return new Dist_exp( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_exp::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_exp_name ) + DistributionContinuous::getClass();
    return rbClass;
}

/** Get member variable rules */
const MemberRules& Dist_exp::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ValueRule( "rate", RealPos_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get random variable type */
const TypeSpec Dist_exp::getVariableType( void ) const {

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
double Dist_exp::lnPdf( const RbLanguageObject* value ) {

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
double Dist_exp::pdf( const RbLanguageObject* value ) {

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
RealPos* Dist_exp::quantile(const double p) {

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
RealPos* Dist_exp::rv( void ) {

    const RealPos*         lambda = static_cast<const RealPos*>( getMemberValue("rate") );
    RandomNumberGenerator* rng    = GLOBAL_RNG;

    double u = rng->uniform01();

    return new RealPos( -( 1.0 / (*lambda) ) * std::log( u ) );
}

