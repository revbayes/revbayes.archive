/**
 * @file
 * This file contains the implementation of a time-calibrated tree. This version of a tree
 * contains nodes with times.
 *
 * @brief Implementation of a time calibrated tree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-17, version 1.0
 *
 * $Id: Tree.cpp 1651 2012-07-05 14:47:08Z hoehna $
 */

#include "DagNode.h"
#include "RbException.h"
#include "RbOptions.h"
#include "Tree.h"
#include "Taxon.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "TypedDagNode.h"

using namespace RevBayesCore;

/* Default constructor */
Tree::Tree(void) : 
    topology( NULL ), 
    changeEventHandler(),
    ownsTopology( true )
{
}


/* Copy constructor */
Tree::Tree(const Tree& t) : 
    topology( NULL ),
    changeEventHandler( ),
    ownsTopology( t.ownsTopology )
{
    
    // set the topology
    if ( ownsTopology ) 
    {
        topology      = new Topology( *t.topology );
    }
    else
    {
        topology      = t.topology;
    }
    
    // add ourselves as a tree using the topology
    topology->addTree( this );
}


/* Destructor */
Tree::~Tree(void) 
{
    if ( ownsTopology )
    {
        delete topology;
    }
    else
    {
        // just remove us as a user of the topology
        topology->removeTree( this );
    }
    
}


Tree& Tree::operator=(const Tree &t)
{
    
    if (this != &t) 
    {
        // nothing really to do here, should be done in the derived classes
        // @todo: Find a better solution - Sebastian
        // Problem: If we redraw the tree because the initial states are invalid, 
        // then we somehow need to remember the tree event change listeners.
        // But it is not nice if the tree distribution needs to remember this!!!
//        changeEventHandler = t.changeEventHandler;
        
        // free the topology if we owned it before
        if ( ownsTopology ) 
        {
            delete topology;
        }
        ownsTopology = t.ownsTopology;
        
        if ( ownsTopology ) 
        {
            topology      = new Topology( *t.topology );
        }
        else
        {
            topology      = t.topology;
        }
        
        // add ourselves as a tree using this topology
        topology->addTree( this );
    }
    
    return *this;
}


bool Tree::operator==(const Tree &t) const
{
    
    return getNewickRepresentation() == t.getNewickRepresentation();
}


bool Tree::operator!=(const Tree &t) const
{
    
    return !operator==(t);
}


bool Tree::operator<(const Tree &t) const
{
    
    return getNewickRepresentation() < t.getNewickRepresentation();
}


bool Tree::operator<=(const Tree &t) const
{
    
    return operator<(t) || operator==(t);
}


void Tree::addBranchParameter(std::string const &name, const std::vector<double> &parameters, bool internalOnly)
{
    
    getRoot().addBranchParameter(name,parameters,internalOnly);
    
}


void Tree::addNodeParameter(std::string const &name, const std::vector<double> &parameters, bool internalOnly)
{
    
    getRoot().addParameter(name,parameters,internalOnly);
    
}

void Tree::addNodeParameter(std::string const &name, const std::vector<std::string*> &parameters, bool internalOnly)
{
    
    getRoot().addParameter(name,parameters,internalOnly);
    
}


void Tree::clearBranchParameters()
{
    
    getRoot().clearBranchParameters();

}


void Tree::clearNodeParameters()
{
    
    getRoot().clearNodeParameters();
	
}



void Tree::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, double &rv) const
{
    
    if ( n == "rootAge" )
    {
        rv = getRoot().getAge();
    }
    else if ( n == "branchLength" )
    {
        int index = static_cast<const TypedDagNode<int> *>( args[0] )->getValue()-1;
        rv = getBranchLength( index );
    }
    else if ( n == "nodeAge" )
    {
        int index = static_cast<const TypedDagNode<int> *>( args[0] )->getValue()-1;
        rv = getAge( index );
    }
    else
    {
        throw RbException("A tree object does not have a member method called '" + n + "'.");
    }
    
}


void Tree::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, int &rv) const
{
    
    if ( n == "parent" )
    {
        int index = static_cast<const TypedDagNode<int> *>( args[0] )->getValue()-1;
        rv = int( getNode( index ).getParent().getIndex() )+1;
    }
    else
    {
        throw RbException("A tree object does not have a member method called '" + n + "'.");
    }
    
}

std::vector<std::string> Tree::getTipNames() const
{

    return topology->getTipNames();
}

std::vector<Taxon> Tree::getTaxa() const
{
    return topology->getTaxa();
}

std::vector<std::string> Tree::getSpeciesNames() const
{
    return topology->getSpeciesNames();
}

TopologyNode& Tree::getNode(size_t idx)
{
    
    return *topology->getNodes()[ idx ];
}

const TopologyNode& Tree::getNode(size_t idx) const
{
    
    return topology->getNode( idx );
}

std::vector<TopologyNode*> Tree::getNodes(void) const
{
    
    return topology->getNodes();
}

size_t Tree::getNumberOfInteriorNodes(void) const
{
    
    return topology->getNumberOfInteriorNodes();
}

size_t Tree::getNumberOfNodes(void) const
{
    
    return topology->getNumberOfNodes();
}

size_t Tree::getNumberOfTips(void) const
{
    
    return topology->getNumberOfTips();
}

const TopologyNode& Tree::getInteriorNode(size_t indx) const
{
    
    return topology->getInteriorNode( indx );
}

const std::string& Tree::getNewickRepresentation() const
{
    
    return topology->getNewickRepresentation();
}

TopologyNode& Tree::getRoot(void)
{
    //return *topology->getNodes()[topology->getNumberOfNodes()-1];
    return topology->getRoot(); //SK
}

const TopologyNode& Tree::getRoot(void) const
{
    return topology->getRoot();
}

TopologyNode& Tree::getTipNode(size_t indx)
{
    return *topology->getNodes()[indx];
}

const TopologyNode& Tree::getTipNode(size_t indx) const
{
    
    return topology->getTipNode(indx);
}


double Tree::getTmrca(const TopologyNode &n)
{
    
    return topology->getRoot().getTmrca( n );
}


TreeChangeEventHandler& Tree::getTreeChangeEventHandler( void ) const
{
    
    return changeEventHandler;
}


const Topology& Tree::getTopology( void ) const
{
    
    return *topology;
}


bool Tree::hasSameTopology(const RevBayesCore::Tree &t) const
{
    
    return hasSameTopology( t.getTopology() );
}


bool Tree::hasSameTopology(const RevBayesCore::Topology &t) const
{
    
    return topology->getPlainNewickRepresentation() == t.getPlainNewickRepresentation();
}


bool Tree::isBinary(void) const 
{
    
    return topology->isBinary();
}


bool Tree::isRooted(void) const 
{
    
    return topology->isRooted();
}


void Tree::setTopology(const Topology *t, bool owns) 
{
    // free the old topology if necessary
    if ( topology != NULL )
    {
        // only delete if we own it
        if ( ownsTopology )
        {
            delete topology;
        }
        else
        {
            // just remove ourselves as a user of the topology
            topology->removeTree( this );
        }
        
    }
    
    ownsTopology = owns;
    
    // set the topology of this tree
    topology = const_cast<Topology*>( t );
    
    // add ourselves as a tree using this topology
    topology->addTree( this );
    
    resizeElementVectors( t->getNumberOfNodes() );

}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const Tree& x)
{
    
    o << x.getNewickRepresentation();
        
    return o;
}
