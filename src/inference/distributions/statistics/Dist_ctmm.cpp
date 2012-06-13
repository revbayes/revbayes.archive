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
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "DiscreteCharacterState.h"
#include "Dist_ctmm.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbOptions.h"
#include "RbUtil.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_ctmm::Dist_ctmm( void ) : DistributionDiscrete(  ) {
}


//Dist_ctmm::Dist_ctmm(const Dist_ctmm& d) : DistributionDiscrete(d), 
//                                           randomVariable(),     
//                                           rateMatrix( d.rateMatrix ),
//                                           time( d.time ),
//                                           initialState( d.initialState ) {
//    
//    if (d.randomVariable != NULL) {
//        randomVariable = d.randomVariable->clone();
//    }
//}
//
//
///** Destructor. We need to free the random variable. */
//Dist_ctmm::~Dist_ctmm( void ) {
//    delete randomVariable;
//}
//
//
//Dist_ctmm& Dist_ctmm::operator=(const Dist_ctmm &d) {
//    
//    if (this != &d) {
//        DistributionDiscrete::operator=(d);
//        
//        randomVariable = d.randomVariable->clone();
//    }
//    
//    return *this;
//}

/** Clone this object */
Dist_ctmm* Dist_ctmm::clone( void ) const {

    return new Dist_ctmm( *this );
}


/** Get the number of states in the distribution */
size_t Dist_ctmm::getNumberOfStates( void ) const {
    
    return initialState.value[0].getNumberOfStates();
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
double Dist_ctmm::lnPdfSingleValue(std::vector<size_t> &offset) const {
    
    // Get the parameters
    const DiscreteCharacterState& start  = initialState.value[offset[0]];
    const RateMatrix&             Q      = rateMatrix.value[offset[1]];
    double&                       t      = time.value[offset[2]];
    const DiscreteCharacterState& stop   = randomVariable.value[offset[3]];
    
    // some assertions
#if defined( ASSERTION_DISTRIBUTIONS )
        if ( start.isAmbiguous() ) {
            std::cerr << "Ambiguous character states not allowed as initial states of Dist_ctmm." << std::endl;
        }
#endif
    
    // calculate the transition probability matrix
    
    // initialize the number of states
    size_t nStates = Q.getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    TransitionProbabilityMatrix m = TransitionProbabilityMatrix(nStates);
    
    // calculate the transition probabilities    
    Q.calculateTransitionProbabilities( t, m );
    
    double lnprob = 0.0;
    size_t indexStart=0;
    size_t indexEnd;
    for (unsigned int itStart = start.getState(); itStart != 0; itStart >>= 1, indexStart++) {
        // test whether the state is set
        if ( (itStart & 1) == 1) {
            for (unsigned int itStop = stop.getState(), indexEnd = 0; itStop != 0; itStop >>= 1, indexEnd++) {
                // test whether the state is set
                if ( (itStop & 1) == 1) {
                    lnprob += log(m[indexStart][indexEnd]);
                }
            }
            
            break;
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
double Dist_ctmm::pdfSingleValue(std::vector<size_t> &offset) const {
    
    // Get the parameters
    const DiscreteCharacterState& start  = initialState.value[offset[0]];
    const RateMatrix&             Q      = rateMatrix.value[offset[1]];
    double&                       t      = time.value[offset[2]];
    const DiscreteCharacterState& stop   = randomVariable.value[offset[3]];
    
    // some assertions
#if defined( ASSERTION_DISTRIBUTIONS )
    if ( start.isAmbiguous() ) {
        std::cerr << "Ambiguous character states not allowed as initial states of Dist_ctmm." << std::endl;
    }
#endif
    
    // calculate the transition probability matrix
    
    // initialize the number of states
    size_t nStates = Q.getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    TransitionProbabilityMatrix m = TransitionProbabilityMatrix(nStates);
    
    // calculate the transition probabilities    
    Q.calculateTransitionProbabilities( t, m );
    
    double prob = 1.0;
    size_t indexStart=0;
    for (unsigned int itStart = start.getState(); itStart != 0; itStart >>= 1, indexStart++) {
        // test whether the state is set
        if ( (itStart & 1) == 1) {
            size_t indexEnd;
            for (unsigned int itStop = stop.getState(), indexEnd = 0; itStop != 0; itStop >>= 1, indexEnd++) {
                // test whether the state is set
                if ( (itStop & 1) == 1) {
                    prob *= m[indexStart][indexEnd];
                }
            }
            
            break;
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
void Dist_ctmm::rvSingleValue( std::vector<size_t> &offset ) {
    
    // Get the parameters
    const DiscreteCharacterState& start  = initialState.value[offset[0]];
    const RateMatrix&             Q      = rateMatrix.value[offset[1]];
    double&                       t      = time.value[offset[2]];
    
    // some assertions
#if defined( ASSERTIONS_DISTRIBUTIONS )
    if ( start.isAmbiguous() ) {
        std::cerr << "Ambiguous character states not allowed as initial states of Dist_ctmm." << std::endl;
    }
#endif
    
    // calculate the transition probability matrix
    
    // initialize the number of states
    size_t nStates = Q.getNumberOfStates();
    
    // construct a rate matrix of the correct dimensions
    TransitionProbabilityMatrix m = TransitionProbabilityMatrix(nStates);
    
    // calculate the transition probabilities    
    Q.calculateTransitionProbabilities( t, m );
    
    size_t indexStart=0;
    size_t indexEnd;
    for (unsigned int itStart = start.getState(); itStart != 0; itStart >>= 1, indexStart++) {
        // test whether the state is set
        if ( (itStart & 1) == 1) {
            RandomNumberGenerator* rng = GLOBAL_RNG;
            double u = rng->uniform01();
            DiscreteCharacterState *randomState = start.clone();
            randomState->setToFirstState();
            for (indexEnd = 0; u > 0; ++(*randomState), indexEnd++) {
                u -= m[indexStart][indexEnd];
            }
            --(*randomState);
            
            randomVariable.value[offset[3]] = *randomState;
            
            break;
        }
        
    }
    
    std::cerr << randomVariable.value[offset[3]].getStringValue() << std::endl;
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_ctmm::setInternalObservedValue(const RbValue<void *> &v) {
    
    randomVariable.value    = static_cast<DiscreteCharacterState *>( v.value );
    randomVariable.lengths  = v.lengths;
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_ctmm::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    initialState.value      = static_cast<DiscreteCharacterState*>( p[0].value );
    initialState.lengths    = p[0].lengths; 
    
    rateMatrix.value        = static_cast<RateMatrix*>( p[1].value );
    rateMatrix.lengths      = p[1].lengths;
    
    time.value              = static_cast<double*>( p[2].value );
    time.lengths            = p[2].lengths;
    
    randomVariable.value    = static_cast<DiscreteCharacterState*>( p[3].value );
    randomVariable.lengths  = p[3].lengths;
    
}

