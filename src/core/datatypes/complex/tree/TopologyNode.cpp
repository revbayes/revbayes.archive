/**
 * @file
 * This file contains the declaration of a TopologyNode.
 *
 * @brief Declaration of TopologyNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "Boolean.h"
#include "ConstantNode.h"
#include "MemberFunction.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "VectorString.h"
#include "TopologyNode.h"

#include <algorithm>



/** Default constructor */
TopologyNode::TopologyNode(void) : MemberObject( getMemberRules() ) {

    name = "";
}


/** constructor with name */
TopologyNode::TopologyNode(const std::string& n) : MemberObject( getMemberRules() ) {

    name = n;
}


/** Clone function */
TopologyNode* TopologyNode::clone(void) const {
    
    return new TopologyNode(*this);
}


DAGNode* TopologyNode::executeOperation(const std::string& name, ArgumentFrame& args) {
    
    if (name == "isTip") 
        {
        return ( new Boolean(isTip()) )->wrapIntoVariable();
        }
    else if (name == "isRoot") 
        {
        return ( new Boolean(isRoot()) )->wrapIntoVariable();
        }
    else if (name == "isInterior") 
        {
        return ( new Boolean(!isTip()) )->wrapIntoVariable();
        }
    else
        throw RbException("No member method called '" + name + "'");
    
    return NULL;
}


/** Get class vector describing type of object */
const VectorString& TopologyNode::getClass() const {
    
    static VectorString rbClass = VectorString(TopologyNode_name) + MemberObject::getClass();
    return rbClass;
}


/** Get method specifications */
const MethodTable& TopologyNode::getMethods(void) const {
    
    static MethodTable   methods;
    static ArgumentRules isTipArgRules;
    static ArgumentRules isRootArgRules;
    static ArgumentRules isInteriorArgRules;
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        // this must be here so the parser can distinguish between different instances of a character matrix
        isTipArgRules.push_back(      new ReferenceRule( "", MemberObject_name ) );
        isRootArgRules.push_back(     new ReferenceRule( "", MemberObject_name ) );
        isInteriorArgRules.push_back( new ReferenceRule( "", MemberObject_name ) );
        
        methods.addFunction("isTip",      new MemberFunction(Boolean_name, isTipArgRules)  );
        methods.addFunction("isRoot",     new MemberFunction(Boolean_name, isRootArgRules)  );
        methods.addFunction("isInterior", new MemberFunction(Boolean_name, isInteriorArgRules)  );
        
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
        }

    return methods;
}


const MemberRules& TopologyNode::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/** Print value for user */
void TopologyNode::printValue(std::ostream& o) const {
    
}


/** Remove a child from the vector of children */
void TopologyNode::removeChild(TopologyNode* p) {
    
    std::vector<TopologyNode*>::iterator it = find(children.begin(), children.end(), p);
    if ( it != children.end() )
        children.erase(it);
    else 
        throw(RbException("Cannot find node in list of children nodes"));
}


std::string TopologyNode::richInfo(void) const {

    std::ostringstream o;
    o <<  "Topology node: ";
    printValue(o);
    return o.str();
}


