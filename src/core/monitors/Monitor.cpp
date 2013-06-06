

#include "DagNode.h"
#include "Monitor.h"
#include "RbException.h"

using namespace RevBayesCore;



Monitor::Monitor(int g) : printgen( g ), model( NULL ) {
    
}

Monitor::Monitor(int g, DagNode *n) : printgen( g ), model( NULL ) {
    
    nodes.insert( n );
}

Monitor::Monitor(int g, const std::set<DagNode *> &n) : printgen( g ), nodes( n ), model( NULL ) {
    
}


Monitor::Monitor(const Monitor &i) : nodes( i.nodes ), model( i.model ) {
    
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


const std::set<DagNode*>& Monitor::getDagNodes( void ) const {
    return nodes;
}


void Monitor::setDagNodes( const std::set<DagNode *> &args) {
    nodes = args;
}


void Monitor::setModel(Model *m) {
    model = m;
}


void Monitor::swapNode(DagNode *oldN, DagNode *newN) {
    // error catching
    if ( nodes.find(oldN) == nodes.end() ) {
        throw RbException("Cannot replace DAG node with name\"" + oldN->getName() + "\" in this monitor because the monitor doesn't hold this DAG node.");
    }
    
    nodes.erase( oldN );
    nodes.insert( newN );
}


std::ostream& operator<<(std::ostream& o, const Monitor& x) {
    o << "Monitor";
    
    return o;
}

