 

#include "SimpleMove.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "RbConstants.h"

#include <set>

using namespace RevBayesCore;

SimpleMove::SimpleMove(DagNode *n, double w, bool t) : MoveOld( n, w, t ),
    theNode( n )
{
    
    changed = false;

}


SimpleMove::~SimpleMove( void ) {
    
}


void SimpleMove::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

void SimpleMove::acceptSimpleMove( void ) {
    // do nothing
    
}


double SimpleMove::performMove( double &probRatio ) {
    
    if (changed) 
    {
        throw RbException("Trying to execute a simple move twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf ) 
    {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    theNode->touch();
    
    // calculate the probability ratio for the node we just changed
    probRatio = theNode->getLnProbabilityRatio();
        
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) 
    {
        
        std::set<DagNode* > affectedNodes;
        theNode->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) 
        {
            DagNode* theAffectedNode = *i;
            //std::cout << theAffectedNode->getName() << "  " << theAffectedNode->getLnProbabilityRatio() << " " << theAffectedNode->getLnProbability() << "\n";
            probRatio += theAffectedNode->getLnProbabilityRatio();
        }
    }

    return hr;
}


void SimpleMove::rejectMove( void )
{
    
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    theNode->touch();
}


void SimpleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    theNode = newN;
    
}
