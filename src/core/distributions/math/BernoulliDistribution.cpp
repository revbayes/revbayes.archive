#include "BernoulliDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

using namespace RevBayesCore;

BernoulliDistribution::BernoulliDistribution(const TypedDagNode<double> *q) : TypedDistribution<int>( new int( 0 ) ), p( q ) {
    // add the lambda parameter as a parent
    addParameter( p );
    
    double u = GLOBAL_RNG->uniform01();
    
    *value = u > p->getValue() ? 0 : 1;
}


BernoulliDistribution::BernoulliDistribution(const BernoulliDistribution &d) : TypedDistribution<int>( d ), p( d.p ) {
    // parameters are automatically copied
}


BernoulliDistribution::~BernoulliDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BernoulliDistribution* BernoulliDistribution::clone( void ) const {
    
    return new BernoulliDistribution( *this );
}


double BernoulliDistribution::computeLnProbability( void ) {
    
    return log( *value == 0 ? ( 1 - p->getValue() ) : p->getValue() );
}



void BernoulliDistribution::redrawValue( void ) {
    
    double u = GLOBAL_RNG->uniform01();
    *value = u > p->getValue() ? 0 : 1;
    
}


void BernoulliDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == p) 
    {
        p = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}


