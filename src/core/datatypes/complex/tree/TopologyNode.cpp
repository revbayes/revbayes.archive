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
#include "MethodTable.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "TopologyNode.h"
#include "ValueRule.h"

#include <algorithm>


/** Default constructor (interior node, no name). Give the node an optional index ID */
TopologyNode::TopologyNode(int indx) : MemberObject( getMemberRules() ), name(""), index(indx), isRootNode( true ), isTipNode( true ), isInteriorNode( false ) {
    
    // initialize
    parent = NULL;
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const std::string& n, int indx) : MemberObject( getMemberRules() ), name(n), index(indx), isRootNode( true ), isTipNode( true ), isInteriorNode( false ) {
    
    // initialize
    parent = NULL;
}


/** Copy constructor. We use a shallow copy. */
TopologyNode::TopologyNode(const TopologyNode &n) : MemberObject(n) {
    
    // copy the members
    name            = n.name;
    index           = n.index;
    parent          = n.parent;
    isInteriorNode  = n.isInteriorNode;
    isTipNode       = n.isTipNode;
    isRootNode      = n.isRootNode;
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
    if (parent != NULL)
        parent->removeChild(this);
}


TopologyNode& TopologyNode::operator=(const TopologyNode &n) {
    
    if (this == &n) {
        // delegate to the parent
        MemberObject::operator=(n);
        
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
    
    isTipNode.setValue(false);
    isInteriorNode.setValue(true);
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


/** Get class name of object */
const std::string& TopologyNode::getClassName(void) { 
    
    static std::string rbClassName = "Topology Node";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TopologyNode::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& TopologyNode::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


bool TopologyNode::equals(const TopologyNode& node) const {
    
    if (this == &node) {
        return true;
    }
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


const RbLanguageObject& TopologyNode::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
    
    if (name == "isTip") {
        return isTipNode;
    }
    else if (name == "isRoot") {
        return isRootNode;
    }
    else if (name == "isInterior") {
        return isInteriorNode;
    }
    else if (name == "ancestor") {
        return *parent;
    }
    else if (name == "getName") {
        return this->name;
    }
    else
        throw RbException("No member method called '" + name + "'");
    
    return RbNullObject::getInstance();
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


/** Get method specifications */
const MethodTable& TopologyNode::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;

    if ( methodsSet == false ) {
        
        ArgumentRules* ancestorRules = new ArgumentRules();
        ArgumentRules* getNameRules = new ArgumentRules();
        ArgumentRules* isTipArgRules = new ArgumentRules();
        ArgumentRules* isRootArgRules = new ArgumentRules();
        ArgumentRules* isInteriorArgRules = new ArgumentRules();
        
        methods.addFunction("ancestor",   new MemberFunction(TopologyNode::getClassTypeSpec(), ancestorRules) );
        methods.addFunction("getName",    new MemberFunction(RbString::getClassTypeSpec(),     getNameRules) );
        methods.addFunction("isTip",      new MemberFunction(RbBoolean::getClassTypeSpec(),    isTipArgRules) );
        methods.addFunction("isRoot",     new MemberFunction(RbBoolean::getClassTypeSpec(),    isRootArgRules) );
        methods.addFunction("isInterior", new MemberFunction(RbBoolean::getClassTypeSpec(),    isInteriorArgRules) );
        
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
//    for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); it++) {
//        TopologyNode* theNode = *it;
//        delete theNode;
//    }
    while (children.size() > 0) {
        TopologyNode* theNode = children[0];
        delete theNode;
    }
    
    // empty the children vector
    children.clear();
    
    name = RbString("");
    
    isTipNode.setValue(true);
    isInteriorNode.setValue(false);
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
    
    isTipNode.setValue(children.size() == 0);
    isInteriorNode.setValue(children.size() > 0);
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
    
    isRootNode.setValue(false);
}


