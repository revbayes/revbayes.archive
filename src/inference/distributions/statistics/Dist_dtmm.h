///**
// * @file
// * This file contains the declaration of Dist_dtmm, which holds
// * the parameters and functions related to a distribution on
// * discrete states from a discrete-time discrete-state Markov
// * model.
// *
// * @brief Declaration of Dist_dtmm
// *
// * (c) Copyright 2009- under GPL version 3
// * @date Last modified: $Date$
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// * @since 2011-12-15, version 1.0
// *
// * $Id$
// */
//
//#ifndef Dist_dtmm_H
//#define Dist_dtmm_H
//
//#include "CharacterStateDiscrete.h"
//#include "DistributionDiscrete.h"
//#include "Simplex.h"
//
//#include <ostream>
//#include <string>
//
//class RateMatrix;
//
//class Dist_dtmm: public DistributionDiscrete {
//    
//public:
//    Dist_dtmm(void);                                                                                    //!< Default constructor
//    
//    // Basic utility functions
//    Dist_dtmm*                              clone(void) const;                                                  //!< Clone object
//    
//    // Discrete distribution functions
//    size_t                                  getNumberOfStates(void) const;                                      //!< Get number of states
//    const Simplex&                          getProbabilityMassVector(void);                                     //!< Get probability mass vector
//    const std::vector<RbLanguageObject*>&   getStateVector(void) const;
//    const TypeSpec&                         getVariableType(void) const;                                        //!< Get random variable type (Simplex)
//    double                                  lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
//    double                                  pdf(const RbLanguageObject& value) const;                           //!< Probability density
//    const RbLanguageObject&                 rv(void);                                                           //!< Generate random variable
//    
//private:
//
//    std::vector<RbLanguageObject*>          stateVector;
//
//    // member function return values
//    CharacterStateDiscrete*                 randomValue;
//    
//};
//
//#endif
//
