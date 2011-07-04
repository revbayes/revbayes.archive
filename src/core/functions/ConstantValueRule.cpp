/**
 * @file
 * This file contains the implementation of ConstantValueRule,
 * which is used to describe value rules that require the
 * variable to be a constant.
 *
 * @brief Implementation of ConstantValueRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "DAGNode.h"
#include "ConstantValueRule.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorString.h"

#include <sstream>


/** Construct rule without default value; use "" for no label. */
ConstantValueRule::ConstantValueRule( const std::string& argName, TypeSpec typeSp )
    : ValueRule( argName, typeSp ) {
}


/** Construct rule with default value; use "" for no label. */
ConstantValueRule::ConstantValueRule(const std::string& argName, TypeSpec typeSp, RbObject* defVal )
    : ValueRule( argName, typeSp, defVal ) {
}


/** Clone object */
ConstantValueRule* ConstantValueRule::clone( void ) const {

    return new ConstantValueRule( *this );
}


/** Get class vector describing type of object */
const VectorString& ConstantValueRule::getClass( void ) const { 

    static VectorString rbClass = VectorString( ConstantValueRule_name ) + ValueRule::getClass();
	return rbClass; 
}


/** Test if argument is valid */
bool ConstantValueRule::isArgValid( DAGNode* var, bool& needsConversion, bool once ) const {

    if ( !var->isConst() )
        return false;
    
    return ValueRule::isArgValid( var, needsConversion, once );
}


/** Print value for user */
void ConstantValueRule::printValue(std::ostream& o) const {

    o << "<Constant> ";
    
    ValueRule::printValue(o);
}


/** Provide complete information about object */
std::string ConstantValueRule::richInfo(void) const {

    std::ostringstream o;

    o << "ConstantValueRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "argSlot       = " << argSlot << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;

    return o.str();
}

