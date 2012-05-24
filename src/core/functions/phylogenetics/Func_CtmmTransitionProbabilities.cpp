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

#include "ConstArgumentRule.h"
#include "Func_CtmmTransitionProbabilities.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RealPos.h"
#include "ParserTransitionProbabilityMatrix.h"
#include "TransitionProbabilityMatrix.h"
#include "UserInterface.h"
#include <sstream>
#include <vector>


/** Clone object */
Func_CtmmTransitionProbabilities* Func_CtmmTransitionProbabilities::clone(void) const {

    return new Func_CtmmTransitionProbabilities( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_CtmmTransitionProbabilities::executeFunction(void) {

//    // get the information from the arguments for reading the file
//    const RateMatrix& q = static_cast<const RateMatrix&>( args[0]->getVariable()->getValue() );
//    const RealPos&    t = static_cast<const RealPos&>(    args[1]->getVariable()->getValue() );
//
//    // initialize the number of states
//    const size_t numStates = q.getNumberOfStates();
//
//    // check that the number of states isn't 1
//    if ( numStates < 2 ) {
//        std::stringstream o;
//        o << "Too few states for the rate matrix";
//        throw( RbException(o.str()) );
//    }
//    
//    // check if the number of states has changed since the last call
//    if (numStates != nStates) {
//        // Yes -> reset the number of states and create a new transition probability matrix with the correct dimension
//        nStates = numStates;
//        // construct a rate matrix of the correct dimensions
//        // \TODO: we might want to resize instead
//        transProbsMatrix = TransitionProbabilityMatrix(nStates);
//    }
//    
//
//    // calculate the transition probabilities
//    q.calculateTransitionProbabilities( t.getValue(), transProbsMatrix );
//
//    // wrap up the rate matrix object and send it on its way to parser-ville
//    return RbPtr<RbLanguageObject>( transProbsMatrix.clone() );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_CtmmTransitionProbabilities::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet)
        {
//        argumentRules.push_back( new ConstArgumentRule( "q", RateMatrix::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "t", RealPos::getClassTypeSpec()    ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_CtmmTransitionProbabilities::getClassName(void) { 
    
    static std::string rbClassName = "Continuous time Markov model transition probability function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_CtmmTransitionProbabilities::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_CtmmTransitionProbabilities::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_CtmmTransitionProbabilities::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ParserTransitionProbabilityMatrix::getClassTypeSpec();
    return returnTypeSpec;
}

