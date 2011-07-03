/**
 * @file
 * This file contains the implementation of Func_reflist, which is
 * the function used to clamp stochastic nodes to an observed value.
 *
 * @brief Implementation of Func_reflist
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
#include "Func_reflist.h"
#include "List.h"
#include "RbNames.h"
#include "VectorString.h"


/** Clone object */
Func_reflist* Func_reflist::clone( void ) const {

    return new Func_reflist( *this );
}


/** Execute function */
DAGNode* Func_reflist::execute( void ) {

    // Create a reference list
    List* tempList = new List( true );

    // Fill the reference list
    for ( size_t i = 0; i < args.size(); i++ )
        tempList->addElement( args[i].getReference(), args.getLabel( i ) );

    // Return the reference list
    return tempList->wrapIntoVariable();
}


/** Get argument rules */
const ArgumentRules& Func_reflist::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new Ellipsis( true ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_reflist::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_reflist_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_reflist::getReturnType( void ) const {

    return TypeSpec( List_name );
}

