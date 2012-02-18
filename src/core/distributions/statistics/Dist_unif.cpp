/**
 * @file
 * This file contains the implementation of Dist_unif, which is used to hold
 * parameters and functions related to a uniform distribution.
 *
 * @brief Implementation of Dist_unif
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

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Dist_unif.h"
#include "Move_mslide.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>


// Definition of the static type spec member
const TypeSpec Dist_unif::typeSpec(Dist_unif_name);
const TypeSpec Dist_unif::varTypeSpec(Real_name);

/** Default constructor for parser use */
Dist_unif::Dist_unif(void) : DistributionContinuous(getMemberRules()), min( TypeSpec( Real_name ) ), max( TypeSpec( Real_name ) ) {

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
double Dist_unif::cdf(const RbLanguageObject& value) {

    double l   = static_cast<      Real&>( min.getValue() ).getValue();
    double u   = static_cast<      Real&>( max.getValue() ).getValue();
    double q   = static_cast<const Real&>( value          ).getValue();

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


/** Get class vector showing type of object */
const VectorString& Dist_unif::getClass(void) const {

    static VectorString rbClass = VectorString(Dist_unif_name) + DistributionContinuous::getClass();
    return rbClass;
}


/** Get max value of distribution */
const Real& Dist_unif::getMax( void ) const {

    return static_cast<const Real&>( max.getValue() );
}


/** Get member variable rules */
const MemberRules& Dist_unif::getMemberRules(void) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back( new ValueRule( "min", Real_name ) );
        memberRules.push_back( new ValueRule( "max", Real_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get min value of distribution */
const Real& Dist_unif::getMin( void ) const {

    return static_cast<const Real&>( min.getValue() );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_unif::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_unif::getVariableType( void ) const {

    return varTypeSpec;
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
double Dist_unif::lnPdf(const RbLanguageObject& value) const {
    
    double l   = static_cast<const Real&>( min.getValue() ).getValue();
    double u   = static_cast<const Real&>( max.getValue() ).getValue();
    double x   = static_cast<const Real&>( value ).getValue();

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
double Dist_unif::pdf(const RbLanguageObject &value) const {
    
    double l   = static_cast<const Real&>( min.getValue() ).getValue();
    double u   = static_cast<const Real&>( max.getValue() ).getValue();
    double x   = static_cast<const Real&>( value ).getValue();

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
const Real& Dist_unif::quantile(const double p) {
    
    double l = static_cast<Real&>( min.getValue() ).getValue();
    double u = static_cast<Real&>( max.getValue() ).getValue();

    quant.setValue( l + ( u - l ) * p );
    return quant;
}


/**
 * This function generates a uniformly-distributed
 * random variable.
 *
 * @brief Random draw from uniform distribution
 *
 * @return      Random draw from uniform distribution
 */
const RbLanguageObject& Dist_unif::rv(void) {
    
    double l = static_cast<const Real&>( min.getValue() ).getValue();
    double u = static_cast<const Real&>( max.getValue() ).getValue();
    RandomNumberGenerator* rng = GLOBAL_RNG;

    double d = rng->uniform01();
    randomVariable.setValue( l + ( u - l ) * d );

    return randomVariable;
}


