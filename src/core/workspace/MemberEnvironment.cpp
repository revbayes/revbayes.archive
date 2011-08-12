/**
 * @file
 * This file contains the implementation of MemberEnvironment, which is
 * used to hold the variables of a member object.
 *
 * @brief Implementation of Workspace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

// Regular include files
#include "MemberEnvironment.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"         // For PRINTF
#include "VectorString.h"

#include <sstream>
#include <vector>


/** Constructor of global workspace */
MemberEnvironment::MemberEnvironment() : Environment() {
    
}


/** Constructor of user workspace */
MemberEnvironment::MemberEnvironment(Environment* parentSpace) : Environment(parentSpace) {
    
}

/** Copy constructor */
MemberEnvironment::MemberEnvironment(const MemberEnvironment &x) : Environment(x) {
    
}


/** Destructor */
MemberEnvironment::~MemberEnvironment() {
    
    
}

/** Get class vector describing type of object */
const VectorString& MemberEnvironment::getClass() const {
    
    static VectorString rbClass = VectorString(MemberEnvironment_name) + Environment::getClass();
    return rbClass;
}

/** Print workspace */
void MemberEnvironment::printValue(std::ostream& o) const {
    
    o << "Variable table:" << std::endl;
    VariableTable::const_iterator it;
    for ( it = variableTable.begin(); it != variableTable.end(); it++) {
        o << (*it).first << " = ";
        (*it).second->printValue( o );
        o << std::endl;
    }
    o << std::endl;
    
}


/** Complete info about object to string */
std::string MemberEnvironment::richInfo( void ) const {
    
    std::ostringstream o;
    o << "MemberEnvironment:" << std::endl;
    printValue( o );
    
    return o.str();
}
