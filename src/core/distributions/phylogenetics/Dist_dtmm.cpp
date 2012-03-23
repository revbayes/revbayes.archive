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

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_dtmm::Dist_dtmm( void ) : DistributionDiscrete( getMemberRules() ), transProbabilityMatrix( NULL ), initialState( NULL ) {
}


/** Clone this object */
Dist_dtmm* Dist_dtmm::clone( void ) const {
    
    return new Dist_dtmm( *this );
}


/** Get class name of object */
const std::string& Dist_dtmm::getClassName(void) { 
    
    static std::string rbClassName = "Discrete time Markov model";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Dist_dtmm::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DistributionDiscrete::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Dist_dtmm::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get member variable rules */
const MemberRules& Dist_dtmm::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if ( !rulesSet ) {
        memberRules.push_back( new ValueRule( "m", TransitionProbabilityMatrix::getClassTypeSpec()) );
        memberRules.push_back( new ValueRule( "a", CharacterStateDiscrete::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_dtmm::getNumberOfStates( void ) const {
    
    size_t numStates  = static_cast<const CharacterStateDiscrete&>( initialState->getValue() ).getNumberOfStates();
    
    return numStates;
}


/** Get the probability mass vector */
const Simplex& Dist_dtmm::getProbabilityMassVector( void ) {
    
    // get the information from the arguments for reading the file
    const TransitionProbabilityMatrix&  m = static_cast<const TransitionProbabilityMatrix&>( transProbabilityMatrix->getValue() );
    const CharacterStateDiscrete&       c = static_cast<const CharacterStateDiscrete&>( initialState->getValue() );
    
    // initialize the number of states
    const size_t stateIndex = c.getUnsignedValue();
    
    const RbVector<Real>& probs = m[stateIndex];
    
    //
    probMassVector.setValue( probs );
    return probMassVector;
}


/** Get the state vector for this distribution */
const std::vector<RbLanguageObject*>& Dist_dtmm::getStateVector( void ) const {
    return stateVector;
}


/** Get random variable type */
const TypeSpec& Dist_dtmm::getVariableType( void ) const {
    
    static TypeSpec varTypeSpec = CharacterStateDiscrete::getClassTypeSpec();
    
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
double Dist_dtmm::lnPdf( const RbLanguageObject& value ) const {
    
    // Get the parameters
    const TransitionProbabilityMatrix&    m      = static_cast<const TransitionProbabilityMatrix&>( transProbabilityMatrix->getValue() );
    const CharacterStateDiscrete&         start  = static_cast<const CharacterStateDiscrete&     >( initialState->getValue() );
    const CharacterStateDiscrete&         stop   = static_cast<const CharacterStateDiscrete&     >( value );
    
//    std::cerr << "Computing lnPdf of DTMM with:\t";
//    start.printValue(std::cerr);
//    std::cerr << " <-> ";
//    stop.printValue(std::cerr);
//    std::cerr << std::endl;
    
    // calculate the transition probability matrix
        
    double lnprob = 0.0;
    const std::vector<bool>& startState = start.getStateVector();
    const std::vector<bool>& stopState  = stop.getStateVector();
    size_t indexStart=0;
    size_t indexEnd=0;
    for (std::vector<bool>::const_iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
        // test whether the state is set
        if (*itStart) {
            for (std::vector<bool>::const_iterator itStop=stopState.begin(); itStop!=stopState.end(); itStop++, indexEnd++) {
                // test whether the state is set
                if (*itStop) {
                    lnprob += log(m[indexStart][indexEnd]);
                }
            }
        }
    }
    
//    std::cerr << "LnProb:\t\t" << lnprob << std::endl; 
    
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
double Dist_dtmm::pdf( const RbLanguageObject& value ) const {
    
    // Get the parameters
    const TransitionProbabilityMatrix&    m      = static_cast<const TransitionProbabilityMatrix&>( transProbabilityMatrix->getValue() );
    const CharacterStateDiscrete&         start  = static_cast<const CharacterStateDiscrete&     >( initialState->getValue() );
    const CharacterStateDiscrete&         stop   = static_cast<const CharacterStateDiscrete&     >( value );
        
    double prob = 1.0;
    const std::vector<bool>& startState = start.getStateVector();
    const std::vector<bool>& stopState  = stop.getStateVector();
    size_t indexStart=0;
    size_t indexEnd=0;
    for (std::vector<bool>::const_iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
        // test whether the state is set
        if (*itStart) {
            for (std::vector<bool>::const_iterator itStop=stopState.begin(); itStop!=stopState.end(); itStop++, indexEnd++) {
                // test whether the state is set
                if (*itStop) {
                    prob *= (m[indexStart][indexEnd]);
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
const RbLanguageObject& Dist_dtmm::rv( void ) {
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Get the parameters
    const TransitionProbabilityMatrix&    m      = static_cast<const TransitionProbabilityMatrix&>( transProbabilityMatrix->getValue() );
    const CharacterStateDiscrete&         start  = static_cast<const CharacterStateDiscrete&     >( initialState->getValue() );
    
    if (randomValue != NULL) {
        delete randomValue;
    }
    randomValue = start.clone();
    const std::vector<bool>& startState = start.getStateVector();
    size_t indexStart=0;
    for (std::vector<bool>::const_iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
        // test whether the state is set
        if (*itStart) {
            const RbVector<Real>& probs = m[indexStart];
            double u = rng->uniform01();
            for (size_t i=0; i<probs.size(); i++) {
                u -= probs[i];
                if (u <= 0) {
                    std::vector<bool> values = std::vector<bool>(start.getNumberOfStates());
                    values[i] = true;
                    randomValue->setValue(values);
                    break;
                }
            }
        }
    }
    
    return *randomValue;
}


/** We intercept a call to set a member variable to make sure that the number of states is consistent */
void Dist_dtmm::setMemberVariable( const std::string& name, const Variable* var ) {
    
    if ( name == "m" ) {
        transProbabilityMatrix = var;
    }
    else if ( name == "a" ) {
        initialState = var;
        
        // reset the state vector
        stateVector.clear();
        const CharacterStateDiscrete& c = static_cast<const CharacterStateDiscrete&>( var->getValue() );
        std::string states = c.getStateLabels();
        for (int i = 0; i < c.getNumberOfStates(); i++) {
            CharacterStateDiscrete* tmp = c.clone();
            tmp->setState( states[i] );
            stateVector.push_back(tmp);
        }
    }
    else {
        DistributionDiscrete::setMemberVariable( name, var );
    }
}

