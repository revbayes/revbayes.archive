/**
 * @file
 * This file contains the implementation of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function.
 *
 * @brief Implementation of Argument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include <sstream>

#include "Argument.h"
#include "RbException.h"
#include "RbUtil.h"


/** Construct from argument label and DAG node */
RevLanguage::Argument::Argument(const RbPtr<Variable> &v, const std::string& argLabel, bool c) {
    
    isConst = c;
    var = v;
    label   = argLabel;
    
}


const std::string& RevLanguage::Argument::getLabel( void ) const {
    return label;
}


RevLanguage::Argument* RevLanguage::Argument::clone( void ) const {
    return new Argument( *this );
}


const RevLanguage::RbPtr<RevLanguage::Variable>& RevLanguage::Argument::getReferenceVariable(void) const {
    if ( isConst ) {
        throw RbException("You naughty boy! You cannot get a non-const reference to a constant variable.");
    }
    return var;
}


RevLanguage::RbPtr<RevLanguage::Variable>& RevLanguage::Argument::getVariable(void) {
    return var;
}


RevLanguage::RbPtr<const RevLanguage::Variable> RevLanguage::Argument::getVariable(void) const {
    return RbPtr<const Variable>( var );
}


bool RevLanguage::Argument::isConstant( void ) const {
    return isConst;
}



/** Complete info about object */
void RevLanguage::Argument::printValue(std::ostream &o) const {
    
    o << label << " = ";
    var->printValue(o);
    
}

