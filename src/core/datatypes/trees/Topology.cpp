//
//  Topology.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/4/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RbException.h"
#include "Topology.h"
#include "TopologyNode.h"

#include <ostream>


using namespace RevBayesCore;


/* Default constructor */
Topology::Topology(void) : 
    root( NULL ), 
    numTips( 0 ), 
    numNodes( 0 ), 
    rooted( false ), 
    binary( true ) 
{
    
}


/* Copy constructor */
Topology::Topology(const Topology& t) : 
    root( NULL ) 
{
    
    // set the parameters
    binary      = t.binary;
    numTips     = t.numTips;
    numNodes    = t.numNodes;
    rooted      = t.rooted;
    
    // need to perform a deep copy of the BranchLengthTree nodes
    if (t.root != NULL) 
    {
        TopologyNode * newRoot = t.getRoot().clone();
        
        // set the root. This will also set the nodes vector.
        setRoot(newRoot);
    }
    
}


/* Destructor */
Topology::~Topology(void) 
{

    nodes.clear();
    
    delete root;
}


Topology& Topology::operator=(const Topology &t) 
{
    
    if (this != &t) 
    {
        
        nodes.clear();
        delete root;
        root = NULL;
        
        binary      = t.binary;
        numTips     = t.numTips;
        numNodes    = t.numNodes;
        rooted      = t.rooted;
        
        TopologyNode* newRoot = t.root->clone();
        
        // set the root. This will also set the nodes vector.
        setRoot(newRoot);
    }
    
    return *this;
}


/* Clone function */
Topology* Topology::clone(void) const 
{
    
    return new Topology(*this);
}


std::vector<std::string> Topology::getTipNames( void ) const 
{
    std::vector<std::string> names;
    for (size_t i = 0; i < getNumberOfTips(); ++i) 
    {
        const TopologyNode& n = getTipNode( i );
        names.push_back( n.getName() );
    }
    
    return names;
}

/* fill the nodes vector by a phylogenetic traversal recursively starting with this node. 
 * The tips fill the slots 0,...,n-1 followed by the internal nodes and then the root.
 */
void Topology::fillNodesByPhylogeneticTraversal(TopologyNode* node) {
    
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


const std::string& Topology::getNewickRepresentation( void ) const {
    
    return root->computeNewick();
}


std::vector<TopologyNode *> Topology::getNodes( void ) const {
    
    return nodes;
}


TopologyNode& Topology::getNode(size_t idx) {
    
    if ( idx >= nodes.size() ) 
    {
        throw RbException("Index out of bounds in getNode of Topology.");
    }
    
    return *nodes[idx];
}


const TopologyNode& Topology::getNode(size_t idx) const {
    
    if ( idx >= nodes.size() ) 
    {
        throw RbException("Index out of bounds in getNode of Topology.");
    }
    
    return *nodes[idx];
}


/** Calculate the number of interior nodes in the BranchLengthTree by deducing the number of
 tips from number of nodes, and then subtract 1 more if the BranchLengthTree is rooted. */
size_t Topology::getNumberOfInteriorNodes( void ) const {
    
    size_t preliminaryNumIntNodes = getNumberOfNodes() - getNumberOfTips();
    
    if ( isRooted() )
        return preliminaryNumIntNodes - 1;
    else
        return preliminaryNumIntNodes;
}


size_t Topology::getNumberOfNodes( void ) const {
    
    return nodes.size();
}


std::string Topology::getPlainNewickRepresentation( void ) const {
    
    return root->computePlainNewick();
}


/** Calculate and return the number of tips on the BranchLengthTree by going through the vector
 of nodes, querying each about its tip status. */
size_t Topology::getNumberOfTips( void ) const {
    
    size_t n = 0;
    for (size_t i=0; i<nodes.size(); i++) {
        if (nodes[i]->isTip() == true)
            n++;
    }
    return n;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the root is always the last and the tips the first in the nodes vector. */
const TopologyNode& Topology::getInteriorNode( size_t indx ) const {
    
    // \TODO: Bound checking, maybe draw from downpass array instead
    return *nodes[ indx + getNumberOfTips() ];
}


TopologyNode& Topology::getRoot( void ) {
    
    return *root;
}


const TopologyNode& Topology::getRoot( void ) const {
    
    return *root;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the tips are first in the nodes vector. */
TopologyNode& Topology::getTipNode( size_t indx ) {
    
#ifdef ASSERTIONS_BranchLengthTree
    if ( indx >= getNumberOfTips() ) {
        throw RbException("Index out of bounds in getTipNode() of BranchLengthTree!");
    }
    if (!nodes[ indx ]->isTip()) {
        throw RbException("Node at index is not a tip but should have been!");
    }
#endif
    
    // \TODO: Bound checking
    return *nodes[ indx ];
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the tips are first in the nodes vector. */
const TopologyNode& Topology::getTipNode( size_t indx ) const {
    
    // \TODO: Bound checking
    return *nodes[ indx ];
}


bool Topology::isBinary( void ) const {
    return binary;
}


bool Topology::isRooted( void ) const {
    return rooted;
}


void Topology::setRooted(bool tf) {
    rooted = tf;
}



void Topology::setRoot( TopologyNode* r) {
    
    // set the root
    root = r;
    
//    root->setTopology( this );
    
    nodes.clear();
    
    // bootstrap all nodes from the root and add the in a pre-order traversal
    // fillNodesByPreorderTraversal(r);
    fillNodesByPhylogeneticTraversal(r);
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        nodes[i]->setIndex(i);
    }
    
//    if (nodesByIndex.size() == 0)
//        fillNodesByIndex();
    
    numNodes = nodes.size();
    
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const Topology& x) {
    o << x.getNewickRepresentation();
    
    return o;
}
