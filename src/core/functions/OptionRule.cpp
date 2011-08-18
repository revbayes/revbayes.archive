/**
 * @file
 * This file contains the implementatin of OptionRule, which is
 * used to describe argument rules corresponding to the
 * selection of one of several string options.
 *
 * @brief Implementation of OptionRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "DAGNode.h"
#include "ConstantNode.h"
#include "OptionRule.h"
#include "Real.h"
#include "RbException.h"
#include "Integer.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct rule without default value; use "" for no label. */
OptionRule::OptionRule( const std::string& argName, VectorString optVals )
    : ValueRule( argName, TypeSpec( RbString_name ) ), options( optVals ) {

    if ( !areOptionsUnique( optVals ) )
        throw RbException( "Options are not unique" );
}


/** Construct rule with default value; use "" for no label. */
OptionRule::OptionRule(const std::string& argName, RbString* defVal, VectorString optVals )
    : ValueRule( argName, defVal ), options( optVals ) {

    if ( !areOptionsUnique( optVals ) )
        throw RbException( "Options are not unique" );
}


/** Help function to test whether a vector of string contains unique string values */
bool OptionRule::areOptionsUnique( const VectorString& optVals ) const {

    for ( size_t i = 0; i < optVals.getLength(); i++ )
        for ( size_t j = i + 1; j < optVals.getLength(); j++ )
            if ( optVals[i] == optVals[j] )
                return false;

    return true;
}


/** Get class vector describing type of object */
const VectorString& OptionRule::getClass( void ) const { 

    static VectorString rbClass = VectorString( OptionRule_name ) + ArgumentRule::getClass();
	return rbClass; 
}


/** Test if argument is valid */
bool OptionRule::isArgValid( DAGNode* var, bool& needsConversion, bool once ) const {

    // Initialize
    needsConversion = false;

    // Make sure we have a constant
    if ( !var->isType( ConstantNode_name ) )
        return false;

    // This will make sure we have a string variable
    if ( !ArgumentRule::isArgumentValid( var, needsConversion, once ) )
        return false;

    // Make sure we have a valid option
    bool        valid = false;
    RbString*   value;
    if ( needsConversion )
        value = static_cast<RbString*>( var->getValue()->convertTo( RbString_name ) );
    else
        value = static_cast<RbString*>( var->getValue()->clone() );

    std::string valString = value->getValue();
    for ( size_t i = 0; i < options.getLength(); i++ ) {
        if ( options[i] == valString ) {
            valid = true;
            break;
        }
    }
    
    delete value;
    return valid;
}


/** Print value for user */
void OptionRule::printValue(std::ostream& o) const {

    ArgumentRule::printValue(o);

    o << "options = " << options << std::endl;
}


/** Provide complete information about object */
std::string OptionRule::richInfo(void) const {

    std::ostringstream o;

    o << "OptionRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "argSlot       = " << argSlot << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;
    o << "options       = " << options << std::endl;

    return o.str();
}

