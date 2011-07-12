/**
 * @file
 * This file contains the implementation of Dist_topologyunif, which holds
 * the parameters and functions related to a uniform distribution on
 * topologies (unrooted or rooted, binary or polytomous).
 *
 * @brief Implementation of Dist_topologyunif
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

#include "Boolean.h"
#include "DAGNode.h"
#include "Dist_topologyunif.h"
#include "Integer.h"
#include "Natural.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbNames.h"
#include "Simplex.h"
#include "Topology.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_topologyunif::Dist_topologyunif( void ) : DistributionDiscrete( getMemberRules() ) {

    // Precalculate probability of topology
    calculateTopologyProb();
}


/** Constructor for internal use */
Dist_topologyunif::Dist_topologyunif( int numTaxa, bool isRooted, bool isBinary ) : DistributionDiscrete( getMemberRules() ) {

    setValue( "numTaxa", new Natural( numTaxa ) );
    setValue( "isRooted", new Boolean( isRooted ) );
    setValue( "isBinary", new Boolean( isBinary ) );

    // Precalculate probability of topology
    calculateTopologyProb();
}



/** Precalculate the probability and log probability of a topology drawn from the distribution. */
void Dist_topologyunif::calculateTopologyProb( void ) {

    // TODO: Calculate probabilities of a topology drawn from the distribution
}


/** Clone this object */
Dist_topologyunif* Dist_topologyunif::clone( void ) const {

    return new Dist_topologyunif( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_topologyunif::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_topologyunif_name ) + DistributionDiscrete::getClass();
    return rbClass;
}


/** Get default move */
Move* Dist_topologyunif::getDefaultMove( StochasticNode* node ) {

	// TODO: Add a default move
    return NULL;
}


/** Get member variable rules */
const MemberRules& Dist_topologyunif::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules.push_back( new ValueRule( "numTaxa" , Natural_name      ) );
        memberRules.push_back( new ValueRule( "isRooted", Boolean_name      ) );
        memberRules.push_back( new ValueRule( "isBinary", new Boolean(true) ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
int Dist_topologyunif::getNumStates( void ) const {

    // TODO: Calculate number of distinct topologies
    return -1;
}


/** Get the probability mass vector */
const Simplex* Dist_topologyunif::getProbabilityMassVector( void ) {

    int             numTaxa  = static_cast<const Natural*>( getValue( "numTaxa"  ) )->getValue();

    if ( numTaxa <= 6 )
        return new Simplex( getNumStates() );
    else
        throw RbException( "Too many topologies to generate probability mass vector" );
}


/** Get random variable type */
const TypeSpec Dist_topologyunif::getVariableType( void ) const {

    return TypeSpec( Topology_name );
}


/**
 * This function calculates the natural log of the
 * probability of a random topology variable.
 *
 * @brief Natural log of probability of topology
 *
 * @param value Observed topology
 * @return      Natural log of the probability
 */
double Dist_topologyunif::lnPdf( const RbObject* value ) {

    // Get the parameters
    const Topology* top      = static_cast<const Topology*    >( value );
    int             numTaxa  = static_cast<const Natural*>( getValue( "numTaxa"  ) )->getValue();
    bool            isRooted = static_cast<const Boolean*>( getValue( "isRooted" ) )->getValue();
    bool            isBinary = static_cast<const Boolean*>( getValue( "isBinary" ) )->getValue();

    // If numTaxa, isRooted and isBinary fits top, then return precalculated probability stored in the class
    // Otherwise return negative infinity
    bool match = false;
    if ( top->getNumberOfNodes() == numTaxa && top->getIsRooted() == isRooted && top->getIsBinary() == isBinary )
        return lnTopologyProb;
    else
        return RbConstants::Double::neginf;
}


/**
 * This function calculates the probability
 * of a categorical random variable.
 *
 * @brief Categorical probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_topologyunif::pdf( const RbObject* value ) {

    // Get the parameters
    const Topology* top      = static_cast<const Topology*    >( value );
    int             numTaxa  = static_cast<const Natural*>( getValue( "numTaxa"  ) )->getValue();
    bool            isRooted = static_cast<const Boolean*>( getValue( "isRooted" ) )->getValue();
    bool            isBinary = static_cast<const Boolean*>( getValue( "isBinary" ) )->getValue();

    // If numTaxa, isRooted and isBinary fits top, then return precalculated probability stored in the class
    // Otherwise return 0.0
    bool match = false;
    if ( top->getNumberOfNodes() == numTaxa && top->getIsRooted() == isRooted && top->getIsBinary() == isBinary )
        return topologyProb;
    else
        return 0.0;
}


/**
 * This function generates a topology from the distribution
 *
 * @brief Random draw from distribution
 *
 * @return      Randomly drawn topology
 */
Topology* Dist_topologyunif::rv( void ) {

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Get the parameters
    int             numTaxa  = static_cast<const Natural*>( getValue( "numTaxa"  ) )->getValue();
    bool            isRooted = static_cast<const Boolean*>( getValue( "isRooted" ) )->getValue();
    bool            isBinary = static_cast<const Boolean*>( getValue( "isBinary" ) )->getValue();

    // TODO: Draw a random topology
    return NULL;
}


/** We intercept a call to set a member variable to make sure that the topology prob gets recalculated */
void Dist_topologyunif::setVariable( const std::string& name, DAGNode* var ) {

    DistributionDiscrete::setVariable( name, var );

    calculateTopologyProb();
}

