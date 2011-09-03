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

#include "RbUtil.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec ValueRule::typeSpec(ValueRule_name);

/** Construct rule based on default value; use "" for no label. */
ValueRule::ValueRule( const std::string& argName, RbLanguageObject* defVal ) : ArgumentRule( argName, defVal ) {
}


/** Construct rule without default value; use "" for no label. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp ) : ArgumentRule( argName, argTypeSp ) {

}


/** Construct rule with default value. We rely on workspace to check the provided type specification. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject* defValue ) : ArgumentRule( argName, argTypeSp, defValue ) {

}


/** Construct rule with default reference or value variable. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable ) : ArgumentRule( argName, argTypeSp, defVariable ) {

}


/** Get class vector describing type of object */
const VectorString& ValueRule::getClass( void ) const { 

    static VectorString rbClass = VectorString( ValueRule_name ) + ArgumentRule::getClass();
	return rbClass; 
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& ValueRule::getTypeSpec(void) const {
    return typeSpec;
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

