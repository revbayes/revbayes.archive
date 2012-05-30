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

class Dist_ctmm: public DistributionDiscrete {

    
public:
    Dist_ctmm(void);                                                                                //!< constructor
    
    // Basic utility functions
    Dist_ctmm*                          clone(void) const;                                                  //!< Clone object
        
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
    RbValue<DiscreteCharacterState*>    initialState;

};

#endif

