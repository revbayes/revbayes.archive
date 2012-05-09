/**
 * @file
 * This file contains the implementation of Monitor, used to hold
 * information about the monitoring of a variable DAG node.
 *
 * @brief Implementation of Monitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "DagNodeContainer.h"
#include "Ellipsis.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "Monitor.h"
#include "RbException.h"
#include "RbUtil.h"
#include "ValueRule.h"
#include "VariableNode.h"

#include <sstream>


/** Constructor */
Monitor::Monitor(void) : MemberObject(getMemberRules()), printgen( NULL )
{
    
}

/** Constructor */
Monitor::Monitor(const MemberRules& rules ) : MemberObject( rules ), printgen( NULL ) {
    
}


/* Add a DAG node */
void Monitor::addDagNode(DAGNode *d) {
    nodes.insert( d );
}


/** Get class name of object */
const std::string& Monitor::getClassName(void) { 
    
    static std::string rbClassName = "Monitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Monitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Monitor::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Return member rules */
const MemberRules& Monitor::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules.push_back( new ValueRule( "printgen"  , Integer::getClassTypeSpec()          ) );
        rulesSet = true;
    }
    
    return memberRules;
}


/** Print value for user */
void Monitor::printValue(std::ostream& o) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Natural&>( printgen->getValue() ).getValue();
    
    o << "Monitor: interval = " << samplingFrequency;
}



void Monitor::setMemberVariable(std::string const &name, const Variable* var) {
    
    // catch setting of the variables 
    if ( name == "printgen" ) {
        printgen = var;
    }
    else {
        // call parent class to set member variable
        MemberObject::setMemberVariable( name, var );
    }
}


/** Tell whether the variable with name is monitored by this monitor */
bool Monitor::monitorsVariable( const RbString& varName) {
    for (std::set<DAGNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        if ( (*it)->getName() == varName ) {
            return( true);
        }
    }
    return false;
}

