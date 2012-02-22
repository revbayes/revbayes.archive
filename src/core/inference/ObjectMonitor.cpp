/**
 * @file
 * This file contains the implementation of FileMonitor, used to save
 * information to file about the monitoring of a variable DAG node.
 *
 * @brief Implementation of FileMonitor
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
#include "ObjectMonitor.h"
#include "RbException.h"
#include "RbUtil.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorString.h"


#include <sstream>


/** Constructor */
ObjectMonitor::ObjectMonitor(void) : Monitor(getMemberRules() ) {
    
}

/** Copy Constructor */
ObjectMonitor::ObjectMonitor(const ObjectMonitor &x) : Monitor(x) {
    
    // shallow copy
    values = x.values;
}

ObjectMonitor::~ObjectMonitor() {
    
}


/** Clone the object */
ObjectMonitor* ObjectMonitor::clone(void) const {
    
    return new ObjectMonitor(*this);
}


/** Get class name of object */
const std::string& ObjectMonitor::getClassName(void) { 
    
    static std::string rbClassName = "Object monitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ObjectMonitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ObjectMonitor::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Return member rules */
const MemberRules& ObjectMonitor::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {

        const MemberRules& parentRules = Monitor::getMemberRules();
        memberRules.insert(memberRules.end(), parentRules.begin(), parentRules.end());
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Monitor value unconditionally */
void ObjectMonitor::monitor(void) {
    
    for (std::vector<DAGNode*>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        
        // save the value        
        RbLanguageObject* temp = (*it)->getValue().clone();
        values[(*it)->getName()].push_back( temp );
    }
    
}


/** Monitor value at generation gen */
void ObjectMonitor::monitor(int gen) {
    
    // get the sampling frequency
    int samplingFrequency = dynamic_cast<const Natural&>( printgen->getValue() ).getValue();

    if (gen % samplingFrequency == 0) {
        
        for (std::vector<DAGNode*>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
            
            // save the value
            RbLanguageObject* temp = (*it)->getValue().clone();
            values[(*it)->getName()].push_back( temp );
        }
        
    }
}


/** Print value for user */
void ObjectMonitor::printValue(std::ostream& o) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Natural&>( printgen->getValue() ).getValue();
    
    o << "Monitor: interval = " << samplingFrequency;
}


void ObjectMonitor::setMemberVariable(std::string const &name, Variable* var) {
    
    // catch setting of the variables 
        
    // call parent class to set member variable
    Monitor::setMemberVariable( name, var );
}


/** returns the values contained in the values vector for variable with name varName */
Vector* ObjectMonitor::getValues(const RbString& varName) {
    std::map<RbString,Vector>::iterator it = values.find(varName);
    if (it != values.end()) {
        Vector* toReturn = it->second.clone();
        return toReturn ;
    }
    
    return NULL;
}
