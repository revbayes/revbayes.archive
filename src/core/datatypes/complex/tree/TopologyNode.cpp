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
    children = n.children;
    
}


/** Destructor */
TopologyNode::~TopologyNode(void) {
    
    // we do not own the parent so we do not delete it
    
    // free memory of children
    removeAllChildren();
}


/** Add a child node. We own it from here on. */
void TopologyNode::addChild(RbPtr<TopologyNode> c) {
    
    // add the child to our internal vector
    children.push_back(c);
    
    name = buildNewickString(RbPtr<TopologyNode>( this ));
}


/* Build newick string */
std::string TopologyNode::buildNewickString(const RbPtr<TopologyNode> node) const {
    // create the newick string
    std::string newick;
    
    // test whether this is a internal or external node
    if (node->isTip()) {
        // this is a tip so we just return the name of the node
        newick = node->getName();
    }
    else {
        newick = "(";
        for (size_t i=0; i<(node->getNumberOfChildren()-1); i++) {
            newick += buildNewickString(node->getChild(i)) + ",";
        }
        newick += buildNewickString(node->getChild(node->getNumberOfChildren()-1)) + ")";
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


bool TopologyNode::equals(RbPtr<TopologyNode> node) const {
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


RbPtr<RbLanguageObject> TopologyNode::executeOperation(const std::string& name, Environment& args) {
    
    if (name == "isTip") {
        return RbPtr<RbLanguageObject>( new RbBoolean(isTip()) );
    }
    else if (name == "isRoot") {
        return RbPtr<RbLanguageObject>( new RbBoolean(isRoot()) );
    }
    else if (name == "isInterior") {
        return RbPtr<RbLanguageObject>( new RbBoolean(!isTip()) );
    }
    else if (name == "ancestor") {
        return RbPtr<RbLanguageObject>( parent );
    }
    else
        throw RbException("No member method called '" + name + "'");
    
    return RbPtr<RbLanguageObject>::getNullPtr();
}


/** Loop over children and get their indices */
std::vector<int> TopologyNode::getChildrenIndices() const {

    std::vector<int> temp;

    for ( std::vector<RbPtr<TopologyNode> >::const_iterator i=children.begin(); i!=children.end(); i++ )
        temp.push_back( (*i)->getIndex() );

    return temp;
}


/** Get class vector describing type of object */
const VectorString& TopologyNode::getClass() const {
    
    static VectorString rbClass = VectorString(TopologyNode_name) + MemberObject::getClass();
    return rbClass;
}


/** Get method specifications */
const RbPtr<MethodTable> TopologyNode::getMethods(void) const {
    
    static RbPtr<MethodTable> methods( new MethodTable() );
    static RbPtr<ArgumentRules> ancestorRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> isTipArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> isRootArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> isInteriorArgRules( new ArgumentRules() );
    static bool          methodsSet = false;

    if ( methodsSet == false ) {
        
        methods->addFunction("ancestor",   RbPtr<RbFunction>( new MemberFunction(TopologyNode_name, ancestorRules) ) );
        methods->addFunction("isTip",      RbPtr<RbFunction>( new MemberFunction(RbBoolean_name,      isTipArgRules) ) );
        methods->addFunction("isRoot",     RbPtr<RbFunction>( new MemberFunction(RbBoolean_name,      isRootArgRules) ) );
        methods->addFunction("isInterior", RbPtr<RbFunction>( new MemberFunction(RbBoolean_name,      isInteriorArgRules) ) );
        
        // Necessary call for proper inheritance
        methods->setParentTable( RbPtr<FunctionTable>( MemberObject::getMethods().get() ) );
        methodsSet = true;
    }

    return methods;
}


const RbPtr<MemberRules> TopologyNode::getMemberRules(void) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/** Print value for user */
void TopologyNode::printValue(std::ostream& o) const {
    
    o << name;
}



void TopologyNode::refreshNewickString(void) {
    name = buildNewickString( RbPtr<TopologyNode>( this ) );
    
    // call the parent to refresh its newick string
    if (parent != NULL) {
        parent->refreshNewickString();
    }
}


/** Remove all children. We need to call intelligently the destructor here. */
void TopologyNode::removeAllChildren(void) {
    
    // empty the children vector
    children.clear();
    
    name = "";
}

/** Remove a child from the vector of children */
void TopologyNode::removeChild(RbPtr<TopologyNode> p) {
    
    std::vector<RbPtr<TopologyNode> >::iterator it = find(children.begin(), children.end(), p);
    if ( it != children.end() ) {
        children.erase(it);
    }
    else 
        throw(RbException("Cannot find node in list of children nodes"));
    
    name = buildNewickString( RbPtr<TopologyNode>( this ) );
}


std::string TopologyNode::richInfo(void) const {

    std::ostringstream o;
    o <<  "Topology node: ";
    printValue(o);
    return o.str();
}


void TopologyNode::setName(std::string const &n) {
    name = n;
    
    // call the parent to refresh its newick string
    if (parent != NULL) {
        parent->refreshNewickString();
    }
}


void TopologyNode::setParent(RbPtr<TopologyNode> p) {
    
    // we only do something if this isn't already our parent
    if (p.get() != parent) {
        // we do not own the parent so we do not have to delete it
        parent = p.get();
        
        parent->refreshNewickString();
    }
}


