/**
 * @file
 * This file contains the implementation of Func_clamp, which is
 * the function used to clamp stochastic nodes to an observed value.
 *
 * @brief Implementation of Func_clamp
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

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_clamp.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <cassert>


// Definition of the static type spec member
const TypeSpec Func_clamp::typeSpec(Func_clamp_name);
const TypeSpec Func_clamp::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_clamp* Func_clamp::clone( void ) const {

    return new Func_clamp( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_clamp::executeFunction( void ) {

    // Get the stochastic node from the variable reference
    RbPtr<DAGNode> theDagNode = (*args)[0]->getDagNode();
    while (theDagNode->isType(DeterministicNode_name)) {
        theDagNode = static_cast<DeterministicNode*>( (DAGNode*)theDagNode )->getFunction()->execute();
    }
    
    RbPtr<StochasticNode> theNode( dynamic_cast<StochasticNode*>( (DAGNode*) theDagNode) );
    if ( !theNode )
        throw RbException( "The variable is not a stochastic node" );
    
    // The following call will throw an error if the value type is wrong
    theNode->clamp( RbPtr<RbLanguageObject>( (*args)[1]->getValue()->clone() ) );

    return RbPtr<RbLanguageObject>::getNullPtr();
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_clamp::getArgumentRules( void ) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule ( "variable", RbObject_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule ( "value",    RbObject_name ) ) );
        rulesSet = true;
    }

    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_clamp::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_clamp_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_clamp::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_clamp::getTypeSpec(void) const {
    return typeSpec;
}

