#include "BinomialDistribution.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

BinomialDistribution::BinomialDistribution(const TypedDagNode<int> *m, const TypedDagNode<double> *q) : TypedDistribution<int>( new int( 0 ) ), n( m ), p( q ) {
    
    *value = RbStatistics::Binomial::rv(n->getValue(), p->getValue(), *GLOBAL_RNG);
}


BinomialDistribution::BinomialDistribution(const BinomialDistribution &d) : TypedDistribution<int>( d ), n( d.n ), p( d.p ) {
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


/** Get the parameters of the distribution */
std::set<const DagNode*> BinomialDistribution::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( p );
    parameters.insert( n );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void BinomialDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == p) {
        p = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == n) {
        n = static_cast<const TypedDagNode<int>* >( newP );
    }
}


