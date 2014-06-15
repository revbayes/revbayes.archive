//
//  AdmixtureShiftNodeAgeAndRate.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 3/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureShiftNodeAgeAndRate.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureShiftNodeAgeAndRate::AdmixtureShiftNodeAgeAndRate(StochasticNode<AdmixtureTree> *v, std::vector<ContinuousStochasticNode*> br, int ni, double d, bool t, double w) : MoveOld( v, w, t), variable( v ), branchRates(br), nodeIndex(ni), changed(false), failed(false), delta(d) {
    
    for (size_t i = 0; i < branchRates.size(); i++)
        nodes.insert(branchRates[i]);
}



/* Clone object */
AdmixtureShiftNodeAgeAndRate* AdmixtureShiftNodeAgeAndRate::clone( void ) const {
    
    return new AdmixtureShiftNodeAgeAndRate( *this );
}



const std::string& AdmixtureShiftNodeAgeAndRate::getMoveName( void ) const {
    static std::string name = "AdmixtureShiftNodeAgeAndRate";
    
    return name;
}


/** Perform the move */
double AdmixtureShiftNodeAgeAndRate::performSimpleMove( void ) {
    
    
    failed = false;
    
    //std::cout << "\nAge-Rate Shift\n";
    
    // clear old rates
    storedRates.clear();
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    AdmixtureNode* node = &tau.getNode(nodeIndex);
    AdmixtureNode& parent = node->getParent();
        
    // we need to work with the times
    double parent_age  = parent.getAge();
    double my_age      = node->getAge();
    double child_Age   = node->getChild( 0 ).getAge();
    if ( node->getNumberOfChildren() > 1 && child_Age < node->getChild( 1 ).getAge()) {
        child_Age = node->getChild( 1 ).getAge();
    }
    
    // admixture node ages are further constrained by their partner's time
    if (&node->getAdmixtureChild() != NULL)
    {
        AdmixtureNode& admixtureChild = node->getAdmixtureChild();
        double other_parent_age = admixtureChild.getParent().getAge();
        if (parent_age > other_parent_age)
            parent_age = other_parent_age;
        double other_child_age = admixtureChild.getChild(0).getAge();
        if (child_Age < other_child_age)
            child_Age = other_child_age;
    }
    else if (&node->getAdmixtureParent() != NULL)
    {
        AdmixtureNode& admixtureParent = node->getAdmixtureParent();
        double other_parent_age = admixtureParent.getParent().getAge();
        if (parent_age > other_parent_age)
            parent_age = other_parent_age;
        double other_child_age = admixtureParent.getChild(0).getAge();
        if (child_Age < other_child_age)
            child_Age = other_child_age;
    }
    
    // now we store all necessary values
    storedNode = node;
    storedAge = my_age;
  
    // draw new age
    double ageRange = parent_age - child_Age;
    double unitAge = (storedAge - child_Age) / ageRange;
    double a = delta * unitAge + 1.0;
    double b = delta * (1.0 - unitAge) + 1.0;
    double newUnitAge = RbStatistics::Beta::rv(a, b, *rng);
    double fwdProposal = RbStatistics::Beta::lnPdf(a, b, newUnitAge);
    double my_new_age = newUnitAge * ageRange + child_Age;
    double new_a = delta * newUnitAge + 1.0;
    double new_b = delta * (1.0 - newUnitAge) + 1.0;
    double bwdProposal = RbStatistics::Beta::lnPdf(new_a, new_b, unitAge);

    // double get branch length
    double old_brlen = node->getTopologyParent().getAge() - my_age;
    double new_brlen = node->getTopologyParent().getAge() - my_new_age;
    double brlen_ratio = old_brlen / new_brlen;
    
    // update branch rate leading to node
    double node_rate = branchRates[node->getIndex()]->getValue();
    //branchRates[node->getIndex()]->touch();
    //std::cout << "br_name" << branchRates[node->getIndex()]->getName() << "\n";
    storedRates[node] = node_rate;
    
    //std::cout << "brlen\t" << old_brlen << " -> " << new_brlen << "   " << brlen_ratio << "\n";
    //std::cout << "node_rate " << node_rate << " -> " << node_rate * brlen_ratio <<"\n";
    node_rate = node_rate * brlen_ratio;
    branchRates[node->getIndex()]->setValue(new double(node_rate));
    
    // update branch rates following from node
    for (size_t i = 0; i < storedNode->getNumberOfChildren(); i++)
    {
        AdmixtureNode* ch = &storedNode->getTopologyChild(i);
        //AdmixtureNode* ch = &storedNode->getChild(i);
        //double ch_ratio = (storedAge - ch->getAge()) / (my_new_age - ch->getAge());
        double ch_ratio = (storedAge - ch->getAge()) / (my_new_age - ch->getAge());
        double ch_rate = branchRates[ch->getIndex()]->getValue();
//        branchRates[ch->getIndex()]->touch();
        storedRates[ch] = ch_rate;
        //branchRates[node->getIndex()]->setValue(new double(ch_rate * ch_ratio));
        //std::cout << "ch_brlen " << i << "  " << (storedAge - ch->getAge()) << " -> " << (my_new_age - ch->getAge()) << "   " << ch_ratio << "\n";
        //std::cout << "ch_rate  " << i << "  " << ch_rate << " -> " << ch_rate * ch_ratio << "\n";
        ch_rate = ch_rate * ch_ratio;
        branchRates[ch->getIndex()]->setValue(new double(ch_rate));
    }
    
    // set the age
    node->setAge( my_new_age );
    
    // set age of admixture partner
    if (&node->getAdmixtureParent() != NULL)
        node->getAdmixtureParent().setAge( my_new_age );
    else if (&node->getAdmixtureChild() != NULL)
        node->getAdmixtureChild().setAge( my_new_age );
    
    
    return bwdProposal - fwdProposal;
}


