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

/** Copy Constructor */
Monitor::Monitor(const Monitor &x) : ConstantMemberObject(x) {
    
    // shallow copy
    nodes = x.nodes;
    separator = x.separator;
    
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
RbPtr<const MemberRules> Monitor::getMemberRules( void ) const {
    
    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "filename"  , TypeSpec(RbString_name)         ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "printgen"  , TypeSpec(Integer_name)          ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "separator" , TypeSpec(RbString_name), RbPtr<RbLanguageObject>(new RbString("\t") ) ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "variable"  , TypeSpec(RbLanguageObject_name) ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new Ellipsis (               TypeSpec(RbLanguageObject_name) ) ) );
        rulesSet = true;
    }
    
    return RbPtr<const MemberRules>( memberRules );
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
            outStream << separator;
        
        // print the value
        (*it)->printValue(outStream);
    }
    
    outStream << std::endl;
    
    
}


/** Monitor value at generation gen */
void Monitor::monitor(int gen) {

    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>( (const RbObject*)getMemberValue("printgen") )->getValue();
    
    if (gen % samplingFrequency == 0) {
        for (std::vector<RbPtr<VariableNode> >::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
            // add a separator before every new element except the first element
            if ( it != nodes.begin() )
                outStream << separator;
            
            // print the value
            (*it)->printValue(outStream);
        }

	outStream << std::endl;
    }
}


/** open the file stream for printing */
void Monitor::openStream(void) {

    // get the filename
    std::string filename = dynamic_cast<const RbString*>( (const RbObject*)getMemberValue("filename") )->getValue();
    
    // open the stream to the file
    outStream.open(filename.c_str());
    
}

/** Print header for monitored values */
void Monitor::printHeader() {
    for (std::vector<RbPtr<VariableNode> >::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element except the first element
        if ( it != nodes.begin() )
            outStream << separator;
        
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
    int samplingFrequency = dynamic_cast<const Integer*>( (const RbObject*)getMemberValue("printgen") )->getValue();
    
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
    int samplingFrequency = dynamic_cast<const Integer*>( (const RbObject*)getMemberValue("printgen") )->getValue();
    
    std::ostringstream o;
    o << "Monitor: interval = " << samplingFrequency;

    return o.str();
}

void Monitor::setMemberVariable(std::string const &name, RbPtr<Variable> var) {
    
    // catch setting of the variables 
    if (name == "variable" || name == "") {
        RbPtr<DAGNode> theNode = var->getDagNode();
        if (theNode->getValue()->isType(DagNodeContainer_name)) {
            RbPtr<DagNodeContainer> theContainer( static_cast<DagNodeContainer*>( (RbLanguageObject*)theNode->getValue() ) );
            for (size_t i = 0; i < theContainer->size(); i++) {
                theNode = static_cast<VariableSlot*>( (RbObject*)theContainer->getElement(i) )->getDagNode();
                if (theNode->isType(VariableNode_name)) {
                    nodes.push_back( RbPtr<VariableNode>( static_cast<VariableNode*>( (DAGNode*)theNode ) ) );
//                } else {
//                    throw RbException("Cannot monitor a constant node!");
                }
            }
        }
        else {
            if (theNode->isType(VariableNode_name)) {
                nodes.push_back( RbPtr<VariableNode>( static_cast<VariableNode*>( (DAGNode*)theNode ) ) );
//            } else {
//                throw RbException("Cannot monitor a constant node!");
            }
        }
    } 
    else if (name == "separator") {
        separator = static_cast<RbString*>( (RbObject*)var->getValue() )->getValue();
        
        // call parent class to set member variable
        ConstantMemberObject::setMemberVariable( name, var );
    } 
    else {
        // call parent class to set member variable
        ConstantMemberObject::setMemberVariable( name, var );
    }
}


