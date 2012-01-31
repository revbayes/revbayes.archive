/**
 * @file
 * This file contains the implementation of Dist_lnorm, which is used to hold
 * parameters and functions related to a lognormal distribution.
 *
 * @brief Implementation of Dist_lnorm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-08-17, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z sibonli $
 */

#include "DAGNode.h"
#include "Dist_lnorm.h"
#include "DistributionLognormal.h"
#include "Move_mslide.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>


// Definition of the static type spec member
const TypeSpec Dist_lnorm::typeSpec(Dist_lnorm_name);
const TypeSpec Dist_lnorm::varTypeSpec(Real_name);

/** Constructor for parser use */
Dist_lnorm::Dist_lnorm( void ) : DistributionContinuous( getMemberRules() ) {
	
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
double Dist_lnorm::cdf( RbPtr<const RbLanguageObject> value ) {
	
    double m    = static_cast<      Real*   >( (RbObject*)getMemberValue( "m" ) )->getValue();
	double s    = static_cast<      RealPos*>( (RbObject*)getMemberValue( "s" ) )->getValue();
    double q    = static_cast<const Real*   >( (const RbLanguageObject*)value           )->getValue();
	
	return RbStatistics::Lognormal::cdf(m, s, q);
}


/** Clone this object */
Dist_lnorm* Dist_lnorm::clone( void ) const {
	
    return new Dist_lnorm( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_lnorm::getClass( void ) const {
	
    static VectorString rbClass = VectorString( Dist_lnorm_name ) + DistributionContinuous::getClass();
    return rbClass;
}


/** Get member variable rules */
RbPtr<const MemberRules> Dist_lnorm::getMemberRules( void ) const {
	
    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;
	
    if ( !rulesSet ) {
		
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "m", Real_name    ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "s"  , RealPos_name ) ) );
		
        rulesSet = true;
    }
	
    return RbPtr<const MemberRules>( memberRules );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_lnorm::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_lnorm::getVariableType( void ) const {
	
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
double Dist_lnorm::lnPdf(const RbLanguageObject *value) const {
	
    double m = static_cast<const Real*   >( (const RbObject*)getMemberValue( "m" ) )->getValue();
    double s = static_cast<const RealPos*>( (const RbObject*)getMemberValue( "s" ) )->getValue();
    double x = static_cast<const Real*   >( value )->getValue();
	
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
double Dist_lnorm::pdf( const RbLanguageObject *value ) const {
	
    double m = static_cast<const Real*   >( (const RbObject*)getMemberValue( "m" ) )->getValue();
    double s = static_cast<const RealPos*>( (const RbObject*)getMemberValue( "s" ) )->getValue();
    double x = static_cast<const Real*   >( value )->getValue();
	
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
RbPtr<Real> Dist_lnorm::quantile( const double p) {
	double m = static_cast<Real*   >( (RbObject*)getMemberValue( "m" ) )->getValue();
    double s = static_cast<RealPos*>( (RbObject*)getMemberValue( "s" ) )->getValue();
	
    return RbPtr<Real>(new Real (RbStatistics::Lognormal::quantile(m, s, p) ) );
    
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
RbPtr<RbLanguageObject> Dist_lnorm::rv(void) {
	
    double m = static_cast<Real*   >( (RbObject*)getMemberValue( "m" ) )->getValue();
    double s = static_cast<RealPos*>( (RbObject*)getMemberValue( "s" ) )->getValue();
	
    RbPtr<RandomNumberGenerator> rng = GLOBAL_RNG;
	//std::vector<double> r(m.size());
	
	return RbPtr<RbLanguageObject>( new Real (RbStatistics::Lognormal::rv(m ,s, rng ) ) );
}


