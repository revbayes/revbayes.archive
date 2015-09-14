//
//  PowermixFunction.cpp
//  RevBayes
//
//  Created by Fredrik Ronquist on 2014-03-07.
//  Copyright (c) 2014 Swedish Museum of Natural History. All rights reserved.
//

#include "PowermixFunction.h"

#include <cmath>

using namespace RevBayesCore;

PowermixFunction::PowermixFunction(const std::vector<const TypedDagNode< RbVector<double> >* > &args) : TypedFunction< RbVector<double> >( new RbVector<double>() ), parameters( args ) {
    // add the parameters as parents
    std::vector<const TypedDagNode< RbVector<double> >* >::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it) {
        this->addParameter( *it );
    }
    
    update();
}


PowermixFunction::~PowermixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



PowermixFunction* PowermixFunction::clone( void ) const {
    return new PowermixFunction( *this );
}


void PowermixFunction::update( void ) {
    
    // empty current simplex
    value->clear();
    
    const std::vector<double>& a = parameters[0]->getValue();
    const std::vector<double>& b = parameters[1]->getValue();
    const std::vector<double>& c = parameters[2]->getValue();
    std::vector<double>  x;

    for ( size_t i = 0; i < a.size(); ++i )
        x.push_back( std::pow(a[i],c[0]) * std::pow(b[i], c[1]) );
    
    double sum = 0.0;
    for ( size_t i = 0; i < x.size(); ++i )
        sum += x[i];

    for ( size_t i = 0; i < x.size(); ++i ) {
        value->push_back( x[i] / sum );
    }
}



void PowermixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (oldP == parameters[i]) {
            parameters[i] = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
            // we can jump out of the loop now
            break;
        }
    }
    
}
