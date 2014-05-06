//
//  MultinomialDistribution.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/6/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "MultinomialDistribution.h"
#include "DistributionMultinomial.h"
#include "RandomNumberFactory.h"

using namespace RevBayesCore;

MultinomialDistribution::MultinomialDistribution(const TypedDagNode< std::vector<double> > *p, const TypedDagNode<int> *n) : TypedDistribution< std::vector<int> >( new std::vector<int>() ), p( p ), n( n ) {
    // add the lambda parameter as a parent
    addParameter( p );
    addParameter( n );
        
    *value = RbStatistics::Multinomial::rv(p->getValue(), n->getValue(), *GLOBAL_RNG);
}


MultinomialDistribution::MultinomialDistribution(const MultinomialDistribution &m) : TypedDistribution<std::vector<int> >( m ), p( m.p ), n (m.n) {
    // parameters are automatically copied
}


MultinomialDistribution::~MultinomialDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



MultinomialDistribution* MultinomialDistribution::clone( void ) const {
    return new MultinomialDistribution( *this );
}


double MultinomialDistribution::computeLnProbability( void ) {
    return RbStatistics::Multinomial::lnPdf(p->getValue(), *value);
}


void MultinomialDistribution::redrawValue( void ) {
    *value = RbStatistics::Multinomial::rv(p->getValue(), n->getValue(), *GLOBAL_RNG);
}


void MultinomialDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == p) {
        p = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    else if (oldP == n)
    {
        n = static_cast<const TypedDagNode<int>* >(newP);
    }
}
