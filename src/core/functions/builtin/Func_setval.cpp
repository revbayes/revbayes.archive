/**
 * @file
 * This file contains the implementation of Func_setval, which is
 * the function used to clamp stochastic nodes to an observed value.
 *
 * @brief Implementation of Func_setval
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
#include "Distribution.h"
#include "Func_setval.h"
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
const TypeSpec Func_setval::typeSpec(Func_setval_name);
const TypeSpec Func_setval::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_setval* Func_setval::clone( void ) const {

    return new Func_setval( *this );
}


/** Execute function */
RbLanguageObject* Func_setval::executeFunction( void ) {

    // Get the stochastic node from the variable reference
    StochasticNode* theNode = dynamic_cast<StochasticNode*>( (DAGNode*)(*args)[0]->getDagNode() );
    if ( !theNode )
        throw RbException( "The variable is not a stochastic node" );
    
    // remove this node as a child from the parameter node
    for (std::set<VariableNode*>::const_iterator it = theNode->getChildren().begin(); it != theNode->getChildren().end(); it++) {
        // test if the child is a deterministic node
        if ( (*it)->isType(DeterministicNode_name) ) {
            DeterministicNode* detNode = static_cast<DeterministicNode*>( *it );
            // test the function
            if ( &detNode->getFunction() == this ) {
                theNode->removeChildNode(detNode);
            }
        }
    }
    
    // The following call will throw an error if the value type is wrong
    RbLanguageObject* newVal = (*args)[1]->getValue();
    if (!newVal->isTypeSpec(theNode->getDistribution().getVariableType() ) ) {
        if (newVal->isConvertibleTo(theNode->getDistribution().getVariableType())) {
            newVal = static_cast<RbLanguageObject*>( newVal->convertTo(theNode->getDistribution().getVariableType() ) );
        } else {
            throw RbException( "Cannot set the value of the stochastic node because the types do not match." );
        }
    }
    theNode->setValue( newVal );

    // todo: Do we want to update the affected nodes?
    theNode->keep();

    return NULL;
}


/** Get argument rules */
const ArgumentRules* Func_setval::getArgumentRules( void ) const {

    static ArgumentRules* argumentRules = new ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules->push_back( new ValueRule ( "variable", RbObject_name ) );
        argumentRules->push_back( new ValueRule ( "value",    RbObject_name ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_setval::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_setval_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_setval::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_setval::getTypeSpec(void) const {
    return typeSpec;
}

