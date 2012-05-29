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
#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "Func__range.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlVector.h"
#include "TypeSpec.h"



/** default constructor */
Func__range::Func__range( void ) : RbFunction( ) {
    
}


/** Clone object */
Func__range* Func__range::clone( void ) const {

    return new Func__range( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func__range::executeFunction( const std::vector<const RbObject*>& args ) {

    int f = static_cast<const Integer*>( args[0] )->getValue();
    int l = static_cast<const Integer*>( args[1] )->getValue();

    RlVector<Integer> *range = new RlVector<Integer>();
    if (f < l) {
        for ( int i = f; i <= l; i++ )
            range->push_back( Integer(i) );
    }
    else {
        for ( int i = f; i >= l; i-- )
            range->push_back( Integer(i) );
    }
    
    return RbPtr<RbLanguageObject>( range );
}


/** Get argument rules */
const ArgumentRules& Func__range::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back( new ConstArgumentRule( "first", Integer::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "last",  Integer::getClassTypeSpec() ) );
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

    static TypeSpec returnTypeSpec = TypeSpec(RlVector<Integer>::getClassTypeSpec(), new TypeSpec( Integer::getClassTypeSpec() ) );
    return returnTypeSpec;
}

