

#include "DagNode.h"
#include "Monitor.h"
#include "RbException.h"

#include <algorithm>

using namespace RevBayesCore;



Monitor::Monitor(unsigned long g) : printgen( g ), model( NULL ), numCycles( 0 ) {
    
}

Monitor::Monitor(unsigned long g, DagNode *n) : printgen( g ), model( NULL ), numCycles( 0 ) {
    
    nodes.push_back( n );
}

Monitor::Monitor(unsigned long g, const std::set<DagNode *> &n) : printgen( g ), model( NULL ), numCycles( 0 ) {
    for (std::set<DagNode*>::iterator it = n.begin(); it != n.end(); it++)
        nodes.push_back(*it);
}

Monitor::Monitor(unsigned long g, const std::vector<DagNode *> &n) : printgen( g ), nodes( n ), model( NULL ), numCycles( 0 ) {
    
}

Monitor::Monitor(const Monitor &i) : nodes( i.nodes ), model( i.model ), numCycles( i.numCycles ) {
    
    // set the print gen
    printgen = i.printgen;
    
}


Monitor::~Monitor( void ) {
    
    // nothing to do
}


Monitor& Monitor::operator=(const Monitor &i) {
    
    // check for self-assignment
    if ( &i != this ) {
        
        // set the nodes (we don't own them)
        nodes = i.nodes;
        
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

    if (it == nodes.end()) {
        throw RbException("Cannot replace DAG node with name\"" + oldN->getName() + "\" in this monitor because the monitor doesn't hold this DAG node.");
    }
    
    it = nodes.insert( it, newN );
    nodes.erase( it + 1 );
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Monitor& x) {
    o << "Monitor";
    
    return o;
}

