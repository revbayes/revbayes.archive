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
RbPtr<RbObject> Func__range::execute( void ) {

    int first = static_cast<const Integer*>( (RbObject*)(*args)[0]->getValue() )->getValue();
    int last  = static_cast<const Integer*>( (RbObject*)(*args)[1]->getValue() )->getValue();

    std::vector<int> temp;
    for ( int i = first; i <= last; i++ )
        temp.push_back(i);
    
    if (first >= 0 && last >= 0) 
        return RbPtr<RbObject>( new VectorNatural(temp) );
    else
        return RbPtr<RbObject>( new VectorInteger(temp) );
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func__range::getArgumentRules( void ) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", Integer_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", Integer_name ) ) );
        rulesSet = true;
    }

    return RbPtr<const ArgumentRules>( argumentRules );
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