void AdmixtureShiftNodeAgeAndRate::rejectSimpleMove( void ) {

    
    // undo the ages
    storedNode->setAge( storedAge );
    
    if (&storedNode->getAdmixtureParent() != NULL)
        storedNode->getAdmixtureParent().setAge( storedAge );
    else if (&storedNode->getAdmixtureChild() != NULL)
        storedNode->getAdmixtureChild().setAge( storedAge );

    // undo the rates
    branchRates[storedNode->getIndex()]->setValue(new double(storedRates[storedNode]));
    
    for (size_t i = 0; i < storedNode->getNumberOfChildren(); i++)
    {
        AdmixtureNode* ch = &storedNode->getTopologyChild(i);
        branchRates[ch->getIndex()]->setValue(new double(storedRates[ch]));
    }
}

void AdmixtureShiftNodeAgeAndRate::acceptMove( void ) {
    // nothing to do
    changed = false;
    //std::cout << "ACCEPT shift\n";
    acceptSimpleMove();
}

void AdmixtureShiftNodeAgeAndRate::acceptSimpleMove( void ) {
    // do nothing
    ;
}

double AdmixtureShiftNodeAgeAndRate::performMove( double &probRatio ) {
    
    //std::cout << "shift\n";
    
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
    
    // calculate the probability ratio for the node we just changed
    probRatio = variable->getLnProbabilityRatio();
    //std::cout << "n\t" << variable->getName() << "\t" << probRatio << "\n";
    
    for (std::map<AdmixtureNode*,double>::iterator it = storedRates.begin(); it != storedRates.end(); it++)
    {
        branchRates[it->first->getIndex()]->touch();
        probRatio += branchRates[it->first->getIndex()]->getLnProbabilityRatio();
        //std::cout << "n\t" << branchRates[it->first->getIndex()]->getName() << "\t" << probRatio << "\n";
    }
        
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            //std::cout << "\tch\t" << theNode->getName() << "\t" << theNode->getLnProbabilityRatio() << "  " <<  probRatio << "\n";
        }
    }
    
    return hr;
}

void AdmixtureShiftNodeAgeAndRate::rejectMove( void ) {
    changed = false;
    //std::cout << "REJECT shift\n";
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    //variable->touch();
    //for (std::map<AdmixtureNode*,double>::iterator it = storedRates.begin(); it != storedRates.end(); it++)
    //    branchRates[it->first->getIndex()]->touch();
}


void AdmixtureShiftNodeAgeAndRate::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
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

void AdmixtureShiftNodeAgeAndRate::tune( void )
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta /= (2.0 - rate/0.234 );
    }
}
