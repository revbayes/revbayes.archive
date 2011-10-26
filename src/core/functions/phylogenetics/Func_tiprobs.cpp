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
//#include "list.h"
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
const TypeSpec Func_tiprobs::typeSpec(Func_tiprobs_name);
const TypeSpec Func_tiprobs::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_tiprobs* Func_tiprobs::clone(void) const {

    return new Func_tiprobs( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_tiprobs::execute(void) {

    // get the information from the arguments for reading the file
    const RbPtr<RateMatrix> q( static_cast<RateMatrix*>( args[0]->getValue().get() ) );
    const RbPtr<RealPos>    t( static_cast<RealPos*>(    args[1]->getValue().get() ) );

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
    RbPtr<TransitionProbabilityMatrix> m( new TransitionProbabilityMatrix(nStates) );

    // calculate the transition probabilities
    q->calculateTransitionProbabilities( t->getValue(), m );

    // wrap up the rate matrix object and send it on its way to parser-ville
    return RbPtr<RbLanguageObject>( m.get() );
}


/** Get argument rules */
const RbPtr<ArgumentRules> Func_tiprobs::getArgumentRules(void) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if (!rulesSet)
        {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "q", RateMatrix_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "t", RealPos_name    ) ) );
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
const TypeSpec& Func_tiprobs::getReturnType(void) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_tiprobs::getTypeSpec(void) const {
    return typeSpec;
}

