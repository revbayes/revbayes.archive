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

/** Copy Constructor */
Monitor::Monitor(const Monitor &x) : ConstantMemberObject(x) {
    
    // shallow copy
    nodes = x.nodes;
    
}

Monitor::~Monitor() {
   
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
const RbPtr<MemberRules> Monitor::getMemberRules( void ) const {
    
    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "filename" , TypeSpec(RbString_name)         ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "printgen" , TypeSpec(Integer_name)          ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "variable" , TypeSpec(RbLanguageObject_name) ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new Ellipsis (              TypeSpec(RbLanguageObject_name) ) ) );
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Monitor::getTypeSpec(void) const {
    return typeSpec;
}


/** Monitor value unconditionally */
void Monitor::monitor(void) {

    for (std::vector<RbPtr<VariableNode> >::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element except the first element
        if ( it != nodes.begin() )
            outStream << "\t";
        
        // print the value
        (*it).get()->printValue(outStream);
    }
    
    outStream << std::endl;
    
    
}


/** Monitor value at generation gen */
void Monitor::monitor(int gen) {

    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>( getMemberValue("printgen").get() )->getValue();
    
    if (gen % samplingFrequency == 0) {
        for (std::vector<RbPtr<VariableNode> >::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
            // add a separator before every new element except the first element
            if ( it != nodes.begin() )
                outStream << " ; ";
            
            // print the value
            (*it).get()->printValue(outStream);
        }

	outStream << std::endl;
    }
}


/** open the file stream for printing */
void Monitor::openStream(void) {

    // get the filename
    std::string filename = dynamic_cast<const RbString*>( getMemberValue("filename").get() )->getValue();
    
    // open the stream to the file
    outStream.open(filename.c_str());
    
}

/** Print header for monitored values */
void Monitor::printHeader() {
    for (std::vector<RbPtr<VariableNode> >::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element except the first element
        if ( it != nodes.begin() )
            outStream << " ; ";
        
         RbPtr<VariableNode> theNode = *it;
        
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
    int samplingFrequency = dynamic_cast<const Integer*>( getMemberValue("printgen").get() )->getValue();
    
    o << "Monitor: interval = " << samplingFrequency;
}


void Monitor::replaceDagNodes(std::vector<RbPtr<VariableNode> > &n) {
    
    // release all nodes
    nodes.clear();
    
    // add all nodes
    for (size_t i=0; i<n.size(); i++) {
        RbPtr<VariableNode> theNode = n[i];
        if (theNode != NULL) {
            nodes.push_back(theNode);
        }
    }
    
}


/** Complete info about object */
std::string Monitor::richInfo(void) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>( getMemberValue("printgen").get() )->getValue();
    
    std::ostringstream o;
    o << "Monitor: interval = " << samplingFrequency;

    return o.str();
}

void Monitor::setMemberVariable(std::string const &name, RbPtr<Variable> var) {
    
    // catch setting of the variables 
    if (name == "variable" || name == "") {
        nodes.push_back(RbPtr<VariableNode>( dynamic_cast<VariableNode*>(var->getDagNodePtr().get() ) ) );
    }
    else {
        // call parent class to set member variable
        ConstantMemberObject::setMemberVariable( name, var );
    }
}


