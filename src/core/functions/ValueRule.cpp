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

#include <sstream>


/** Construct rule based on default value; use "" for no label. */
ValueRule::ValueRule( const std::string& argName, RbLanguageObject *defVal ) : ArgumentRule( argName, defVal ) {
}


/** Construct rule without default value; use "" for no label. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp, bool optional ) : ArgumentRule( argName, argTypeSp, optional ) {

}


/** Construct rule with default value. We rely on workspace to check the provided type specification. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject *defValue ) : ArgumentRule( argName, argTypeSp, defValue ) {

}


/** Construct rule with default reference or value variable. */
ValueRule::ValueRule( const std::string& argName, const TypeSpec& argTypeSp, DAGNode *defVariable ) : ArgumentRule( argName, argTypeSp, defVariable ) {

}


/** Get class name of object */
const std::string& ValueRule::getClassName(void) { 
    
    static std::string rbClassName = "Value rule";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ValueRule::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ArgumentRule::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ValueRule::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Provide complete information about object */
std::string ValueRule::debugInfo(void) const {

    std::ostringstream o;

    o << "ValueRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;
    o << "defaultVaribale   = ";
    if ( defaultVariable != NULL && defaultVariable->getDagNode() != NULL ) {
        defaultVariable->getValue().printValue(o);
    } 
    else {
        o << "NULL";
    }
    o << std::endl;

    return o.str();
}

