/**
 * @file
 * This file contains the implementation of Func_vallist, which is
 * the function used to clamp stochastic nodes to an observed value.
 *
 * @brief Implementation of Func_vallist
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

#include "Ellipsis.h"
#include "Func_vallist.h"
#include "List.h"
#include "RbNames.h"
#include "VectorString.h"


/** Clone object */
Func_vallist* Func_vallist::clone( void ) const {

    return new Func_vallist( *this );
}


/** Execute function */
DAGNode* Func_vallist::execute( void ) {

    // Create a value list
    List* tempList = new List( false );

    // Fill the value list
    for ( size_t i = 0; i < args.size(); i++ )
        tempList->addElement( args[i].getValue()->clone()->wrapIntoVariable(), args.getLabel( i ) );

    // Return the value list
    return tempList->wrapIntoVariable();
}


/** Get argument rules */
const ArgumentRules& Func_vallist::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new Ellipsis( false ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_vallist::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_vallist_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_vallist::getReturnType( void ) const {

    return TypeSpec( List_name );
}

