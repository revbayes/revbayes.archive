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

/** Default constructor for parser use */
Dist_unif::Dist_unif(void) : DistributionContinuous(getMemberRules()) {

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
double Dist_unif::cdf(const RbLanguageObject* value) {

    double min = static_cast<const Real*>( getMemberValue("min") )->getValue();
    double max = static_cast<const Real*>( getMemberValue("max") )->getValue();
    double q   = static_cast<const Real*>( value           )->getValue();

    if ( q < min )
        return 0.0;
    else if ( q > max )
        return 1.0;

    return ( q - min ) / ( max - min );
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
const Real* Dist_unif::getMax( void ) const {

    return static_cast<const Real*>( getMemberValue("max") );
}


/** Get member variable rules */
const MemberRules& Dist_unif::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back(new ValueRule("min", Real_name));
        memberRules.push_back(new ValueRule("max", Real_name));

        rulesSet = true;
    }

    return memberRules;
}


/** Get min value of distribution */
const Real* Dist_unif::getMin( void ) const {

    return static_cast<const Real*>( getMemberValue("min") );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_unif::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec Dist_unif::getVariableType( void ) const {

    return TypeSpec( Real_name );
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
double Dist_unif::lnPdf(const RbLanguageObject* value) {

    double min = ((Real*) getMemberValue("min"))->getValue();
    double max = ((Real*) getMemberValue("max"))->getValue();
    double x   = ((Real*) value)->getValue();

    if ( x < min || x > max )
        return RbConstants::Double::neginf;

    return -std::log( max - min );
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
double Dist_unif::pdf(const RbLanguageObject* value) {

    double min = ((Real*) getMemberValue("min"))->getValue();
    double max = ((Real*) getMemberValue("max"))->getValue();
    double x   = ((Real*) value)->getValue();

    if ( x < min || x > max )
        return 0.0;

    return 1.0 / ( max - min );
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
Real* Dist_unif::quantile(const double p) {

    double min = static_cast<const Real*>( getMemberValue("min") )->getValue();
    double max = static_cast<const Real*>( getMemberValue("max") )->getValue();

    return new Real( min + ( max - min ) * p );
}


/**
 * This function generates a uniformly-distributed
 * random variable.
 *
 * @brief Random draw from uniform distribution
 *
 * @return      Random draw from uniform distribution
 */
Real* Dist_unif::rv(void) {

    double                 min = ((Real*) getMemberValue("min"))->getValue();
    double                 max = ((Real*) getMemberValue("max"))->getValue();
    RandomNumberGenerator* rng = GLOBAL_RNG;

    double u = rng->uniform01();

    return new Real( min + ( max - min ) * u );
}


