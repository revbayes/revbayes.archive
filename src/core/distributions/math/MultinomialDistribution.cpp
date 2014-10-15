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

MultinomialDistribution::MultinomialDistribution(const TypedDagNode< RbVector<double> > *p, const TypedDagNode<int> *n) : TypedDistribution< RbVector<int> >( new RbVector<int>() ),
    p( p ),
    n( n )
{
        
    *value = RbStatistics::Multinomial::rv(p->getValue(), size_t( n->getValue() ), *GLOBAL_RNG);
}


MultinomialDistribution::MultinomialDistribution(const MultinomialDistribution &m) : TypedDistribution< RbVector<int> >( m ), p( m.p ), n (m.n) {
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
    *value = RbStatistics::Multinomial::rv(p->getValue(), size_t( n->getValue() ), *GLOBAL_RNG);
}


/** Get the parameters of the distribution */
std::set<const DagNode*> MultinomialDistribution::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( p );
    parameters.insert( n );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void MultinomialDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == p)
    {
        p = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == n)
    {
        n = static_cast<const TypedDagNode<int>* >(newP);
    }
}
