//
//  TestFreeK.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 10/9/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "TestFreeK.h"

#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "ExponentialDistribution.h"
#include "FreeKRateMatrixFunction.h"
#include "RateMatrix.h"
#include "RateMatrix_FreeK.h"
#include "StochasticNode.h"
#include "TransitionProbabilityMatrix.h"

#include <vector>

using namespace RevBayesCore;

TestFreeK::TestFreeK(void)
{
    ;
}

TestFreeK::~TestFreeK() {
    // nothing to do
}

bool TestFreeK::run(void)
{
    size_t ns = 4;
    ConstantNode<std::vector<double> >* pi_pr = new ConstantNode<std::vector<double> >("pi_pr", new std::vector<double>(ns, 1.0) );
    ConstantNode<std::vector<double> >* tr_pr = new ConstantNode<std::vector<double> >("tr_pr", new std::vector<double>(ns*ns-ns, 1.0) );

    StochasticNode<std::vector<double> >* pi = new StochasticNode<std::vector<double> >("pi", new DirichletDistribution(pi_pr));
    StochasticNode<std::vector<double> >* tr = new StochasticNode<std::vector<double> >("tr", new DirichletDistribution(tr_pr));
    
    tr->setValue(std::vector<double>(ns*ns-ns, 1.0));
    pi->setValue(std::vector<double>(ns,1.0/ns));
    
    
    double v[12] = { 2.1, 2.0, 2.0, 0.1, 2.2, 1.7, 1.5, 0.5, 1.0, 1.0, 1.0, 1.0  };
    tr->setValue(std::vector<double>(v, v + sizeof(v) / sizeof(double)));

    DeterministicNode<RateMatrix>* q = new DeterministicNode<RateMatrix>("Q", new FreeKRateMatrixFunction(tr, pi));
    
    std::cout << "rate matrix\n";
    std::cout << q->getValue() << "\n";
    
    TransitionProbabilityMatrix tp(ns);
    q->getValue().calculateTransitionProbabilities(1.0, tp);
    
    std::cout << "transition probs\n";
    std::cout << tp << "\n";
    
    
    return true;
}

