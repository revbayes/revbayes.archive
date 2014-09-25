//
//  AdmixtureEdgeSlideLength.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/10/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "AdmixtureTree.h"
#include "AdmixtureEdgeSlideLength.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeSlideLength::AdmixtureEdgeSlideLength(StochasticNode<AdmixtureTree> *v, double w, unsigned long g, double d) : MoveOld(v, w), variable( v ) {
    
    activeGen = g;
    changed = false;
    failed = false;
    delta = d;
    
}

bool AdmixtureEdgeSlideLength::isActive(unsigned long g) const
{
    return g > activeGen;
}

/* Clone object */
AdmixtureEdgeSlideLength* AdmixtureEdgeSlideLength::clone( void ) const {
    
    return new AdmixtureEdgeSlideLength( *this );
}

const std::string& AdmixtureEdgeSlideLength::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeSlideLength";
    
    return name;
}

void AdmixtureEdgeSlideLength::findNewBrothers(std::vector<AdmixtureNode *> &b, AdmixtureNode &p, AdmixtureNode *n) {
    
    if (&p != n) {
        
        size_t numChildren = n->getNumberOfChildren();
        AdmixtureNode* child;
        
        for (size_t i = 0; i < numChildren; i++)
        {
            child = &n->getChild( i );
            if ( child->getAge() < p.getAge() ) {
                b.push_back( child );
            } else {
                findNewBrothers(b, p, child);
            }
        }
    }
}


double AdmixtureEdgeSlideLength::topologySlide(AdmixtureNode* ap, AdmixtureNode* ac)
{
    double lnP = 0.0;
    
    // sample slide distance and direction
    double distance = RbStatistics::Exponential::rv(delta, *GLOBAL_RNG);
    int direction = ( GLOBAL_RNG->uniform01() < 0.5 ? -1 : 1);
//    lnP += RbStatistics::Exponential::lnPdf(delta, distance);

    // slide event
    while (distance > 0.0)
    {
        oneSlide(ap, ac, distance, direction);
    }
    
    return lnP;
}

double AdmixtureEdgeSlideLength::oneSlide(AdmixtureNode* ap, AdmixtureNode* ac, double& distance, int& direction) {
    
    double lnP = 0.0;
    
    double ages[2] = { ap->getAge(), ac->getAge() };

//    double childAge = ac->getAge();
    
    
    // rootwards (age increase)
    if (direction == 1)
    {
        // record rootward ages and find next node to slide over
        double adjacentAges[2] = { ap->getParent().getAge(), ac->getParent().getAge() };
        size_t limitingIndex = 0;
        if (adjacentAges[0] > adjacentAges[1])
            limitingIndex = 1;
        
        double ageDifference = adjacentAges[limitingIndex] - ages[limitingIndex];
        
        // slide is within next segment
        if (distance < ageDifference)
        {
            
        }
        
        // slide is outside next segment
        else
        {
            // internal node
            
            
            // root
        }
        
    }
    
    // tipwards (age decrease)
    else if (direction == -1)
    {
        
    }
    else
    {
        ; // throw error
    }
    
    return lnP;
}


