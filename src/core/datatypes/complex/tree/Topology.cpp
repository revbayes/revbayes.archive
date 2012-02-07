/**
 * @file
 * This file contains the implementation of a Topology.
 *
 * @brief Implementation of Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends MemberObject
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "MutableMemberObject.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "VectorString.h"
#include "ValueRule.h"


// Definition of the static type spec member
const TypeSpec Topology::typeSpec(Topology_name);

/* Default constructor */
Topology::Topology(void) : ConstantMemberObject( getMemberRules() ) {

}


/* Copy constructor */
Topology::Topology(const Topology& t) : ConstantMemberObject( getMemberRules() ) {
    // set the parameters
    isRooted = t.isRooted;
    isBinary = t.isBinary;

    // need to perform a deep copy of the tree nodes
    root = cloneTree(*t.getRoot());
    
    // fill the nodes vector
//    fillNodesByPreorderTraversal(root);
    fillNodesByPhylogeneticTraversal(root);
}


/* Destructor */
Topology::~Topology(void) {
    
    nodes.clear();
}



TopologyNode* Topology::cloneTree(const TopologyNode& parent) {
    // get first a shallow copy
    TopologyNode* node = parent.clone();
    
    // replace all children by depp copies of the children
    for (size_t i=0; i<node->getNumberOfChildren(); i++) {
        // get the old child
        const TopologyNode* oldChild = node->getChild(0);
        
        // get a deep copy of the child
        TopologyNode* newChild = cloneTree(*oldChild);
        node->removeChild(oldChild);
        node->addChild(newChild);
        
        // set this node as the parent of the new deep copy
        newChild->setParent(node);
    }
    
    return node;
}

/* Clone function */
Topology* Topology::clone(void) const {

    return new Topology(*this);
}


/* Map calls to member methods */
RbLanguageObject* Topology::executeOperationSimple(const std::string& name, Environment* args) {
    
    if (name == "ntips") {
    
        return new Natural( (int)getNumberOfTips() );
    }
    else if (name == "nnodes") {
    
        return new Natural( (int)getNumberOfNodes() );
    }

    return MemberObject::executeOperationSimple( name, args );
}


/* fill the nodes vector by a preorder traversal recursively starting with this node. */
void Topology::fillNodesByPreorderTraversal(TopologyNode* node) {
    // this is preorder so add yourself first
    nodes.push_back(node);
    
    // now call this function recursively for all your children
    for (size_t i=0; i<node->getNumberOfChildren(); i++) {
        fillNodesByPreorderTraversal(node->getChild(i));
    }
}

/* fill the nodes vector by a phylogenetic traversal recursively starting with this node. 
 * The tips fill the slots 0,...,n-1 followed by the internal nodes and then the root.
 */
void Topology::fillNodesByPhylogeneticTraversal(TopologyNode* node) {
    
    // now call this function recursively for all your children
    for (size_t i=0; i<node->getNumberOfChildren(); i++) {
        fillNodesByPhylogeneticTraversal(node->getChild(i));
    }
    
    if (node->isTip()) {
        // all the tips go to the beginning
        nodes.insert(nodes.begin(), node);
    } 
    else {
        // this is phylogenetic ordering so the internal nodes come last
        nodes.push_back(node);
    }
}



/* Get class information */
const VectorString& Topology::getClass(void) const {
    
    static VectorString rbClass = VectorString(Topology_name) + ConstantMemberObject::getClass();
    return rbClass;
}


/* Get member rules */
const MemberRules* Topology::getMemberRules(void) const {
    
    static MemberRules* memberRules = new MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
const MethodTable* Topology::getMethods(void) const {
    
    static MethodTable* methods = new MethodTable();
    static ArgumentRules* ntipsArgRules = new ArgumentRules();
    static ArgumentRules* nnodesArgRules = new ArgumentRules();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        methods->addFunction("ntips",  new MemberFunction(TypeSpec(Natural_name), ntipsArgRules) );
        methods->addFunction("nnodes", new MemberFunction(TypeSpec(Natural_name), nnodesArgRules) );
        
        // necessary call for proper inheritance
        methods->setParentTable( MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/** Calculate the number of interior nodes in the tree by deducing the number of
 tips from number of nodes, and then subtract 1 more if the tree is rooted. */
size_t Topology::getNumberOfInteriorNodes(void) const {
    
    size_t preliminaryNumIntNodes = getNumberOfNodes() - getNumberOfTips();
    
    if ( isRooted )
        return preliminaryNumIntNodes - 1;
    else
        return preliminaryNumIntNodes;
}


/** Calculate and return the number of tips on the tree by going through the vector
 of nodes, querying each about its tip status. */
size_t Topology::getNumberOfTips(void) const {
    
    size_t n = 0;
    for (size_t i=0; i<nodes.size(); i++)
    {
        if (nodes[i]->isTip() == true)
            n++;
    }
    return n;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the root is always the last and the tips the first in the nodes vector. */
const TopologyNode* Topology::getInteriorNode( int indx ) const {
    
    // TODO: Bound checking, maybe draw from downpass array instead
    return nodes[ indx + getNumberOfTips() ];
}


const TopologyNode* Topology::getRoot( void ) const {
    
    return root;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the tips are first in the nodes vector. */
const TopologyNode* Topology::getTipNode( size_t indx ) const {
    
    // TODO: Bound checking
    return nodes[ indx ];
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Topology::getTypeSpec(void) const {
    return typeSpec;
}


/* Print the tree */
void Topology::printValue(std::ostream& o) const {

    o << root->getName();
}


/* Get a lot of information about the topology */
std::string Topology::richInfo(void) const {

    std::ostringstream o;
    o <<  "Tree: ";
    printValue(o);
    return o.str();
}

void Topology::setRoot( TopologyNode* r) {
    // set the root
    root = r;
    
    nodes.clear();
    
    // bootstrap all nodes from the root and add the in a pre-order traversal
//    fillNodesByPreorderTraversal(r);
    fillNodesByPhylogeneticTraversal(r);
}

