/**
 * @file
 * This file contains the implementation of Dist_logis, which is used to hold
 * parameters and functions related to a Logistic distribution.
 *
 * @brief Implementation of Dist_logis
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
#include "Dist_logis.h"
#include "Move_msimplex.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "DistributionLogistic.h"
#include "Real.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "VectorRealPos.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>

#include "MatrixReal.h"
#include "RbMathMatrix.h"


// Definition of the static type spec member
const TypeSpec Dist_logis::typeSpec(Dist_logis_name);
const TypeSpec Dist_logis::varTypeSpec(Real_name);

/** Default constructor for parser use */
Dist_logis::Dist_logis( void ) : DistributionContinuous( getMemberRules() ) {

}



/** Clone this object */
Dist_logis* Dist_logis::clone( void ) const {

    return new Dist_logis( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_logis::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_logis_name ) + DistributionContinuous::getClass();
    return rbClass;
}


/** Get member variable rules */
const MemberRules& Dist_logis::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ){
        memberRules.push_back( new ValueRule( "location",  Real_name    ) );
        memberRules.push_back( new ValueRule( "scale",     RealPos_name ) );
        rulesSet = true;
    }

    return memberRules;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_logis::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_logis::getVariableType( void ) const {

    return varTypeSpec;
}

/**
 * This function calculates the probability density
 * for a Logistic-distributed random variable.
 *
 * @brief Logistic probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_logis::pdf( const RbLanguageObject& value ) const {
    
    double location =   static_cast<const Real&     >( getMemberValue("location")   ).getValue();
    double scale =      static_cast<const RealPos&  >( getMemberValue("scale")      ).getValue();
    double x =          static_cast<const Real&     >( value ).getValue();

	return RbStatistics::Logistic::pdf( location, scale, x );

}



/**
 * This function calculates the natural log of the probability
 * density for a logistic-distributed random variable.
 *
 * @brief Natural log of logistic probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_logis::lnPdf( const RbLanguageObject& value ) const {

	// Get the value and the parameters of the Logistic
    double location =   static_cast<const Real&     >( getMemberValue("location")   ).getValue();
    double scale =      static_cast<const RealPos&  >( getMemberValue("scale")      ).getValue();
    double x =          static_cast<const Real&     >( value ).getValue();

	return RbStatistics::Logistic::lnPdf( location, scale, x );
}


/**
 * This function calculates the cumulative probability for
 * a Logistic-distributed random variable.
 *
 * @brief Logistic cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_logis::cdf( const RbLanguageObject& value ) {

    double location =   static_cast<      Real&     >( getMemberValue("location")   ).getValue();
    double scale =      static_cast<      RealPos&  >( getMemberValue("scale")      ).getValue();
    double x =          static_cast<const Real&     >( value                        ).getValue();

	return RbStatistics::Logistic::cdf( location, scale, x );
}



/**
 * This function calculates the quantile for a
 * Logistic-distributed random variable.
 *
 * @brief Quantile of Logistic probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
Real* Dist_logis::quantile( const double p ) {

    double location =   static_cast<Real&     >( getMemberValue("location")   ).getValue();
    double scale =      static_cast<RealPos&  >( getMemberValue("scale")      ).getValue();

	double q = RbStatistics::Logistic::quantile(location, scale, p);
	return new Real(q);

}


/**
 * This function generates a Logistic-distributed
 * random variable.
 *
 * @brief Random draw from Logistic distribution
 *
 * @return      Random draw from Logistic distribution
 */



RbLanguageObject* Dist_logis::rv(void) {

    double location =   static_cast<Real&     >( getMemberValue("location") ).getValue();
    double scale =      static_cast<RealPos&  >( getMemberValue("scale")    ).getValue();

    RandomNumberGenerator* rng = GLOBAL_RNG;
	return new Real( RbStatistics::Logistic::rv(location, scale, *rng) );

}

