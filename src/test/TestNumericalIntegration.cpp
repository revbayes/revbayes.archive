//
//  TestNumericalIntegration.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ConstantNode.h"
#include "Function.h"
#include "SimpsonsRule.h"
#include "TestNumericalIntegration.h"

#include <iostream>

using namespace RevBayesCore;

TestNumericalIntegration::TestNumericalIntegration() {
    
}

TestNumericalIntegration::~TestNumericalIntegration() {
    
}

bool TestNumericalIntegration::run() {
    
//    ConstantNode<Function>* val = new ConstantNode<Function>("", ValueFunction() );
//    ConstantNode<Function>* c = new ConstantNode<Function>( "", ConstantFunction(1.0) );
//    ConstantNode<Function>* f = new ConstantNode<Function>( "", BinaryDivision(c,val) );
//    
//    SimpsonsRule s;
//    double integral = s.integrate(f->getValue(), 1E-10, 100);
//    
//    std::cout << "Integral:\t\t" << integral << std::endl;
//    
//    delete f;
//    delete c;
//    delete val;
    
    return true;
}
