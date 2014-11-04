//
//  MaxFunction.cpp
//  RevBayes
//
//  Created by Bastien on 02/11/14.
//  Copyright (c) 2014 Bastien Boussau. All rights reserved.
//

#include "MaxFunction.h"

using namespace RevBayesCore;

MaxFunction::MaxFunction(const TypedDagNode<RbVector<double> > *v) : TypedFunction<double>( new double(0.0) ), vals( v ) {
    // add the parameters as parents
    this->addParameter( vals );
    
    update();
}


MaxFunction::MaxFunction(const MaxFunction &n) : TypedFunction<double>( n ), vals( n.vals ) {
    // no need to add parameters, happens automatically
    
    update();
}


MaxFunction::~MaxFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



MaxFunction* MaxFunction::clone( void ) const {
    return new MaxFunction( *this );
}


void MaxFunction::update( void ) {
    
    const std::vector<double> &v = vals->getValue();
    double m = *(v.begin());
    if (v.size() > 1)
    {
        for ( std::vector<double>::const_iterator it = v.begin()+1; it != v.end(); ++it) {
            if (  *it > m) {
                m = *it;
            }
        }
    }
    *this->value = m ;
    
}



void MaxFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == vals ) {
        vals = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    
}

