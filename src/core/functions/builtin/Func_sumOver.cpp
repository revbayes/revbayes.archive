/**
 * @file
 * This file contains the implementation of Func_sumOver, which is
 * the function used to flag stochastic nodes to sum over its potential values.
 *
 * @brief Implementation of Func_sumOver
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_sumOver.cpp 1247 2012-02-25 09:17:07Z hoehna $
 */

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "Func_sumOver.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"

#include <cassert>


/** Clone object */
Func_sumOver* Func_sumOver::clone( void ) const {
    
    return new Func_sumOver( *this );
}


/** Execute function */
const RbLanguageObject& Func_sumOver::executeFunction( void ) {
    
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
    
    bool instantiated = !static_cast<const RbBoolean&>( value->getValue() ).getValue();
    theNode->setInstantiated( instantiated );
    
    // call a touch which will flag for recalculation of the probabilities
    theNode->touch();
    // we keep the new value and probabilities
    theNode->keep();
    
    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_sumOver::getArgumentRules( void ) const {
    
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
const std::string& Func_sumOver::getClassName(void) { 
    
    static std::string rbClassName = "Set value function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_sumOver::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_sumOver::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_sumOver::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_sumOver::setArgumentVariable(std::string const &name, const Variable* var) {
    
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

