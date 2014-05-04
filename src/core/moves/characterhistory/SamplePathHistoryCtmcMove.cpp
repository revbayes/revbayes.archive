//
//  HistoryCtmcSamplePath.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "SamplePathHistoryCtmcMove.h"
#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include "BiogeographicTreeHistoryCtmc.h"
#include "StandardState.h"
#include "TimeTree.h"

using namespace RevBayesCore;

SamplePathHistoryCtmcMove::SamplePathHistoryCtmcMove( StochasticNode<AbstractCharacterData> *n, double l, bool tuning, double w) :
    SimpleMove(n, w, tuning),
    variable(n),
    storedValue(0,0,0),
    lambda(l)
{
    numCharacters = n->getValue().getNumberOfCharacters();
    numStates = static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates();
}

// Basic utility functions
SamplePathHistoryCtmcMove* SamplePathHistoryCtmcMove::clone(void) const
{
    return new SamplePathHistoryCtmcMove( *this );
}

void SamplePathHistoryCtmcMove::swapNode(DagNode *oldN, DagNode *newN)
{
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AbstractCharacterData>* >( newN );
}

const std::string& SamplePathHistoryCtmcMove::getMoveName(void) const
{
    static std::string name = "SamplePathHistoryCtmcMove";
    return name;
}

double SamplePathHistoryCtmcMove::performSimpleMove(void)
{
    
    storedIndex = GLOBAL_RNG->uniform01() * numNodes;
//    const BranchHistory& bh;
    
//    static_cast< BiogeographicTreeHistoryCtmc<StandardState, TimeTree> >(variable->getDistribution()).getHistory(storedIndex);
//    static_cast< BiogeographicTreeHistoryCtmc<StandardState, TimeTree> >( AbstractTreeHistoryCtmc<StandardState,TimeTree>(variable->getDistribution()) ).getHistory(storedIndex);
    
    //variable->getDistribution().
    
    //storedValue = BranchHistory()
    // store old history
    //storedValue = BranchHistory(variable->getValue());
    //storedValue = variable->getValue();
    
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
//    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >( &variable->getDistribution() );
    
    //std::cout << "OLD P\n";
    //p->getValue().print();
    //std::cout << "OLD P (stored)\n";
    //storedValue.print();
    
//    double lnProposalBwd = p->computeLnProposal();
//    p->samplePath(updateSet);
//    double lnProposalFwd = p->computeLnProposal();
    
    //std::cout << "NEW P (stored)\n";
    //storedValue.print();
    //std::cout << "NEW P\n";
    //p->getValue().print();
    
    //return 0.0;
//    return lnProposalBwd - lnProposalFwd;
    return 0.0;
}

void SamplePathHistoryCtmcMove::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}

void SamplePathHistoryCtmcMove::rejectSimpleMove(void)
{
    // variable->setValue( new BranchHistory(storedValue) );
    //std::cout << "REJECT PATH\n";
}

void SamplePathHistoryCtmcMove::acceptSimpleMove(void)
{
    //std::cout << "ACCEPT PATH\n";
}

void SamplePathHistoryCtmcMove::tune(void)
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