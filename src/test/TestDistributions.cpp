//
//  TestDistributions.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ConstantNode.h"
#include "TestDistributions.h"

#include "DistributionBeta.h"

using namespace RevBayesCore;

TestDistributions::TestDistributions() {
    
}


TestDistributions::~TestDistributions() {
    
}


bool TestDistributions::run(void) {
    std::vector<double> q;
    for (int i = 0; i <= 10; ++i) {
        q.push_back( i / 10.0 );
    }
    std::vector<double> x;
    for (int i = -10; i <= 10; ++i) {
        x.push_back( i / 10.0 );
    }
    
    std::vector<double> quantiles(q.size());
    for (size_t i = 0; i < q.size(); ++i) {
        quantiles[i] = RbStatistics::Beta::quantile(0.3, 1.0, q[i]);
    }
    std::cout << "R:\t\t\t\t(0.000, 0.0004641589 0.0046784284 0.0180746897 0.0471556032 0.0992125657 0.1821814557 0.3045510726 0.4752986966 0.7038417614 1.0)" << std::endl;
    std::cout << "RevBayes:\t\t" << quantiles << std::endl;
    
    
    std::vector<double> densities_pos(q.size());
    for (size_t i = 0; i < q.size(); ++i) {
        densities_pos[i] = RbStatistics::Beta::pdf(0.3, 1.0, q[i]);
    }
    std::cout << "R:\t\t\t\t(Inf 1.5035617 0.9255508 0.6968453 0.5697433 0.4873514 0.4289586 0.3850815 0.3507182 0.3229621 0.3)" << std::endl;
    std::cout << "RevBayes:\t\t" << densities_pos << std::endl;
    
    return true;
}
