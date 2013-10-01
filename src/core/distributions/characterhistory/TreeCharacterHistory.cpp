//
//  TreeCharacterHistory.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/26/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "StochasticNode.h"
#include "TreeCharacterHistory.h"

using namespace RevBayesCore;

TreeCharacterHistory::TreeCharacterHistory(std::vector<const TypedDagNode<BranchHistory>* > bh, const TypedDagNode<TimeTree>* tr, size_t nc, size_t ns) : TypedDistribution<RevBayesCore::BranchHistory>(new BranchHistory(nc,ns)), numCharacters(nc), numStates(ns), tree(tr)
{
    
    for (size_t i = 0; i < bh.size(); i++)
    {
        branchHistories.push_back(bh[i]);
        addParameter(bh[i]);
    }
    
    addParameter(tree);
    
    //dirtyBranch = std::vector<bool>(branchHistories.size(), true);
}

TreeCharacterHistory::TreeCharacterHistory(const TreeCharacterHistory& m) : TypedDistribution<RevBayesCore::BranchHistory>(m), numCharacters(m.numCharacters), numStates(m.numStates), tree(m.tree), branchHistories(m.branchHistories)
{
    
}

void TreeCharacterHistory::redrawValue(void)
{

    const TimeTree* tau = &tree->getValue();
    std::vector<TopologyNode*> nodes = tau->getNodes();
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < numCharacters; i++)
        indexSet.insert(i);
    
    // update internal node states
    for (int i = (int)nodes.size()-1; i >= 0; i--)
    {
        std::cout << "redrawValue() for BranchHistory " << i << "\n";
        
        TopologyNode* p = nodes[i];
        //size_t nodeIdx = p->getIndex();
        size_t parentIdx = 0;
        if (!p->isRoot())
            parentIdx = p->getParent().getIndex();
        
        // cast away constness like a villain
        const StochasticNode<BranchHistory>* cbh = static_cast<const StochasticNode<BranchHistory>* >(branchHistories[i]);
        StochasticNode<BranchHistory>* bh = const_cast<StochasticNode<BranchHistory>* >(cbh);
  
        // ... now I can setValue()
        std::multiset<CharacterEvent*, CharacterEventCompare> updateSet;
        std::set<CharacterEvent*> parentSet;
        std::set<CharacterEvent*> childSet;
        std::set<size_t> indexSet;
        std::vector<CharacterEvent*> parentVector, childVector;
        
        if (p->isRoot())
        {
            bh->getDistribution().redrawValue();
            bh->getValue().setRedrawParentCharacters(false);
        }
        else
        {
            parentVector = branchHistories[parentIdx]->getValue().getChildCharacters();
            for (size_t i = 0; i < parentVector.size(); i++)
                parentSet.insert(parentVector[i]);
            bh->getValue().setParentCharacters(parentSet);
            bh->getValue().setRedrawParentCharacters(false);
        }
        
        if (p->isTip())
        {
            // childVector = observedCharacters;
            //for (size_t i = 0; i < childVector.size(); i++)
            //    childSet.insert(childVector[i]);
            //bh->getValue().setChildCharacters(childSet);
            
            bh->getDistribution().redrawValue();
        }
        else
        {
            bh->getDistribution().redrawValue();
        }
        
        bh->getValue().print();
        std::cout << "--------\n";
    }
    
    // enable redraw for parent/child/path
    for (int i = 0; i < branchHistories.size(); i++)
    {
        const StochasticNode<BranchHistory>* cbh = static_cast<const StochasticNode<BranchHistory>* >(branchHistories[i]);
        StochasticNode<BranchHistory>* bh = const_cast<StochasticNode<BranchHistory>* >(cbh);
        bh->getValue().setRedrawParentCharacters(true);
        bh->getValue().setRedrawChildCharacters(true);
        bh->getValue().setRedrawHistory(true);
    }
}

void TreeCharacterHistory::simulate(void)
{
    
    
}

TreeCharacterHistory* TreeCharacterHistory::clone(void) const
{
    return new TreeCharacterHistory( *this );
}

void TreeCharacterHistory::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    for (size_t i = 0; i < branchHistories.size(); i++)
        if (oldP == branchHistories[i])
            branchHistories[i] = static_cast<const TypedDagNode<BranchHistory>* >( newP );
    
    if (oldP == tree)
        tree = static_cast<const TypedDagNode<TimeTree>* >(newP);
}

double TreeCharacterHistory::computeLnProbability(void)
{

    double lnProb = 0.0;
    
    
    return lnProb;
}
