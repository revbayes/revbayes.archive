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
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <cassert>


// Definition of the static type spec member
const TypeSpec Func_unclamp::typeSpec(Func_unclamp_name);

/** Clone object */
Func_unclamp* Func_unclamp::clone( void ) const {

    return new Func_unclamp( *this );
}


/** Execute function */
RbLanguageObject* Func_unclamp::execute( void ) {

    // Get the stochastic node from the variable reference or lookup
    StochasticNode* theNode = dynamic_cast<StochasticNode*>( args[0].getDagNodePtr() );
    if ( !theNode )
        throw RbException( "The variable is not a stochastic node" );
    
    // Now unclamp the node
    theNode->unclamp();

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_unclamp::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ValueRule( "variable",   RbObject_name ));
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_unclamp::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_unclamp_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_unclamp::getReturnType( void ) const {

    return TypeSpec( RbVoid_name );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_unclamp::getTypeSpec(void) const {
    return typeSpec;
}

