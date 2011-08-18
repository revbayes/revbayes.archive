/**
 * @file
 * This file contains the implementation of Func_tiprobs a class used to construct
 * a transition probability matrix.
 *
 * @brief Implementation of Func_tiprobs
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

#include "Func_tiprobs.h"
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



/** Clone object */
Func_tiprobs* Func_tiprobs::clone(void) const {
    
    return new Func_tiprobs( *this );
}


/** Execute function */
RbLanguageObject* Func_tiprobs::execute(void) {

    // get the information from the arguments for reading the file
    const RateMatrix* q = static_cast<const RateMatrix*>( args[0].getValue() );
    const RealPos*    t = static_cast<const RealPos*>(    args[1].getValue() );
        
    // initialize the number of states
    const size_t nStates = q->getNumberOfStates();
    
    // check that the number of states isn't 1
    if ( nStates < 2 )
        {
        std::stringstream o;
        o << "Too few states for the rate matrix";
        throw( RbException(o.str()) );
        }
    
    // construct a rate matrix of the correct dimensions
    TransitionProbabilityMatrix* m = new TransitionProbabilityMatrix(nStates);
   
    // calculate the transition probabilities    
    q->calculateTransitionProbabilities( t->getValue(), *m );
    
    // wrap up the rate matrix object and send it on its way to parser-ville
    return m;
}


/** Get argument rules */
const ArgumentRules& Func_tiprobs::getArgumentRules(void) const {
    
    static ArgumentRules argumentRules;
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
const VectorString& Func_tiprobs::getClass(void) const {
    
    static VectorString rbClass = VectorString( Func_tiprobs_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_tiprobs::getReturnType(void) const {
    
    return TypeSpec( RbVoid_name );
}

