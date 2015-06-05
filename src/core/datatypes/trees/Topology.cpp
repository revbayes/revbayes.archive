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
    binary( true ),
    rooted( false ),
    numTips( 0 ), 
    numNodes( 0 ),
    treesUsingThisTopology()
{
}


/* Copy constructor */
Topology::Topology(const Topology& t) : 
    root( NULL ),
    treesUsingThisTopology()
{

    // set the parameters
    binary      = t.binary;
    numTips     = t.numTips;
    numNodes    = t.numNodes;
    rooted      = t.rooted;
    
    // nobody is using us yet, so we leave treesUsingThisTopology empty

    // need to perform a deep copy of the BranchLengthTree nodes
    if (t.root != NULL) 
    {
        TopologyNode * newRoot = t.getRoot().clone();
        
        // set the root. This will also set the nodes vector.
        setRoot(newRoot);
    }
    
}

//SK
Topology::Topology(TopologyNode* Root, std::vector<TopologyNode*> Nodes) :
root(Root),
nodes(Nodes),
treesUsingThisTopology()
{
	numNodes = nodes.size();
	numTips = 0;
	for (size_t i = 0; i < numNodes; i++) {
		if (nodes[i]->getNumberOfChildren() == 0)
			numTips++;
	}
    
	if (root->getNumberOfChildren() == 2) {
		rooted = true;
		if (numNodes == 2 * numTips - 1) {
			binary = true;
		} else binary = false;
	}
	else {
		rooted = false;
		if (numNodes == 2 * numTips - 2) {
			binary = true;
		}
		else binary = false;
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
        
        // nobody is using the new topology yet
        treesUsingThisTopology.clear();
    }
    
    return *this;
}


/**
 * Add a tree using this topology. If it is the first tree,
 * we set the tree pointer of all topology nodes to this tree.
 * If it is not the first tree, we can leave the tree pointer
 * of the topology nodes as is.
 */
void Topology::addTree( Tree* t )
{
    if ( treesUsingThisTopology.empty() )
        root->setTree( t );
    
    treesUsingThisTopology.insert( t );
}


/* Clone function */
Topology* Topology::clone(void) const 
{
    return new Topology(*this);
}


