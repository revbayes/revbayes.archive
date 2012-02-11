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
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z msuchard $
 */

#include "DAGNode.h"
#include "Dist_gamma.h"
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
#include "DistributionGamma.h"
#include "RbStatisticsHelper.h"

#include <cmath>
#include <sstream>


// Definition of the static type spec member
const TypeSpec Dist_gamma::typeSpec(Dist_gamma_name);
const TypeSpec Dist_gamma::varTypeSpec(RealPos_name);

/** Default constructor for parser use */
Dist_gamma::Dist_gamma( void ) : DistributionContinuous( getMemberRules() ) {
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
double Dist_gamma::cdf( const RbLanguageObject& value ) {
    
    double shape  = static_cast<      RealPos&>(getMemberValue("shape") ).getValue();
    double lambda = static_cast<      RealPos&>(getMemberValue("rate")  ).getValue();
    double x      = static_cast<const RealPos&>(value                   ).getValue();    

    return RbStatistics::Gamma::cdf(shape, lambda, x);
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

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ValueRule( "shape", RealPos_name) );
        memberRules.push_back( new ValueRule( "rate", RealPos_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_gamma::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_gamma::getVariableType( void ) const {

    return varTypeSpec;
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
double Dist_gamma::lnPdf( const RbLanguageObject& value ) const {
    
    double shape  = static_cast<const RealPos&>(getMemberValue("shape") ).getValue();
    double lambda = static_cast<const RealPos&>(getMemberValue("rate")  ).getValue();
    double x      = static_cast<const RealPos&>(value ).getValue();  
  
    return RbStatistics::Gamma::lnPdf(shape, lambda, x); 
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
double Dist_gamma::pdf( const RbLanguageObject& value ) const {
    
    double shape  = static_cast<const RealPos&>(getMemberValue("shape") ).getValue();
    double lambda = static_cast<const RealPos&>(getMemberValue("rate")  ).getValue();
    double x      = static_cast<const RealPos&>( value ).getValue();   
    
    return RbStatistics::Gamma::pdf(shape, lambda, x, false);    
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
Real* Dist_gamma::quantile(const double p) {
    
    double shape  = static_cast<      RealPos&>(getMemberValue("shape") ).getValue();
    double lambda = static_cast<      RealPos&>(getMemberValue("rate")  ).getValue();
    
    double quantile = RbStatistics::Gamma::quantile(shape, lambda, p);     
    return new RealPos(quantile);
}


/**
 * This function generates a gamma-distributed
 * random variable.
 *
 * @brief Random draw from gamma distribution
 *
 * @return      Random draw from gamma distribution
 */
RbLanguageObject* Dist_gamma::rv( void ) {
    
    double shape  = static_cast<      RealPos&>(getMemberValue("shape") ).getValue();
    double lambda = static_cast<      RealPos&>(getMemberValue("rate")  ).getValue();
    
    RandomNumberGenerator* rng = GLOBAL_RNG;        
    double rv = RbStatistics::Gamma::rv(shape, lambda, *rng);
    return new RealPos(rv);
}



