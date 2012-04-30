//
//  InferenceMoveSimple.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/30/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "InferenceMoveSimple.h"
#include "RbException.h"
#include "StochasticInferenceNode.h"

#include <set>

InferenceMoveSimple::InferenceMoveSimple(void) : InferenceMove() {
    changed = false;
}


InferenceMoveSimple::~InferenceMoveSimple( void ) {
    
}


void InferenceMoveSimple::acceptMove( void ) {
    // nothing to do
    changed = false;
}


double InferenceMoveSimple::performMove( double &probRatio ) {
    
    if (changed) {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    probRatio = nodes[0]->getLnProbabilityRatio();
    
    std::set<StochasticInferenceNode* > affectedNodes;
    nodes[0]->getAffectedNodes(affectedNodes);
    //    std::cerr << "Number of affected nodes:\t\t" << affectedNodes.size() << std::endl;
    for (std::set<StochasticInferenceNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
        StochasticInferenceNode* theNode = *i;
        //        std::cerr << "computing probability of affected node:\t" << theNode->getName() << std::endl;
        probRatio += theNode->getLnProbabilityRatio();
    }
    
    return hr;
}


void InferenceMoveSimple::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
}
