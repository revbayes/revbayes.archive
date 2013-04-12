//
//  AdmixtureEdgeSlide.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/1/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeSlide.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeSlide::AdmixtureEdgeSlide(StochasticNode<AdmixtureTree> *v, double l, double w) : Move( v, w), variable( v ), lambda(l), changed(false), failed(false) {
    
}



/* Clone object */
AdmixtureEdgeSlide* AdmixtureEdgeSlide::clone( void ) const {
    
    return new AdmixtureEdgeSlide( *this );
}



const std::string& AdmixtureEdgeSlide::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeSlide";
    
    return name;
}

void AdmixtureEdgeSlide::findNewBrothers(std::vector<AdmixtureNode *> &b, AdmixtureNode &p, AdmixtureNode *n) {
    
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

/** Perform the move */
double AdmixtureEdgeSlide::performSimpleMove( void ) {
    
    std::cout << "slide\n";
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    std::vector<AdmixtureNode*> admixtureParents = tau.getAdmixtureParents();
    
    // if no admixtureParent exists, the proposal fails
    if (admixtureParents.size() == 0)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // otherwise, proceed
    else
    {
        failed = false;
        
        // sample a random admixture parent node
        double u = rng->uniform01();
        size_t index = std::floor(admixtureParents.size() * u);
        
        // store admixture edge position
        storedAdmixtureParent = admixtureParents[index];
        storedAdmixtureChild = &storedAdmixtureParent->getAdmixtureChild();
        storedAdmixtureChildParent = &storedAdmixtureChild->getParent();
        storedAdmixtureChildChild = &storedAdmixtureChild->getChild(0);
        storedAdmixtureParentParent = &storedAdmixtureParent->getParent();
        storedAdmixtureParentChild = &storedAdmixtureParent->getChild(0);
     
        // remove admixture edge from graph
        storedAdmixtureChild->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild);
        
        storedAdmixtureParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);
        
        // get age for admixture event
        storedAge = storedAdmixtureChild->getAge();
        
        // find parent range
        double lnFwdProposal = 0.0;
        double lnBwdProposal = 0.0;
        
        // get older node limit
        AdmixtureNode* olderNode = storedAdmixtureParentParent;
        if (&olderNode->getParent() != NULL && storedAdmixtureChildParent != storedAdmixtureParentParent)
        {
            olderNode = &olderNode->getParent();
            lnBwdProposal += log(1.0 / olderNode->getNumberOfChildren());
        }
        
        // get younger node limit
        AdmixtureNode* youngerNode = storedAdmixtureParentChild;
        size_t numChildren = youngerNode->getNumberOfChildren();
        if (numChildren > 0)
        {
            youngerNode = &youngerNode->getChild(numChildren * rng->uniform01());
            lnFwdProposal += log(1.0 / numChildren);
        }
        
        // get age range between nodes
        double olderAge = olderNode->getAge();
        double youngerAge = youngerNode->getAge();
        double ageRange = olderAge - youngerAge;
        
        //std::cout << "\t" << youngerAge << "\t" << olderAge << "\t" << ageRange << "\n";
        
        // forwards proposal
        double unitAge = (storedAge - youngerAge) / ageRange;
        double a = lambda * unitAge + 1.0;
        double b = lambda * (1.0 - unitAge) + 1.0;
//        double fwdAlpha = lambda * unitAge / (1.0 - unitAge) + 1.0;
  //      double newUnitAge = RbStatistics::Beta::rv(fwdAlpha, lambda, *rng);
//        lnFwdProposal += RbStatistics::Beta::lnPdf(fwdAlpha, lambda, newUnitAge);
        double newUnitAge = RbStatistics::Beta::rv(a, b, *rng);
        lnFwdProposal += RbStatistics::Beta::lnPdf(a, b, newUnitAge);
        double newAge = newUnitAge * ageRange + youngerAge;
        //std::cout << "\tunit age\t" << unitAge << " -> " << newUnitAge << "\n";
        //std::cout << "\ttrue age\t" << storedAge << " -> " << newAge << "\n";
        
        // backwards proposal
//        double bwdAlpha = lambda * newUnitAge / (1.0 - newUnitAge) + 1.0;
        double new_a = lambda * newUnitAge + 1.0;
        double new_b = lambda * (1.0 - newUnitAge) + 1.0;
        lnBwdProposal += RbStatistics::Beta::lnPdf(new_a, new_b, unitAge);
        
        /*
        // now we store all necessary values
        storedNode = node;
        storedAge = my_age;
        
        // draw new ages and compute the hastings ratio at the same time
        double m = (my_age-child_Age) / (parent_age-child_Age);
        double a = delta * m + 1.0;
        double b = delta * (1.0-m) + 1.0;
        double new_m = RbStatistics::Beta::rv(a, b, *rng);
        double my_new_age = (parent_age-child_Age) * new_m + child_Age;
        
        // compute the Hastings ratio
        double forward = RbStatistics::Beta::lnPdf(a, b, new_m);
        double new_a = delta * new_m + 1.0;
        double new_b = delta * (1.0-new_m) + 1.0;
        double backward = RbStatistics::Beta::lnPdf(new_a, new_b, m);
        
        // set the age
        node->setAge( my_new_age );
        
        return backward - forward;
        */
        
        
        // new graph pointers
        newAdmixtureChildChild = storedAdmixtureChildChild;
        newAdmixtureParentChild = storedAdmixtureParentChild;
        newAdmixtureChildParent = storedAdmixtureChildParent;
        newAdmixtureParentParent = storedAdmixtureParentParent;

        
        // place admixture parent for new age if outside original edge
        storedAdmixtureParent->setAge(newAge);
        if (newAge < storedAdmixtureParentChild->getAge())
        {
            newAdmixtureParentChild = youngerNode;
            newAdmixtureParentParent = storedAdmixtureParentChild;
        }
        else if (newAge > storedAdmixtureParentParent->getAge())
        {
            newAdmixtureParentChild = storedAdmixtureParentParent;
            newAdmixtureParentParent = olderNode;
        }
        
        
        // place admixture child for new age if outside original edge
        storedAdmixtureChild->setAge(newAge);
        while (newAge < newAdmixtureChildChild->getAge())
        {
            //std::cout << newAge << " < " << newAdmixtureChildChild->getAge() << "\n";
            size_t numChildren = newAdmixtureChildChild->getNumberOfChildren();
            std::cout << newAdmixtureChildChild->getAge() << "\t" << numChildren << "\n";
            newAdmixtureChildParent = newAdmixtureChildChild;
            newAdmixtureChildChild = &newAdmixtureChildChild->getChild(numChildren * rng->uniform01());
            lnFwdProposal += log(1.0 / numChildren);
        }
        while (newAge > newAdmixtureChildParent->getAge())
        {
            //std::cout << newAge << " > " << newAdmixtureChildParent->getAge() << "\n";
            newAdmixtureChildChild = newAdmixtureChildParent;
            newAdmixtureChildParent = &newAdmixtureChildParent->getParent();
            lnBwdProposal += log(1.0 / newAdmixtureChildParent->getNumberOfChildren());
        }

        /*
        std::cout << "AC\n";
        
        std::cout << storedAdmixtureChildChild->getAge() << " < ";
        std::cout << storedAdmixtureChild->getAge() << " < ";
        std::cout << storedAdmixtureChildParent->getAge() << "\n";
        std::cout << storedAdmixtureChild->getNumberOfChildren() << "\n";
        std::cout << storedAdmixtureChildChild->getNumberOfChildren() << "\t" << storedAdmixtureChildParent->getNumberOfChildren() << "\n";
        std::cout << newAdmixtureChildChild->getNumberOfChildren() << "\t" << newAdmixtureChildParent->getNumberOfChildren() << "\n";
        std::cout << storedAdmixtureChild << "\n";
        std::cout << storedAdmixtureChildChild << "\t" << storedAdmixtureChildParent << "\n";
        std::cout << newAdmixtureChildChild << "\t" << newAdmixtureChildParent << "\n";
        */
         
        // insert admixtureChild to new position
        storedAdmixtureChild->setParent(newAdmixtureChildParent);
        storedAdmixtureChild->addChild(newAdmixtureChildChild);
        newAdmixtureChildChild->setParent(storedAdmixtureChild);
        newAdmixtureChildParent->removeChild(newAdmixtureChildChild);
        newAdmixtureChildParent->addChild(storedAdmixtureChild);
    
        /*
        std::cout << "AP\n";
        
        std::cout << storedAdmixtureParentChild->getAge() << " < ";
        std::cout << storedAdmixtureParent->getAge() << " < ";
        std::cout << storedAdmixtureParentParent->getAge() << "\n";
        std::cout << storedAdmixtureParent->getNumberOfChildren() << "\n";
        std::cout << storedAdmixtureParentChild->getNumberOfChildren() << "\t" << storedAdmixtureParentParent->getNumberOfChildren() << "\n";
        std::cout << newAdmixtureParentChild->getNumberOfChildren() << "\t" << newAdmixtureParentParent->getNumberOfChildren() << "\n";
        std::cout << storedAdmixtureParent << "\n";
        std::cout << storedAdmixtureParentChild << "\t" << storedAdmixtureParentParent << "\n";
        std::cout << newAdmixtureParentChild << "\t" << newAdmixtureParentParent << "\n";
        */
         
        // insert admixtureParent to new position
        storedAdmixtureParent->setParent(newAdmixtureParentParent);
        storedAdmixtureParent->addChild(newAdmixtureParentChild);
        newAdmixtureParentChild->setParent(storedAdmixtureParent);
        newAdmixtureParentParent->removeChild(newAdmixtureParentChild);
        newAdmixtureParentParent->addChild(storedAdmixtureParent);
        
        
        std:: cout << "slide lnPropRat\t" << lnBwdProposal - lnFwdProposal << " = " << lnBwdProposal << " - " << lnFwdProposal << "\n";
        // Hastings ratio
        return lnBwdProposal - lnFwdProposal;
    }
    
}


