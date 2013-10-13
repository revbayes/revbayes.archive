//
//  TreeCharacterHistory.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/26/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AbstractCharacterHistoryCtmc.h"
#include "StochasticNode.h"
#include "TreeCharacterHistory.h"

using namespace RevBayesCore;

TreeCharacterHistory::TreeCharacterHistory(std::vector<const TypedDagNode<BranchHistory>* > bh, const TypedDagNode<TimeTree>* tr, size_t nc, size_t ns) : TypedDistribution<RevBayesCore::BranchHistory>(new BranchHistory(nc,ns,0)), numCharacters(nc), numStates(ns), tree(tr)
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
    
    // update node states
    for (int i = (int)nodes.size()-1; i >= 0; i--)
    {
        std::cout << "redrawValue() for BranchHistory " << i << "\n";

        TopologyNode* p = nodes[i];
        size_t parentIdx = 0;
        if (!p->isRoot())
            parentIdx = p->getParent().getIndex();
        
        // cast away constness like a villain
        const StochasticNode<BranchHistory>* cbh = static_cast<const StochasticNode<BranchHistory>* >(branchHistories[i]);
        StochasticNode<BranchHistory>* bh = const_cast<StochasticNode<BranchHistory>* >(cbh);

        std::cout << "BEFORE NODE\n";
        bh->getValue().print();
        
        // ... now I can setValue()
        std::multiset<CharacterEvent*, CharacterEventCompare> updateSet;
        std::set<CharacterEvent*> parentSet;
        std::set<CharacterEvent*> childSet;
        std::set<size_t> indexSet;
        std::vector<CharacterEvent*> parentChildVector, parentVector, childVector;
        
        if (p->isRoot())
        {
            std::cout << "isRoot()\n";
            bh->getDistribution().redrawValue();
        }
        else
        {
            //std::cout << "idx" << parentIdx << " " << i << "\n";
            parentChildVector = branchHistories[parentIdx]->getValue().getChildCharacters();
            parentVector = branchHistories[i]->getValue().getParentCharacters();
            for (size_t j = 0; j < parentVector.size(); j++)
                parentVector[j]->setState(parentChildVector[j]->getState());
                //parentSet.insert(new CharacterEvent(*parentVector[j]));
            //std::cout << "p "; for (size_t j = 0; j < parentVector.size(); j++) std::cout << parentVector[j]->getState() << " "; std::cout << "\n";
            //std::cout << "c "; for (size_t j = 0; j < childVector.size(); j++) std::cout << childVector[j]->getState() << " "; std::cout << "\n";
            bh->getValue().setParentCharacters(parentVector);
            bh->getValue().setRedrawParentCharacters(false);
            if (p->isTip())
                bh->getValue().setRedrawChildCharacters(false);
            bh->getDistribution().redrawValue();
            
        }
        bh->getValue().setRedrawChildCharacters(false);
        

        std::cout << "AFTER NODE\n";
        bh->getValue().print();
    }
    
    
    
    // update paths
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
        
        std::cout << "BEFORE PATH \n";
        bh->getValue().print();
        
        // ... now I can setValue()
        std::multiset<CharacterEvent*, CharacterEventCompare> updateSet;
        std::set<CharacterEvent*> parentSet;
        std::set<CharacterEvent*> childSet;
        std::set<size_t> indexSet;
        std::vector<CharacterEvent*> parentVector, childVector;
    
        if (p->isTip())
        {
            // childVector = observedCharacters;
            //for (size_t i = 0; i < childVector.size(); i++)
            //    childSet.insert(childVector[i]);
            //bh->getValue().setChildCharacters(childSet);
            std::cout << "isTip()\n";
            bh->getDistribution().redrawValue();
            bh->getValue().setRedrawHistory(false);
        }
        else
        {
            bh->getDistribution().redrawValue();
//            bh->getValue().setRedrawChildCharacters(false);
            bh->getValue().setRedrawHistory(false);
        }

        std::cout << "AFTER\n";
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
    const TimeTree* tau = &tree->getValue();
    std::vector<TopologyNode*> nodes = tau->getNodes();
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < numCharacters; i++)
        indexSet.insert(i);
    
    // update internal node states
    for (int i = (int)nodes.size()-1; i >= 0; i--)
    {
        std::cout << "simulate() for BranchHistory " << i << "\n";
        
        TopologyNode* p = nodes[i];
        //size_t nodeIdx = p->getIndex();
        size_t parentIdx = 0;
        if (!p->isRoot())
            parentIdx = p->getParent().getIndex();
        
        // cast away constness like a villain
        const StochasticNode<BranchHistory>* cbh = static_cast<const StochasticNode<BranchHistory>* >(branchHistories[i]);
        StochasticNode<BranchHistory>* bh = const_cast<StochasticNode<BranchHistory>* >(cbh);
        AbstractCharacterHistoryCtmc* achc = static_cast<AbstractCharacterHistoryCtmc*>(&bh->getDistribution());
        
        // ... now I can setValue()
        std::multiset<CharacterEvent*, CharacterEventCompare> updateSet;
        std::set<CharacterEvent*> parentSet;
        std::set<CharacterEvent*> childSet;
        std::set<size_t> indexSet;
        std::vector<CharacterEvent*> parentVector, childVector;
        
        // set parentCharacters per BranchHistory
        if (p->isRoot())
        {
            bh->getValue().setRedrawChildCharacters(false);
            bh->getValue().setRedrawHistory(false);
            bh->getValue().setRedrawParentCharacters(true);
            bh->getDistribution().redrawValue();
            bh->getValue().setRedrawParentCharacters(false);
        }
        else
        {
            parentVector = branchHistories[parentIdx]->getValue().getChildCharacters();
            bh->getValue().setParentCharacters(parentVector);
        }
        
        // simulate path towards childCharacters
        achc->simulate();
        
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