/** Perform the move */
double AdmixtureEdgeSlideLength::performSimpleMove( void ) {
    
    double lnP = 0.0;
    
    failed = false;
    AdmixtureTree& tau = variable->getValue();
    
    // prepare move
    std::vector<AdmixtureNode*> admixtureParents = tau.getAdmixtureParents();
    size_t numAdmixtureEvents = admixtureParents.size();
    if (numAdmixtureEvents == 0)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    AdmixtureNode* ap = admixtureParents[ GLOBAL_RNG->uniform01() * numAdmixtureEvents ];
    AdmixtureNode* ac = &ap->getAdmixtureChild();
    storedAdmixtureEdges.push_back( AdmixtureEdgePosition(ap, ac, &ap->getChild(0), &ac->getChild(0), ap->getAge(), ac->getWeight()) );
    
    // perform move
    
    
    
    return lnP;
    
//    //std::cout << "add\n";
//    
//    failed = false;
//    
//    // Get random number generator
//    RandomNumberGenerator* rng     = GLOBAL_RNG;
//    
//    // sample a random position on the tree
//    AdmixtureTree& tau = variable->getValue();
//    double treeLength = tau.getTreeLength();
//    double u = rng->uniform01() * treeLength;
//    size_t numNodes = tau.getNumberOfNodes();
//    
//    // record position
//    double v = 0.0;
//    double admixtureAge = 0.0;
//    double branchLength = 0.0;
//    AdmixtureNode* child = NULL;
//    AdmixtureNode* parent = NULL;
//    for (size_t i = 0; i < numNodes; i++)
//    {
//        child = &tau.getNode(i);
//        if (!child->isRoot())
//        {
//            parent = &child->getParent();
//            branchLength = parent->getAge() - child->getAge();
//            v += branchLength;
//            if (v > u)
//            {
//                admixtureAge = child->getAge() - (u - v);
//                break;
//            }
//        }
//    }
//    
//    storedAdmixtureAge = admixtureAge;
//    
//    // store adjacent nodes to new parent node
//    storedAdmixtureParentChild = child;
//    storedAdmixtureParentParent = parent;
//    
//    // insert admixtureParent into graph
//    storedAdmixtureParent = new AdmixtureNode();
//    storedAdmixtureParent->setAge(admixtureAge);
//    storedAdmixtureParent->setParent(storedAdmixtureParentParent);
//    storedAdmixtureParent->addChild(storedAdmixtureParentChild);
//    storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
//    storedAdmixtureParentParent->addChild(storedAdmixtureParent);
//    storedAdmixtureParentChild->setParent(storedAdmixtureParent);
//    tau.pushAdmixtureNode(storedAdmixtureParent);
//    
//    // sample random position for admixture edge destination for a given tree height
//    std::vector<AdmixtureNode*> brothers;
//    findNewBrothers(brothers, *storedAdmixtureParent, &tau.getRoot());
//    int index = int(rng->uniform01() * brothers.size());
//    
//    // sample admixtureChild from brothers
//    // forward proposal prob == -log(numBrothers) ??
//    
//    // store adjacent nodes to new child node
//    storedAdmixtureChildChild = brothers[index];
//    storedAdmixtureChildParent = &storedAdmixtureChildChild->getParent();
//    
//    // insert admixtureChild into graph
//    storedAdmixtureChild = new AdmixtureNode();
//    storedAdmixtureChild->setAge(admixtureAge);
//    storedAdmixtureChild->setParent(storedAdmixtureChildParent);
//    storedAdmixtureChild->addChild(storedAdmixtureChildChild);
//    storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
//    storedAdmixtureChildParent->addChild(storedAdmixtureChild);
//    storedAdmixtureChildChild->setParent(storedAdmixtureChild);
//    double w = rng->uniform01() * 0.5;
//    storedAdmixtureChild->setWeight(w);
//    tau.pushAdmixtureNode(storedAdmixtureChild);
//    
//    // create admixture edge
//    storedAdmixtureChild->setAdmixtureParent(storedAdmixtureParent);
//    storedAdmixtureParent->setAdmixtureChild(storedAdmixtureChild);
//    
//    // update with outgroup settings
//    storedAdmixtureChild->setOutgroup(storedAdmixtureChildParent->isOutgroup());
//    storedAdmixtureParent->setOutgroup(storedAdmixtureParentParent->isOutgroup());
//    
//    // prior * proposal ratio
//    size_t numEvents = tau.getNumberOfAdmixtureChildren();
//    double unitTreeLength = tau.getUnitTreeLength();
//    //double lnP = log( (rate->getValue() * unitTreeLength) / numEvents );
//    double lnP = 0.0;
//    //std::cout << "add\t" << lnP << "\t" << rate->getValue() << "\t" << unitTreeLength << "\t" << numEvents << "\n";
//    
//    // bombs away
//    return lnP;// + lnJ;
    
}


void AdmixtureEdgeSlideLength::rejectSimpleMove( void ) {
    
    if (!failed)
    {
        // revert edges
        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);
        
        // remove nodes from graph structure
        AdmixtureTree& tau = variable->getValue();
        
        tau.eraseAdmixtureNode(storedAdmixtureParent);
        tau.eraseAdmixtureNode(storedAdmixtureChild);
        
        // std::cout << "add fail\t" << tau.getNumberOfAdmixtureChildren() << "\n";
        //std::cout << "\nafter AEA reject\n";
        //tau.checkAllEdgesRecursively(&tau.getRoot());
    }
    
}

void AdmixtureEdgeSlideLength::acceptSimpleMove(void)
{
    // std::cout << "add success";
    //  std::cout << "\t" << variable->getValue().getNumberOfAdmixtureChildren();
    //  std::cout << "\n";
}


void AdmixtureEdgeSlideLength::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        //std::cout << "AEA\ttau\n";
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    }
}


void AdmixtureEdgeSlideLength::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeSlideLength::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

double AdmixtureEdgeSlideLength::performMove( double &probRatio ) {
    
    
    if (changed) {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf ) {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    variable->touch();
    
    probRatio = 0.0;
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            //   std::cout << theNode->getName() << "\t" << probRatio << "\n";
        }
    }
    
    return hr;
}