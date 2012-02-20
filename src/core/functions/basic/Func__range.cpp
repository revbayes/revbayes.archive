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


// Definition of the static type spec member
const TypeSpec Func__range::typeSpec(Func__range_name);
const TypeSpec Func__range::returnTypeSpec(VectorInteger_name);

/** Clone object */
Func__range* Func__range::clone( void ) const {

    return new Func__range( *this );
}


/** Execute function */
const RbLanguageObject& Func__range::executeFunction( void ) {

    int f = static_cast<const Integer&>( first->getValue() ).getValue();
    int l = static_cast<const Integer&>( last->getValue() ).getValue();

    std::vector<int> temp;
    if (first < last) {
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

        argumentRules.push_back( new ValueRule( "first", Integer_name ) );
        argumentRules.push_back( new ValueRule( "last", Integer_name ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func__range::getClass( void ) const {

    static VectorString rbClass = VectorString( Func__range_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func__range::getTypeSpec(void) const {
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func__range::getReturnType( void ) const {

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

