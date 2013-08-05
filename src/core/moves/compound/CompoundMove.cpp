

#include "CompoundMove.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "RbConstants.h"

#include <set>
#include <vector>

using namespace RevBayesCore;

CompoundMove::CompoundMove(std::vector<DagNode*> n, double w, bool t) : Move( n, w, t ), theNodes( n ) {
    
    changed = false;

}


CompoundMove::~CompoundMove( void ) {
    
}


void CompoundMove::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptCompoundMove();
}

void CompoundMove::acceptCompoundMove( void ) {
    // do nothing
    
}


double CompoundMove::performMove( double &probRatio ) {
    
    if (changed) 
    {
        throw RbException("Trying to execute a Compound moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performCompoundMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf ) 
    {
        return RbConstants::Double::neginf;
    }
    
    std::set<DagNode* > affectedNodes;
    for (std::vector<DagNode*>::iterator it = theNodes.begin(); it != theNodes.end(); it++)
    {
        // touch each node
        (*it)->touch();
    
        // calculate the probability ratio for the node we just changed
        //std::cout << (*it)->getName() << " " << (*it)->getLnProbabilityRatio() << " " << (*it)->getLnProbability() << "\n";
            
        probRatio += (*it)->getLnProbabilityRatio();
 
        if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf )
        {
            // should contain unique nodes, since it is a set
            (*it)->getAffectedNodes(affectedNodes);
        }
    }
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf )
    {
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i)
        {
            DagNode* theAffectedNode = *i;
            
            // do not double-count the prob ratio for any elt in both theNodes and affectedNodes
            //if ( find(theNodes.begin(), theNodes.end(), *i) == theNodes.end() )
            {
                //std::cout << "  " << theAffectedNode->getName() << "  " << theAffectedNode->getLnProbabilityRatio() <<  " " << theAffectedNode->getLnProbability() << "\n";
                probRatio += theAffectedNode->getLnProbabilityRatio();
            }
        }
    }

    
    return hr;
}


void CompoundMove::rejectMove( void ) {
    
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectCompoundMove();
    
    // touch the node
    for (std::vector<DagNode*>::iterator it = theNodes.begin(); it != theNodes.end(); it++)
        (*it)->touch();
}


void CompoundMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
    for (std::vector<DagNode*>::iterator it = theNodes.begin(); it != theNodes.end(); it++)
    {
        // MJL 070413: need to statically type newN before assignment when oldN's type is unknown... figure out later
        if (*it == oldN)
            *it = newN;
    }
}
