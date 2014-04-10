#include "BinomialDistribution.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

BinomialDistribution::BinomialDistribution(const TypedDagNode<int> *m, const TypedDagNode<double> *q) : TypedDistribution<int>( new int( 0 ) ), n( m ), p( q ) {
    // add the lambda parameter as a parent
    addParameter( p );
    addParameter( n );
    
    *value = RbStatistics::Binomial::rv(n->getValue(), p->getValue(), *GLOBAL_RNG);
}


BinomialDistribution::BinomialDistribution(const BinomialDistribution &d) : TypedDistribution<int>( d ), n( d.n ), p( d.p ) {
    // parameters are automatically copied
}


BinomialDistribution::~BinomialDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BinomialDistribution* BinomialDistribution::clone( void ) const {
    return new BinomialDistribution( *this );
}


double BinomialDistribution::computeLnProbability( void ) {
    return RbStatistics::Binomial::lnPdf(n->getValue(), p->getValue(), *value);
}



void BinomialDistribution::redrawValue( void ) {
    *value = RbStatistics::Binomial::rv(n->getValue(), p->getValue(), *GLOBAL_RNG);
}


void BinomialDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == p) {
        p = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == n) {
        n = static_cast<const TypedDagNode<int>* >( newP );
    }
}


