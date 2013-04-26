//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ConstantStepFunction.h"
#include <cmath>

using namespace RevBayesCore;

ConstantStepFunction::ConstantStepFunction(const TypedDagNode<double> *x, const std::vector<double> &v, double l, double u) : StepFunction( new double(0.0), x, v ), from( l ), to( u ) {

}


ConstantStepFunction* ConstantStepFunction::clone( void ) const {
    return new ConstantStepFunction(*this);
}

double  ConstantStepFunction::evaluate(double x) const {
    
    if ( x >= to ) {
        return values[values.size()-1];
    }
    else if ( x < from) {
        return values[0];
    }
    else {
        double intervalSize = (to - from) / (values.size()-1);
        int index = (int)( ( x - from ) / intervalSize );
        
        return values[index];
    }
}



double ConstantStepFunction::integrate(double l, double u, NumericalIntegration &integr) const {
    
    double a = 0.0;
    
    double intervalSize = (to - from) / (values.size()-1);
    int startIndex = (int) fmax(0,( l - from ) / intervalSize );
    int endIndex = (int) fmin(values.size()-1,( u - from ) / intervalSize );
    if ( startIndex < endIndex ) {
        if ( l < from ) {
            a = values[0] * ( from - l );
        }
        else if ( from + intervalSize*startIndex != l) {
            startIndex++;
            a = values[startIndex-1] * ( from + intervalSize*startIndex - l);
        }
        if ( u > to ) {
            a += values[values.size()] * ( u - to );
        }
        else if ( from + intervalSize*endIndex != u) {
            a += values[endIndex] * ( u - from - intervalSize*endIndex);
        }
        
        for (int i=startIndex; i < endIndex; ++i) {
            a += values[i] * intervalSize;
        }
        
    }
    else {
        a = (u-l)*values[startIndex];
    }
    
    return a;
}


void ConstantStepFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == x) {
        x = static_cast<const TypedDagNode<double>* >( newP );
    }
}

void ConstantStepFunction::update( void ) {
    
    if ( x->getValue() >= to ) {
        *value = values[values.size()-1];
    }
    else if ( x->getValue() < from) {
        *value = values[0];
    }
    else {
        double intervalSize = (to - from) / (values.size()-1);
        int index = (int)( ( x->getValue() - from ) / intervalSize );
        
        *value = values[index];
    }
}


