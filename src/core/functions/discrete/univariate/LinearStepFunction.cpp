//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "LinearStepFunction.h"
#include <cmath>

using namespace RevBayesCore;

LinearStepFunction::LinearStepFunction(const TypedDagNode<double> *x, const std::vector<double> &v, double l, double u) : StepFunction( new double(0.0), x, v ), from( l ), to( u ) {
    this->addParameter( x );

}


LinearStepFunction* LinearStepFunction::clone( void ) const {
    return new LinearStepFunction(*this);
}

double LinearStepFunction::evaluate(double x) const {
    
    if ( x >= to ) 
    {
        return values[values.size()-1];
    }
    else if ( x < from) 
    {
        return values[0];
    }
    else 
    {
        double intervalSize = (to - from) / (values.size()-1);
        int index = (int)( ( x - from ) / intervalSize );
        
        return values[index] + (values[index+1] - values[index])*( x - from - (index*intervalSize))/intervalSize;
    }
}



double LinearStepFunction::integrate(double l, double u, NumericalIntegration &integr) const {
    
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
            double c = values[startIndex-1] + (values[startIndex]-values[startIndex-1]) * ( l - from - intervalSize*(startIndex-1) )/intervalSize;
            a = (values[startIndex]+c)/2.0 * (from + intervalSize*startIndex - l);
        }
        if ( u > to ) {
            a += values[values.size()] * ( u - to );
        }
        else if ( from + intervalSize*endIndex != u) {
            double c = values[endIndex] + (values[endIndex+1]-values[endIndex]) * ( u - from - intervalSize*endIndex)/intervalSize;
            a += (c+values[endIndex])/2.0 * (u - from - intervalSize*endIndex);
        }
        
        for (int i=startIndex; i < endIndex; ++i) {
            a += (values[i]+values[i+1]) / 2.0 * intervalSize;
        }
        
    }
    else {
        // everything happened before the beginning of the function
        if ( startIndex == 0) {
            a = (u-l)*values[startIndex];
        }
        else if ( endIndex == int(values.size())-1) {
            a = (u-l)*values[endIndex];
        }
        else {
            double c = values[startIndex] + (values[startIndex+1]-values[startIndex]) * ( l - from - intervalSize*startIndex )/intervalSize;
            double d = values[endIndex] + (values[endIndex+1]-values[endIndex]) * ( u - from - intervalSize*endIndex)/intervalSize;
            a = (c+d)/2.0 * (u-l);
        }
    }
    
    return a;
}

void LinearStepFunction::update( void ) {
    
    if ( x->getValue() >= to ) 
    {
        *value = values[values.size()-1];
    }
    else if ( x->getValue() < from) 
    {
        *value = values[0];
    }
    else 
    {
        double intervalSize = (to - from) / (values.size()-1);
        int index = (int)( ( x->getValue() - from ) / intervalSize );
    
        *value = values[index] + (values[index+1] - values[index])*( x->getValue() - from - (index*intervalSize))/intervalSize;
    }
}


