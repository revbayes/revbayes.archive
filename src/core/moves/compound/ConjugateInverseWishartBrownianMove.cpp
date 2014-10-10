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
#include "TimeTree.h"
#include "DistributionInverseWishart.h"

#include <cmath>

using namespace RevBayesCore;

// ConjugateInverseWishartBrownianMove::ConjugateInverseWishartBrownianMove(StochasticNode<MatrixRealSymmetric>* s, StochasticNode<MultivariateRealNodeContainer>* p, StochasticNode<TimeTree>* t, TypedDagNode<double>* k, TypedDagNode<int>* d, double w) : CompoundMove(std::vector<DagNode*>(),w,false)    {
ConjugateInverseWishartBrownianMove::ConjugateInverseWishartBrownianMove(StochasticNode<MatrixRealSymmetric>* s, StochasticNode<MultivariateRealNodeContainer>* p, TypedDagNode<double>* k, TypedDagNode<int>* d, double w) : CompoundMove(std::vector<DagNode*>(),w,false)    {

    process = p;
//    tau = t;
    sigma = s;
    kappa = k;
    df = d;
    
    nodes.insert( process );
    nodes.insert( sigma );
    nodes.insert( kappa );
    nodes.insert( df );
//    nodes.insert( tau );
}

ConjugateInverseWishartBrownianMove* ConjugateInverseWishartBrownianMove::clone( void ) const {
    
    return new ConjugateInverseWishartBrownianMove( *this );
}


const std::string& ConjugateInverseWishartBrownianMove::getMoveName( void ) const {
    static std::string name = "ConjugateInverseWishartBrownianMove";
    
    return name;
}


double ConjugateInverseWishartBrownianMove::performCompoundMove( void ) {
       
    // std::cerr << "perform move\n";
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    size_t dim = sigma->getValue().getDim();
    
    bksigma = sigma->getValue();

    // calculate sufficient statistics based on current process
    int nnode = 0;
    MatrixRealSymmetric A = process->getValue().getBranchContrasts(nnode);    
    for (size_t i=0; i<dim; i++)    {
        A[i][i] += kappa->getValue();
    }
    A.touch();
    A.update();

    // calculate old posterior for sigma based on current process
    double logs1 = RbStatistics::InverseWishart::lnPdf(A,nnode + df->getValue(),sigma->getValue());

    sigma->getValue().touch();

    // resample sigma based on new sufficient statistics
    sigma->setValue( RbStatistics::InverseWishart::rv(A,nnode + df->getValue(),*rng) );
    
    sigma->getValue().update();

    // calculate posterior for sigma based on current process
    double logs2 = RbStatistics::InverseWishart::lnPdf(A,nnode + df->getValue(),sigma->getValue());
    
    // log hastings ratio
    // should cancel out the ratio of probabilities of the finak and initial configuration
    return logs1 - logs2;

}

void ConjugateInverseWishartBrownianMove::acceptCompoundMove()   {
}

void ConjugateInverseWishartBrownianMove::printParameterSummary(std::ostream &o) const {

}


void ConjugateInverseWishartBrownianMove::rejectCompoundMove( void ) {

    std::cerr << "in ConjugateInverseWishartBrownianMove: move should not be rejected??\n";
    exit(1);
//    sigma->getValue() = bksigma;
//    sigma->getValue().touch();
//    sigma->getValue().update();
}

void ConjugateInverseWishartBrownianMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    CompoundMove::swapNode(oldN, newN);
    if (oldN == sigma)
        sigma = static_cast<StochasticNode<MatrixRealSymmetric>*> (newN);
    if (oldN == process)
        process = static_cast<StochasticNode<MultivariateRealNodeContainer>*> (newN);
    if (oldN == kappa)
        kappa = static_cast<TypedDagNode<double>*> (newN);
    if (oldN == df)
        df = static_cast<TypedDagNode<int>*> (newN);    
//    if (oldN == tau)
//        tau = static_cast<StochasticNode<TimeTree>*> (newN);    
}

