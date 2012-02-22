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
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <cassert>


/** Clone object */
Func_clamp* Func_clamp::clone( void ) const {

    return new Func_clamp( *this );
}


/** Execute function */
const RbLanguageObject& Func_clamp::executeFunction( void ) {

    // Get the stochastic node from the variable reference
    DAGNode* theDagNode = variable->getDagNode();
    
    StochasticNode* theNode = dynamic_cast<StochasticNode*>( theDagNode );
    if ( !theNode )
        throw RbException( "The variable is not a stochastic node" );
    
    // The following call will throw an error if the value type is wrong
    theNode->clamp( value->getValue().clone() );

    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_clamp::getArgumentRules( void ) const {

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
const std::string& Func_clamp::getClassName(void) { 
    
    static std::string rbClassName = "Clamp function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_clamp::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_clamp::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_clamp::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_clamp::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
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

