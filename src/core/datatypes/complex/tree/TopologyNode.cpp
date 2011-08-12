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
#include "TopologyNode.h"
#include "VectorString.h"
#include "ValueRule.h"

#include <algorithm>



/** Default constructor (interior node, no name). Give the node an optional index ID */
TopologyNode::TopologyNode(int indx) : name(""), index(indx), ConstantMemberObject( getMemberRules() ) {
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const std::string& n, int indx) : name(n), index(indx), ConstantMemberObject( getMemberRules() ) {
}


/** Clone function */
TopologyNode* TopologyNode::clone(void) const {
    
    return new TopologyNode(*this);
}


RbLanguageObject* TopologyNode::executeOperation(const std::string& name, Environment& args) {
    
    if (name == "isTip") 
        {
        return ( new Boolean(isTip()) );
        }
    else if (name == "isRoot") 
        {
        return ( new Boolean(isRoot()) );
        }
    else if (name == "isInterior") 
        {
        return ( new Boolean(!isTip()) );
        }
    else
        throw RbException("No member method called '" + name + "'");
    
    return NULL;
}


/** Loop over children and get their indices */
std::vector<int> TopologyNode::getChildrenIndices() const {

    std::vector<int> temp;

    for ( std::vector<TopologyNode*>::const_iterator i=children.begin(); i!=children.end(); i++ )
        temp.push_back( (*i)->getIndex() );

    return temp;
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
        // This must be here so the parser can distinguish between different instances of a character matrix
        isTipArgRules.push_back(      new ValueRule( "", MemberObject_name ) );
        isRootArgRules.push_back(     new ValueRule( "", MemberObject_name ) );
        isInteriorArgRules.push_back( new ValueRule( "", MemberObject_name ) );
        
        methods.addFunction("isTip",      new MemberFunction(Boolean_name, isTipArgRules)  );
        methods.addFunction("isRoot",     new MemberFunction(Boolean_name, isRootArgRules)  );
        methods.addFunction("isInterior", new MemberFunction(Boolean_name, isInteriorArgRules)  );
        
        // Necessary call for proper inheritance
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


