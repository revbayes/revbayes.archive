/**
 * @file
 * This file contains the declaration of Func_unclamp, which
 * is used to unclamp a stochastic node.
 *
 * @brief Implementation of Func_unclamp
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
#include "Func_unclamp.h"
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
Func_unclamp* Func_unclamp::clone( void ) const {

    return new Func_unclamp( *this );
}


/** Execute function */
const RbLanguageObject& Func_unclamp::executeFunction( void ) {

    // Get the stochastic node from the variable
    StochasticNode* theNode = dynamic_cast<StochasticNode*>( const_cast<DAGNode*>( variable->getDagNode() ) );
    if ( !theNode )
        throw RbException( "The variable is not a stochastic node" );
    
    // Now unclamp the node
    theNode->unclamp();

    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_unclamp::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ValueRule( "variable",   RbObject::getClassTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_unclamp::getClassName(void) { 
    
    static std::string rbClassName = "Unclamp function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_unclamp::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_unclamp::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_unclamp::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_unclamp::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "variable" ) {
        variable = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

