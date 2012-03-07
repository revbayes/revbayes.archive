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
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <cassert>


/** Clone object */
Func_setval* Func_setval::clone( void ) const {

    return new Func_setval( *this );
}


/** Execute function */
const RbLanguageObject& Func_setval::executeFunction( void ) {

    // Get the stochastic node from the variable reference
    DAGNode* theDagNode = const_cast<DAGNode*>( (const DAGNode*)variable->getDagNode() );
    StochasticNode* theNode = dynamic_cast<StochasticNode*>( theDagNode );
    if ( !theNode )
        throw RbException( "The variable is not a stochastic node" );
    
    // remove this node as a child from the parameter node
    // this is necessary because when we set the value, the stovhastic node will call touch, which then will touch us and we will touch the node again.
    // -> there is a cycle ...
    for (std::set<VariableNode*>::const_iterator it = theNode->getChildren().begin(); it != theNode->getChildren().end(); it++) {
        // test if the child is a deterministic node
        if ( (*it)->isTypeSpec( DeterministicNode::getClassTypeSpec() ) ) {
            DeterministicNode* detNode = static_cast<DeterministicNode*>( *it );
            // test the function
            if ( &detNode->getFunction() == this ) {
                theNode->removeChildNode(detNode);
            }
        }
    }
    
    // The following call will throw an error if the value type is wrong
    RbLanguageObject* newVal = value->getValue().clone();
    if (!newVal->isTypeSpec(theNode->getDistribution().getVariableType() ) ) {
        if (newVal->isConvertibleTo(theNode->getDistribution().getVariableType())) {
            newVal = static_cast<RbLanguageObject*>( newVal->convertTo(theNode->getDistribution().getVariableType() ) );
        } else {
            throw RbException( "Cannot set the value of the stochastic node because the types do not match." );
        }
    }
    theNode->setValue( newVal );

    theNode->keep();

    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_setval::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ValueRule ( "variable", RbObject::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule ( "value",    RbObject::getClassTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_setval::getClassName(void) { 
    
    static std::string rbClassName = "Set value function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_setval::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_setval::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_setval::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_setval::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "variable" ) {
        variable = var;
    }
    else if ( name == "value" ) {
        value = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

