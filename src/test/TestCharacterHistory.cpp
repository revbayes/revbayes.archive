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

using namespace RevBayesCore;

TestCharacterHistory::TestCharacterHistory(int gen) : mcmcGenerations( gen ){
    
}

TestCharacterHistory::~TestCharacterHistory() {
    // nothing to do
}

bool TestCharacterHistory::run( void ) {
    
    size_t numChars = 10;
    size_t numEvents = 5;
    size_t numStates =10;
    
    
    /////////////
    
    std::string labels = "0123456789";
    std::string stateStr = "";
    for (size_t i = 0; i < numChars; i++)
        stateStr += "0";
    
    StandardState egSs('a',"acgt");
    BranchHistory* bh = new BranchHistory(&egSs, numChars);
    
    std::multiset<CharacterEvent*,CharacterEventCompare> updateSet;
    std::set<CharacterEvent*> parentSet, childSet;
    std::set<size_t> indexSet;
    
    
    for (size_t i = 0; i < numChars; i++)
    {
        std::stringstream ss;
        ss << i;
        parentSet.insert(new CharacterEvent(new StandardState(ss.str(),labels),i,0.0));
    }
    
    for (size_t i = 0; i < numChars; i++)
    {
        std::stringstream ss;
        ss << numChars-i-1;
        childSet.insert(new CharacterEvent(new StandardState(ss.str(),labels),i,1.0));
    }
    
    for (size_t i = 0; i < numEvents; i++)
    {
        std::stringstream ss;
        ss << i;
        updateSet.insert(new CharacterEvent(new StandardState(ss.str(),labels), 2*i, 0.9-0.1*i));
    }
    
    for (size_t i = 0; i < numChars; i++)
    {
        indexSet.insert(i);
    }
    
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
    
    const DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );

    std::vector<AbstractCharacterHistoryRateModifier*> rateMods;
    
    AreaHistoryCtmc* ahc = new AreaHistoryCtmc(bh, q, numChars, labels, rateMods);
    
    return true;
}
