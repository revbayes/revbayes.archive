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
const RbPtr<MemberRules> Dist_ctmm::getMemberRules( void ) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "Q", RateMatrix_name             ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "v", RealPos_name                ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "a", CharacterStateDiscrete_name ) ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_ctmm::getNumberOfStates( void ) const {

    size_t numStates  = static_cast<const CharacterStateDiscrete*>( getMemberValue( "a"  ).get() )->getNumberOfStates();
    
    return numStates;
}


/** Get the probability mass vector */
const RbPtr<Simplex> Dist_ctmm::getProbabilityMassVector( void ) {

    // get the information from the arguments for reading the file
    const RbPtr<RateMatrix>                q = RbPtr<RateMatrix>( static_cast<RateMatrix*>( (*members)[0]->getValue().get() ) );
    const RbPtr<RealPos>                   t = RbPtr<RealPos>( static_cast<RealPos*>( (*members)[1]->getValue().get() ) );
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
    RbPtr<TransitionProbabilityMatrix> m( new TransitionProbabilityMatrix(nStates) );
    
    // calculate the transition probabilities    
    q->calculateTransitionProbabilities( t->getValue(), m );
    
    //
    return RbPtr<Simplex>::getNullPtr(); // TODO Return vector
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
double Dist_ctmm::lnPdf( const RbPtr<RbLanguageObject> value ) {

    // Get the parameters
    const RbPtr<RateMatrix>             Q      = RbPtr<RateMatrix>( static_cast<RateMatrix*            >( getMemberValue( "Q" ).get() ) );
    double                              t      = static_cast<const RealPos*               >( getMemberValue( "v" ).get() )->getValue();
    const RbPtr<CharacterStateDiscrete> start  = RbPtr<CharacterStateDiscrete>( static_cast<CharacterStateDiscrete*>( getMemberValue( "a" ).get() ) );
    const RbPtr<CharacterStateDiscrete> stop   = RbPtr<CharacterStateDiscrete>( static_cast<CharacterStateDiscrete* >( value.get() ) );
    
    // calculate the transition probability matrix
    
    // initialize the number of states
    const size_t nStates = Q->getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    RbPtr<TransitionProbabilityMatrix> m( new TransitionProbabilityMatrix(nStates) );
    
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
double Dist_ctmm::pdf( const RbPtr<RbLanguageObject> value ) {

    // Get the parameters
    const RbPtr<RateMatrix>             Q      = RbPtr<RateMatrix>( static_cast<RateMatrix*            >( getMemberValue( "Q" ).get() ) );
    double                              t      = static_cast<const RealPos*               >( getMemberValue( "v" ).get() )->getValue();
    const RbPtr<CharacterStateDiscrete> start  = RbPtr<CharacterStateDiscrete>( static_cast<CharacterStateDiscrete*>( getMemberValue( "a" ).get() ) );
    const RbPtr<CharacterStateDiscrete> stop   = RbPtr<CharacterStateDiscrete>( static_cast<CharacterStateDiscrete* >( value.get() ) );

    // calculate the transition probability matrix
    
    // initialize the number of states
    const size_t nStates = Q->getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    RbPtr<TransitionProbabilityMatrix> m( new TransitionProbabilityMatrix(nStates) );
    
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
RbPtr<RbLanguageObject> Dist_ctmm::rv( void ) {

    // Get the rng
    RbPtr<RandomNumberGenerator> rng = GLOBAL_RNG;
    
    // Get the parameters
    const RbPtr<RateMatrix>             Q      = RbPtr<RateMatrix>( static_cast<RateMatrix*            >( getMemberValue( "Q" ).get() ) );
    double                              t      = static_cast<const RealPos*               >( getMemberValue( "v" ).get() )->getValue();
    const RbPtr<CharacterStateDiscrete> start  = RbPtr<CharacterStateDiscrete>( static_cast<CharacterStateDiscrete*>( getMemberValue( "a" ).get() ) );
    
    // calculate the transition probability matrix
    
    // initialize the number of states
    const size_t nStates = Q->getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    RbPtr<TransitionProbabilityMatrix> m( new TransitionProbabilityMatrix(nStates) );
    
    // calculate the transition probabilities    
    Q->calculateTransitionProbabilities( t, m );
    
    RbPtr<CharacterStateDiscrete> draw( start->clone() );
    std::vector<bool> startState = start->getStateVector();
    size_t indexStart=0;
    for (std::vector<bool>::iterator itStart=startState.begin() ; itStart!=startState.end(); itStart++, indexStart++) {
        // test whether the state is set
        if (*itStart) {
            RbPtr<VectorReal> probs = (*m)[indexStart];
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
    
    return RbPtr<RbLanguageObject>( draw.get() );
}


/** We intercept a call to set a member variable to make sure that the number of states is consistent */
void Dist_ctmm::setMemberVariable( const std::string& name, RbPtr<Variable> var ) {

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

