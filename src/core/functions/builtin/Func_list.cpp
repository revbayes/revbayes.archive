/**
 * @file
 * This file contains the implementation of Func_list, which is
 * the function used to clamp stochastic nodes to an observed value.
 *
 * @brief Implementation of Func_list
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
#include "Ellipsis.h"
#include "Func_list.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <cassert>

/** Clone object */
Func_list* Func_list::clone( void ) const {

    return new Func_list( *this );
}


/** Execute function */
DAGNode* Func_list::execute( void ) {

    // Create a value list
    List* tempList = new List( false );

    // Fill the value list
    for ( size_t i = 0; i < args.size(); i++ )
        tempList->addElement( args[i].getLabel(), args[i].getValue()->clone()->wrapIntoVariable() );

    // Return the value list
    return tempList->wrapIntoVariable();
}


/** Get argument rules */
const ArgumentRules& Func_list::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new Ellipsis( "", RbObject_name, false ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_list::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_list_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_list::getReturnType( void ) const {

    return TypeSpec( List_name );
}

