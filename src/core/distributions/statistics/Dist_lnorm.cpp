/**
 * @file
 * This file contains the implementation of Dist_lnorm, which is used to hold
 * parameters and functions related to a lognormal distribution.
 *
 * @brief Implementation of Dist_lnorm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-08-17, version 1.0
 *
 * $Id$
 */

#include "DAGNode.h"
#include "Dist_lnorm.h"
#include "DistributionLognormal.h"
#include "MemberFunction.h"
#include "Move_mslide.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser use */
Dist_lnorm::Dist_lnorm( void ) : DistributionContinuous( getMemberRules() ), mu( NULL ), sigma( NULL ) {
	
}


/**
 * This function calculates the cumulative probability for a
 * lognormally-distributed random variable.
 *
 * @brief Lognormal cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_lnorm::cdf( const RbLanguageObject& value ) {
	
    double m    = static_cast<const Real&   >( mu->getValue()    ).getValue();
	double s    = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double q    = static_cast<const RealPos&>( value             ).getValue();
	
	return RbStatistics::Lognormal::cdf(m, s, q);
}


/** Clone this object */
Dist_lnorm* Dist_lnorm::clone( void ) const {
	
    return new Dist_lnorm( *this );
}


/** Get class name of object */
const std::string& Dist_lnorm::getClassName(void) { 
    
    static std::string rbClassName = "Lognormal distribution";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Dist_lnorm::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DistributionContinuous::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Dist_lnorm::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member variable rules */
const MemberRules& Dist_lnorm::getMemberRules( void ) const {
	
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
	
    if ( !rulesSet ) {
		
        memberRules.push_back( new ValueRule( "mu",    Real::getClassTypeSpec()    ) );
        memberRules.push_back( new ValueRule( "sigma", RealPos::getClassTypeSpec() ) );
		
        rulesSet = true;
    }
	
    return memberRules;
}


/** Get random variable type */
const TypeSpec& Dist_lnorm::getVariableType( void ) const {
	
    static TypeSpec varTypeSpec = RealPos::getClassTypeSpec();
    
    return varTypeSpec;
}


/**
 * This function calculates the natural log of the probability
 * density for a lognormally-distributed random variable.
 *
 * @brief Natural log of lognormal probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_lnorm::lnPdf(const RbLanguageObject& value) const {
	
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double x = static_cast<const RealPos&>( value             ).getValue();
	
    return RbStatistics::Lognormal::lnPdf(m, s, x);
}


/**
 * This function calculates the probability density
 * for a lognormally-distributed random variable.
 *
 * @brief Lognormal probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_lnorm::pdf( const RbLanguageObject& value ) const {
	
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double x = static_cast<const RealPos&>( value             ).getValue();
	
    return RbStatistics::Lognormal::pdf(m, s, x);
}


/**
 * This function calculates the quantile for a
 * normally-distributed random variable.
 *
 * @brief Quantile of normal probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
const Real& Dist_lnorm::quantile( const double p) {
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
	
    quant.setValue( RbStatistics::Lognormal::quantile(m, s, p) );
    
    return quant;
    
}


/**
 * This function generates a lognormally-distributed
 * random variable.
 *
 *
 * @brief Random draw from lognormal distribution
 *
 * @return      Random draw
 */
const RbLanguageObject& Dist_lnorm::rv(void) {
	
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
	
    RandomNumberGenerator* rng = GLOBAL_RNG;
	randomVariable.setValue( RbStatistics::Lognormal::rv(m ,s, *rng ) );
	
	return randomVariable;
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_lnorm::setMemberVariable(std::string const &name, const Variable *var) {
    
    if ( name == "mu" ) {
        mu = var;
    }
    else if ( name == "sigma" ){
        sigma = var;
    }
    else {
        DistributionContinuous::setMemberVariable(name, var);
    }
}



