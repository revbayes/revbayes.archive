//
//  AdmixtureNearestNeighborInterchangeAndRateShift.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureNearestNeighborInterchangeAndRateShift.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureNearestNeighborInterchangeAndRateShift::AdmixtureNearestNeighborInterchangeAndRateShift(StochasticNode<AdmixtureTree> *v, std::vector<ContinuousStochasticNode*> br, double d, bool t, double w) : MoveOld( v, w, t), variable( v ), branchRates(br), failed(false), changed(false), delta(d) {
    
    for (size_t i = 0; i < branchRates.size(); i++)
        nodes.insert(branchRates[i]);
}

AdmixtureNearestNeighborInterchangeAndRateShift::~AdmixtureNearestNeighborInterchangeAndRateShift(void)
{
    
}


/* Clone object */
AdmixtureNearestNeighborInterchangeAndRateShift* AdmixtureNearestNeighborInterchangeAndRateShift::clone( void ) const {
    
    return new AdmixtureNearestNeighborInterchangeAndRateShift( *this );
}



const std::string& AdmixtureNearestNeighborInterchangeAndRateShift::getMoveName( void ) const {
    static std::string name = "AdmixtureNNIAndRateShift";
    
    return name;
}


/** Perform the move */
double AdmixtureNearestNeighborInterchangeAndRateShift::performSimpleMove( void ) {
    
    //std::cout << "\nDiv Node NNI & Rate Rescale\n";
    
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
    
    int nodeChildMoveIdx = (int)std::floor(rng->uniform01() * 2.0);
    int nodeBrotherIdx = 0;
    
    
    //std::cout << "nodeChildeMoveIdx  " << nodeChildMoveIdx << "\n";
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
    
    if (brother->getAge() > node->getAge())
    {
        //std::cout << "failed, bro > node\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    else if (childMove->getAge() > parent->getAge())
    {
        //std::cout << "failed, child > parent\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    else if (childMove->isOutgroup() != brother->isOutgroup())
    {
        //std::cout << "failed, outgroup mismatch\n";
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
    storedChildRateIndex = (int)node->getTopologyChild(nodeChildMoveIdx).getIndex();
    storedBrotherRateIndex = (int)parent->getTopologyChild(nodeBrotherIdx).getIndex();
//    storedNodeRateIndex = node->getIndex();

    
    // store branch rate values
    storedChildRate = branchRates[storedChildRateIndex]->getValue();
 //   storedNodeRate = branchRates[storedNodeRateIndex]->getValue();
    storedBrotherRate = branchRates[storedBrotherRateIndex]->getValue();
    
    // update
    //double cnr = 0.0;
    //double snr = 0.0;
    //double sbr = 0.0;
    
    double scaleChildRate = exp(delta*(rng->uniform01() - 0.5));
 //   double scaleNodeRate = exp(delta*(rng->uniform01() - 0.5));
    double scaleBrotherRate = exp(delta*(rng->uniform01() - 0.5));
    branchRates[storedChildRateIndex]->setValue(new double(storedChildRate*scaleChildRate));
  //  branchRates[storedNodeRateIndex]->setValue(new double(storedNodeRate * scaleNodeRate));
    branchRates[storedBrotherRateIndex]->setValue(new double(storedBrotherRate * scaleBrotherRate));
    //branchRates[storedChildRateIndex]->touch();
    
    //std::cout << "chld rate  " << storedChildRate << " -> " << storedChildRate*scaleChildRate << "\n";
   // std::cout << "node rate  " << storedNodeRate << " -> " << storedNodeRate*scaleNodeRate << "\n";
    //std::cout << "bro  rate  " << storedBrotherRate << " -> " << storedBrotherRate*scaleBrotherRate << "\n";
    
    if (storedChildRate * scaleChildRate == 0.0)
    {
        ;//std::cout << "new scaledChildRate == 0.0\n";
    }
    
    //if (storedNodeRate * scaleNodeRate == 0.0)
    //{
     //   std::cout << "new scaledNodeRate == 0.0\n";
    //}
    if (storedBrotherRate * scaleBrotherRate == 0.0)
    {
        ;//std::cout << "new scaledBrotherRate == 0.0\n";
    }
    //std::cout << scaleChildRate*scaleBrotherRate << "  " << log(scaleChildRate*scaleBrotherRate) << "\n";

    
    // MH
    
  // return 0.0;
    return log(scaleChildRate*scaleBrotherRate);
}

void AdmixtureNearestNeighborInterchangeAndRateShift::acceptSimpleMove(void) {
    
    //std::cout << "accept NNI\n";
}

void AdmixtureNearestNeighborInterchangeAndRateShift::rejectSimpleMove( void ) {
    
    if (!failed)
    {
        //std::cout << "reject NNI\n";
        
        // undo the proposal
        storedNode->removeChild(storedBrother);
        storedNodeParent->removeChild(storedNodeChildMove);
        
        storedNode->addChild(storedNodeChildMove);
        storedNodeParent->addChild(storedBrother);
        
        storedBrother->setParent(storedNodeParent);
        storedNodeChildMove->setParent(storedNode);
        
        branchRates[storedChildRateIndex]->setValue(new double(storedChildRate));
        //branchRates[storedNodeRateIndex]->setValue(new double(storedNodeRate));
        branchRates[storedBrotherRateIndex]->setValue(new double(storedBrotherRate));
    }
    
}


void AdmixtureNearestNeighborInterchangeAndRateShift::acceptMove( void ) {
    // nothing to do
    changed = false;
    //std::cout << "accept NNI\n";
   // acceptSimpleMove();
}

/*
void AdmixtureNearestNeighborInterchangeAndRateShift::acceptSimpleMove( void ) {
    // do nothing
    ;
}*/

double AdmixtureNearestNeighborInterchangeAndRateShift::performMove( double &probRatio ) {
    
    //std::cout << "NNI::performMove()\n";
    
    if (changed) {
   //;     throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf ) {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    variable->touch();

    // calculate the probability ratio for the node we just changed
    probRatio = variable->getLnProbabilityRatio();
    //std::cout << "n\t" << variable->getName() << "\t" << probRatio << "\n";
 
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        if (!failed)
        {
            // these three rates should be touched due to setValue()
            branchRates[storedChildRateIndex]->touch();
            probRatio += branchRates[storedChildRateIndex]->getLnProbabilityRatio();
            branchRates[storedBrotherRateIndex]->touch();
            probRatio += branchRates[storedBrotherRateIndex]->getLnProbabilityRatio();
           // probRatio += branchRates[storedNodeRateIndex]->getLnProbabilityRatio();
           // std::cout << probRatio << "\n";
        }
    
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            //std::cout << "\tch\t" << theNode->getName() << "\t" << theNode->getLnProbabilityRatio() << "  " <<  probRatio << "\n";
        }
    }
    //std::cout << "pr " << probRatio << "    hr " << hr << "\n";
    
    return hr;
}

void AdmixtureNearestNeighborInterchangeAndRateShift::rejectMove( void ) {
    changed = false;
    //std::cout << "REJECT\n";
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    //variable->touch();
    //for (std::map<AdmixtureNode*,double>::iterator it = storedRates.begin(); it != storedRates.end(); it++)
    //    branchRates[it->first->getIndex()]->touch();
}


void AdmixtureNearestNeighborInterchangeAndRateShift::swapNode(DagNode *oldN, DagNode *newN) {
    
    MoveOld::swapNode(oldN,newN);
    
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


void AdmixtureNearestNeighborInterchangeAndRateShift::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta /= (2.0 - rate/0.234 );
    }
}
