//
//  SamplePathHistoryCtmcMove.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__SamplePathHistoryCtmcMove__
#define __rb_mlandis__SamplePathHistoryCtmcMove__

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
    class SamplePathHistoryCtmcMove : public SimpleMove {
        
    public:
        SamplePathHistoryCtmcMove( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, double l, bool tuning, double w);                                    //!<  constructor
        
        // Basic utility functions
        SamplePathHistoryCtmcMove* clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        void                            acceptSimpleMove(void);
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);
        
    private:
        
        StochasticNode<AbstractCharacterData>*  variable;
        StochasticNode<treeType>*               tree;
        BranchHistory                           storedValue;
        size_t                                  storedIndex;
        
        // parameters
        double                          lambda;
        size_t                          numNodes;
        size_t                          numCharacters;
        size_t                          numStates;
        
        
    };
    
}

#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

template<class charType, class treeType>
RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::SamplePathHistoryCtmcMove( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, double l, bool tuning, double w) :
SimpleMove(n, w, tuning),
variable(n),
tree(t),
storedValue(0,0,0),
lambda(l)
{
    numNodes = t->getValue().getNumberOfNodes();
    numCharacters = n->getValue().getNumberOfCharacters();
    numStates = static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates();
}

// Basic utility functions
template<class charType, class treeType>
RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>* RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::clone(void) const
{
    return new SamplePathHistoryCtmcMove( *this );
}

template<class charType, class treeType>
void RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN)
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
const std::string& RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::getMoveName(void) const
{
    static std::string name = "SamplePathHistoryCtmcMove";
    return name;
}

template<class charType, class treeType>
double RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::SamplePathHistoryCtmcMove::performSimpleMove(void)
{
    
    // sample branch to update and store values
    storedIndex = GLOBAL_RNG->uniform01() * numNodes;
    storedValue = static_cast< AbstractTreeHistoryCtmc<charType, treeType>& >(variable->getDistribution()).getHistory(storedIndex);
    
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
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    
    //std::cout << "OLD P\n";
    //p->getValue().print();
    //std::cout << "OLD P (stored)\n";
    //storedValue.print();
    
    //    double lnProposalBwd = p->computeLnProposal();
    p->samplePathHistory(tree->getValue().getNode(storedIndex), updateSet);
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
void RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}

template<class charType, class treeType>
void RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::rejectSimpleMove(void)
{
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    p->setHistory(storedValue, storedIndex);
    // variable->setValue( new BranchHistory(storedValue) );
}

template<class charType, class treeType>
void RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::acceptSimpleMove(void)
{

}

template<class charType, class treeType>
void RevBayesCore::SamplePathHistoryCtmcMove<charType, treeType>::tune(void)
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

#endif /* defined(__rb_mlandis__SamplePathHistoryCtmcMove__) */
