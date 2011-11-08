/**
 * @file
 * This file contains the implementation of Dist_beta, which is used to hold
 * parameters and functions related to an beta distribution.
 *
 * @brief Implementation of Dist_beta
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z msuchard $
 */

#include "DAGNode.h"
#include "Dist_beta.h"
#include "Move_mscale.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbUtil.h"
#include "Real.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "DistributionBeta.h"
#include "RbStatisticsHelper.h"

#include <cmath>
#include <sstream>


// Definition of the static type spec member
const TypeSpec Dist_beta::typeSpec(Dist_beta_name);
const TypeSpec Dist_beta::varTypeSpec(RealPos_name);

/** Default constructor for parser use */
Dist_beta::Dist_beta( void ) : DistributionContinuous( getMemberRules() ) {
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
double Dist_beta::cdf( RbPtr<const RbObject> value ) {

    double shape1 = static_cast<RealPos*>((RbObject*)getMemberValue("shape1"))->getValue();
    double shape2 = static_cast<RealPos*>((RbObject*)getMemberValue("shape2"))->getValue();
    double x      = static_cast<const RealPos*>((const RbObject*)value)->getValue();

    return RbStatistics::Beta::cdf(shape1, shape2, x);
}


/** Clone this object */
Dist_beta* Dist_beta::clone( void ) const {

    return new Dist_beta( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_beta::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_beta_name ) + DistributionContinuous::getClass();
    return rbClass;
}

/** Get member variable rules */
RbPtr<const MemberRules> Dist_beta::getMemberRules( void ) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "shape1", RealPos_name) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "shape2", RealPos_name ) ) );

        rulesSet = true;
    }

    return RbPtr<const MemberRules>( memberRules );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_beta::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_beta::getVariableType( void ) const {

    return varTypeSpec;
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
double Dist_beta::lnPdf( RbPtr<const RbObject> value ) const {

    double shape1 = static_cast<const RealPos*>((const RbObject*)getMemberValue("shape1"))->getValue();
    double shape2 = static_cast<const RealPos*>((const RbObject*)getMemberValue("shape2"))->getValue();
    double x      = static_cast<const RealPos*>((const RbObject*)value)->getValue();

    return RbStatistics::Beta::lnPdf(shape1, shape2, x);
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
double Dist_beta::pdf( RbPtr<const RbObject> value ) const {
    
    double shape1 = static_cast<const RealPos*>((const RbObject*)getMemberValue("shape1"))->getValue();
    double shape2 = static_cast<const RealPos*>((const RbObject*)getMemberValue("shape2"))->getValue();
    double x      = static_cast<const RealPos*>((const RbObject*)value)->getValue();

    return RbStatistics::Beta::pdf(shape1, shape2, x);
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
RbPtr<Real> Dist_beta::quantile(const double p) {

    double shape1 = static_cast<RealPos*>((RbObject*)getMemberValue("shape1"))->getValue();
    double shape2 = static_cast<RealPos*>((RbObject*)getMemberValue("shape2"))->getValue();

    double quantile = RbStatistics::Beta::quantile(shape1, shape2, p);
    return RbPtr<Real>( new RealPos(quantile) );
}


/**
 * This function generates a beta-distributed
 * random variable.
 *
 * @brief Random draw from beta distribution
 *
 * @return      Random draw from beta distribution
 */
RbPtr<RbObject> Dist_beta::rv( void ) {

    double shape1 = static_cast<RealPos*>((RbObject*)getMemberValue("shape1"))->getValue();
    double shape2 = static_cast<RealPos*>((RbObject*)getMemberValue("shape2"))->getValue();

    RbPtr<RandomNumberGenerator> rng = GLOBAL_RNG;
    double rv = RbStatistics::Beta::rv(shape1, shape2, rng);
    //TODO implement RbMath::isFinite
    return RbPtr<RbObject>( new RealPos(rv) );
}



