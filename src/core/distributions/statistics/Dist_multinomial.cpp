/**
 * @file
 * This file contains the implementation of Dist_multinomial, which is used to hold
 * parameters and functions related to a multinomial distribution.
 *
 * @brief Implementation of Dist_multinomial
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
#include "Dist_multinomial.h"
#include "DistributionMultinomial.h"
#include "Move_mmultinomial.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "Real.h"
#include "RbException.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "Simplex.h"
#include "VectorInteger.h"
#include "VectorReal.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_multinomial::Dist_multinomial( void ) : DistributionContinuous( getMemberRules() ) {
}


/** Constructor for internal use */
Dist_multinomial::Dist_multinomial( std::vector<double> a ) : DistributionContinuous( getMemberRules() ) {

    setValue( "p", new Simplex( a ) );
}


/** Cdf function */
double Dist_multinomial::cdf( const RbObject* value ) {

    throw RbException( "Cdf function of multinomial not implemented yet" );
}


/** Clone this object */
Dist_multinomial* Dist_multinomial::clone( void ) const {

    return new Dist_multinomial( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_multinomial::getClass( void ) const {

    static VectorString rbClass = VectorString(Dist_multinomial_name) + DistributionContinuous::getClass();
    return rbClass;
}


/** Get default move */
Move* Dist_multinomial::getDefaultMove( StochasticNode* node ) {

	// Default move for a stochastic node having a Dirichlet distribution
    return new Move_mmultinomial( node, 300.0, 4, 1.0 );
}


/** Get member variable rules */
const MemberRules& Dist_multinomial::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules.push_back( new ReferenceRule( "p", Simplex_name ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get random variable type */
const TypeSpec Dist_multinomial::getVariableType( void ) const {

    return TypeSpec( Natural_name, 1 );
}


/**
 * This function calculates the natural log of the probability
 * density for a multinomially-distributed random variable.
 *
 * @brief Natural log of multinomial probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_multinomial::lnPdf( const RbObject* value ) {

	// Get the value and the parameters of the Dirichlet
    std::vector<double> p = static_cast<const Simplex*      >( getValue( "p" ) )->getValue();
    std::vector<int   > x = static_cast<const VectorNatural*>( value           )->getValue();

	// Check that the vectors are both the same size
	if ( p.size() != x.size() )
		throw RbException( "Inconsistent size of vectors when calculating Dirichlet log probability density" );

	return RbStatistics::Multinomial::lnPdf( p, x );
}


/**
 * This function calculates the probability
 * for a multinomially-distributed random variable.
 *
 * @brief Multinomial probability
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_multinomial::pdf( const RbObject* value ) {

	// Get the value and the parameters of the Dirichlet
    std::vector<double> p = static_cast<const Simplex*      >( getValue( "p" ) )->getValue();
    std::vector<int   > x = static_cast<const VectorNatural*> (value           )->getValue();

	// check that the vectors are both the same size
	if ( p.size() != x.size() )
		throw RbException( "Inconsistent size of vectors when calculating Dirichlet log probability density" );

	return RbStatistics::Multinomial::pdf( p, x );
}


/** Quantile function */
VectorNatural* Dist_multinomial::quantile(const double p) {

    throw RbException( "Quantile function of multinomial not implemented yet" );
}


/**
 * This function generates an multinomial-distributed
 * random variable.
 *
 * @brief Random draw from multinomial distribution
 *
 * @return      Random draw from multinomial distribution
 */
VectorNatural* Dist_multinomial::rv( void ) {

    std::vector<double> p      = static_cast<const Simplex*>( getValue( "p" ) )->getValue();
    RandomNumberGenerator* rng = GLOBAL_RNG;
	std::vector<int>       r( p.size() );

	r = RbStatistics::Multinomial::rv( p, rng );
    return new VectorNatural( r );
}

