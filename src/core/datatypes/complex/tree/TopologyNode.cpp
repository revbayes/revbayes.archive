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
#include "RbUtil.h"
#include "TopologyNode.h"
#include "VectorString.h"
#include "ValueRule.h"

#include <algorithm>


// Definition of the static type spec member
const TypeSpec TopologyNode::typeSpec(TopologyNode_name);

/** Default constructor (interior node, no name). Give the node an optional index ID */
TopologyNode::TopologyNode(int indx) : ConstantMemberObject( getMemberRules() ), name(""), index(indx) {
    
    // initialize
    parent = NULL;
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const std::string& n, int indx) : ConstantMemberObject( getMemberRules() ), name(n), index(indx) {
    
    // initialize
    parent = NULL;
}


/** Copy constructor. We use a shallow copy. */
TopologyNode::TopologyNode(const TopologyNode &n) : ConstantMemberObject(n) {
    
    // copy the members
    name = n.name;
    index = n.index;
    parent = n.parent;
    // copy the children
    for (std::vector<TopologyNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++) {
        TopologyNode* theNode = *it;
        TopologyNode* theClone = theNode->clone();
        children.push_back( theClone );
        theClone->setParent(this);
    }
    
    // add myself as a new child to the parent node
//    if (parent != NULL) {
//        parent->addChild(this);
//    }
    
}


/** Destructor */
TopologyNode::~TopologyNode(void) {
    
    // we do not own the parent so we do not delete it
    
    // free memory of children
    removeAllChildren();
    
    // make sure that I was removed from my parent
    parent->removeChild(this);
}


TopologyNode& TopologyNode::operator=(const TopologyNode &n) {
    
    if (this == &n) {
        removeAllChildren();
        
        // copy the members
        name = n.name;
        index = n.index;
        parent = n.parent;
        // copy the children
        for (std::vector<TopologyNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++) {
            children.push_back( (*it)->clone() );
        }
        
        // add myself as a new child to the parent node
        parent->addChild(this);
    }
    
    return *this;
}


/** Add a child node. We own it from here on. */
void TopologyNode::addChild(TopologyNode* c) {
    
    // add the child to our internal vector
    children.push_back(c);
    
    name = buildNewickString( *this );
}


/* Build newick string */
std::string TopologyNode::buildNewickString(const TopologyNode& node) const {
    // create the newick string
    std::string newick;
    
    // test whether this is a internal or external node
    if (node.isTip()) {
        // this is a tip so we just return the name of the node
        newick = node.getName();
    }
    else {
        newick = "(";
        for (size_t i=0; i<(node.getNumberOfChildren()-1); i++) {
            newick += buildNewickString(node.getChild(i)) + ",";
        }
        newick += buildNewickString(node.getChild(node.getNumberOfChildren()-1)) + ")";
    }
    
    return newick;
}


/** Clone function */
TopologyNode* TopologyNode::clone(void) const {
    
    return new TopologyNode(*this);
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& TopologyNode::getTypeSpec(void) const {
    return typeSpec;
}


bool TopologyNode::equals(const TopologyNode& node) const {
    // test if the name is the same
    if (name != node.name) {
        return false;
    }
    
    // test if the index is the same
    if (index != node.index) {
        return false;
    }
    
    // test if the parent is the same
    if (parent != node.parent) {
        return false;
    }
    
    // test if the size of the children differs
    if (children.size() != node.children.size()) {
        return false;
    }
    
    // test if all children are the same
    for (size_t i=0; i<children.size(); i++) {
        if (children[i]->equals(*node.children[i])) {
            return false;
        }
    }
    
    return true;
}


RbLanguageObject* TopologyNode::executeOperationSimple(const std::string& name, Environment& args) {
    
    if (name == "isTip") {
        return new RbBoolean( isTip() );
    }
    else if (name == "isRoot") {
        return new RbBoolean( isRoot() );
    }
    else if (name == "isInterior") {
        return new RbBoolean( !isTip() );
    }
    else if (name == "ancestor") {
        return parent->clone();
    }
    else
        throw RbException("No member method called '" + name + "'");
    
    return NULL;
}


/** Get child at index i */
const TopologyNode& TopologyNode::getChild(size_t i) const {
    
    return *children[i];
}


/** Get child at index i */
TopologyNode& TopologyNode::getChild(size_t i) {
    
    return *children[i];
}


/** Loop over children and get their indices */
std::vector<int> TopologyNode::getChildrenIndices() const {

    std::vector<int> temp;

    for ( std::vector<TopologyNode* >::const_iterator i=children.begin(); i!=children.end(); i++ )
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
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* ancestorRules = new ArgumentRules();
    static ArgumentRules* isTipArgRules = new ArgumentRules();
    static ArgumentRules* isRootArgRules = new ArgumentRules();
    static ArgumentRules* isInteriorArgRules = new ArgumentRules();
    static bool          methodsSet = false;

    if ( methodsSet == false ) {
        
        methods.addFunction("ancestor",   new MemberFunction(TopologyNode_name, ancestorRules) );
        methods.addFunction("isTip",      new MemberFunction(RbBoolean_name,      isTipArgRules) );
        methods.addFunction("isRoot",     new MemberFunction(RbBoolean_name,      isRootArgRules) );
        methods.addFunction("isInterior", new MemberFunction(RbBoolean_name,      isInteriorArgRules) );
        
        // Necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }

    return methods;
}


const MemberRules& TopologyNode::getMemberRules(void) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


const TopologyNode& TopologyNode::getParent(void) const { 
    return *parent; 

}


/** Print value for user */
void TopologyNode::printValue(std::ostream& o) const {
    
    o << name;
}



void TopologyNode::refreshNewickString(void) {
    name = buildNewickString( *this );
    
    // call the parent to refresh its newick string
    if (parent != NULL) {
        parent->refreshNewickString();
    }
}


/** Remove all children. We need to call intelligently the destructor here. */
void TopologyNode::removeAllChildren(void) {
    
    // free the memory
    for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); it++) {
        TopologyNode* theNode = *it;
        delete theNode;
    }
    
    // empty the children vector
    children.clear();
    
    name = "";
}

/** Remove a child from the vector of children */
void TopologyNode::removeChild(TopologyNode* p) {
    
    std::vector<TopologyNode* >::iterator it = find(children.begin(), children.end(), p);
    if ( it != children.end() ) {
        children.erase(it);
//        delete p;
    }
//    else 
//        throw(RbException("Cannot find node in list of children nodes"));
    
    name = buildNewickString( *this );
}


void TopologyNode::setName(std::string const &n) {
    name = n;
    
    // call the parent to refresh its newick string
    if (parent != NULL) {
        parent->refreshNewickString();
    }
}


void TopologyNode::setParent(TopologyNode* p) {
    
    // we only do something if this isn't already our parent
    if (p != parent) {
        // we do not own the parent so we do not have to delete it
        parent = p;
        
        parent->refreshNewickString();
    }
}


