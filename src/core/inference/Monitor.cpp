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


/** Constructor */
Monitor::Monitor(void) : ConstantMemberObject(getMemberRules()), printgen( NULL )
{
    
}

/** Constructor */
Monitor::Monitor(const MemberRules& rules ) : ConstantMemberObject( rules ), printgen( NULL ) {
    
}

/** Copy Constructor */
Monitor::Monitor(const Monitor &x) : ConstantMemberObject(x), printgen( x.printgen ) {
    
    // shallow copy
    nodes = x.nodes;
    
}

Monitor::~Monitor() {
    
}


/** Get class name of object */
const std::string& Monitor::getClassName(void) { 
    
    static std::string rbClassName = "Monitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Monitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ConstantMemberObject::getClassTypeSpec() ) );
    
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
        memberRules.push_back( new ValueRule( "variable"  , RbLanguageObject::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis (               RbLanguageObject::getClassTypeSpec() ) );
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



void Monitor::replaceDagNodes(std::vector<VariableNode*> &n) {
    
    // release all nodes
    nodes.clear();
    
    // add all nodes
    for (size_t i=0; i<n.size(); i++) {
        VariableNode* theNode = n[i];
        if (theNode != NULL) {
            nodes.push_back(new Variable( theNode) );
        }
    }
    
}


void Monitor::setMemberVariable(std::string const &name, Variable* var) {
    
    // catch setting of the variables 
    if (name == "variable" || name == "") {
        if (var->getValue().isTypeSpec( DagNodeContainer::getClassTypeSpec() )) {
            DagNodeContainer& theContainer = static_cast<DagNodeContainer&>( var->getValue() );
            for (size_t i = 0; i < theContainer.size(); i++) {
                const RbVariablePtr& theVar = static_cast<VariableSlot&>( theContainer.getElement(i) ).getVariablePtr();
//                if (theVar->getDagNode()->isTypeSpec( VariableNode::getClassTypeSpec() ) ) {
                    nodes.push_back( theVar );
                    //                } else {
                    //                    throw RbException("Cannot monitor a constant node!");
//                }
            }
        }
        else {
//            if (var->getDagNode()->isTypeSepc( VariableNode::getClassTypeSpec() ) ) {
                nodes.push_back( var );
                //            } else {
                //                throw RbException("Cannot monitor a constant node!");
//            }
        }
    } 
    else if ( name == "printgen" ) {
        printgen = var;
    }
    else {
        // call parent class to set member variable
        ConstantMemberObject::setMemberVariable( name, var );
    }
}


/** Tell whether the variable with name is monitored by this monitor */
bool Monitor::monitorsVariable( const RbString& varName) {
    for (size_t j=0; j<nodes.size(); j++) {
        if (nodes[j]->getDagNode()->getName() == varName) {
            return( true);
        }
    }
    return false;
}