void AdmixtureEdgeSlide::rejectSimpleMove( void ) {
    

    // undo the proposal
    if (!failed)
    {
        
        std::cout << "reject edge slide\n";
        
        // NOTE: root used to protect from infinite recursions caused by AdmixtureNode::flagNewickComputation() from addChild,removeChild,setParent.
        AdmixtureNode* root = &variable->getValue().getRoot();

        // revert admixture parent
        if (storedAdmixtureParentParent == newAdmixtureParentParent && storedAdmixtureParentChild == newAdmixtureParentChild)
        {
            ; // do nothing
        }
        else
        {
            storedAdmixtureParent->setParent(root);
            
            if (storedAdmixtureParentParent != newAdmixtureParentParent)
            {
                newAdmixtureParentParent->removeChild(storedAdmixtureParent);
                newAdmixtureParentChild->setParent(newAdmixtureParentParent);
                newAdmixtureParentParent->addChild(newAdmixtureParentChild);
                storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
                storedAdmixtureParentParent->addChild(storedAdmixtureParent);
                storedAdmixtureParent->setParent(storedAdmixtureParentParent);
            }
            
            if (storedAdmixtureParentChild != newAdmixtureParentChild)
            {
                newAdmixtureParentChild->setParent(newAdmixtureParentParent);
                storedAdmixtureParentChild->setParent(storedAdmixtureParent);
                storedAdmixtureParent->removeChild(newAdmixtureParentChild);
                storedAdmixtureParent->addChild(storedAdmixtureParentChild);
            }
            
            storedAdmixtureParent->setParent(storedAdmixtureParentParent);
        }
        storedAdmixtureParent->setAge(storedAge);

        // revert admixture child
        if (storedAdmixtureChildParent == newAdmixtureChildParent && storedAdmixtureChildChild == newAdmixtureChildChild)
        {
            ; // do nothing
        }
        else
        {
            storedAdmixtureChild->setParent(root);
            
            if (storedAdmixtureChildParent != newAdmixtureChildParent)
            {
                newAdmixtureChildParent->removeChild(storedAdmixtureChild);
                newAdmixtureChildChild->setParent(newAdmixtureChildParent);
                newAdmixtureChildParent->addChild(newAdmixtureChildChild);
                storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
                storedAdmixtureChildParent->addChild(storedAdmixtureChild);
                storedAdmixtureChild->setParent(storedAdmixtureChildParent);
            }

            if (storedAdmixtureChildChild != newAdmixtureChildChild)
            {
                newAdmixtureChildChild->setParent(newAdmixtureChildParent);
                storedAdmixtureChildChild->setParent(storedAdmixtureChild);
                storedAdmixtureChild->removeChild(newAdmixtureChildChild);
                storedAdmixtureChild->addChild(storedAdmixtureChildChild);
            }

            storedAdmixtureChild->setParent(storedAdmixtureChildParent);
                      
        }
        storedAdmixtureChild->setAge(storedAge);
        
    }
}

void AdmixtureEdgeSlide::acceptSimpleMove(void) {
    std::cout << "accept edge slide\n";
}


void AdmixtureEdgeSlide::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}



void AdmixtureEdgeSlide::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeSlide::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

double AdmixtureEdgeSlide::performMove( double &probRatio ) {
    
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
            //std::cout << theNode->getName() << "\t" << probRatio << "\n";
        }
    }
    
    return hr;
}
