/**
 * @file
 * This file contains the implementation of Func_plate.
 *
 * @brief Implementation of Func_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_plate.cpp 1327 2012-03-10 11:55:11Z hoehna $
 */

#include "DAGNode.h"
#include "DagNodeContainer.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "Func_plate.h"
#include "Parser.h"
#include "ParserDistribution.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbVector.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"

#include <cassert>
#include <cmath>
#include <sstream>


Func_plate::Func_plate(void) : RbFunction() {
    
}


/** Clone object */
Func_plate* Func_plate::clone( void ) const {
    
    return new Func_plate( *this );
}


/** Execute function */
const RbLanguageObject& Func_plate::execute( void ) {
    
    // get the argument
    const Argument& theArg = args[0];
    
    // get the DAG node
    const DAGNode *theNode = theArg.getVariable().getDagNode();
    
    // get the size
    size_t n = static_cast<const Natural&>( args[1].getVariable().getValue() ).getValue();
    
    // expand the DAG node
    // \TODO: We shouldn't use const-casts.
    const_cast<DAGNode *>( theNode )->expand( );
    
    
    return RbNullObject::getInstance();
}



/** Get argument rules */
const ArgumentRules& Func_plate::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ValueRule( "x",    RbObject::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "size", Natural::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_plate::getClassName(void) { 
    
    static std::string rbClassName = "plate function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_plate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_plate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = TypeSpec(RbVoid_name);
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_plate::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}
