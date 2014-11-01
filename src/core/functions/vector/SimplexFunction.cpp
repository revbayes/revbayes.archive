//
//  SimplexFunction.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SimplexFunction.h"
#include "TypedDagNode.h"

using namespace RevBayesCore;

SimplexFunction::SimplexFunction(const std::vector<const TypedDagNode<double> *> &args) : TypedFunction< RbVector<double> >( new RbVector<double>() ),
    parameters( args )
{
    // add the lambda parameter as a parent
    std::vector<const TypedDagNode<double>* >::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it) {
        this->addParameter( *it );
    }
    
    update();
}


SimplexFunction::~SimplexFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



SimplexFunction* SimplexFunction::clone( void ) const {
    return new SimplexFunction( *this );
}


void SimplexFunction::update( void ) {
    
    // empty current simplex
    value->clear();
    
    double sum = 0.0;
    std::vector<const TypedDagNode<double>* >::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it) {
        sum += (*it)->getValue();
    }
    for (it = parameters.begin(); it != parameters.end(); ++it) {
        value->push_back( (*it)->getValue() / sum );
    }
}



void SimplexFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (oldP == parameters[i]) {
            parameters[i] = static_cast<const TypedDagNode<double>* >( newP );
            // we can jump out of the loop now
            break;
        }
    }
    
}
