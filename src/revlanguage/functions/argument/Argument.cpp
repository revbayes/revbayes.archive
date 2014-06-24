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

using namespace RevLanguage;


/** Construct from argument label and DAG node */
Argument::Argument(const RbPtr<Variable> &v, const std::string& argLabel, bool c) :
    var( v ),
    isConst( c ),
    label( argLabel )
{
    
}


const std::string& Argument::getLabel( void ) const {
    return label;
}


Argument* Argument::clone( void ) const {
    return new Argument( *this );
}


const RbPtr<Variable>& Argument::getReferenceVariable(void) const {
    if ( isConst ) {
        throw RbException("Illegal attempt to get a non-const reference to a constant variable.");
    }
    return var;
}


RbPtr<Variable>& Argument::getVariable(void) {
    return var;
}


RbPtr<const Variable> Argument::getVariable(void) const {
    return RbPtr<const Variable>( var );
}


bool Argument::isConstant( void ) const {
    return isConst;
}



/** Complete info about object */
void Argument::printValue(std::ostream &o) const {
    
    o << label << " = ";
    var->printValue(o);
    
}

