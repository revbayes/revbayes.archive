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
    else if (oldN == tree)
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

    // store adjacent histories
    TimeTree* tau = &tree->getValue();
    TopologyNode* nd = &tau->getNode(index);
    storedValue[index] = new BranchHistory(variable->getValue());
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t ch_idx = nd->getChild(i).getIndex();
        BranchHistory* bp = new BranchHistory( bh_vector[ch_idx]->getValue() );
        storedValue[ch_idx] = bp;
    }
    
    // sample characters to update
    std::set<size_t> updateSet;
    updateSet.insert(GLOBAL_RNG->uniform01() * numCharacters); // at least one is inserted
    for (size_t i = 0; i < numCharacters; i++)
    {
        if (GLOBAL_RNG->uniform01() < lambda)
        {
            updateSet.insert(i);
        }
    }
    
    // propose new value
    AbstractCharacterHistoryCtmc* p = static_cast< AbstractCharacterHistoryCtmc* >( &variable->getDistribution() );
    p->sampleChildCharacterState(updateSet);
    p->samplePath(updateSet);
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t ch_idx = nd->getChild(i).getIndex();
        AbstractCharacterHistoryCtmc* q = static_cast< AbstractCharacterHistoryCtmc* >( &bh_vector[ch_idx]->getDistribution() );
        q->getValue().setParentCharacters( p->getValue().getParentCharacters() );
        q->samplePath(updateSet);
    }
    
    //std::cout << variable->getName() << " " << ne0 << " " << ne1 << "\n";
    
    return 0.0;
}

void CharacterHistoryCtmcNodeUpdate::printParameterSummary(std::ostream &o) const
{
     o << "lambda = " << lambda;
}

void CharacterHistoryCtmcNodeUpdate::rejectSimpleMove(void)
{
    // store adjacent histories
    TimeTree* tau = &tree->getValue();
    TopologyNode* nd = &tau->getNode(index);
    variable->setValue(storedValue[index]);
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        size_t ch_idx = nd->getChild(i).getIndex();
        bh_vector[ch_idx]->setValue(storedValue[ch_idx]);
    }
    
    // free memory
    storedValue.clear();

}


void CharacterHistoryCtmcNodeUpdate::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    //acceptSimpleMove();
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
    
    // touch the node
    variable->touch();
    
    // calculate the probability ratio for the node we just changed
    probRatio = variable->getLnProbabilityRatio();
    //std::cout << "nd+path  : " << probRatio << "\n";
    
    // and for child path lnProbs
    TopologyNode* nd = &tree->getValue().getNode(index);
    for (size_t i = 0; i < nd->getNumberOfChildren(); i++)
    {
        double v = bh_vector[nd->getChild(i).getIndex()]->getLnProbabilityRatio();
        //std::cout << "ch" << i << " path : " << v << "\n";
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

}

void CharacterHistoryCtmcNodeUpdate::tune(void)
{
    
}
