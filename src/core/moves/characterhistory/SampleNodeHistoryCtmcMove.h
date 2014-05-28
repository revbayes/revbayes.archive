//
//  SampleNodeHistoryCtmcMove.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__SampleNodeHistoryCtmcMove__
#define __rb_mlandis__SampleNodeHistoryCtmcMove__

#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "AbstractTreeHistoryCtmc.h"
#include "BranchHistory.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class SampleNodeHistoryCtmcMove : public SimpleMove {
        
    public:
        SampleNodeHistoryCtmcMove( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, double l, bool tuning, double w);                                    //!<  constructor
        
        // Basic utility functions
        SampleNodeHistoryCtmcMove*              clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        void                                    acceptSimpleMove(void);
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    tune(void);
        
    private:
        
        StochasticNode<AbstractCharacterData>*  variable;
        StochasticNode<treeType>*               tree;

        TopologyNode*                           storedNode;
        std::vector<TopologyNode*>              storedAdjNodes;
        BranchHistory*                          storedValue;
        std::vector<BranchHistory*>             storedAdjValues;
        
        
        // parameters
        double                                  lambda;
        size_t                                  numNodes;
        size_t                                  numCharacters;
        size_t                                  numStates;
        
    };
}

#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

template<class charType, class treeType>
RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::SampleNodeHistoryCtmcMove( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, double l, bool tuning, double w) :
SimpleMove(n, w, tuning),
variable(n),
tree(t),
lambda(l)
{
    numNodes = t->getValue().getNumberOfNodes();
    numCharacters = n->getValue().getNumberOfCharacters();
    numStates = static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates();
}

// Basic utility functions
template<class charType, class treeType>
RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>* RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::clone(void) const
{
    return new SampleNodeHistoryCtmcMove( *this );
}

template<class charType, class treeType>
void RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN)
{
    SimpleMove::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        variable = static_cast<StochasticNode<AbstractCharacterData>* >( newN );
    }
    else if (oldN == tree)
    {
        tree = static_cast<StochasticNode<TimeTree>* >( newN );
    }
}

template<class charType, class treeType>
const std::string& RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::getMoveName(void) const
{
    static std::string name = "SampleNodeHistoryCtmcMove";
    return name;
}

template<class charType, class treeType>
double RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::SampleNodeHistoryCtmcMove::performSimpleMove(void)
{
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    
    // sample node/branches to update and store values
    std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
    size_t nodeIdx = GLOBAL_RNG->uniform01() * nodes.size();
    storedNode = nodes[nodeIdx];
    
    if (storedNode->isRoot())
        return RbConstants::Double::neginf;
    
    storedValue = new BranchHistory(p->getHistory(*storedNode));
    
    storedAdjNodes.clear();
    storedAdjValues.clear();
    for (size_t i = 0; i < storedNode->getNumberOfChildren(); i++)
    {
        TopologyNode* nd = &storedNode->getChild(i);
        storedAdjNodes.push_back(nd);
        storedAdjValues.push_back(new BranchHistory(p->getHistory(*nd)));
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
    
    
    //std::cout << "OLD P\n";
    //p->getValue().print();
    //std::cout << "OLD P (stored)\n";
    //storedValue.print();
    
    //    double lnProposalBwd = p->computeLnProposal();
    p->samplePathEnd(*storedNode, updateSet);
    p->samplePathHistory(*storedNode, updateSet);
    for (size_t i = 0; i < storedAdjNodes.size(); i++)
    {
        p->samplePathStart(*storedAdjNodes[i], updateSet);
        p->samplePathHistory(*storedAdjNodes[i], updateSet);
    }
    //    double lnProposalFwd = p->computeLnProposal();
    
    //std::cout << "NEW P (stored)\n";
    //storedValue.print();
    //std::cout << "NEW P\n";
    //p->getValue().print();
    
    //return 0.0;
    //    return lnProposalBwd - lnProposalFwd;
    return 0.0;
}

template<class charType, class treeType>
void RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}

template<class charType, class treeType>
void RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::rejectSimpleMove(void)
{
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());   

    p->setHistory(*storedValue, storedNode->getIndex());
    for (size_t i = 0; i < storedAdjNodes.size(); i++)
    {
        p->setHistory(*storedAdjValues[i], storedAdjNodes[i]->getIndex());
    }
    
}

template<class charType, class treeType>
void RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::acceptSimpleMove(void)
{
    //std::cout << "ACCEPT PATH\n";
}

template<class charType, class treeType>
void RevBayesCore::SampleNodeHistoryCtmcMove<charType, treeType>::tune(void)
{
    /*
     double rate = numAccepted / double(numTried);
     
     if ( rate > 0.44 )
     {
     lambda *= (1.0 + ((rate-0.44)/0.56) );
     }
     else
     {
     lambda /= (2.0 - rate/0.44 );
     }
     */
}

#endif /* defined(__rb_mlandis__SampleNodeHistoryCtmcMove__) */
