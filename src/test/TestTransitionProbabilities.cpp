//
//  TestTransitionProbabilities.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/25/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "GtrRateMatrix.h"
#include "RateMatrix_F81.h"
#include "RateMatrix_GTR.h"
#include "RateMatrix_HKY.h"
#include "RateMatrix_JC.h"
#include "TestTransitionProbabilities.h"
#include "TransitionProbabilityMatrix.h"

#include <iostream>

using namespace RevBayesCore;


TestTransitionProbabilities::TestTransitionProbabilities() {
    
}


TestTransitionProbabilities::~TestTransitionProbabilities() {
    
}


bool TestTransitionProbabilities::run( void ) {
    
    double t = 10.0;
    
    size_t nStates= 4;
    
    TransitionProbabilityMatrix tpm(nStates);
    RateMatrix_F81 f81(nStates);
    RateMatrix_GTR gtr(nStates);
    RateMatrix_HKY hky;
    RateMatrix_JC jc(nStates);
    
    // initialize rate matrices
    gtr.update();
    gtr.updateEigenSystem();
    
    // JC examples
    
    f81.calculateTransitionProbabilities(t, tpm);
    std::cout << "F81:\n" << tpm << std::endl;
    
    gtr.calculateTransitionProbabilities(t, tpm);
    std::cout << "GTR:\n" << tpm << std::endl;
    
    hky.calculateTransitionProbabilities(t, tpm);
    std::cout << "HKY:\n" << tpm << std::endl;
    
    jc.calculateTransitionProbabilities(t, tpm);
    std::cout << "JC:\n" << tpm << std::endl;
    
    
    // F81 example
    std::vector<double> pi;
    pi.push_back(0.1);
    pi.push_back(0.2);
    pi.push_back(0.3);
    pi.push_back(0.4);
    f81.setStationaryFrequencies( pi );
    gtr.setStationaryFrequencies( pi );
    hky.setStationaryFrequencies( pi );
    
    std::vector<double> r(6,1.0);
    
    // update rate matrices
    // delegate the call to our internal function
    RbPhylogenetics::Gtr::computeRateMatrix(r, pi, &gtr );
    
    f81.calculateTransitionProbabilities(t, tpm);
    std::cout << "F81:\n" << tpm << std::endl;
    
    gtr.calculateTransitionProbabilities(t, tpm);
    std::cout << "GTR:\n" << tpm << std::endl;
    
    hky.calculateTransitionProbabilities(t, tpm);
    std::cout << "HKY:\n" << tpm << std::endl;
    
    
    return true;
}


