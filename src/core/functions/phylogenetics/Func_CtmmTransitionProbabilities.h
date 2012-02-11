/**
 * @file
 * This file contains the declaration of Func_CtmmTransitionProbabilities, which is used to construct
 * a transition probability matrix
 * 
 * @brief Declaration of Func_CtmmTransitionProbabilities
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_CtmmTransitionProbabilities_H
#define Func_CtmmTransitionProbabilities_H

#include "RbFunction.h"
#include "TransitionProbabilityMatrix.h"

class DAGNode;
class VectorString;



const std::string Func_CtmmTransitionProbabilities_name = "Continuous time Markov model transition probability function";

class Func_CtmmTransitionProbabilities :  public RbFunction {
    
    public:
        // Basic utility functions
        Func_CtmmTransitionProbabilities*   clone(void) const;                                                      //!< Clone the object
        const VectorString&                 getClass(void) const;                                                   //!< Get class vector
        const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&                getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&                     getReturnType(void) const;                                              //!< Get type of return value

    protected:
        RbLanguageObject*                   executeFunction(void);                                                  //!< Execute function

    private:
        size_t                              nStates;                                                                //!< The number of states in the model
        TransitionProbabilityMatrix*        transProbsMatrix;                                                       //!< The transition probability matrix
    
        static const TypeSpec               typeSpec;
        static const TypeSpec               returnTypeSpec;

};

#endif

