#include "DagNode.h"
#include "Proposal.h"
#include "RbException.h"


using namespace RevBayesCore;



Proposal::Proposal(void) :
    nodes()
{
    
}


Proposal::Proposal(const Proposal &p)  :
    nodes( p.nodes )
{
    
    for (std::set<DagNode*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        (*it)->incrementReferenceCount();
    }
    
}


Proposal::~Proposal( void )
{
    
    for (std::set<DagNode*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        DagNode *theNode = *it;
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete theNode;
        }
    }
    
}



Proposal& Proposal::operator=(const Proposal &p)
{
    
    if ( this != &p )
    {
        
        for (std::set<DagNode*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
        {
            DagNode *theNode = *it;
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete theNode;
            }
        }
        nodes.clear();
        
        nodes = p.nodes;
        
        for (std::set<DagNode*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
        {
            (*it)->incrementReferenceCount();
        }
    }
    
    return *this;
}



/**
 * Add this node to our set of nodes.
 */
void Proposal::addNode( DagNode *n)
{
    
    nodes.insert( n );
    
    // increment reference count
    n->incrementReferenceCount();
    
}



/**
 * Get the vector of nodes on which this proposal is working on.
 *
 * \return  Const reference to a vector of nodes pointer on which the proposal operates.
 */
const std::set<DagNode*>& Proposal::getNodes( void ) const
{
    
    return nodes;
}





/**
 * Remove this node from our list of nodes.
 */
void Proposal::removeNode( RevBayesCore::DagNode *n )
{
    std::set< DagNode *>::iterator it = nodes.find( n );
    if ( it != nodes.end() )
    {
        nodes.erase( it );
    }
    
    if ( n->decrementReferenceCount() == 0 )
    {
        delete n;
    }
}

/**
 * Swap the old node with a new one.
 * This will be called for example when the entire model graph is cloned or
 * when we replace a variable with the same name (re-assignment).
 * Here we update our set and delegate to the derived class.
 */
void Proposal::swapNode(DagNode *oldP, DagNode *newP) {
    
    std::set<DagNode *>::iterator position = nodes.find(oldP);
    if ( position != nodes.end() )
    {
        nodes.erase( position );
        nodes.insert( newP );
        swapNodeInternal( oldP, newP );
        
        // increment and decrement the reference counts
        newP->incrementReferenceCount();
        if ( oldP->decrementReferenceCount() == 0 )
        {
            throw RbException("Memory leak in Function. Please report this bug to Sebastian.");
        }
    }
    else
    {
        throw RbException("Could not find the Proposal parameter to be swapped: " + oldP->getName());
    }
    
}