/**
 * @file
 * This file contains the implementation of Dist_dirichlet, which is used to hold
 * parameters and functions related to a Dirichlet distribution.
 *
 * @brief Implementation of Dist_dirichlet
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
#include "Dist_dirichlet.h"
#include "Move_msimplex.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbNames.h"
#include "DistributionDirichlet.h"
#include "Real.h"
#include "ReferenceRule.h"
#include "Simplex.h"
#include "VectorRealPos.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_dirichlet::Dist_dirichlet( void ) : DistributionInterval( getMemberRules() ) {

}


/** Constructor for internal use */
Dist_dirichlet::Dist_dirichlet( std::vector<double> a ) : DistributionInterval( getMemberRules() ) {

    setValue( "alpha", new VectorRealPos(a) );
}


/**
 * This function calculates the cumulative probability for
 * a Dirichlet-distributed random variable.
 *
 * @brief Dirichlet cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_dirichlet::cdf( const RbObject* value ) {

	/* TO DO: We should implement the cumulative probability for the Dirichlet. The most recent
	   algorithms are discussed in:
	   
	   Gouda, A. A., and T. Szantai. 2010. On numerical calculation of probabilities according 
	   to Dirichlet distribution. Ann. Oper. Res. 177:185–200. 
       
       Also, note that implementing the CDF here will require some changes to the object
       hierarchy; The cdf function is declared as pure virtual in the base classes of this object,
       and the pure virtual base class definition only takes in a single double. To properly 
       implement the cdf of the Dirichlet, however, you need to pass in a vector (a simplex)
       of values. */

    throw RbException( "Cdf function of Dirichlet distribution not implemented yet" );
}


/** Clone this object */
Dist_dirichlet* Dist_dirichlet::clone( void ) const {

    return new Dist_dirichlet( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_dirichlet::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_dirichlet_name ) + DistributionInterval::getClass();
    return rbClass;
}


/** Get default move */
Move* Dist_dirichlet::getDefaultMove( StochasticNode* node ) {

	// Default move for a stochastic node having a Dirichlet distribution
    return new Move_msimplex( node, 300.0, 4, 1.0 );
}


/** Get member variable rules */
const MemberRules& Dist_dirichlet::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules.push_back( new ReferenceRule( "alpha", VectorRealPos_name ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get random variable type */
const TypeSpec Dist_dirichlet::getVariableType( void ) const {

    return TypeSpec( Simplex_name );
}

/**
 * This function calculates the natural log of the probability
 * density for a Dirichlet-distributed random variable.
 *
 * @brief Natural log of Dirichlet probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_dirichlet::lnPdf( const RbObject* value ) {

	// Get the value and the parameters of the Dirichlet
    std::vector<double> a = static_cast<const VectorReal*>( getValue("alpha") )->getValue();
    std::vector<double> x = static_cast<const Simplex*   >( value             )->getValue();

	// Check that the vectors are both the same size
	if ( a.size() != x.size() )
		throw RbException( "Inconsistent size of vectors when calculating Dirichlet log probability density" );

	return RbStatistics::Dirichlet::lnPdf( a, x );
}

/**
 * This function calculates the probability density
 * for a Dirichlet-distributed random variable.
 *
 * @brief Dirichlet probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_dirichlet::pdf( const RbObject* value ) {

	// Get the value and the parameters of the Dirichlet
    std::vector<double> a = static_cast<const VectorReal*>( getValue( "alpha" ) )->getValue();
    std::vector<double> x = static_cast<const Simplex*   >( value               )->getValue();

	// Check that the vectors are both the same size
	if ( a.size() != x.size() )
		throw RbException( "Inconsistent size of vectors when calculating Dirichlet log probability density" );

	return RbStatistics::Dirichlet::pdf( a, x );
}

/**
 * This function calculates the quantile for a
 * Dirichlet-distributed random variable.
 *
 * @brief Quantile of Dirichlet probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
Simplex* Dist_dirichlet::quantile( const double p ) {

    /* TO DO: See notes (above) on the cdf function for a Dirichlet rv. */
    throw RbException( "Quantile function of Dirichlet distribution not implemented yet" );
}


/**
 * This function generates a Dirichlet-distributed
 * random variable.
 *
 * @brief Random draw from Dirichlet distribution
 *
 * @return      Random draw from Dirichlet distribution
 */
Simplex* Dist_dirichlet::rv( void ) {

    std::vector<double> a      = static_cast<const VectorReal*>( getValue("alpha") )->getValue();
    RandomNumberGenerator* rng = GLOBAL_RNG;
	std::vector<double> r(a.size());

	r = RbStatistics::Dirichlet::rv( a, rng );
    return new Simplex( r );
}

