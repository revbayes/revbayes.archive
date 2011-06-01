/**
 * @file
 * This file contains the implementation of ValueRule, which is
 * used for argument rules pertaining to value variables.
 *
 * @brief Implementation of ValueRule 
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbNames.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <sstream>


/** Construct rule based on default value; use "" for no label. */
ValueRule::ValueRule( const std::string& argName, RbObject* defVal )
    : ArgumentRule( argName, defVal ) {
}


/** Construct rule without default value; use "" for no label. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp )
    : ArgumentRule( argName, argTypeSp ) {

    argSlot.setReferenceFlag( false );
}


/** Construct rule with default value. We rely on workspace to check the provided type specification. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp, RbObject* defValue )
    : ArgumentRule( argName, argTypeSp, defValue ) {

    argSlot.setReferenceFlag( false );
}


/** Construct rule with default reference or value variable. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable )
    : ArgumentRule( argName, argTypeSp, defVariable ) {

    argSlot.setReferenceFlag( false );
}


/** Get class vector describing type of object */
const VectorString& ValueRule::getClass( void ) const { 

    static VectorString rbClass = VectorString( ValueRule_name ) + ArgumentRule::getClass();
	return rbClass; 
}


/** Provide complete information about object */
std::string ValueRule::richInfo(void) const {

    std::ostringstream o;

    o << "ValueRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "argSlot       = " << argSlot << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;

    return o.str();
}