size_t Topology::getTipIndex( std::string name ) const
{
    for (size_t i = 0; i < getNumberOfTips(); ++i)
    {
        const TopologyNode& n = getTipNode( i );
        if (name == n.getName())
        {
            return n.getIndex();
        }
    }
    // if name not found
    return ((size_t)-1);
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


std::vector< Taxon > Topology::getTaxa( void ) const
{
    std::vector< Taxon > taxa;
    for (size_t i = 0; i < getNumberOfTips(); ++i)
    {
        const TopologyNode& n = getTipNode( i );
        taxa.push_back( n.getTaxon() );
    }
    
    return taxa;
}

std::vector<std::string> Topology::getSpeciesNames() const
{
    std::vector< std::string > snames;
    for (size_t i = 0; i < getNumberOfTips(); ++i)
    {
        const TopologyNode& n = getTipNode( i );
        snames.push_back( n.getTaxon().getSpeciesName() );
    }
    
    return snames;
}

/* fill the nodes vector by a phylogenetic traversal recursively starting with this node.
 * The tips fill the slots 0,...,n-1 followed by the internal nodes and then the root.
 */
void Topology::fillNodesByPhylogeneticTraversal(TopologyNode* node)
{
    
    // now call this function recursively for all your children
    for (size_t i=0; i<node->getNumberOfChildren(); i++)
    {
        fillNodesByPhylogeneticTraversal(&node->getChild(i));
    }
    
    if (node->isTip())
    {
        // all the tips go to the beginning
        nodes.insert(nodes.begin(), node);
    } 
    else
    {
        // this is phylogenetic ordering so the internal nodes come last
        nodes.push_back(node);
    }
}


const std::string& Topology::getNewickRepresentation( void ) const
{
    
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


const TopologyNode& Topology::getNode(size_t idx) const
{
    
    if ( idx >= nodes.size() ) 
    {
        throw RbException("Index out of bounds in getNode of Topology.");
    }
    
    return *nodes[idx];
}


/** Calculate the number of interior nodes in the BranchLengthTree by deducing the number of
 tips from number of nodes, and then subtract 1 more if the BranchLengthTree is rooted. */
size_t Topology::getNumberOfInteriorNodes( void ) const
{
    
    size_t preliminaryNumIntNodes = getNumberOfNodes() - getNumberOfTips();
    
    if ( isRooted() )
    {
        return preliminaryNumIntNodes - 1;
    }
    else
    {
        return preliminaryNumIntNodes;
    }
    
}


size_t Topology::getNumberOfNodes( void ) const {
    
    return nodes.size();
}


std::string Topology::getPlainNewickRepresentation( void ) const {
    
    return root->computePlainNewick();
}


/** Calculate and return the number of tips on the BranchLengthTree by going through the vector
 of nodes, querying each about its tip status. */
size_t Topology::getNumberOfTips( void ) const
{
    
    size_t n = 0;
    for (size_t i=0; i<nodes.size(); i++)
    {
        if (nodes[i]->isTip() == true)
        {
            n++;
        }
    }
    return n;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the root is always the last and the tips the first in the nodes vector. */
const TopologyNode& Topology::getInteriorNode( size_t indx ) const
{
    
    // \TODO: Bound checking, maybe draw from downpass array instead
    return *nodes[ indx + getNumberOfTips() ];
}


TopologyNode& Topology::getRoot( void )
{
    
    return *root;
}


const TopologyNode& Topology::getRoot( void ) const
{
    
    return *root;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the tips are first in the nodes vector. */
TopologyNode& Topology::getTipNode( size_t indx )
{
    
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
const TopologyNode& Topology::getTipNode( size_t indx ) const
{
    
    // \TODO: Bound checking
    return *nodes[ indx ];
}


bool Topology::isBinary( void ) const
{
    return binary;
}


bool Topology::isRooted( void ) const
{
    return rooted;
}


/**
 * Remove a tree using this topology. We do not know if the tree to
 * be removed is currently used as the tree pointer by the topology
 * nodes. To make sure that the topology nodes have a valid tree
 * pointer after the removal, we reset their tree pointer to one of
 * the remaining trees.
 */
void Topology::removeTree( Tree* t )
{
    treesUsingThisTopology.erase( t );
    
    if ( treesUsingThisTopology.empty() )
        root->setTree( NULL );
    else
        root->setTree( *treesUsingThisTopology.begin() );
}


void Topology::setRooted(bool tf) {
    rooted = tf;
}



void Topology::setRoot( TopologyNode* r, bool resetIndex )
{
    
    // set the root
    root = r;
    
    nodes.clear();
    
    // bootstrap all nodes from the root and add the in a pre-order traversal
    // fillNodesByPreorderTraversal(r);
    fillNodesByPhylogeneticTraversal(r);
    
    if ( resetIndex == true )
    {
        for (unsigned int i = 0; i < nodes.size(); ++i)
        {
            nodes[i]->setIndex(i);
        }
    }
    
    numNodes = nodes.size();
    
    // Set the tree pointer of the nodes
    if ( treesUsingThisTopology.empty() )
        root->setTree( NULL );
    else
        root->setTree( *treesUsingThisTopology.begin() );
    
}


// method to reindex nodes by their order
// necessary to keep track of branch lengths
void Topology::reindexNodes()
{
	
    for (unsigned int i = 0; i < nodes.size(); ++i)
    {
        nodes[i]->setIndex(i);
    }
    
}


// method to order nodes by their existing index
// used when reading in tree with existing node indexes we need to keep
void Topology::orderNodesByIndex()
{
	
	std::vector<TopologyNode*> nodes_copy = std::vector<TopologyNode*>(numNodes);
	for (int i = 0; i < numNodes; i++)
    {
		for (int j = 0; j < numNodes; j++)
        {
			if (i == nodes[j]->getIndex())
            {
				nodes_copy[i] = nodes[j];
			}
		}
	}
	nodes = nodes_copy;
    
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const Topology& x) {
    o << x.getNewickRepresentation();
    
    return o;
}
