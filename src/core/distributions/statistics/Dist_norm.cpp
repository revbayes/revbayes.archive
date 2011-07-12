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
#include "RbNames.h"
#include "ReferenceRule.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser use */
Dist_norm::Dist_norm( void ) : DistributionContinuous( getMemberRules() ) {

}


/** Constructor for internal use */
Dist_norm::Dist_norm( double mu, double sigma ) : DistributionContinuous( getMemberRules() ) {

    setValue( "mean",  new Real   ( mu    ) );
    setValue( "sigma", new RealPos( sigma ) );
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
double Dist_norm::cdf( const RbObject* value ) {

    const Real*    mu    = static_cast<const Real*   >( getValue("mean") );
    const RealPos* sigma = static_cast<const RealPos*>( getValue("sd"  ) );
    const Real*    q     = static_cast<const Real*   >( value            );

    double p;
    double z = ( (*q) - (*mu) ) / (*sigma);

    /* |X| <= 1.28 */
    if ( fabs(z) <= 1.28 ) {
        double a1 = 0.398942280444;
        double a2 = 0.399903438504;
        double a3 = 5.75885480458;
        double a4 = 29.8213557808;
        double a5 = 2.62433121679;
        double a6 = 48.6959930692;
        double a7 = 5.92885724438;
        double y = 0.5 * z * z;
        p = 0.5 - fabs(z) * ( a1 - a2 * y / ( y + a3 - a4 / ( y + a5 + a6 / ( y + a7 ) ) ) );
    }
    else if ( fabs(z) <= 12.7 ) {
        double b0 = 0.398942280385;
        double b1 = 3.8052E-08;
        double b2 = 1.00000615302;
        double b3 = 3.98064794E-04;
        double b4 = 1.98615381364;
        double b5 = 0.151679116635;
        double b6 = 5.29330324926;
        double b7 = 4.8385912808;
        double b8 = 15.1508972451;
        double b9 = 0.742380924027;
        double b10 = 30.789933034;
        double b11 = 3.99019417011;
        double y = 0.5 * z * z;
        p = exp(-y) * b0 / (fabs(z) - b1 + b2 / (fabs(z) + b3 + b4 / (fabs(z) - b5 + b6 / (fabs(z) + b7 - b8 / (fabs(z) + b9 + b10 / (fabs(z) + b11))))));
    }
    else {
        p = 0.0;
    }

    if ( z < 0.0 )
        return p;
    else
        return 1.0 - p;
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


/** Get default move; TODO: Normal move is more natural default move */
Move* Dist_norm::getDefaultMove( StochasticNode* node ) {

    const RealPos* sd = static_cast<const RealPos*>( getValue("sd") );

    return new Move_mslide( node, (*sd) / 100.0, 1.0 );
}


/** Get member variable rules */
const MemberRules& Dist_norm::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ReferenceRule( "mean", Real_name    ) );
        memberRules.push_back( new ReferenceRule( "sd"  , RealPos_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get random variable type */
const TypeSpec Dist_norm::getVariableType( void ) const {

    return TypeSpec( Real_name );
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
double Dist_norm::lnPdf(const RbObject* value) {

    double mu    = static_cast<const Real*   >( getValue( "mean" ) )->getValue();
    double sigma = static_cast<const RealPos*>( getValue( "sd"   ) )->getValue();
    double x     = static_cast<const Real*   >( value              )->getValue();

    double z = ( x - mu ) / sigma;

    return ( -0.5 * z * z ) - 0.5 * std::log ( 2.0 * RbConstants::PI * sigma );
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
double Dist_norm::pdf( const RbObject* value ) {

    double mu    = static_cast<const Real*   >( getValue( "mean" ) )->getValue();
    double sigma = static_cast<const RealPos*>( getValue( "sd"   ) )->getValue();
    double x     = static_cast<const Real*   >( value              )->getValue();

    double z = (x - mu) / sigma;

    return exp( -0.5 * z * z ) / ( sigma * sqrt ( 2.0 * RbConstants::PI ) );
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
Real* Dist_norm::quantile( const double p) {

    double mu    = static_cast<const Real*   >( getValue( "mean" ) )->getValue();
    double sigma = static_cast<const RealPos*>( getValue( "sd"   ) )->getValue();

    double a0 = -0.322232431088;
    double a1 = -1.0;
    double a2 = -0.342242088547;
    double a3 = -0.0204231210245;
    double a4 = -0.453642210148e-4;
    double b0 = 0.0993484626060;
    double b1 = 0.588581570495;
    double b2 = 0.531103462366; 
    double b3 = 0.103537752850; 
    double b4 = 0.0038560700634;

    double p1 = ( p < 0.5 ? p : 1.0 - p);
    if ( p1 < 1e-20 ) 
       throw RbException( "Probability of normal quantile out of range" );

    double y = sqrt( log(1.0/(p1*p1)) );   
    double z = y + ((((y*a4+a3)*y+a2)*y+a1)*y+a0) / ((((y*b4+b3)*y+b2)*y+b1)*y+b0);

    if ( p < 0.5 )
        z = -z;

    return new Real( z * sigma + mu );
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
Real* Dist_norm::rv(void) {

    static bool   availableNormalRv = false;
    static double extraNormalRv;

    if ( availableNormalRv ) {
        availableNormalRv = false;
        return new Real( extraNormalRv );
    }

    double mu    = static_cast<const Real*   >( getValue( "mean" ) )->getValue();
    double sigma = static_cast<const RealPos*>( getValue( "sd"   ) )->getValue();

    RandomNumberGenerator* rng = GLOBAL_RNG;

    double v1  = 0.0;
    double v2  = 0.0;
    double rsq = 0.0;
    do {
        v1 = 2.0 * rng->uniform01() - 1.0;
        v2 = 2.0 * rng->uniform01() - 1.0;
        rsq = v1 * v1 + v2 * v2;
    } while ( rsq >= 1.0 || rsq == 0.0 );

    double fac = sqrt( -2.0 * log( rsq ) / rsq );

    extraNormalRv = ( mu + sigma * ( v1 * fac ) );
    availableNormalRv = true;

    return new Real( mu + sigma * (v2 * fac) );
}


