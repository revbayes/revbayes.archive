/**
 * @file
 * This file contains the implementation of Dist_norm, which is used to hold
 * parameters and functions related to a normal distribution.
 *
 * @brief Implementation of Dist_norm
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
#include "Dist_norm.h"
#include "Move_mslide.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"
#include "DistributionNormal.h"

#include <cmath>


// Definition of the static type spec member
const TypeSpec Dist_norm::typeSpec(Dist_norm_name);
const TypeSpec Dist_norm::varTypeSpec(Real_name);

/** Constructor for parser use */
Dist_norm::Dist_norm( void ) : DistributionContinuous( getMemberRules() ), mu( NULL ), sigma( NULL ) {

}


/**
 * This function calculates the cumulative probability for a
 * normally-distributed random variable.
 *
 * @brief Normal cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 * @see Adams, A. G. 1969. Areas under the normal curve. Computer J. 12:197-198.
 */
double Dist_norm::cdf( const RbLanguageObject& value ) {

	double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double q = static_cast<const Real&   >( value            ).getValue();
	return RbStatistics::Normal::cdf(m, s, q);

}


/** Clone this object */
Dist_norm* Dist_norm::clone( void ) const {

    return new Dist_norm( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_norm::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_norm_name ) + DistributionContinuous::getClass();
    return rbClass;
}


/** Get member variable rules */
const MemberRules& Dist_norm::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ValueRule( "mean", Real_name   , new Real(0.0)    ) );
        memberRules.push_back( new ValueRule( "sd"  , RealPos_name, new RealPos(1.0) ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_norm::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_norm::getVariableType( void ) const {

    return varTypeSpec;
}


/**
 * This function calculates the natural log of the probability
 * density for a normally-distributed random variable.
 *
 * @brief Natural log of normal probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_norm::lnPdf(const RbLanguageObject& value) const {
    
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double x = static_cast<const Real&   >( value            ).getValue();


    return RbStatistics::Normal::lnPdf(m, s, x);

}


/**
 * This function calculates the probability density
 * for a normally-distributed random variable.
 *
 * @brief Normal probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_norm::pdf( const RbLanguageObject& value ) const {
    
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double x = static_cast<const Real&   >( value            ).getValue();

    return RbStatistics::Normal::pdf(m, s, x);

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
 * @see Odeh, R. E. and J. O. Evans. 1974. The percentage points of the normal
 *      distribution. Applied Statistics, 22:96-97.
 * @see Wichura, M. J.  1988. Algorithm AS 241: The percentage points of the
 *      normal distribution. 37:477-484.
 * @see Beasley, JD & S. G. Springer. 1977. Algorithm AS 111: The percentage
 *      points of the normal distribution. 26:118-121.
 */
const Real& Dist_norm::quantile( const double p) {
    
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
	
    quant.setValue( RbStatistics::Normal::quantile(m, s, p) );
    
    return quant;
}


/**
 * This function generates a normally-distributed
 * random variable.
 *
 * @todo What algorithm is this? It is not Box-Muller, not Ziggurat. It does
 *       not resemble the R code, which uses Ahrens & Dieter and Kinderman & Ramage.
 *       Code for extra normal rv based on guesswork using comments in RbStatistics.cpp.
 *
 * @brief Random draw from normal distribution
 *
 * @return      Random draw
 */
const RbLanguageObject& Dist_norm::rv(void) {

    static bool   availableNormalRv = false;
    static double extraNormalRv;

    if ( availableNormalRv ) {
        availableNormalRv = false;
        randomVariable.setValue( extraNormalRv );
        return randomVariable;
    }
    
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();

    RandomNumberGenerator* rng = GLOBAL_RNG;
    randomVariable.setValue( RbStatistics::Normal::rv(m, s, *rng) );
    
	return randomVariable;

}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_norm::setMemberVariable(std::string const &name, Variable *var) {
    
    if ( name == "mean" ) {
        mu = var;
    }
    else if ( name == "sd" ) {
        sigma = var;
    }
    else {
        DistributionContinuous::setMemberVariable(name, var);
    }
}

