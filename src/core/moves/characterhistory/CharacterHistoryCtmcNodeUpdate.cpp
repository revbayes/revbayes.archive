//
//  CharacterHistoryCtmcNodeUpdate.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "CharacterHistoryCtmcNodeUpdate.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

using namespace RevBayesCore;

CharacterHistoryCtmcNodeUpdate::CharacterHistoryCtmcNodeUpdate( StochasticNode<BranchHistory>* n, std::vector< StochasticNode<BranchHistory> *> bhv, StochasticNode<TimeTree>* t, double l, bool tuning, double w) : Move(n, w, tuning), variable(n), bh_vector(bhv), lambda(l), numCharacters(n->getValue().getNumCharacters()), numStates(n->getValue().getNumStates()), tree(t), index(n->getValue().getIndex()), changed(false)

{
    // add tree to parameter list (used to find adjacent branches)
    nodes.insert(tree);
    for (size_t i = 0; i < bh_vector.size(); i++)
        nodes.insert(bh_vector[i]);

    storedValue.clear();
}

// Basic utility functions
CharacterHistoryCtmcNodeUpdate* CharacterHistoryCtmcNodeUpdate::clone(void) const
{
    return new CharacterHistoryCtmcNodeUpdate( *this );
}

void CharacterHistoryCtmcNodeUpdate::swapNode(DagNode *oldN, DagNode *newN)
{
    Move::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        variable = static_cast<StochasticNode<BranchHistory>* >( newN );
    }
    if (oldN == tree)
    {
        tree = static_cast<StochasticNode<TimeTree>* >(newN);
    }
    for (size_t i = 0; i < bh_vector.size(); i++)
    {
        if (oldN == bh_vector[i])
        {
            bh_vector[i] = static_cast<StochasticNode<BranchHistory>* >(newN);
        }
    }
    
}

const std::string& CharacterHistoryCtmcNodeUpdate::getMoveName(void) const
{
    static std::string name = "CharacterHistoryCtmcNodeUpdate";
    return name;
}

double CharacterHistoryCtmcNodeUpdate::performSimpleMove(void)
{

    //std::cout << "\nNODE UPDATE\n";
    // store adjacent histories
    TimeTree* tau = &tree->getValue();
    TopologyNode* nd = &tau->getNode(index);

    storedValue.clear();
    storedValue[index] = new BranchHistory( bh_vector[index]->getValue() );
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t ch_idx = nd->getChild(i).getIndex();
        BranchHistory* bp = new BranchHistory( bh_vector[ch_idx]->getValue() );
        storedValue[ch_idx] = bp;
    }
    
    // sample characters to update
    std::set<size_t> updateSet;
    updateSet.insert(GLOBAL_RNG->uniform01() * numCharacters); // at least one is inserted
    //std::cout << "updateSet ";
    for (size_t i = 0; i < numCharacters; i++)
    {
        if (GLOBAL_RNG->uniform01() < lambda)
        {
            updateSet.insert(i);
        }
    }
    //for (std::set<size_t>::iterator it = updateSet.begin(); it != updateSet.end(); it++) std::cout << *it << " "; std::cout << "\n";
    
    // propose new value
    double lnProposal = 0.0;
    AbstractCharacterHistoryCtmc* p = static_cast< AbstractCharacterHistoryCtmc* >( &variable->getDistribution() );
    //std::cout << "OLD P\n";
    //p->getValue().print();
    lnProposal -= p->computeLnProposal();
    p->sampleChildCharacterState(updateSet);
    p->samplePath(updateSet);
    lnProposal += p->computeLnProposal();
    //std::cout << "NEW P\n";
    //p->getValue().print();
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t ch_idx = nd->getChild(i).getIndex();
        AbstractCharacterHistoryCtmc* q = static_cast< AbstractCharacterHistoryCtmc* >( &bh_vector[ch_idx]->getDistribution() );
        //std::cout << "OLD Q " << i << "\n";
        //q->getValue().print();
        lnProposal -= q->computeLnProposal();
        q->getValue().setParentCharacters( p->getValue().getChildCharacters() );
        q->samplePath(updateSet);
        lnProposal += q->computeLnProposal();
        //std::cout << "NEW Q " << i << "\n";
        //q->getValue().print();
    }
    
    //std::cout << variable->getName() << " " << ne0 << " " << ne1 << "\n";
    
    //std::cout << lnProposal << "\n";
    
    return lnProposal;
}

void CharacterHistoryCtmcNodeUpdate::printParameterSummary(std::ostream &o) const
{
     o << "lambda = " << lambda;
}

void CharacterHistoryCtmcNodeUpdate::rejectSimpleMove(void)
{
    //std::cout << "---------\n";
    // store adjacent histories
    TimeTree* tau = &tree->getValue();
    TopologyNode* nd = &tau->getNode(index);
    
    bh_vector[index]->setValue(storedValue[index]);
    //storedValue[index]->print();
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t ch_idx = nd->getChild(i).getIndex();
        bh_vector[ch_idx]->setValue(storedValue[ch_idx]);
    }
    
    //std::cout << "REJECT\n\n";
    // free memory
    storedValue.clear();

}


void CharacterHistoryCtmcNodeUpdate::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    //std::cout << "ACCEPT\n\n";
    acceptSimpleMove();
}

double CharacterHistoryCtmcNodeUpdate::performMove(double& probRatio)
{
    if (changed)
    {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf )
    {
        return RbConstants::Double::neginf;
    }
    
    // touch nodes
    variable->touch();
    TopologyNode* nd = &tree->getValue().getNode(index);
    bh_vector[index]->touch();
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t bh_idx = nd->getChild(i).getIndex();
        //std::cout << "touch()\n";
        bh_vector[bh_idx]->touch();
    }
    
    // calculate the probability ratio for the node we just changed
    probRatio = variable->getLnProbabilityRatio();
    //std::cout << "nd+path  : " << probRatio << "\n";
    
    // and for child path lnProbs
    //TopologyNode* nd = &tree->getValue().getNode(index);
    double v = bh_vector[index]->getLnProbabilityRatio();
    double w = bh_vector[index]->getLnProbability();
    //std::cout << "nd " << index << " path : " << v << " " << w << "\n";
    probRatio += v;
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t bh_idx = nd->getChild(i).getIndex();
        v = bh_vector[bh_idx]->getLnProbabilityRatio();
        w = bh_vector[bh_idx]->getLnProbability();
        //std::cout << "ch" << i << " " << bh_idx << " path : " << v << " " << w << "\n";
        probRatio += v;
    }
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf )
    {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i)
        {
            DagNode* theAffectedNode = *i;
            //std::cout << theAffectedNode->getName() << "  " << theAffectedNode->getLnProbabilityRatio() << "\n";
            probRatio += theAffectedNode->getLnProbabilityRatio();
        }
    }
    
    
    return hr;
}

void CharacterHistoryCtmcNodeUpdate::rejectMove(void)
{
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
    // and for child path lnProbs
    
    TopologyNode* nd = &tree->getValue().getNode(index);
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t bh_idx = nd->getChild(i).getIndex();
        bh_vector[bh_idx]->touch();
    }

}

void CharacterHistoryCtmcNodeUpdate::acceptSimpleMove(void)
{
    ;
}

void CharacterHistoryCtmcNodeUpdate::tune(void)
{
    
}
