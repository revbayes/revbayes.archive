/**
 * @file
 * This file contains the declaration of Dist_ctmm, which holds
 * the parameters and functions related to a distribution on
 * discrete states from a continuous-time discrete-state Markov
 * model.
 *
 * @brief Declaration of Dist_ctmm
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

#ifndef Dist_ctmm_H
#define Dist_ctmm_H

#include "DiscreteCharacterState.h"
#include "DistributionDiscrete.h"
#include "Variable.h"

#include <ostream>
#include <string>

class RateMatrix;

template <typename charType>
class Dist_ctmm: public DistributionDiscrete {

    
public:
    Dist_ctmm(void);                                                                                //!< constructor
    
    // Basic utility functions
    Dist_ctmm*                          clone(void) const;                                                  //!< Clone object
    size_t                              getNumberOfStates(void) const;
        
protected:
    double                              lnPdfSingleValue(std::vector<size_t> &offset) const;                //!< Ln probability density
    double                              pdfSingleValue(std::vector<size_t> &offset) const;                  //!< Probability density
    void                                rvSingleValue(std::vector<size_t> &offset);                         //!< Generate random variable
    void                                setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    void                                setInternalObservedValue(const RbValue<void*> &v);                  //!< Set the pointers to the observation (random variable) of the distribution.

private:
    
    // parameters
    RbValue<RateMatrix*>                rateMatrix;
    RbValue<double*>                    time;
    RbValue<charType*>                  initialState;
    RbValue<charType*>                  randomVariable;

};


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
template <typename charType>
Dist_ctmm<charType>::Dist_ctmm( void ) : DistributionDiscrete(  ) {
}


/** Clone this object */
template <typename charType>
Dist_ctmm<charType>* Dist_ctmm<charType>::clone( void ) const {
    
    return new Dist_ctmm<charType>( *this );
}


/** Get the number of states in the distribution */
template <typename charType>
size_t Dist_ctmm<charType>::getNumberOfStates( void ) const {
    
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
template <typename charType>
double Dist_ctmm<charType>::lnPdfSingleValue(std::vector<size_t> &offset) const {
    
    // Get the parameters
    const charType&     start  = initialState.value[offset[0]];
    const RateMatrix&   Q      = rateMatrix.value[offset[1]];
    double&             t      = time.value[offset[2]];
    const charType&     stop   = randomVariable.value[offset[3]];
    
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
template <typename charType>
double Dist_ctmm<charType>::pdfSingleValue(std::vector<size_t> &offset) const {
    
    // Get the parameters
    const charType&     start  = initialState.value[offset[0]];
    const RateMatrix&   Q      = rateMatrix.value[offset[1]];
    double&             t      = time.value[offset[2]];
    const charType&     stop   = randomVariable.value[offset[3]];
    
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
template <typename charType>
void Dist_ctmm<charType>::rvSingleValue( std::vector<size_t> &offset ) {
    
    // Get the parameters
    const charType&     start  = initialState.value[offset[0]];
    const RateMatrix&   Q      = rateMatrix.value[offset[1]];
    double&             t      = time.value[offset[2]];
    
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
            charType *randomState = start.clone();
            randomState->setToFirstState();
            for (indexEnd = 0; u > 0; ++(*randomState), indexEnd++) {
                u -= m[indexStart][indexEnd];
            }
            --(*randomState);
            
            randomVariable.value[offset[3]] = *randomState;
            
            break;
        }
        
    }
}


/** We catch here the setting of the member variables to store our parameters. */
template <typename charType>
void Dist_ctmm<charType>::setInternalObservedValue(const RbValue<void *> &v) {
    
    randomVariable.value    = static_cast<charType *>( v.value );
    randomVariable.lengths  = v.lengths;
    
}


/** We catch here the setting of the member variables to store our parameters. */
template <typename charType>
void Dist_ctmm<charType>::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    initialState.value      = static_cast<charType*>( p[0].value );
    initialState.lengths    = p[0].lengths; 
    
    rateMatrix.value        = static_cast<RateMatrix*>( p[1].value );
    rateMatrix.lengths      = p[1].lengths;
    
    time.value              = static_cast<double*>( p[2].value );
    time.lengths            = p[2].lengths;
    
    randomVariable.value    = static_cast<charType*>( p[3].value );
    randomVariable.lengths  = p[3].lengths;
    
}


#endif

