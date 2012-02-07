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
#include "Monitor.h"
#include "RbException.h"
#include "RbUtil.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec Monitor::typeSpec(Monitor_name);

/** Constructor */
Monitor::Monitor(void) : ConstantMemberObject(getMemberRules()) {
    
}

/** Constructor */
Monitor::Monitor(const MemberRules* rules ) : ConstantMemberObject( rules ) {
    
}

/** Copy Constructor */
Monitor::Monitor(const Monitor &x) : ConstantMemberObject(x) {
    
    // shallow copy
    nodes = x.nodes;
    
}

Monitor::~Monitor() {
    
}


/** Get class vector describing type of object */
const VectorString& Monitor::getClass() const {
    
    static VectorString rbClass = VectorString(Monitor_name) + ConstantMemberObject::getClass();
    return rbClass;
}

/** Return member rules */
const MemberRules* Monitor::getMemberRules( void ) const {
    
    static MemberRules* memberRules = new MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules->push_back( new ValueRule( "printgen"  , TypeSpec(Integer_name)          ) );
        memberRules->push_back( new ValueRule( "variable"  , TypeSpec(RbLanguageObject_name) ) );
        memberRules->push_back( new Ellipsis (               TypeSpec(RbLanguageObject_name) ) );
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Monitor::getTypeSpec(void) const {
    return typeSpec;
}


/** Print value for user */
void Monitor::printValue(std::ostream& o) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>( (const RbObject*)getMemberValue("printgen") )->getValue();
    
    o << "Monitor: interval = " << samplingFrequency;
}



void Monitor::replaceDagNodes(std::vector<VariableNode*> &n) {
    
    // release all nodes
    nodes.clear();
    
    // add all nodes
    for (size_t i=0; i<n.size(); i++) {
        VariableNode* theNode = n[i];
        if (theNode != NULL) {
            nodes.push_back(theNode);
        }
    }
    
}


/** Complete info about object */
std::string Monitor::richInfo(void) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>( (const RbObject*)getMemberValue("printgen") )->getValue();
    
    std::ostringstream o;
    o << "Monitor: interval = " << samplingFrequency;
    
    return o.str();
}

void Monitor::setMemberVariable(std::string const &name, Variable* var) {
    
    // catch setting of the variables 
    if (name == "variable" || name == "") {
        DAGNode* theNode = var->getDagNode();
        if (theNode->getValue()->isType(DagNodeContainer_name)) {
            DagNodeContainer* theContainer = static_cast<DagNodeContainer*>( theNode->getValue() );
            for (size_t i = 0; i < theContainer->size(); i++) {
                theNode = static_cast<VariableSlot*>( theContainer->getElement(i) )->getDagNode();
                if (theNode->isType(VariableNode_name)) {
                    nodes.push_back( static_cast<VariableNode*>( theNode ) );
                    //                } else {
                    //                    throw RbException("Cannot monitor a constant node!");
                }
            }
        }
        else {
            if (theNode->isType(VariableNode_name)) {
                nodes.push_back( static_cast<VariableNode*>( theNode ) );
                //            } else {
                //                throw RbException("Cannot monitor a constant node!");
            }
        }
    } 
    else {
        // call parent class to set member variable
        ConstantMemberObject::setMemberVariable( name, var );
    }
}


/** Tell whether the variable with name is monitored by this monitor */
bool Monitor::monitorsVariable (RbString varName) {
    for (size_t j=0; j<nodes.size(); j++) {
        if (nodes[j]->getName() == varName) {
            return( true);
        }
    }
    return false;
}

