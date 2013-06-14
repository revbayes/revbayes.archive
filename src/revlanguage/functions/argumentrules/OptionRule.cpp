/**
 * @file
 * This file contains the implementatin of OptionRule, which is
 * used to describe argument rules corresponding to the
 * selection of one of several RlString options.
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

#include "OptionRule.h"
#include "Real.h"
#include "RbException.h"
#include "Integer.h"
#include "RbUtil.h"
#include "RlString.h"
#include "Vector.h"
#include "Workspace.h"

#include <sstream>
#include <typeinfo>

using namespace RevLanguage;

/** Construct rule without default value; use "" for no label. */
OptionRule::OptionRule( const std::string& argName, const Vector<RlString> &optVals ) : ArgumentRule( argName, true, RlString::getClassTypeSpec() ), options( optVals ) {

    if ( !areOptionsUnique( optVals ) )
        throw RbException( "Options are not unique" );
}


/** Construct rule with default value; use "" for no label. */
OptionRule::OptionRule(const std::string& argName, RlString *defVal, const Vector<RlString> &optVals ) : ArgumentRule( argName, true, RlString::getClassTypeSpec(), defVal ), options( optVals ) {

    if ( !areOptionsUnique( optVals ) )
        throw RbException( "Options are not unique" );
}


/** Help function to test whether a vector of RlString contains unique RlString values */
bool OptionRule::areOptionsUnique( const Vector<RlString>& optVals ) const {

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
//    o << "options       = " << options << std::endl;
    
    return o.str();
}


/** Print value for user */
void OptionRule::printValue(std::ostream& o) const {

    ArgumentRule::printValue(o);

    o << " = ";
    for (Vector<RlString>::const_iterator it = options.begin(); it != options.end(); ++it) {
        if ( it != options.begin() ) {
            o << "|";
        }
        o << *it;
    }
    o << std::endl;
}


