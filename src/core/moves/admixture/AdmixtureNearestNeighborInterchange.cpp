//
//  AdmixtureNearestNeighborInterchange.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureNearestNeighborInterchange.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureNearestNeighborInterchange::AdmixtureNearestNeighborInterchange(StochasticNode<AdmixtureTree> *v, std::vector<ContinuousStochasticNode*> br, double d, bool t, double w) : SimpleMove( v, w, t), delta(d), variable( v ), branchRates(br), failed(false) {
    
    for (size_t i = 0; i < branchRates.size(); i++)
        nodes.insert(branchRates[i]);
}



/* Clone object */
AdmixtureNearestNeighborInterchange* AdmixtureNearestNeighborInterchange::clone( void ) const {
    
    return new AdmixtureNearestNeighborInterchange( *this );
}



const std::string& AdmixtureNearestNeighborInterchange::getMoveName( void ) const {
    static std::string name = "AdmixtureNearestNeighborInterchange";
    
    return name;
}


/** Perform the move */
double AdmixtureNearestNeighborInterchange::performSimpleMove( void ) {
    
    std::cout << "\nDiv Node NNI & Rate Rescale\n";
    
    failed = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    AdmixtureNode* node;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->getTopologyParent().isRoot() ||  node->getNumberOfChildren() != 2);
    
    int nodeChildMoveIdx = std::floor(rng->uniform01() * 2.0);
    int nodeBrotherIdx = 0;
    
    
    std::cout << "nodeChildeMoveIdx  " << nodeChildMoveIdx << "\n";
    // divergence location
    AdmixtureNode* childMove = &node->getChild(nodeChildMoveIdx);
    AdmixtureNode* parent = &node->getTopologyParent();
    AdmixtureNode* brother = &parent->getTopologyChild( nodeBrotherIdx );
  
    // check if we got the correct child lineage
    if ( brother == node )
    {
        if (nodeBrotherIdx == 0)
            nodeBrotherIdx = 1;
        else
            nodeBrotherIdx = 0;
    }
    
    // get brother of node (not divergence child node)
    brother = &parent->getChild(nodeBrotherIdx);
   
    if (childMove->getAge() == 0.0 && brother->getAge() == 0.0)
    {
        std::cout << "nni " << childMove->getIndex() << " " << brother->getIndex() << "\n";
        std::cout << "childMv  " << childMove->getIndex() << "  " << childMove->getAge() << "\n";
        std::cout << "node     " << node->getIndex() << "  " << node->getAge() << "\n";
        std::cout << "brother  " << brother->getIndex() << "  " << brother->getAge() << "\n";
        std::cout << "parent   " << parent->getIndex() << "  " << parent->getAge() << "\n";
    }
    
    if (brother->getAge() > node->getAge())
    {
        std::cout << "failed, bro > node\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    else if (childMove->getAge() > parent->getAge())
    {
        std::cout << "failed, child > parent\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    else if (childMove->isOutgroup() != brother->isOutgroup())
    {
        std::cout << "failed, outgroup mismatch\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    
    // update parent clade
    storedNodeParent = parent;
    storedNodeChildMove = childMove;
    storedBrother = brother;
    storedNode = node;

    // swap
    storedNode->removeChild(storedNodeChildMove);
    storedNodeParent->removeChild(storedBrother);
    
    storedNode->addChild(storedBrother);
    storedNodeParent->addChild(storedNodeChildMove);
    
    storedBrother->setParent(storedNode);
    storedNodeChildMove->setParent(storedNodeParent);

    // get branch rate index
    storedBrotherRateIndex = parent->getTopologyChild(nodeBrotherIdx).getIndex();
    storedNodeRateIndex = node->getTopologyChild(nodeChildMoveIdx).getIndex();
    
    // store branch rate values
    storedNodeRate = branchRates[storedNodeRateIndex]->getValue();
    storedBrotherRate = branchRates[storedBrotherRateIndex]->getValue();
    
    // update
    double scaleNodeRate = exp(delta*(rng->uniform01() - 0.5));
    double scaleBrotherRate = exp(delta*(rng->uniform01() - 0.5));
    branchRates[storedNodeRateIndex]->setValue(new double(storedNodeRate * scaleNodeRate));
    branchRates[storedBrotherRateIndex]->setValue(new double(storedBrotherRate * scaleBrotherRate));
    
    std::cout << "node rate  " << storedNodeRate << " -> " << scaleNodeRate << "\n";
    std::cout << "bro rate   " << storedBrotherRate << " -> " << scaleBrotherRate << "\n";
    
    if (storedNodeRate * scaleNodeRate == 0.0)
    {
        std::cout << "new scaledNodeRate == 0.0\n";
    }
    if (storedBrotherRate * scaleBrotherRate == 0.0)
    {
        std::cout << "new scaledBrotherRate == 0.0\n";
    }

    
    // MH
    return log(scaleNodeRate*scaleBrotherRate);
}


void AdmixtureNearestNeighborInterchange::rejectSimpleMove( void ) {
    
    if (!failed)
    {
        std::cout << "reject NNI\n";
        
        // undo the proposal
        storedNode->removeChild(storedBrother);
        storedNodeParent->removeChild(storedNodeChildMove);
        
        storedNode->addChild(storedNodeChildMove);
        storedNodeParent->addChild(storedBrother);
        
        storedBrother->setParent(storedNodeParent);
        storedNodeChildMove->setParent(storedNode);
        
        branchRates[storedNodeRateIndex]->setValue(new double(storedNodeRate));
        branchRates[storedBrotherRateIndex]->setValue(new double(storedBrotherRate));
    }
    
}


void AdmixtureNearestNeighborInterchange::swapNode(DagNode *oldN, DagNode *newN) {
    
    SimpleMove::swapNode(oldN,newN);
    
    if (oldN == variable)
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    for (size_t i = 0; i < branchRates.size(); i++)
    {
        if (oldN == branchRates[i])
        {
            branchRates[i] = static_cast<ContinuousStochasticNode*>(newN) ;
        }
    }
}


void AdmixtureNearestNeighborInterchange::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta /= (2.0 - rate/0.234 );
    }
}