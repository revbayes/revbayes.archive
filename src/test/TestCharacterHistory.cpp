//
//  TestCharacterHistory.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TestCharacterHistory.h"

#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "Model.h"
#include "Move.h"
#include "StochasticNode.h"
#include "TestCharacterHistory.h"

#include "AreaHistoryCtmc.h"
#include "GeographicDistanceRateModifier.h"
#include "BranchHistory.h"
#include "CharacterEvent.h"
#include "StandardState.h"
#include "RateMatrix_GTR.h"
#include "DirichletDistribution.h"
#include "BetaDistribution.h"
#include "GtrRateMatrixFunction.h"
#include "DeterministicNode.h"
#include "VectorFunction.h"

using namespace RevBayesCore;

TestCharacterHistory::TestCharacterHistory(int gen) : mcmcGenerations( gen ){
    
}

TestCharacterHistory::~TestCharacterHistory() {
    // nothing to do
}

bool TestCharacterHistory::run( void ) {
    
    size_t numChars = 9;
    size_t numStates = 2;
    
    //////////
    
    BranchHistory* bh = new BranchHistory(numStates, numChars);
    
    std::multiset<CharacterEvent*,CharacterEventCompare> updateSet;
    std::set<CharacterEvent*> parentSet, childSet;
    std::set<size_t> indexSet;
    
    
    for (size_t i = 0; i < numChars; i++)
        parentSet.insert(new CharacterEvent(i, 0, 0.0));
    
    for (size_t i = 0; i < numChars; i++)
        childSet.insert(new CharacterEvent(i, 1, 1.0));
    
    double dt = 1.0 / (numChars+1);
    for (size_t i = 0; i < numChars; i++)
    {
        updateSet.insert(new CharacterEvent(i, 1, dt*(i+1)));
    }
    
    for (size_t i = 0; i < numChars; i++)
        indexSet.insert(i);
    
    bh->updateHistory(updateSet, parentSet, childSet, indexSet);
    bh->print();
  
    // GTR
    // gtr model priors
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(2,1.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(2,1.0) );
 
    
    
  //  std::cout << "bf:\t" << bf->getValue() << std::endl;
//    std::cout << "e:\t" << e->getValue() << std::endl;
    
    // then the parameters
    StochasticNode<std::vector<double> > *pi = new StochasticNode<std::vector<double> >( "pi", new DirichletDistribution(bf) );
    StochasticNode<std::vector<double> > *er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) );
    
    
    //    pi->setValue( new std::vector<double>(4,1.0/4.0) );
    //    er->setValue( new std::vector<double>(6,1.0/6.0) );
    
    std::cout << "pi:\t" << pi->getValue() << std::endl;
    std::cout << "er:\t" << er->getValue() << std::endl;
    
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );

    // rate modifier functions ... add distance function here
    std::vector<const TypedDagNode<AbstractCharacterHistoryRateModifier*> *> rateMods;
    //DeterministicNode* geo_dist = new DeterministicNode("geo_dist", new LognormalDistribution(branchRateC, branchRateD));
    //rateModes.push_back(new Stoc)
    
    DeterministicNode< std::vector<AbstractCharacterHistoryRateModifier*> >* rm_vector = new DeterministicNode< std::vector< AbstractCharacterHistoryRateModifier* > >( "rm_vector", new VectorFunction< AbstractCharacterHistoryRateModifier* >( rateMods ) );



    AreaHistoryCtmc* ahc = new AreaHistoryCtmc(bh, q, numStates, numChars, rm_vector);
    StochasticNode<BranchHistory>* bh_rv = new StochasticNode<BranchHistory>( "bh_rv", ahc);
    
    std::cout << bh_rv->getLnProbability() << "\n";
    std::cout << "lnL\t" << ahc->computeLnProbability() << "\n";

    
    return true;
}
