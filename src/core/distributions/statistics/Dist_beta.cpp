/**
 * @file
 * This file contains the implementation of Dist_beta, which is used to hold
 * parameters and functions related to an beta distribution.
 *
 * @brief Implementation of Dist_beta
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
#include "Dist_beta.h"
#include "Move_mscale.h"
#include "Probability.h"
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


/** Default constructor for parser use */
Dist_beta::Dist_beta( void ) : DistributionContinuous( getMemberRules() ), alpha( NULL ), beta( NULL ) {
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
double Dist_beta::cdf( const RbLanguageObject& value ) {

    double shape1 = static_cast<RealPos&>( alpha->getValue() ).getValue();
    double shape2 = static_cast<RealPos&>( beta->getValue()  ).getValue();
    double x      = static_cast<const RealPos&>(value).getValue();

    return RbStatistics::Beta::cdf(shape1, shape2, x);
}


/** Clone this object */
Dist_beta* Dist_beta::clone( void ) const {

    return new Dist_beta( *this );
}


/** Get class name of object */
const std::string& Dist_beta::getClassName(void) { 
    
    static std::string rbClassName = "Beta distribution";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Dist_beta::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DistributionContinuous::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Dist_beta::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member variable rules */
const MemberRules& Dist_beta::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ValueRule( "alpha", RealPos::getClassTypeSpec() ) );
        memberRules.push_back( new ValueRule( "beta",  RealPos::getClassTypeSpec() ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get random variable type */
const TypeSpec& Dist_beta::getVariableType( void ) const {

    static TypeSpec varTypeSpec = Probability::getClassTypeSpec();
    
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
double Dist_beta::lnPdf( const RbLanguageObject& value ) const {

    double shape1 = static_cast<const RealPos&    >( alpha->getValue() ).getValue();
    double shape2 = static_cast<const RealPos&    >( beta->getValue()  ).getValue();
    double x      = static_cast<const Probability&>( value            ).getValue();

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
double Dist_beta::pdf( const RbLanguageObject& value ) const {
    
    double shape1 = static_cast<const RealPos&    >( alpha->getValue() ).getValue();
    double shape2 = static_cast<const RealPos&    >( beta->getValue()  ).getValue();
    double x      = static_cast<const Probability&>( value            ).getValue();

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
const Real& Dist_beta::quantile(const double p) {

    double shape1 = static_cast<RealPos&>( alpha->getValue() ).getValue();
    double shape2 = static_cast<RealPos&>( beta->getValue()  ).getValue();

    double quantile = RbStatistics::Beta::quantile(shape1, shape2, p);
    quant.setValue( quantile );
    return quant;
}


/**
 * This function generates a beta-distributed
 * random variable.
 *
 * @brief Random draw from beta distribution
 *
 * @return      Random draw from beta distribution
 */
const RbLanguageObject& Dist_beta::rv( void ) {

    double shape1 = static_cast<RealPos&>( alpha->getValue() ).getValue();
    double shape2 = static_cast<RealPos&>( beta->getValue()  ).getValue();

    RandomNumberGenerator* rng = GLOBAL_RNG;
    double rv = RbStatistics::Beta::rv(shape1, shape2, *rng);
    //TODO implement RbMath::isFinite
    randomVariable.setValue( rv );
    return randomVariable;
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_beta::setMemberVariable(std::string const &name, Variable *var) {
    
    if ( name == "alpha" ) {
        alpha = var;
    }
    else if ( name == "beta" ){
        beta = var;
    }
    else {
        DistributionContinuous::setMemberVariable(name, var);
    }
}




