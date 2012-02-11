/**
 * @file
 * This file contains the implementation of Func_CtmmTransitionProbabilities a class used to construct
 * a transition probability matrix of a continuous time Markov model. The transition probability matrix 
 * returned by this function can be used by a discrete time Markov model as a parameter. 
 * It should be more effecient to calculate once in a deterministic node the transition probabilities
 * and then use a discrete time Markov model instead of the continuous time Markov model.
 *
 * @brief Implementation of Func_CtmmTransitionProbabilities
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "Func_CtmmTransitionProbabilities.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RealPos.h"
#include "TransitionProbabilityMatrix.h"
#include "UserInterface.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorString.h"
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Func_CtmmTransitionProbabilities::typeSpec(Func_CtmmTransitionProbabilities_name);
const TypeSpec Func_CtmmTransitionProbabilities::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_CtmmTransitionProbabilities* Func_CtmmTransitionProbabilities::clone(void) const {

    return new Func_CtmmTransitionProbabilities( *this );
}


/** Execute function */
RbLanguageObject* Func_CtmmTransitionProbabilities::executeFunction(void) {

    // get the information from the arguments for reading the file
    RateMatrix& q = static_cast<RateMatrix&>( (*args)[0].getValue() );
    RealPos&    t = static_cast<RealPos&>(    (*args)[1].getValue() );

    // initialize the number of states
    const size_t numStates = q.getNumberOfStates();

    // check that the number of states isn't 1
    if ( numStates < 2 ) {
        std::stringstream o;
        o << "Too few states for the rate matrix";
        throw( RbException(o.str()) );
    }
    
    // check if the number of states has changed since the last call
    if (numStates != nStates) {
        // Yes -> reset the number of states and create a new transition probability matrix with the correct dimension
        nStates = numStates;
        // construct a rate matrix of the correct dimensions
        // TODO: we might want to resize instead
        transProbsMatrix = new TransitionProbabilityMatrix(nStates);
    }

    // calculate the transition probabilities
    q.calculateTransitionProbabilities( t.getValue(), *transProbsMatrix );

    // wrap up the rate matrix object and send it on its way to parser-ville
    return transProbsMatrix;
}


/** Get argument rules */
const ArgumentRules& Func_CtmmTransitionProbabilities::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet)
        {
        argumentRules.push_back( new ValueRule( "q", RateMatrix_name ) );
        argumentRules.push_back( new ValueRule( "t", RealPos_name    ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_CtmmTransitionProbabilities::getClass(void) const {

    static VectorString rbClass = VectorString( Func_CtmmTransitionProbabilities_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_CtmmTransitionProbabilities::getReturnType(void) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_CtmmTransitionProbabilities::getTypeSpec(void) const {
    return typeSpec;
}

