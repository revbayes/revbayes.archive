/**
 * @file
 * This file contains the implementation of Dist_ctmm, which holds
 * the parameters and functions related to a distribution on
 * discrete states from a continuous-time discrete-state Markov
 * model.
 *
 * @brief Implementation of Dist_ctmm
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

#include "CharacterStateDiscrete.h"
#include "DAGNode.h"
#include "Dist_ctmm.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbNames.h"
#include "RealPos.h"
#include "ReferenceRule.h"
#include "Simplex.h"
#include "VectorString.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_ctmm::Dist_ctmm( void ) : DistributionDiscrete( getMemberRules() ) {
}


/** Constructor for internal use */
Dist_ctmm::Dist_ctmm( RateMatrix* Q, double v, CharacterStateDiscrete* a ) : DistributionDiscrete( getMemberRules() ) {

    setValue( "Q", Q );
    setValue( "v", new RealPos( v ) );
    setValue( "a", a );
}


/** Clone this object */
Dist_ctmm* Dist_ctmm::clone( void ) const {

    return new Dist_ctmm( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_ctmm::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_ctmm_name ) + DistributionDiscrete::getClass();
    return rbClass;
}


/** Get default move */
Move* Dist_ctmm::getDefaultMove( StochasticNode* node ) {

	// TODO: Add a default move
    return NULL;
}


/** Get member variable rules */
const MemberRules& Dist_ctmm::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules.push_back( new ReferenceRule( "Q", RateMatrix_name             ) );
        memberRules.push_back( new ReferenceRule( "v", RealPos_name                ) );
        memberRules.push_back( new ReferenceRule( "a", CharacterStateDiscrete_name ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_ctmm::getNumStates( void ) const {

    size_t numStates  = static_cast<const CharacterStateDiscrete*>( getValue( "a"  ) )->getNumStates();
    
    return numStates;
}


/** Get the probability mass vector */
const Simplex* Dist_ctmm::getProbabilityMassVector( void ) {

    // TODO:
    // Calculate the transition probability matrix
    // Calculate and return the probability mass vector on the states

    return NULL;
}


/** Get random variable type */
const TypeSpec Dist_ctmm::getVariableType( void ) const {

    return TypeSpec( CharacterStateDiscrete_name );
}


/**
 * This function calculates the natural log of the
 * probability of a random character state variable.
 *
 * @brief Natural log of probability of character state
 *
 * @param value Observed character state
 * @return      Natural log of the probability
 */
double Dist_ctmm::lnPdf( const RbObject* value ) {

    // Get the parameters
    const RateMatrix*             Q      = static_cast<const RateMatrix*            >( getValue( "Q" ) );
    double                        v      = static_cast<const RealPos*               >( getValue( "v" ) )->getValue();
    const CharacterStateDiscrete* start  = static_cast<const CharacterStateDiscrete*>( getValue( "a" ) );

    // Get the value
    const CharacterStateDiscrete* stop  = static_cast<const CharacterStateDiscrete* >( value );

    // TODO: Calculate log probability
    return 0.0;
}


/**
 * This function calculates the probability
 * of a random character state variable.
 *
 * @brief Probability of character state
 *
 * @param value Observed character state
 * @return      Probability
 */
double Dist_ctmm::pdf( const RbObject* value ) {

    // Get the parameters
    const RateMatrix*             Q      = static_cast<const RateMatrix*            >( getValue( "Q" ) );
    double                        v      = static_cast<const RealPos*               >( getValue( "v" ) )->getValue();
    const CharacterStateDiscrete* start  = static_cast<const CharacterStateDiscrete*>( getValue( "a" ) );

    // Get the value
    const CharacterStateDiscrete* stop  = static_cast<const CharacterStateDiscrete* >( value );

    // TODO: Calculate log probability
    return 1.0;
}


/**
 * This function generates a character state from the distribution
 *
 * @brief Random draw from distribution
 *
 * @return      Randomly drawn character state
 */
CharacterStateDiscrete* Dist_ctmm::rv( void ) {

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Get the parameters
    const RateMatrix*             Q      = static_cast<const RateMatrix*            >( getValue( "Q" ) );
    double                        v      = static_cast<const RealPos*               >( getValue( "v" ) )->getValue();
    const CharacterStateDiscrete* start  = static_cast<const CharacterStateDiscrete*>( getValue( "a" ) );

    // TODO: Draw a random character state
    return NULL;
}


/** We intercept a call to set a member variable to make sure that the number of states is consistent */
void Dist_ctmm::setVariable( const std::string& name, DAGNode* var ) {

    DistributionDiscrete::setVariable( name, var );

    if ( name == "Q" || name == "a" ) {
        
        const RateMatrix*             Q      = static_cast<const RateMatrix*            >( getValue( "Q" ) );
        const CharacterStateDiscrete* start  = static_cast<const CharacterStateDiscrete*>( getValue( "a" ) );

        if ( start->getNumStates() != Q->getNumStates() )
            throw RbException( "Starting state and rate matrix need to have the same number of states" );
    }
}

