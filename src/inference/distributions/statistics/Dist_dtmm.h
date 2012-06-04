/**
 * @file
 * This file contains the declaration of Dist_dtmm, which holds
 * the parameters and functions related to a distribution on
 * discrete states from a discrete-time discrete-state Markov
 * model.
 *
 * @brief Declaration of Dist_dtmm
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

#ifndef Dist_dtmm_H
#define Dist_dtmm_H

#include "DiscreteCharacterState.h"
#include "DistributionDiscrete.h"

#include <ostream>
#include <string>

class TransitionProbabilityMatrix;

class Dist_dtmm: public DistributionDiscrete {
    
public:
    Dist_dtmm(void);                                                                                //!< constructor
    
    // Basic utility functions
    Dist_dtmm*                              clone(void) const;                                                  //!< Clone object
    
protected:
    double                                  lnPdfSingleValue(std::vector<size_t> &offset) const;                //!< Ln probability density
    double                                  pdfSingleValue(std::vector<size_t> &offset) const;                  //!< Probability density
    void                                    rvSingleValue(std::vector<size_t> &offset);                         //!< Generate random variable
    void                                    setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    void                                    setInternalObservedValue(const RbValue<void*> &v);                  //!< Set the pointers to the observation (random variable) of the distribution.

private:
    // parameters
    RbValue<TransitionProbabilityMatrix*>   probabilityMatrix;
    RbValue<DiscreteCharacterState*>        initialState;
    
};

#endif

