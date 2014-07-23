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
    df = static_cast<StochasticNode<int>* > (n[3]);    
    
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
    A.touch();
    A.update();

    // calculate old posterior for sigma based on current process
    double logs1 = RbStatistics::InverseWishart::lnPdf(A,nnode + df->getValue(),sigma->getValue());
    MatrixReal& v = process->getValue();

    if (rng->uniform01() > 0.5) {

        // const TypedDagNode< TimeTree >* tau = variable->getValue().getTree();
        const TimeTree* tau = process->getValue().getTimeTree();

        // choose an index
        size_t component = size_t(rng->uniform01() * v.getNumberOfColumns());

        double u = rng->uniform01();
        double slidingFactor = lambda * 0.2 * (u - 0.5);
        recursiveTranslate(tau->getRoot(), component, slidingFactor);
       
    }
    else    {
        // move process
        size_t component = size_t(rng->uniform01() * v.getNumberOfColumns());
        size_t node = size_t(rng->uniform01() * v.getNumberOfRows());

        double u = rng->uniform01();
        double slidingFactor = lambda * (u - 0.5);

        if (!process->getValue().isClamped(node, component)) {
            v[node][component] += slidingFactor;
        }
    }
    
    // re-calculate sufficient statistics based on new process
    nnode = 0;
    PrecisionMatrix B = process->getValue().getBranchContrasts(nnode);    
    for (size_t i=0; i<dim; i++)    {
        B[i][i] += kappa->getValue();
    }
    B.touch();
    B.update();
    
    sigma->getValue().touch();

    // resample sigma based on new sufficient statistics
    sigma->getValue() = RbStatistics::InverseWishart::rv(B,nnode + df->getValue(),*rng);

    sigma->getValue().update();

    // calculate posterior for sigma based on current process
    double logs2 = RbStatistics::InverseWishart::lnPdf(B,nnode + df->getValue(),sigma->getValue());

    // log hastings ratio
    return logs1 - logs2;

}


void BrownianInverseWishartSlidingMove::recursiveTranslate(const TopologyNode& from, int component, double slide)   {

    MatrixReal& value = process->getValue();
    size_t index = from.getIndex();
    if (! process->getValue().isClamped(index,component))   {
        value[index][component] += slide;
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveTranslate(from.getChild(i), component, slide);
    }
    
}


void BrownianInverseWishartSlidingMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void BrownianInverseWishartSlidingMove::rejectCompoundMove( void ) {
    	
    process->getValue() = bkprocess;
    sigma->getValue().touch();
    sigma->getValue() = bksigma;
    sigma->getValue().update();
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
    if(oldN == df)
		df = static_cast<StochasticNode<int>* >( newN ) ;
}


void BrownianInverseWishartSlidingMove::tune( void )
{
//    double rate = numAccepted / double(numTried);
    
    // do nothing right now
}


