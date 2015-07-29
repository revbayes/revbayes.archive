#include "AbstractMove.h"
#include "DagNode.h"
#include "RbException.h"

using namespace RevBayesCore;


/** 
 * Constructor
 *
 * Here we simply allocate and initialize the move object.
 *
 * \param[in]    w   The weight how often the proposal will be used (per iteration).
 * \param[in]    t   If auto tuning should be used.
 */
AbstractMove::AbstractMove( double w, bool t ) :
    nodes(  ),
    affectedNodes(  ),
    weight( w ),
    autoTuning( t ) 
{
    
}


/**
 * Constructor
 *
 * Here we simply allocate and initialize the move object.
 *
 * \param[in]    w   The weight how often the proposal will be used (per iteration).
 * \param[in]    t   If auto tuning should be used.
 */
AbstractMove::AbstractMove( const std::vector<DagNode*> &n, double w, bool t ) :
    nodes( n ),
    affectedNodes( ),
    weight( w ),
    autoTuning( t )
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        // get the pointer to the current node
        DagNode* theNode = *it;
        
        // add myself to the set of moves
        theNode->addMove( this );
        
        // increase the DAG node reference count because we also have a pointer to it
        theNode->incrementReferenceCount();
        
    }
    
    
    // remove all "core" nodes from affectedNodes so their probabilities are not double-counted
    for (size_t i = 0; i < affectedNodes.size(); ++i)
    {
        std::set<DagNode*>::iterator it = affectedNodes.begin();
        std::advance(it, i);
        
        for (size_t j = 0; j < nodes.size(); ++j)
        {
            if ( nodes[j] == *it )
            {
                affectedNodes.erase(*it);
                --i;
                break;
            }
            
        }
        
    }
    
}


AbstractMove::AbstractMove( const AbstractMove &m ) : Move( m ),
    nodes( m.nodes ),
    affectedNodes( m.affectedNodes ),
    weight( m.weight ),
    autoTuning( m.autoTuning  )
{
    
    
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        // get the pointer to the current node
        DagNode* theNode = nodes[i];
        
        // add myself to the set of moves
        theNode->addMove( this );
        
        // increase the DAG node reference count because we also have a pointer to it
        theNode->incrementReferenceCount();
        
    }
    
}



/**
 * Basic destructor doing nothing.
 */
AbstractMove::~AbstractMove( void )
{
    
}

/**
 * Overloaded assignment operator.
 * We need a deep copy of the operator.
 */
AbstractMove& AbstractMove::operator=(const RevBayesCore::AbstractMove &m)
{
    
    if ( this != &m )
    {
        // delegate
        Move::operator=(m);
        
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            // get the pointer to the current node
            DagNode* theNode = nodes[i];
            
            // add myself to the set of moves
            theNode->removeMove( this );
            
            // decrease the DAG node reference count because we also have a pointer to it
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete theNode;
            }
            
        }
        
        affectedNodes   = m.affectedNodes;
        nodes           = m.nodes;
        
        
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            // get the pointer to the current node
            DagNode* theNode = nodes[i];
            
            // add myself to the set of moves
            theNode->addMove( this );
            
            // increase the DAG node reference count because we also have a pointer to it
            theNode->incrementReferenceCount();
            
        }
        
    }
    
    return *this;
}


/**
 * Add this node to our set of nodes.
 */
void AbstractMove::addNode( DagNode *n )
{
    bool exists = false;
    
    for (std::vector<DagNode*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        if ( *it == n )
        {
            exists = true;
            break;
        }
    }
    
    // only add the node if it doesn't exist already
    if ( exists == false )
    {
        nodes.push_back( n );
        
        // add myself to the set of moves
        n->addMove( this );
        
        // increment reference count
        n->incrementReferenceCount();
    }
    
}


/**
 * Automatically tune the move.
 * This will delegate the call to the derived class but only if autotuning was enabled.
 */
void AbstractMove::autoTune( void )
{
    
    // only call tuning if it is enabled for this move.
    if ( autoTuning ) 
    {
        tune();
        
        // we need to reset the counters so that the next tuning only uses the new acceptance rate.
        resetCounters();
    }
    
}


/**
 * Get the set of affected nodes by a this move excluding the "core" nodes.
 *
 * \return The set of affected nodes.
 */
const std::set<DagNode*>& AbstractMove::getAffectedNodes( void ) const
{
    
    return affectedNodes;
}


/**
 * Get the set of nodes on which this move is working on.
 *
 * \return The set of nodes.
 */
const std::vector<DagNode*>& AbstractMove::getDagNodes( void ) const
{
    
    return nodes;
}


/**
 * Get the update weight of how often the move should be used.
 *
 * \return    The update weight.
 */
double AbstractMove::getUpdateWeight( void ) const 
{
    return weight;
}


/**
 * Is the move active at the given generation?
 * The move itself will determine if it was switched off before or after
 * some given iteration.
 */
bool AbstractMove::isActive(unsigned long gen) const
{
    return true;
}



/**
 * Perform the move. 
 * Here we store some info and delegate to performMove.
 */
void AbstractMove::perform( double lHeat, double pHeat )
{
    // increment the tries counter
    numTried++;
    
    // delegate to derived class
    performMove(lHeat, pHeat);
    
}




/**
 * Remove this node from our list of nodes.
 */
void AbstractMove::removeNode( RevBayesCore::DagNode *n )
{
    
    for (std::vector<DagNode*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        if ( *it == n )
        {
            nodes.erase( it );
            break;
        }
    }
    
    if ( n->decrementReferenceCount() == 0 )
    {
        delete n;
    }
    
}



/**
 * Reset the counters. Here we reset the counters stored in the base class
 * and delegate the call to the potentially overwritten method from the 
 * derived classes.
 *
 */
void AbstractMove::resetCounters( void )
{
    numTried = 0;
    
    // delegate call
    resetMoveCounters();
}

/**
 * Reset the move counters. This method is only a hook for derived classes 
 * and here we provide only a dummy implementation.
 *
 */
void AbstractMove::resetMoveCounters( void )
{
    // dummy implementation
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new variable.
 */
void AbstractMove::swapNode(DagNode *oldN, DagNode *newN)
{
    
    // find the old node
    
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        // get the pointer to the current node
        DagNode* theNode = nodes[i];
        if ( theNode == oldN )
        {
            nodes.erase( nodes.begin() + i );
        }
        
    }
    
    // insert the new node
    nodes.push_back( newN );
    
    // remove myself from the old node and add myself to the new node
    oldN->removeMove( this );
    newN->addMove( this );
    
    // increment and decrement the reference counts
    newN->incrementReferenceCount();
    if ( oldN->decrementReferenceCount() == 0 )
    {
        throw RbException("Memory leak in Metropolis-Hastings move. Please report this bug to Sebastian.");
    }
    
    affectedNodes.clear();
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        // get the pointer to the current node
        DagNode* theNode = *it;
        
        // get the affected nodes if we would update this node
        // then we don't need to get the affected nodes every time again
        theNode->getAffectedNodes( affectedNodes );
    }
    
    // remove all "core" nodes from affectedNodes so their probabilities are not double-counted
    for (size_t i = 0; i < affectedNodes.size(); ++i)
    {
        std::set<DagNode*>::iterator it = affectedNodes.begin();
        std::advance(it, i);
        
        for (size_t j = 0; j < nodes.size(); ++j)
        {
            if ( nodes[j] == *it )
            {
                affectedNodes.erase(*it);
                --i;
                break;
            }
            
        }
        
    }
    
    swapNodeInternal(oldN, newN);
    
}







