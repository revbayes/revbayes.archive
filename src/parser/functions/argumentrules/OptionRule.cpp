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
#include "RbString.h"
#include "RlVector.h"
#include "Workspace.h"

#include <sstream>
#include <typeinfo>



/** Construct rule without default value; use "" for no label. */
OptionRule::OptionRule( const std::string& argName, const RlVector<RbString> &optVals ) : ArgumentRule( argName, true, RbString::getClassTypeSpec() ), options( optVals ) {

    if ( !areOptionsUnique( optVals ) )
        throw RbException( "Options are not unique" );
}


/** Construct rule with default value; use "" for no label. */
OptionRule::OptionRule(const std::string& argName, const RbPtr<RbString> &defVal, const RlVector<RbString> &optVals ) : ArgumentRule( argName, true, RbString::getClassTypeSpec(), RbPtr<RbLanguageObject>( (RbString*) defVal ) ), options( optVals ) {

    if ( !areOptionsUnique( optVals ) )
        throw RbException( "Options are not unique" );
}


/** Help function to test whether a vector of string contains unique string values */
bool OptionRule::areOptionsUnique( const RlVector<RbString>& optVals ) const {

    for ( size_t i = 0; i < optVals.size(); i++ )
        for ( size_t j = i + 1; j < optVals.size(); j++ )
            if ( optVals[i] == optVals[j] )
                return false;

    return true;
}


/** Provide complete information about object */
std::string OptionRule::debugInfo(void) const {
    
    std::ostringstream o;
    
    o << "OptionRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;
    o << "options       = " << options << std::endl;
    
    return o.str();
}


/** Get class name of object */
const std::string& OptionRule::getClassName(void) { 
    
    static std::string rbClassName = "Option rule";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& OptionRule::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ArgumentRule::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& OptionRule::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void OptionRule::printValue(std::ostream& o) const {

    ArgumentRule::printValue(o);

    o << "options = " << options << std::endl;
}


