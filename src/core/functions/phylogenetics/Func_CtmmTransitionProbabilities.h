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


class Func_CtmmTransitionProbabilities :  public RbFunction {
    
    public:
        // Basic utility functions
        Func_CtmmTransitionProbabilities*   clone(void) const;                                                      //!< Clone the object
        static const std::string&           getClassName(void);                                                     //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&                getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&                     getReturnType(void) const;                                              //!< Get type of return value

    protected:
        const RbLanguageObject&             executeFunction(void);                                                  //!< Execute function
        void                                setArgumentVariable(const std::string& name, const RbVariablePtr& var);

    private:
        size_t                              nStates;                                                                //!< The number of states in the model
        TransitionProbabilityMatrix         transProbsMatrix;                                                       //!< The transition probability matrix
    
        // Arguments
        RbVariablePtr                       rateMatrix;
        RbVariablePtr                       time;

};

#endif

