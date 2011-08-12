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

#include "Ellipsis.h"
#include "Monitor.h"
#include "MonitorsContainer.h"
#include "RbException.h"
#include "RbNames.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorString.h"

#include <sstream>


/** Constructor */
Monitor::Monitor(void) : ConstantMemberObject(getMemberRules()) {

}

/** Copy Constructor */
Monitor::Monitor(const Monitor &x) : ConstantMemberObject(x) {
    
    // shallow copy
    nodes = x.nodes;
    
    // retain each node
    for (std::vector<VariableNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++) {
        (*it)->retain();
    }
    
}

Monitor::~Monitor() {
    // release each node
    for (std::vector<VariableNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++) {
        if ((*it) != NULL) {
            (*it)->release();
            if ((*it)->isUnreferenced()) {
                delete *it;
            }
        }
    }
}


/** Convert to object of type */
RbLanguageObject* Monitor::convertTo(const std::string &type) const {
    
    // test the type and convert if possible
    if (type == MonitorsContainer_name) {
        MonitorsContainer *monitors = new MonitorsContainer();
        
        monitors->push_back(this->clone());
        
        // return the monitors
        return monitors;
    }
    
    // call base class
    return ConstantMemberObject::convertTo(type);
}


/** Clone the object */
Monitor* Monitor::clone(void) const {

    return new Monitor(*this);
}


/** Get class vector describing type of object */
const VectorString& Monitor::getClass() const {

    static VectorString rbClass = VectorString(Monitor_name) + ConstantMemberObject::getClass();
    return rbClass;
}

/** Return member rules */
const MemberRules& Monitor::getMemberRules( void ) const {
    
    static MemberRules memberRules;
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules.push_back(new ValueRule( "filename" , TypeSpec(RbString_name)         ) );
        memberRules.push_back(new ValueRule( "printgen" , TypeSpec(Integer_name)          ) );
        memberRules.push_back(new ValueRule( "variable" , TypeSpec(RbLanguageObject_name) ) );
        memberRules.push_back(new Ellipsis (              TypeSpec(RbLanguageObject_name) ) );
        rulesSet = true;
    }
    
    return memberRules;
}

/** Is convertible to? */
bool Monitor::isConvertibleTo(std::string const &type, bool once) const {
    
    // test the type
    if (type == MonitorsContainer_name) {
        return true;
    }
    
    // call base class
    return ConstantMemberObject::isConvertibleTo(type, once);
}


/** Monitor value unconditionally */
void Monitor::monitor(void) {

    for (std::vector<VariableNode*>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element except the first element
        if ( it != nodes.begin() )
            outStream << " ; ";
        
        // print the value
        (*it)->printValue(outStream);
    }
    
    outStream << std::endl;
    
    
}


/** Monitor value at generation gen */
void Monitor::monitor(int gen) {

    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>(getMemberValue("printgen"))->getValue();
    
    if (gen % samplingFrequency == 0) {
        for (std::vector<VariableNode*>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
            // add a separator before every new element except the first element
            if ( it != nodes.begin() )
                outStream << " ; ";
            
            // print the value
            (*it)->printValue(outStream);
        }
    }
    
    outStream << std::endl;
}


/** open the file stream for printing */
void Monitor::openStream(void) {

    // get the filename
    std::string filename = dynamic_cast<const RbString*>(getMemberValue("filename"))->getValue();
    
    // open the stream to the file
    outStream.open(filename.c_str());
    
}

/** Print header for monitored values */
void Monitor::printHeader() {
    for (std::vector<VariableNode*>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element except the first element
        if ( it != nodes.begin() )
            outStream << " ; ";
        
         VariableNode *theNode = *it;
        
        // print the header
        if (theNode->getName() != "")
            outStream << theNode->getName();
        else
            outStream << "Unnamed " << theNode->getType();
    }
    
    outStream << std::endl;
}


/** Print value for user */
void Monitor::printValue(std::ostream& o) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>(getMemberValue("printgen"))->getValue();
    
    o << "Monitor: interval = " << samplingFrequency;
}


void Monitor::replaceDagNodes(std::vector<VariableNode *> &n) {
    
    // release all nodes
    for (size_t i=0; i<nodes.size(); i++) {
        VariableNode *theNode = nodes[i];
        if (theNode != NULL) {
            theNode->release();
            if (theNode->isUnreferenced())
                delete theNode;
        }
    }
    nodes.clear();
    
    // add all nodes
    for (size_t i=0; i<n.size(); i++) {
        VariableNode *theNode = n[i];
        if (theNode != NULL) {
            theNode->retain();
            nodes.push_back(theNode);
        }
    }
    
}


/** Complete info about object */
std::string Monitor::richInfo(void) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>(getMemberValue("printgen"))->getValue();
    
    std::ostringstream o;
    o << "Monitor: interval = " << samplingFrequency;

    return o.str();
}

void Monitor::setMemberVariable(std::string const &name, Variable *var) {
    
    // catch setting of the variables 
    if (name == "variable" || name == "") {
        var->retain();
        nodes.push_back(dynamic_cast<VariableNode*>(var->getDagNodePtr()));
    }
    else {
        // call parent class to set member variable
        ConstantMemberObject::setMemberVariable( name, var );
    }
}


