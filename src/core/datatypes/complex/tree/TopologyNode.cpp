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
#include "RbBoolean.h"
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
    
    // initialize
    parent = NULL;
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const std::string& n, int indx) : name(n), index(indx), ConstantMemberObject( getMemberRules() ) {
    
    // initialize
    parent = NULL;
}


/** Copy constructor. We use a shallow copy. */
TopologyNode::TopologyNode(const TopologyNode &n) : ConstantMemberObject(n) {
    
    // copy the members
    name = n.name;
    index = n.index;
    parent = n.parent;
    children = n.children;
    
    // we have to retain all the children because we own them now too
    for (std::vector<TopologyNode*>::iterator it=children.begin(); it!=children.end(); it++) {
        (*it)->retain();
    }
    
}


/** Destructor */
TopologyNode::~TopologyNode(void) {
    
    // we do not own the parent so we do not delete it
    
    // free memory of children
    removeAllChildren();
}


/** Add a child node. We own it from here on. */
void TopologyNode::addChild(TopologyNode *c) {
    
    // retain the child
    c->retain();
    
    // add the child to our internal vector
    children.push_back(c);
}


/** Clone function */
TopologyNode* TopologyNode::clone(void) const {
    
    return new TopologyNode(*this);
}


bool TopologyNode::equals(TopologyNode *node) const {
    // test if the name is the same
    if (name != node->name) {
        return false;
    }
    
    // test if the index is the same
    if (index != node->index) {
        return false;
    }
    
    // test if the parent is the same
    if (parent != node->parent) {
        return false;
    }
    
    // test if the size of the children differs
    if (children.size() != node->children.size()) {
        return false;
    }
    
    // test if all children are the same
    for (size_t i=0; i<children.size(); i++) {
        if (children[i] != node->children[i]) {
            return false;
        }
    }
    
    return true;
}


RbLanguageObject* TopologyNode::executeOperation(const std::string& name, Environment& args) {
    
    if (name == "isTip") {
        return ( new RbBoolean(isTip()) );
    }
    else if (name == "isRoot") {
        return ( new RbBoolean(isRoot()) );
    }
    else if (name == "isInterior") {
        return ( new RbBoolean(!isTip()) );
    }
    else if (name == "ancestor") {
        return parent;
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
    static ArgumentRules ancestorRules;
    static ArgumentRules isTipArgRules;
    static ArgumentRules isRootArgRules;
    static ArgumentRules isInteriorArgRules;
    static bool          methodsSet = false;

    if ( methodsSet == false ) {
        
        methods.addFunction("ancestor",   new MemberFunction(TopologyNode_name, ancestorRules)  );
        methods.addFunction("isTip",      new MemberFunction(RbBoolean_name,      isTipArgRules)  );
        methods.addFunction("isRoot",     new MemberFunction(RbBoolean_name,      isRootArgRules)  );
        methods.addFunction("isInterior", new MemberFunction(RbBoolean_name,      isInteriorArgRules)  );
        
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
    
    if (name != "")
        o << name << ":";
    else
        o << "Unnamed Topology Node:";
}


/** Remove all children. We need to call intelligently the destructor here. */
void TopologyNode::removeAllChildren(void) {
    
    // free the memory
    for (std::vector<TopologyNode*>::iterator it=children.begin(); it!=children.end(); it++) {
        (*it)->release();
        if ((*it)->isUnreferenced()) {
            delete *it;
        }
    }
    
    // empty the children vector
    children.clear();
}

/** Remove a child from the vector of children */
void TopologyNode::removeChild(TopologyNode* p) {
    
    std::vector<TopologyNode*>::iterator it = find(children.begin(), children.end(), p);
    if ( it != children.end() ) {
        (*it)->release();
        if ((*it)->isUnreferenced()) {
            delete *it;
        }
        children.erase(it);
    }
    else 
        throw(RbException("Cannot find node in list of children nodes"));
}


std::string TopologyNode::richInfo(void) const {

    std::ostringstream o;
    o <<  "Topology node: ";
    printValue(o);
    return o.str();
}


void TopologyNode::setParent(TopologyNode *p) {
    
    // we only do something if this isn't already our parent
    if (p != parent) {
        // we do not own the parent so we do not have to delete it
//        if (parent != NULL) {
//            parent->release();
//            if (parent->isUnreferenced()) {
//                delete parent;
//            }
//        }
        
        // set and retain the new parent
        parent = p;
//        parent->retain();
    }
}


