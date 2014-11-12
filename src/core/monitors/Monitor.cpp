

#include "DagNode.h"
#include "Monitor.h"
#include "RbException.h"

#include <algorithm>

using namespace RevBayesCore;



Monitor::Monitor(unsigned long g) :
    printgen( g ),
    model( NULL ),
    numCycles( 0 )
{
    
}

Monitor::Monitor(unsigned long g, DagNode *n) :
    printgen( g ),
    model( NULL ),
    numCycles( 0 )
{
    
    nodes.push_back( n );
    
    // tell the node that we have a reference to it (avoids deletion)
    n->incrementReferenceCount();
}

Monitor::Monitor(unsigned long g, const std::set<DagNode *> &n) :
    printgen( g ),
    model( NULL ),
    numCycles( 0 )
{
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        DagNode *theNode = *it;
        
        // add myself to the set of monitors
        theNode->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        theNode->incrementReferenceCount();
    }
}

Monitor::Monitor(unsigned long g, const std::vector<DagNode *> &n) :
    printgen( g ),
    nodes( n ),
    model( NULL ),
    numCycles( 0 )
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        DagNode *theNode = *it;
        
        // add myself to the set of monitors
        theNode->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        theNode->incrementReferenceCount();
    }
}

Monitor::Monitor(const Monitor &i) :
    nodes( i.nodes ),
    model( i.model ),
    numCycles( i.numCycles )
{
    
    // set the print gen
    printgen = i.printgen;
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        DagNode *theNode = *it;
        
        // add myself to the set of monitors
        theNode->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        theNode->incrementReferenceCount();
    }
    
}


Monitor::~Monitor( void )
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *theNode = *it;
        
        // remove myself to the set of monitors
        theNode->removeMonitor( this );
        
        
        // tell the node that we have a reference to it (avoids deletion)
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete *it;
        }
    }

}


Monitor& Monitor::operator=(const Monitor &i) {
    
    // check for self-assignment
    if ( &i != this ) {
        
        
        for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            DagNode *theNode = *it;
            
            // remove myself to the set of monitors
            theNode->removeMonitor( this );

            
            // tell the node that we have a reference to it (avoids deletion)
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete *it;
            }
        }
        
        // set the nodes (we don't own them)
        nodes = i.nodes;
        
        for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            
            DagNode *theNode = *it;
            
            // add myself to the set of monitors
            theNode->addMonitor( this );

            // tell the node that we have a reference to it (avoids deletion)
            theNode->incrementReferenceCount();
        }
        
        // set the printgen
        printgen = i.printgen;
        
        // set number of cycles
        numCycles = i.numCycles;
        
        model = i.model;
    }
    
    return *this;
}

void Monitor::closeStream(void)
{
    ; // dummy fn
}

void Monitor::openStream(void)
{
    ; // dummy fn
}

void Monitor::printHeader(void)
{
    ; // dummy fn
}


const std::vector<DagNode*>& Monitor::getDagNodes( void ) const {
    
    return nodes;
}


void Monitor::setDagNodes( const std::set<DagNode *> &args) 
{
    
    for (std::set<DagNode*>::iterator it = args.begin(); it != args.end(); it++)
        nodes.push_back(*it);
}

void Monitor::setDagNodes( const std::vector<DagNode *> &args) {
    nodes = args;

}

void Monitor::setModel(Model *m) 
{
    model = m;
}

void Monitor::setMcmc(Mcmc *m)
{
    mcmc = m;
}


void Monitor::swapNode(DagNode *oldN, DagNode *newN) 
{
    // error catching
    std::vector<DagNode*>::iterator it = find(nodes.begin(), nodes.end(), oldN);

    if (it == nodes.end())
    {
        throw RbException("Cannot replace DAG node with name\"" + oldN->getName() + "\" in this monitor because the monitor doesn't hold this DAG node.");
    }
    
    // remove myself from the old node and add myself to the new node
    oldN->removeMonitor( this );
    newN->addMonitor( this );
    
    // increment and decrement the reference counts
    newN->incrementReferenceCount();
    if ( oldN->decrementReferenceCount() == 0 )
    {
        throw RbException("Memory leak in monitor. Please report this bug to Sebastian.");
    }
    
    it = nodes.insert( it, newN );
    nodes.erase( it + 1 );
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Monitor& x) {
    o << "Monitor";
    
    return o;
}

