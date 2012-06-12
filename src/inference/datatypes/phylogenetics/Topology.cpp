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

#include "RbException.h"
#include "RbUtil.h"
#include "Topology.h"
#include "TopologyNode.h"


/* Default constructor */
Topology::Topology(void) : root( NULL ) {

}


/* Copy constructor */
Topology::Topology(const Topology& t) : root( NULL ) {

    // set the parameters
    isRooted = t.isRooted;
    isBinary = t.isBinary;

    // need to perform a deep copy of the tree nodes
    if (t.root != NULL) {
        root = t.getRoot().clone();
        
        // fill the nodes vector
        //    fillNodesByPreorderTraversal(root);
        fillNodesByPhylogeneticTraversal(root);
    }
    
}


/* Destructor */
Topology::~Topology(void) {
    
    nodes.clear();
    
    delete root;
}


Topology& Topology::operator=(const Topology &t) {
    
    if (this != &t) {
        
        nodes.clear();
        delete root;
        
        isBinary = t.isBinary;
        isRooted = t.isRooted;
        
        // set the root. This will also set the nodes vector.
        setRoot(t.root->clone());
    }
    
    return *this;
}


/* Clone function */
Topology* Topology::clone(void) const {

    return new Topology(*this);
}


/* fill the nodes vector by a preorder traversal recursively starting with this node. */
void Topology::fillNodesByPreorderTraversal(const TopologyNode* node) {

    // this is preorder so add yourself first
    nodes.push_back(node);
    
    // now call this function recursively for all your children
    for (size_t i=0; i<node->getNumberOfChildren(); i++) {
        fillNodesByPreorderTraversal(&node->getChild(i));
    }
}

/* fill the nodes vector by a phylogenetic traversal recursively starting with this node. 
 * The tips fill the slots 0,...,n-1 followed by the internal nodes and then the root.
 */
void Topology::fillNodesByPhylogeneticTraversal(const TopologyNode* node) {
    
    // now call this function recursively for all your children
    for (size_t i=0; i<node->getNumberOfChildren(); i++) {
        fillNodesByPhylogeneticTraversal(&node->getChild(i));
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


bool Topology::getIsBinary( void ) const {
    return isBinary;
}


bool Topology::getIsRooted( void ) const {
    return isRooted;
}


const std::vector<const TopologyNode *>& Topology::getNodes( void ) const {
    return nodes;
}


/** Calculate the number of interior nodes in the tree by deducing the number of
 tips from number of nodes, and then subtract 1 more if the tree is rooted. */
size_t Topology::getNumberOfInteriorNodes( void ) const {
    
    size_t preliminaryNumIntNodes = getNumberOfNodes() - getNumberOfTips();
    
    if ( isRooted )
        return preliminaryNumIntNodes - 1;
    else
        return preliminaryNumIntNodes;
}


size_t Topology::getNumberOfNodes( void ) const {
    return nodes.size();
}


/** Calculate and return the number of tips on the tree by going through the vector
 of nodes, querying each about its tip status. */
size_t Topology::getNumberOfTips( void ) const {
    
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
const TopologyNode& Topology::getInteriorNode( int indx ) const {
    
    // \TODO: Bound checking, maybe draw from downpass array instead
    return *nodes[ indx + getNumberOfTips() ];
}


const TopologyNode& Topology::getRoot( void ) const {
    
    return *root;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the tips are first in the nodes vector. */
const TopologyNode& Topology::getTipNode( size_t indx ) const {
    
    // \TODO: Bound checking
    return *nodes[ indx ];
}


void Topology::setRoot( TopologyNode* r) {

    // set the root
    root = r;
    
    nodes.clear();
    
    // bootstrap all nodes from the root and add the in a pre-order traversal
    // fillNodesByPreorderTraversal(r);
    fillNodesByPhylogeneticTraversal(r);
}

