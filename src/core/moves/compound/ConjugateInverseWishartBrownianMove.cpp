/* 
 * File:   ConjugateInverseWishartBrownianMove.cpp
 * Author: nl
 * 
 * Created on 23 juillet 2014, 16:48
 */

//
//  ConjugateInverseWishartBrownianMove.cpp


#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "ConjugateInverseWishartBrownianMove.h"

#include "DistributionInverseWishart.h"

#include <cmath>

using namespace RevBayesCore;

ConjugateInverseWishartBrownianMove::ConjugateInverseWishartBrownianMove(StochasticNode<PrecisionMatrix>* s, StochasticNode<MultivariatePhyloProcess>* p, TypedDagNode<double>* k, TypedDagNode<int>* d, double w) : SimpleMove(s,w,false)    {

    process = p;
    sigma = s;
    kappa = k;
    df = d;
}

ConjugateInverseWishartBrownianMove::ConjugateInverseWishartBrownianMove(ConjugateInverseWishartBrownianMove* from) : SimpleMove(from->sigma, from->weight, false) {

    process = from->process;
    sigma = from->sigma;
    kappa = from->kappa;
    df = from->df;    
}

ConjugateInverseWishartBrownianMove* ConjugateInverseWishartBrownianMove::clone( void ) const {
    
    return new ConjugateInverseWishartBrownianMove( *this );
}


const std::string& ConjugateInverseWishartBrownianMove::getMoveName( void ) const {
    static std::string name = "ConjugateInverseWishartBrownianMove";
    
    return name;
}


double ConjugateInverseWishartBrownianMove::performSimpleMove( void ) {
       
    // std::cerr << "perform move\n";
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    size_t dim = sigma->getValue().getDim();
    
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
        
    sigma->getValue().touch();

    // resample sigma based on new sufficient statistics
    sigma->getValue() = RbStatistics::InverseWishart::rv(A,nnode + df->getValue(),*rng);

    sigma->getValue().update();

    // calculate posterior for sigma based on current process
    double logs2 = RbStatistics::InverseWishart::lnPdf(A,nnode + df->getValue(),sigma->getValue());

//    std::cerr << logs1 - logs2 << '\n';
    // log hastings ratio
    // should cancel out the ratio of probabilities of the finak and initial configuration
    return logs1 - logs2;

}

void ConjugateInverseWishartBrownianMove::acceptSimpleMove()   {
//    std::cerr << "move accepted\n";
}

void ConjugateInverseWishartBrownianMove::printParameterSummary(std::ostream &o) const {

}


void ConjugateInverseWishartBrownianMove::rejectSimpleMove( void ) {

    std::cerr << "move rejected??\n";
    exit(1);
    sigma->getValue() = bksigma;
    sigma->getValue().touch();
    sigma->getValue().update();
}

void ConjugateInverseWishartBrownianMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    if (oldN == sigma)
        sigma = static_cast<StochasticNode<PrecisionMatrix>*> (newN);
    if (oldN == process)
        process = static_cast<StochasticNode<MultivariatePhyloProcess>*> (newN);
    if (oldN == kappa)
        kappa = static_cast<StochasticNode<double>*> (newN);
    if (oldN == df)
        df = static_cast<StochasticNode<int>*> (newN);    
}

