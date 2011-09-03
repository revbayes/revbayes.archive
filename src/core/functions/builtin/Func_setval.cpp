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

/** Clone object */
Func_setval* Func_setval::clone( void ) const {

    return new Func_setval( *this );
}


/** Execute function */
RbLanguageObject* Func_setval::execute( void ) {

    // Get the stochastic node from the variable reference
    StochasticNode* theNode = dynamic_cast<StochasticNode*>( args[0].getDagNodePtr() );
    if ( !theNode )
        throw RbException( "The variable is not a stochastic node" );
    
    // The following call will throw an error if the value type is wrong
    std::set<StochasticNode*> affected;
    theNode->setValue( args[1].getValue()->clone(), affected );

    // todo: Do we want to update the affected nodes?

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_setval::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ValueRule ( "variable", RbObject_name ));
        argumentRules.push_back( new ValueRule ( "value",    RbObject_name ));
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
const TypeSpec Func_setval::getReturnType( void ) const {

    return TypeSpec( RbVoid_name );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_setval::getTypeSpec(void) const {
    return typeSpec;
}

