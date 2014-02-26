#include "Move.h"
#include "DagNode.h"
#include "RbException.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>


using namespace RevBayesCore;

Move::Move( DagNode *n, double w, bool t ) : weight( w ), numAccepted( 0 ), numTried( 0 ), autoTuning( t ) {
    nodes.insert( n );
}


Move::Move( std::vector<DagNode*> n, double w, bool t ) : weight( w ), numAccepted( 0 ), numTried( 0 ), autoTuning( t ) {

    for (std::vector<DagNode*>::iterator it = n.begin(); it != n.end(); it++)
        nodes.insert( *it );
}


Move::~Move( void ) {
    
}


void Move::accept( void ) {
    // increment the counter for accepted
    numAccepted++;
    
    // call accept for each node
    for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
        (*i)->keep();
    }
    
    // delegate the call to the derived class
    acceptMove();
}


void Move::autoTune( void ) {
    
    // only do something if tuning is turned on
    if ( autoTuning ) {
        // delegate to derived class
        tune();
        
        // reset counters
        resetCounters();
    }
}


double Move::getAcceptanceRatio( void ) const {
    return numTried / double( numAccepted );
}


const std::set<DagNode *>& Move::getDagNodes( void ) const 
{

    return nodes;
}


double Move::getUpdateWeight( void ) const {
    return weight;
}


bool Move::isActive( unsigned long g ) const 
{

    return true;
}


bool Move::isGibbs( void ) const 
{

    return false;
}


double Move::perform(double &probRatio) 
{
    // increment the counter for tried
    numTried++;
    
    // delegate the call to the derived class
    return performMove(probRatio);
}


void Move::performGibbs(void) 
{
    // increment the counter for tried
    numTried++;
    
    // delegate the call to the derived class
    performGibbsMove();
    
    // increment the counter for accepted
    numAccepted++;
    
    // call accept for each node
    for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
        (*i)->keep();
    }
    
    // delegate the call to the derived class
    acceptMove();
}




void Move::performGibbsMove(void) {
    // dummy method
}


void Move::printSummary( std::ostream &o ) const {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the name
    const std::string &n = getMoveName();
    size_t spaces = 40 - (n.length() > 40 ? 40 : n.length());
    o << n;
    for (size_t i = 0; i < spaces; ++i) {
        o << " ";
    }
    o << " ";
   
    // print the DagNode name
    const std::string &dn_name = (*nodes.begin())->getName();
    spaces = 20 - (dn_name.length() > 20 ? 20 : dn_name.length());
    o << dn_name;
    for (size_t i = 0; i < spaces; ++i) {
        o << " ";
    }
    o << " ";
    
    // print the weight
    int w_length = 4 - (int)log10(weight);
    for (int i = 0; i < w_length; ++i) {
        o << " ";
    }
    o << weight;
    o << " ";
    
    // print the number of tries
    int t_length = 9 - (int)log10(numTried);
    for (int i = 0; i < t_length; ++i) {
        o << " ";
    }
    o << numTried;
    o << " ";
    
    // print the number of accepted
    int a_length = 9;
    if (numAccepted > 0) a_length -= (int)log10(numAccepted);
    
    for (int i = 0; i < a_length; ++i) {
        o << " ";
    }
    o << numAccepted;
    o << " ";
    
    // print the acceptance ratio
    double ratio = numAccepted / (double)numTried;
    if (numTried == 0) ratio = 0;
    int r_length = 5;
    
    for (int i = 0; i < r_length; ++i) {
        o << " ";
    }
    o << ratio;
    o << " ";
    
    printParameterSummary( o );
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    
}


void Move::printParameterSummary(std::ostream &o) const {
    // no parameter -> nothing to print
}


void Move::reject( void ) {
    
    // delegate the call to the derived class
    rejectMove();
    
    // call restore for each node
    for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
        (*i)->restore();
    }
    
}


void Move::resetCounters( void ) {
    numTried = 0;
    numAccepted = 0;
}


void Move::swapNode(DagNode *oldN, DagNode *newN) {
    // error catching
    if ( nodes.find(oldN) == nodes.end() ) {
        throw RbException("Cannot replace DAG node in this move because the move doesn't hold this DAG node.");
    }
    
    nodes.erase( oldN );
    nodes.insert( newN );
}


void Move::tune( void ) {
    // the default implementation does nothing
}

std::ostream& operator<<(std::ostream& o, const Move& x) {
    o << "Move";
    
    return o;
}

