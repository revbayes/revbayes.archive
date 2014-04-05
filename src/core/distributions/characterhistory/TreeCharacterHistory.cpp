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

TreeCharacterHistory::TreeCharacterHistory(std::vector<const TypedDagNode<BranchHistory>* > bh, const TypedDagNode<TimeTree>* tr, size_t nc, size_t ns) :
    TypedDistribution<RevBayesCore::BranchHistory>(new BranchHistory(nc,ns,0)),
    tree(tr),
    numStates(ns),
    numCharacters(nc)
{
    
    for (size_t i = 0; i < bh.size(); i++)
    {
        branchHistories.push_back(bh[i]);
        addParameter(bh[i]);
    }
    
    addParameter(tree);
    
    //dirtyBranch = std::vector<bool>(branchHistories.size(), true);
}

TreeCharacterHistory::TreeCharacterHistory(const TreeCharacterHistory& m) : TypedDistribution<RevBayesCore::BranchHistory>(m), branchHistories(m.branchHistories), tree(m.tree), numStates(m.numStates), numCharacters(m.numCharacters)
{
    
}

void TreeCharacterHistory::redrawValue(void)
{

    const TimeTree* tau = &tree->getValue();
    std::vector<TopologyNode*> nodes = tau->getNodes();
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < numCharacters; i++)
        indexSet.insert(i);
    
    // update node states, from tips to root
    for (int i = 0; i < (int)nodes.size(); i++)
    {
        //std::cout << "redrawValue() for BranchHistory " << i << "\n";

        TopologyNode* p = nodes[i];
        size_t parentIdx = 0;
        
        if (!p->isRoot())
            parentIdx = p->getParent().getIndex();
        
        // cast away constness like a villain
        const StochasticNode<BranchHistory>* cbh = static_cast<const StochasticNode<BranchHistory>* >(branchHistories[i]);
        StochasticNode<BranchHistory>* bh = const_cast<StochasticNode<BranchHistory>* >(cbh);

        //std::cout << "BEFORE NODE\n";
        //bh->getValue().print();
        
        // ... now I can setValue()
        std::multiset<CharacterEvent*, CharacterEventCompare> updateSet;
        std::set<CharacterEvent*> parentSet;
        std::set<CharacterEvent*> childSet;
        std::set<size_t> indexSet;
        std::vector<CharacterEvent*> parentChildVector, nodeParentVector, nodeChildVector, childParentVector;
        
        if (!p->isTip())
        {
            //std::cout << "redrwaValue() idx" << parentIdx << " " << i << "\n";
            //parentChildVector = branchHistories[parentIdx]->getValue().getChildCharacters();
            //nodeParentVector = branchHistories[i]->getValue().getParentCharacters();
            //for (size_t j = 0; j < nodeParentVector.size(); j++)
            //    nodeParentVector[j]->setState(parentChildVector[j]->getState());
                //parentSet.insert(new CharacterEvent(*parentVector[j]));

            //std::cout << "p "; for (size_t j = 0; j < parentVector.size(); j++) std::cout << parentVector[j]->getState() << " "; std::cout << "\n";
            //std::cout << "c "; for (size_t j = 0; j < childVector.size(); j++) std::cout << childVector[j]->getState() << " "; std::cout << "\n";
            //bh->getValue().setParentCharacters(nodeParentVector);
            //bh->getValue().setRedrawParentCharacters(false);

            
            // sample new child characters for the internal node
            bh->getValue().setRedrawParentCharacters(false);
            bh->getValue().setRedrawHistory(false);
            bh->getValue().setRedrawChildCharacters(true);
            nodeChildVector = bh->getValue().getChildCharacters();
            //std::cout << "c "; for (size_t j = 0; j < nodeChildVector.size(); j++) std::cout << nodeChildVector[j]->getState() << " "; std::cout << "\n";
            bh->getDistribution().redrawValue();
            nodeChildVector = bh->getValue().getChildCharacters();
            bh->getValue().setRedrawChildCharacters(false);
            //std::cout << "c "; for (size_t j = 0; j < nodeChildVector.size(); j++) std::cout << nodeChildVector[j]->getState() << " "; std::cout << "\n";

            /*
            // update parent characters for internal node's descendants
            for (size_t j = 0; j < p->getNumberOfChildren(); j++)
            {
                TopologyNode* q = &p->getChild(j);
                int q_idx = q->getIndex();
                const StochasticNode<BranchHistory>* cqbh = static_cast<const StochasticNode<BranchHistory>* >(branchHistories[q_idx]);
                StochasticNode<BranchHistory>* qbh = const_cast<StochasticNode<BranchHistory>* >(cbh);
                //qbh->getDistribution().getValue().setParentCharacters(nodeChildVector);
                childParentVector = qbh->getValue().getParentCharacters();
                for (size_t k = 0; k < nodeChildVector.size(); k++) childParentVector[k]->setState( nodeChildVector[k]->getState() );
                
                std::vector<CharacterEvent*> v = bh->getDistribution().getValue().getChildCharacters();
                std::cout << "c_q "; for (size_t k = 0; k < v.size(); k++) std::cout << v[k]->getState() << " "; std::cout << "\n";
            }
            */
            
            //bh->getValue().setRedrawParentCharacters(false);
            //bh->getValue().setRedrawHistory(false);
            
        }
        else if (p->isTip())
        {
            bh->getValue().setRedrawParentCharacters(false);
            bh->getValue().setRedrawHistory(false);
            bh->getValue().setRedrawChildCharacters(false);
        }


        //std::cout << "AFTER NODE\n";
        //bh->getValue().print();
    }
    
    // update node parent states (same as parent child states for sympatry)
    for (int i = 0; i < (int)nodes.size(); i++)
    {
        TopologyNode* p = nodes[i];
        size_t parentIdx = 0;
        
        // cast away constness like a villain
        const StochasticNode<BranchHistory>* cbh = static_cast<const StochasticNode<BranchHistory>* >(branchHistories[i]);
        StochasticNode<BranchHistory>* bh = const_cast<StochasticNode<BranchHistory>* >(cbh);
        
        if (!p->isRoot())
        {
            parentIdx = p->getParent().getIndex();
            bh->getValue().setParentCharacters( branchHistories[parentIdx]->getValue().getChildCharacters() );
        }
        else if (p->isRoot())
        {

            bh->getValue().setRedrawParentCharacters(true);
            bh->getDistribution().redrawValue();
            bh->getValue().setRedrawParentCharacters(false);
        }

    }
    
    
    // update paths
    
    //int numEvents = 0;
    
    //for (int i = (int)nodes.size()-1; i >= 0; i--)
    for (int i = 0; i < (int)nodes.size(); i++)
    {
        //std::cout << "redrawValue() for BranchHistory " << i << "\n";
        
        TopologyNode* p = nodes[i];
        //size_t nodeIdx = p->getIndex();
        size_t parentIdx = 0;
        if (!p->isRoot())
            parentIdx = p->getParent().getIndex();
        
        // cast away constness like a villain
        const StochasticNode<BranchHistory>* cbh = static_cast<const StochasticNode<BranchHistory>* >(branchHistories[i]);
        StochasticNode<BranchHistory>* bh = const_cast<StochasticNode<BranchHistory>* >(cbh);
        
        //std::cout << "BEFORE PATH \n";
        //bh->getValue().print();
        
        // ... now I can setValue()
        std::multiset<CharacterEvent*, CharacterEventCompare> updateSet;
        std::set<CharacterEvent*> parentSet;
        std::set<CharacterEvent*> childSet;
        std::set<size_t> indexSet;
        std::vector<CharacterEvent*> nodeParentVector, nodeChildVector;
    
        
        bh->getValue().setRedrawHistory(true);
        
        if (p->isTip())
        {
            // childVector = observedCharacters;
            //for (size_t i = 0; i < childVector.size(); i++)
            //    childSet.insert(childVector[i]);
            //bh->getValue().setChildCharacters(childSet);
           // std::cout << "isTip()\n";
            bh->getDistribution().redrawValue();
            bh->getValue().setRedrawHistory(false);
        }
        else
        {
            bh->getDistribution().redrawValue();
//            bh->getValue().setRedrawChildCharacters(false);
            bh->getValue().setRedrawHistory(false);
        }

        //if (!p->isRoot()) numEvents += bh->getValue().getNumEvents();
        
        //std::cout << "AFTER\n";
//        std::cout << "redrawValue() for BranchHistory " << i << "\n";
//        bh->getValue().print();
//        std::cout << "--------\n";
    }
    
    //std::cout << (double)numEvents / numCharacters << " " << numEvents << " " << tau->getTreeLength() / tau->getRoot().getAge() << "\n";
    
    ;
    // enable redraw for parent/child/path
    for (size_t i = 0; i < branchHistories.size(); i++)
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
    
    //int numEvents = 0;
    
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
    
        //if (!p->isRoot()) numEvents += bh->getValue().getNumEvents();
        
        bh->getValue().print();
        std::cout << "--------\n";
    }
    
    //std::cout << (double)numEvents / numCharacters << " " << numEvents << " " << tau->getTreeLength() / tau->getRoot().getAge() << "\n";
    
    ;
    
    // enable redraw for parent/child/path
    for (size_t i = 0; i < branchHistories.size(); i++)
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
