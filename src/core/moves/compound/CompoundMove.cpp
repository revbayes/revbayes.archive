

#include "CompoundMove.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "RbConstants.h"

#include <set>
#include <vector>

using namespace RevBayesCore;

CompoundMove::CompoundMove(std::vector<DagNode*> n, double w, bool t) : MoveOld( n, w, t ) {
    
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
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *theNode = *it;
        
        // touch each node
        theNode->touch();
    
        // calculate the probability ratio for the node we just changed
        //std::cout << (*it)->getName() << " " << (*it)->getLnProbabilityRatio() << " " << (*it)->getLnProbability() << "\n";
            
        probRatio += theNode->getLnProbabilityRatio();
 
        if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf )
        {
            // should contain unique nodes, since it is a set
            theNode->getAffectedNodes(affectedNodes);
        }
    }
    
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
//        if ( nodes.find(oldN) == nodes.end() ) {
//            throw RbException("Cannot replace DAG node in this move because the move doesn't hold this DAG node.");
//        }
        
        affectedNodes.erase( *it );
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
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
        (*it)->touch();
}


void CompoundMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    // Sebastian (2014-07-24): The base class should take care of this!
//    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
//    {
//        // MJL 070413: need to statically type newN before assignment when oldN's type is unknown... figure out later
//        if (*it == oldN)
//            *it = newN;
//    }
}
