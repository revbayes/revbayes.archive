//
//  GeneralUnivariateFunction.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 10/30/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GeneralUnivariateFunction.h"

using namespace RevBayesCore;


GeneralUnivariateFunction::GeneralUnivariateFunction(double *v, const TypedDagNode<double> *x, TypedDagNode<double> *result, const std::set<DagNode*> &n) : UnivariateFunction(v,x), nodes( n ), y(result)  {
    // We need to exchange the actual parameter by a dummy variable so that we can use the evaluate function
    parameter = new ConstantNode<double>("", new double(x->getValue()));
    const std::set<DagNode *> &x_children = x->getChildren();
    for (std::set<DagNode *>::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
        if ( x_children.find( *it ) != x_children.end() ) {
            (*it)->swapParent( x, parameter);
        }
    }
}


GeneralUnivariateFunction::GeneralUnivariateFunction(const GeneralUnivariateFunction &f) : UnivariateFunction(f) {
    
}


GeneralUnivariateFunction* GeneralUnivariateFunction::clone( void ) const {
    return new GeneralUnivariateFunction(*this);
}


double GeneralUnivariateFunction::evaluate(double t) const {
    
    // set x to the value for that we want to evaluate the function
    parameter->setValue(t);
    return y->getValue();
    
}


double GeneralUnivariateFunction::integrate(double l, double u, RevBayesCore::NumericalIntegration &i) const {
    return i.integrate(*this, l, u);
}


void GeneralUnivariateFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    // delegate the call
    UnivariateFunction::swapParameterInternal(oldP, newP);
}

void GeneralUnivariateFunction::update( void ) {
    
    parameter->setValue( x->getValue() );
    *value = y->getValue();
}



