/* 
 * File:   BrownianInverseWishartSlidingMove.cpp
 * Author: nl
 * 
 * Created on 23 juillet 2014, 16:48
 */

//
//  BrownianInverseWishartSlidingMove.cpp


#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "BrownianInverseWishartSlidingMove.h"

#include "DistributionInverseWishart.h"

#include <cmath>

using namespace RevBayesCore;

BrownianInverseWishartSlidingMove::BrownianInverseWishartSlidingMove( std::vector<DagNode*> n, double l, bool t, double w) : CompoundMove( n, w, t), lambda( l ) {
    
    process = static_cast<StochasticNode<MultivariatePhyloProcess>* > (n[0]);
    sigma = static_cast<StochasticNode<PrecisionMatrix>* > (n[1]);    
    kappa = static_cast<StochasticNode<double>* > (n[2]);    
    
}



/* Clone object */
BrownianInverseWishartSlidingMove* BrownianInverseWishartSlidingMove::clone( void ) const {
    
    return new BrownianInverseWishartSlidingMove( *this );
}



const std::string& BrownianInverseWishartSlidingMove::getMoveName( void ) const {
    static std::string name = "BrownianInverseWishartSlidingMove";
    
    return name;
}


/** Perform the move */
double BrownianInverseWishartSlidingMove::performCompoundMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    size_t dim = sigma->getValue().getDim();
    
    bkprocess = process->getValue();
    bksigma = sigma->getValue();

    // calculate sufficient statistics based on current process
    int nnode = 0;
    PrecisionMatrix A = process->getValue().getBranchContrasts(nnode);    
    for (size_t i=0; i<dim; i++)    {
        A[i][i] += kappa->getValue();
    }

    // calculate old posterior for sigma based on current process
    double logs1 = RbStatistics::InverseWishart::lnPdf(A,nnode,sigma->getValue());

    // move process
    MatrixReal& v = process->getValue();
    size_t component = size_t( rng->uniform01() * v.getNumberOfColumns() );
    size_t node = size_t( rng->uniform01() * v.getNumberOfRows() );
    
    double u = rng->uniform01();
    double slidingFactor = lambda * ( u - 0.5 );

    if (! process->getValue().isClamped(node,component))   {
        v[node][component] += slidingFactor;
    }

    // re-calculate sufficient statistics based on new process
    nnode = 0;
    PrecisionMatrix B = process->getValue().getBranchContrasts(nnode);    
    for (size_t i=0; i<dim; i++)    {
        B[i][i] += kappa->getValue();
    }
    
    // resample sigma based on new sufficient statistics
    sigma->getValue() = RbStatistics::InverseWishart::rv(B,nnode,*rng);

    // calculate posterior for sigma based on current process
    double logs2 = RbStatistics::InverseWishart::lnPdf(B,nnode,sigma->getValue());

    // log hastings ratio
    return logs1 - logs2;

}


void BrownianInverseWishartSlidingMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void BrownianInverseWishartSlidingMove::rejectCompoundMove( void ) {
    	
    process->getValue() = bkprocess;
    sigma->getValue() = bksigma;

}


void BrownianInverseWishartSlidingMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    CompoundMove::swapNode(oldN, newN);
    
    if(oldN == process)
		process = static_cast<StochasticNode<MultivariatePhyloProcess>* >(newN) ;
    if(oldN == sigma)
		sigma = static_cast<StochasticNode<PrecisionMatrix>* >( newN ) ;
    if(oldN == kappa)
		kappa = static_cast<StochasticNode<double>* >( newN ) ;
}


void BrownianInverseWishartSlidingMove::tune( void )
{
//    double rate = numAccepted / double(numTried);
    
    // do nothing right now
}


