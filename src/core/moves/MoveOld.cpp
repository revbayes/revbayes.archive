#include "MoveOld.h"
#include "DagNode.h"
#include "RbException.h"
#include "StringUtilities.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>


using namespace RevBayesCore;

MoveOld::MoveOld( DagNode *n, double w, bool t ) : AbstractOldMove(),
    weight( w ),
    numAccepted( 0 ),
    numTried( 0 ),
    autoTuning( t )
{
    nodes.insert( n );
}


MoveOld::MoveOld( std::vector<DagNode*> n, double w, bool t ) : AbstractOldMove(),
    weight( w ),
    numAccepted( 0 ),
    numTried( 0 ),
    autoTuning( t )
{

    for (std::vector<DagNode*>::iterator it = n.begin(); it != n.end(); it++)
    {
        nodes.insert( *it );
    }
}


MoveOld::~MoveOld( void ) {
    
}


void MoveOld::accept( void ) {
    // increment the counter for accepted
    numAccepted++;
    
    // call accept for each node
    for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
        (*i)->keep();
    }
    
    // delegate the call to the derived class
    acceptMove();
}


void MoveOld::autoTune( void ) {
    
    // only do something if tuning is turned on
    if ( autoTuning ) {
        // delegate to derived class
        tune();
        
        // reset counters
        resetCounters();
    }
}


double MoveOld::getAcceptanceRatio( void ) const {
    return numTried / double( numAccepted );
}


const std::set<DagNode *>& MoveOld::getDagNodes( void ) const
{

    return nodes;
}


double MoveOld::getUpdateWeight( void ) const {
    return weight;
}


bool MoveOld::isActive( unsigned long g ) const
{

    return true;
}


bool MoveOld::isGibbs( void ) const
{

    return false;
}


double MoveOld::performOld(double &probRatio)
{
    // increment the counter for tried
    numTried++;
    
    // delegate the call to the derived class
    return performMove(probRatio);
}


void MoveOld::performGibbs(void)
{
    // increment the counter for tried
    numTried++;
    
    // delegate the call to the derived class
    performGibbsMove();
    
    // increment the counter for accepted
    numAccepted++;
    
    // call accept for each node
    for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
        (*i)->keep();
    }
    
    // delegate the call to the derived class
    acceptMove();
}




void MoveOld::performGibbsMove(void)
{
    // dummy method
}


void MoveOld::printSummary( std::ostream &o ) const
{
    
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the name
    std::string n = getMoveName();
    StringUtilities::fillWithSpaces(n,40,true);
    
    o << n;
    o << " ";
   
    // print the DagNode name
    std::string dn_name = (*nodes.begin())->getName();
    StringUtilities::fillWithSpaces(dn_name,20,true);
    o << dn_name;
    o << " ";
    
    // print the weight
    int w_length = 4 - (int)log10(weight+1);
    for (int i = 0; i < w_length; ++i)
    {
        o << " ";
    }
    o << weight;
    o << " ";
    
    // print the number of tries
    int t_length = 9 - (int)log10(numTried+1);
    for (int i = 0; i < t_length; ++i)
    {
        o << " ";
    }
    o << numTried;
    o << " ";
    
    // print the number of accepted
    int a_length = 9;
    if (numAccepted > 0) a_length -= (int)log10(numAccepted+1);
    
    for (int i = 0; i < a_length; ++i)
    {
        o << " ";
    }
    o << numAccepted;
    o << " ";
    
    // print the acceptance ratio
    double ratio = numAccepted / (double)numTried;
    if (numTried == 0) ratio = 0;
    int r_length = 5;
    
    for (int i = 0; i < r_length; ++i)
    {
        o << " ";
    }
    o << ratio;
    o << " ";
    
    printParameterSummary( o );
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    
}


void MoveOld::printParameterSummary(std::ostream &o) const {
    // no parameter -> nothing to print
}


void MoveOld::reject( void ) {
    
    // delegate the call to the derived class
    rejectMove();
    
    // call restore for each node
    for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
        (*i)->restore();
    }
    
}


void MoveOld::resetCounters( void ) {
    numTried = 0;
    numAccepted = 0;
}


void MoveOld::swapNode(DagNode *oldN, DagNode *newN) {
    // error catching
    if ( nodes.find(oldN) == nodes.end() ) {
        throw RbException("Cannot replace DAG node in this move because the move doesn't hold this DAG node.");
    }
    
    nodes.erase( oldN );
    nodes.insert( newN );
}


void MoveOld::tune( void ) {
    // the default implementation does nothing
}

std::ostream& operator<<(std::ostream& o, const MoveOld& x) {
    o << "Old-Move";
    
    return o;
}

