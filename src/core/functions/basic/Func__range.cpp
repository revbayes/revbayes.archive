/**
 * @file
 * This file contains the implementation of Func__range, which is
 * the function used to create models.
 *
 * @brief Implementation of Func__range
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

#include "ConstantNode.h"
#include "DAGNode.h"
#include "Func__range.h"
#include "VectorInteger.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorNatural.h"
#include "VectorString.h"


/** Clone object */
Func__range* Func__range::clone( void ) const {

    return new Func__range( *this );
}


/** Execute function */
const RbLanguageObject& Func__range::executeFunction( void ) {

    int f = static_cast<const Integer&>( first->getValue() ).getValue();
    int l = static_cast<const Integer&>( last->getValue() ).getValue();

    std::vector<int> temp;
    if (f < l) {
        for ( int i = f; i <= l; i++ )
            temp.push_back(i);
    }
    else {
        for ( int i = f; i >= l; i-- )
            temp.push_back(i);
    }
    
    range.setValue( temp );
    
    return range;
}


/** Get argument rules */
const ArgumentRules& Func__range::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back( new ValueRule( "first", Integer::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "last",  Integer::getClassTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func__range::getClassName(void) { 
    
    static std::string rbClassName = "Range function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func__range::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func__range::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func__range::getReturnType( void ) const {

    static TypeSpec returnTypeSpec = VectorInteger::getClassTypeSpec();
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func__range::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "first" ) {
        first = var;
    }
    else if ( name == "last" ) {
        last = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

