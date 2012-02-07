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
#include "RbUtil.h"
#include "RealPos.h"
#include "Simplex.h"
#include "TransitionProbabilityMatrix.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cmath>
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Dist_ctmm::typeSpec(Dist_ctmm_name);
const TypeSpec Dist_ctmm::varTypeSpec(CharacterStateDiscrete_name);

/** Default constructor for parser use */
Dist_ctmm::Dist_ctmm( void ) : DistributionDiscrete( getMemberRules() ) {
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



/** Get member variable rules */
const MemberRules* Dist_ctmm::getMemberRules( void ) const {

    static MemberRules* memberRules = new MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ) {
        memberRules->push_back( new ValueRule( "Q", RateMatrix_name             ) );
        memberRules->push_back( new ValueRule( "v", RealPos_name                ) );
        memberRules->push_back( new ValueRule( "a", CharacterStateDiscrete_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_ctmm::getNumberOfStates( void ) const {

    size_t numStates  = static_cast<const CharacterStateDiscrete*>( getMemberValue( "a"  ) )->getNumberOfStates();
    
    return numStates;
}


/** Get the probability mass vector */
Simplex* Dist_ctmm::getProbabilityMassVector( void ) {

    // get the information from the arguments for reading the file
    RateMatrix*                q = static_cast<RateMatrix*>( (*members)[0]->getValue() );
    RealPos*                   t = static_cast<RealPos*>( (*members)[1]->getValue() );
//    const CharacterStateDiscrete*    c = static_cast<const CharacterStateDiscrete*>(    members[2].getValue() );
    
    // initialize the number of states
    const size_t nStates = q->getNumberOfStates();
    
    // check that the number of states isn't 1
    if ( nStates < 2 )
    {
        std::stringstream o;
        o << "Too few states for the rate matrix";
        throw( RbException(o.str()) );
    }
    
    // construct a rate matrix of the correct dimensions
    TransitionProbabilityMatrix* m = new TransitionProbabilityMatrix(nStates);
    
    // calculate the transition probabilities    
    q->calculateTransitionProbabilities( t->getValue(), m );
    
    //
    return NULL; // TODO Return vector
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_ctmm::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_ctmm::getVariableType( void ) const {

    return varTypeSpec;
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
double Dist_ctmm::lnPdf( const RbLanguageObject *value ) const {

    // Get the parameters
    const RateMatrix*             Q      = static_cast<const RateMatrix*            >( getMemberValue( "Q" ) );
    double                        t      = static_cast<const RealPos*               >( getMemberValue( "v" ) )->getValue();
    const CharacterStateDiscrete* start  = static_cast<const CharacterStateDiscrete*>( getMemberValue( "a" ) );
    const CharacterStateDiscrete* stop   = static_cast<const CharacterStateDiscrete*>( value );
    
    // calculate the transition probability matrix
    
    // initialize the number of states
    size_t nStates = Q->getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    TransitionProbabilityMatrix* m = new TransitionProbabilityMatrix(nStates);
    
    // calculate the transition probabilities    
    Q->calculateTransitionProbabilities( t, m );
    
    double lnprob = 0.0;
    std::vector<bool> startState = start->getStateVector();
    std::vector<bool> stopState  = stop->getStateVector();
    size_t indexStart=0;
    size_t indexEnd=0;
    for (std::vector<bool>::iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
        // test whether the state is set
        if (*itStart) {
            for (std::vector<bool>::iterator itStop=stopState.begin(); itStop!=stopState.end(); itStop++, indexEnd++) {
                // test whether the state is set
                if (*itStop) {
                    lnprob += log((*(*m)[indexStart])[indexEnd]);
                }
            }
        }
    }
    
    return lnprob;
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
double Dist_ctmm::pdf( const RbLanguageObject *value ) const {

    // Get the parameters
    const RateMatrix*             Q      = static_cast<const RateMatrix*            >( getMemberValue( "Q" ) );
    double                        t      = static_cast<const RealPos*               >( getMemberValue( "v" ) )->getValue();
    const CharacterStateDiscrete* start  = static_cast<const CharacterStateDiscrete*>( getMemberValue( "a" ) );
    const CharacterStateDiscrete* stop   = static_cast<const CharacterStateDiscrete*>( value );

    // calculate the transition probability matrix
    
    // initialize the number of states
    size_t nStates = Q->getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    TransitionProbabilityMatrix* m = new TransitionProbabilityMatrix(nStates);
    
    // calculate the transition probabilities    
    Q->calculateTransitionProbabilities( t, m );
    
    double prob = 1.0;
    std::vector<bool> startState = start->getStateVector();
    std::vector<bool> stopState  = stop->getStateVector();
    size_t indexStart=0;
    size_t indexEnd=0;
    for (std::vector<bool>::iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
        // test whether the state is set
        if (*itStart) {
            for (std::vector<bool>::iterator itStop=stopState.begin(); itStop!=stopState.end(); itStop++, indexEnd++) {
                // test whether the state is set
                if (*itStop) {
                    prob *= ((*(*m)[indexStart])[indexEnd]);
                }
            }
        }
    }

    return prob;
}


/**
 * This function generates a character state from the distribution
 *
 * @brief Random draw from distribution
 *
 * @return      Randomly drawn character state
 */
RbLanguageObject* Dist_ctmm::rv( void ) {

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Get the parameters
    RateMatrix*             Q      = static_cast<RateMatrix*            >( getMemberValue( "Q" ) );
    double                  t      = static_cast<const RealPos*         >( getMemberValue( "v" ) )->getValue();
    CharacterStateDiscrete* start  = static_cast<CharacterStateDiscrete*>( getMemberValue( "a" ) );
    
    // calculate the transition probability matrix
    
    // initialize the number of states
    const size_t nStates = Q->getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    TransitionProbabilityMatrix* m = new TransitionProbabilityMatrix(nStates);
    
    // calculate the transition probabilities    
    Q->calculateTransitionProbabilities( t, m );
    
    CharacterStateDiscrete* draw = start->clone();
    std::vector<bool> startState = start->getStateVector();
    size_t indexStart=0;
    for (std::vector<bool>::iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
        // test whether the state is set
        if (*itStart) {
            VectorReal* probs = (*m)[indexStart];
            double u = rng->uniform01();
            for (size_t i=0; i<probs->size(); i++) {
                u -= (*probs)[i];
                if (u <= 0) {
                    std::vector<bool> values = std::vector<bool>(start->getNumberOfStates());
                    values[i] = true;
                    draw->setValue(values);
                    break;
                }
            }
        }
    }
    
    return draw;
}


/** We intercept a call to set a member variable to make sure that the number of states is consistent */
void Dist_ctmm::setMemberVariable( const std::string& name, Variable* var ) {

    DistributionDiscrete::setMemberVariable( name, var );

    // we cannot do the following because first only one variable is set and hence the following code crashes
    // nevertheless a test like that might be useful
//    if ( name == "Q" || name == "a" ) {
//        
//        const RateMatrix*             Q      = static_cast<const RateMatrix*            >( getMemberValue( "Q" ) );
//        const CharacterStateDiscrete* start  = static_cast<const CharacterStateDiscrete*>( getMemberValue( "a" ) );
//
//        if ( start->getNumberOfStates() != Q->getNumberOfStates() )
//            throw RbException( "Starting state and rate matrix need to have the same number of states" );
//    }
}

