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

#include "CharacterStateDiscrete.h"
#include "DistributionDiscrete.h"

#include <ostream>
#include <string>

class RateMatrix;
class VectorString;

const std::string Dist_dtmm_name = "Discrete time Markov model distribution";

class Dist_dtmm: public DistributionDiscrete {
    
public:
    Dist_dtmm(void);                                                                                    //!< Default constructor
    
    // Basic utility functions
    Dist_dtmm*                      clone(void) const;                                                  //!< Clone object
    const VectorString&             getClass(void) const;                                               //!< Get class vector
    const TypeSpec&                 getTypeSpec(void) const;                                            //!< Get language type of the object
    
    // Member variable setup
    const MemberRules&              getMemberRules(void) const;                                         //!< Get member variable rules
    void                            setMemberDagNode(const std::string& name, DAGNode* var);            //!< Set member variable (ensure number of states is consistent)
    
    // Discrete distribution functions
    size_t                          getNumberOfStates(void) const;                                      //!< Get number of states
    virtual const Simplex&          getProbabilityMassVector(void);                                     //!< Get probability mass vector
    const TypeSpec&                 getVariableType(void) const;                                        //!< Get random variable type (Simplex)
    double                          lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
    double                          pdf(const RbLanguageObject& value) const;                           //!< Probability density
    const RbLanguageObject&         rv(void);                                                           //!< Generate random variable
    
private:
    
    // member function return values
    CharacterStateDiscrete*         randomValue;
    
    Variable                        transProbabilityMatrix;
    Variable                        initialState;
    
    static const TypeSpec           typeSpec;
    static const TypeSpec           varTypeSpec;
};

#endif

