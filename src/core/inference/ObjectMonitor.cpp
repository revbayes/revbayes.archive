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


// Definition of the static type spec member
const TypeSpec ObjectMonitor::typeSpec(ObjectMonitor_name);

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


/** Get class vector describing type of object */
const VectorString& ObjectMonitor::getClass() const {
    
    static VectorString rbClass = VectorString(ObjectMonitor_name) + ConstantMemberObject::getClass();
    return rbClass;
}

/** Return member rules */
const MemberRules& ObjectMonitor::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
//        memberRules.push_back( new ValueRule( "printgen"  , TypeSpec(Integer_name)          ) );
//        memberRules.push_back( new ValueRule( "variable"  , TypeSpec(RbLanguageObject_name) ) );
//        memberRules.push_back( new Ellipsis (               TypeSpec(RbLanguageObject_name) ) );

        const MemberRules& parentRules = Monitor::getMemberRules();
        memberRules.insert(memberRules.end(), parentRules.begin(), parentRules.end());
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& ObjectMonitor::getTypeSpec(void) const {
    return typeSpec;
}


/** Monitor value unconditionally */
void ObjectMonitor::monitor(void) {
    
    for (std::vector<RbDagNodePtr>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        
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
        
        for (std::vector<RbDagNodePtr>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
            
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
