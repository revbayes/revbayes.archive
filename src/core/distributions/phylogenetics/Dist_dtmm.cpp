/**
 * @file
 * This file contains the implementation of Dist_dtmm, which holds
 * the parameters and functions related to a distribution on
 * discrete states from a discrete-time discrete-state Markov
 * model.
 *
 * @brief Implementation of Dist_dtmm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-12-15, version 1.0
 *
 * $Id$
 */

#include "CharacterStateDiscrete.h"
#include "DAGNode.h"
#include "Dist_dtmm.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
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
const TypeSpec Dist_dtmm::typeSpec(Dist_dtmm_name);
const TypeSpec Dist_dtmm::varTypeSpec(CharacterStateDiscrete_name);

/** Default constructor for parser use */
Dist_dtmm::Dist_dtmm( void ) : DistributionDiscrete( getMemberRules() ) {
}


/** Clone this object */
Dist_dtmm* Dist_dtmm::clone( void ) const {
    
    return new Dist_dtmm( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_dtmm::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Dist_dtmm_name ) + DistributionDiscrete::getClass();
    return rbClass;
}



/** Get member variable rules */
RbPtr<const MemberRules> Dist_dtmm::getMemberRules( void ) const {
    
    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;
    
    if ( !rulesSet ) {
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "m", TransitionProbabilityMatrix_name             ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "a", CharacterStateDiscrete_name ) ) );
        
        rulesSet = true;
    }
    
    return RbPtr<const MemberRules>( memberRules );
}


/** Get the number of states in the distribution */
size_t Dist_dtmm::getNumberOfStates( void ) const {
    
    size_t numStates  = static_cast<const CharacterStateDiscrete*>( (const RbObject*)getMemberValue( "a"  ) )->getNumberOfStates();
    
    return numStates;
}


/** Get the probability mass vector */
RbPtr<Simplex> Dist_dtmm::getProbabilityMassVector( void ) {
    
    // get the information from the arguments for reading the file
    RbPtr<TransitionProbabilityMatrix>  m = RbPtr<TransitionProbabilityMatrix>( static_cast<TransitionProbabilityMatrix*>( (RbObject*)(*members)[0]->getValue() ) );
    const CharacterStateDiscrete*       c = static_cast<const CharacterStateDiscrete*>(    (RbObject*)(*members)[1]->getValue() );
    
    // initialize the number of states
    const size_t stateIndex = c->getUnsignedValue();
    
    RbPtr<VectorReal> probs = (*m)[stateIndex];
    
    //
    return RbPtr<Simplex>( new Simplex(*probs) );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_dtmm::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_dtmm::getVariableType( void ) const {
    
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
double Dist_dtmm::lnPdf( RbPtr<const RbLanguageObject> value ) const {
    
    // Get the parameters
    RbPtr<const TransitionProbabilityMatrix>    m      = RbPtr<const TransitionProbabilityMatrix>( static_cast<const TransitionProbabilityMatrix*>( (const RbObject*)getMemberValue( "m" ) ) );
    RbPtr<const CharacterStateDiscrete>         start  = RbPtr<const CharacterStateDiscrete     >( static_cast<const CharacterStateDiscrete*     >( (const RbObject*)getMemberValue( "a" ) ) );
    RbPtr<const CharacterStateDiscrete>         stop   = RbPtr<const CharacterStateDiscrete     >( static_cast<const CharacterStateDiscrete*     >( (const RbLanguageObject*)value ) );
    
    // calculate the transition probability matrix
        
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
double Dist_dtmm::pdf( RbPtr<const RbLanguageObject> value ) const {
    
    // Get the parameters
    RbPtr<const TransitionProbabilityMatrix>    m      = RbPtr<const TransitionProbabilityMatrix>( static_cast<const TransitionProbabilityMatrix*>( (const RbObject*)getMemberValue( "m" ) ) );
    RbPtr<const CharacterStateDiscrete>         start  = RbPtr<const CharacterStateDiscrete     >( static_cast<const CharacterStateDiscrete*     >( (const RbObject*)getMemberValue( "a" ) ) );
    RbPtr<const CharacterStateDiscrete>         stop   = RbPtr<const CharacterStateDiscrete     >( static_cast<const CharacterStateDiscrete*     >( (const RbLanguageObject*)value ) );
        
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
RbPtr<RbLanguageObject> Dist_dtmm::rv( void ) {
    
    // Get the rng
    RbPtr<RandomNumberGenerator> rng = GLOBAL_RNG;
    
    // Get the parameters
    RbPtr<const TransitionProbabilityMatrix>    m      = RbPtr<const TransitionProbabilityMatrix>( static_cast<const TransitionProbabilityMatrix*>( (const RbObject*)getMemberValue( "m" ) ) );
    RbPtr<const CharacterStateDiscrete>         start  = RbPtr<const CharacterStateDiscrete     >( static_cast<const CharacterStateDiscrete*     >( (const RbObject*)getMemberValue( "a" ) ) );
    
    RbPtr<CharacterStateDiscrete> draw( start->clone() );
    std::vector<bool> startState = start->getStateVector();
    size_t indexStart=0;
    for (std::vector<bool>::iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
        // test whether the state is set
        if (*itStart) {
            RbPtr<const VectorReal> probs = (*m)[indexStart];
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
    
    return RbPtr<RbLanguageObject>( draw );
}


/** We intercept a call to set a member variable to make sure that the number of states is consistent */
void Dist_dtmm::setMemberVariable( const std::string& name, RbPtr<Variable> var ) {
    
    DistributionDiscrete::setMemberVariable( name, var );
    
}

