/*
 * File:   BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess.cpp
 * Author: Bastien Boussau
 *
 * Created on 04/10/14
 */

#include "BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess.h"
#include "ConstantNode.h"
#include "DistributionNormal.h"
#include "GtrRateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <iostream>



using namespace RevBayesCore;


// constructor(s)
BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* m, const TypedDagNode< double >* p): TypedDistribution< RealNodeContainer >(new RealNodeContainer(&t->getValue())),
    tau( t ),
    homogeneousSigma( s ),
    homogeneousMean( m ),
    homogeneousPhi( p ) {
        heterogeneousSigma = NULL;
        heterogeneousMean = NULL;
        heterogeneousPhi = NULL;
        rootMean = NULL;
        rootStdDev = NULL;
//        simulate();
}


// constructor(s)
BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess(const TypedDagNode< TimeTree > *t): TypedDistribution< RealNodeContainer >(new RealNodeContainer(&t->getValue())),
tau( t ) {
    homogeneousSigma = NULL;
    homogeneousMean = NULL;
    homogeneousPhi = NULL;
    heterogeneousSigma = NULL;
    heterogeneousMean = NULL;
    heterogeneousPhi = NULL;
    rootMean = NULL;
    rootStdDev = NULL;
    
 //   simulate();
}


BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess(const BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess &n): TypedDistribution< RealNodeContainer >( n ), tau( n.tau ), homogeneousSigma( n.homogeneousSigma ), homogeneousMean(n.homogeneousMean), homogeneousPhi( n.homogeneousPhi ), heterogeneousSigma( n.heterogeneousSigma ), heterogeneousMean(n.heterogeneousMean), heterogeneousPhi( n.heterogeneousPhi ), rootMean(n.rootMean), rootStdDev(n.rootStdDev) {
    // nothing to do here since the parameters are copied automatically
    
}



BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess* BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::clone(void) const {
    return new BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess( *this );
}


double BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::computeLnProbability(void) {
    
    double ret = recursiveLnProb(tau->getValue().getRoot());
    return ret;
}


double BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::recursiveLnProb( const TopologyNode& from ) {
    
    double lnProb = 0.0;
    size_t index = from.getIndex();
    double val = (*value)[index];
    
    if (from.isRoot()) {
        
   /*     double m = mean->getValue();
        double standDev = sigma->getValue() / sqrt(2 * phi->getValue());
        
        lnProb += RbStatistics::Normal::lnPdf(val, standDev, m);*/
        double m = rootMean->getValue();
        double standDev = rootStdDev->getValue();
        lnProb += RbStatistics::Normal::lnPdf(val, standDev, m);
    }
    else    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        double upval = (*value)[from.getParent().getIndex()];
        double t = from.getBranchLength();
        if ( homogeneousPhi != NULL ) { // homogeneous
            double e = exp(-homogeneousPhi->getValue() * t);
            double e2 = exp(-2 * homogeneousPhi->getValue() * t);
            double m = e * upval + (1 - e) * homogeneousMean->getValue();
            double standDev = homogeneousSigma->getValue() * sqrt((1 - e2) / 2 / homogeneousPhi->getValue());
        
            lnProb += RbStatistics::Normal::lnPdf(val, standDev, m);
        }
        else { //heterogeneous
            size_t nodeIndex = from.getIndex();
            double e = exp(-heterogeneousPhi->getValue()[nodeIndex] * t);
            double e2 = exp(-2 * heterogeneousPhi->getValue()[nodeIndex] * t);
            double m = e * upval + (1 - e) * heterogeneousMean->getValue()[nodeIndex];
            double standDev = heterogeneousSigma->getValue()[nodeIndex] * sqrt((1 - e2) / 2 / heterogeneousPhi->getValue()[nodeIndex]);
            
            lnProb += RbStatistics::Normal::lnPdf(val, standDev, m);

        }
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        lnProb += recursiveLnProb(from.getChild(i));
    }
    
    return lnProb;
    
}

void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::redrawValue(void) {
    simulate();
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::recursiveSimulate(const TopologyNode& from)  {
    
    size_t index = from.getIndex();
    
    if (from.isRoot())    {
        
        // x ~ normal(mean, sigma^2 / 2 phi)
        
        double m = rootMean->getValue();
        double standDev = rootStdDev->getValue();
        
      /*  double m = mean->getValue();
        double standDev = sigma->getValue() / sqrt(2 * phi->getValue());
        */
        RandomNumberGenerator* rng = GLOBAL_RNG;
        (*value)[index] = RbStatistics::Normal::rv(m, standDev, *rng);
    }
    else    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        double upval = (*value)[from.getParent().getIndex()];
        double t = from.getBranchLength();
        double m ;
        double standDev ;
        if ( homogeneousPhi != NULL ) { // homogeneous
            double e = exp(-homogeneousPhi->getValue() * t);
            double e2 = exp(-2 * homogeneousPhi->getValue() * t);
            m = e * upval + (1 - e) * homogeneousMean->getValue();
            standDev = homogeneousSigma->getValue() * sqrt((1 - e2) / 2 / homogeneousPhi->getValue());
        }
        else {
            size_t nodeIndex = from.getIndex();
            double e = exp(-heterogeneousPhi->getValue()[nodeIndex] * t);
            double e2 = exp(-2 * heterogeneousPhi->getValue()[nodeIndex] * t);
            m = e * upval + (1 - e) * heterogeneousMean->getValue()[nodeIndex];
            standDev = heterogeneousSigma->getValue()[nodeIndex] * sqrt((1 - e2) / 2 / heterogeneousPhi->getValue()[nodeIndex]);
        }
        // simulate the new Val
        RandomNumberGenerator* rng = GLOBAL_RNG;
        (*value)[index] = RbStatistics::Normal::rv(m, standDev, *rng);
        
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveSimulate(from.getChild(i));
    }
    
}


/** Get the parameters of the distribution */
std::set<const DagNode*> BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( tau );
    parameters.insert( homogeneousSigma );
    parameters.insert( homogeneousMean );
    parameters.insert( homogeneousPhi );
    parameters.insert( homogeneousSigma );
    parameters.insert( homogeneousMean );
    parameters.insert( homogeneousPhi );
    parameters.insert( rootMean );
    parameters.insert( rootStdDev );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == homogeneousSigma ) {
        homogeneousSigma = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == homogeneousMean ) {
        homogeneousMean = static_cast< const TypedDagNode< double > * >( newP );
    }
    
    if ( oldP == homogeneousPhi ) {
        homogeneousPhi = static_cast< const TypedDagNode< double > * >( newP );
    }
    
    if ( oldP == heterogeneousSigma ) {
        heterogeneousSigma = static_cast< const TypedDagNode<std::vector< double > > * >( newP );
    }
    
    if ( oldP == heterogeneousMean ) {
        heterogeneousMean = static_cast< const TypedDagNode< std::vector< double > > * >( newP );
    }
    
    if ( oldP == heterogeneousPhi ) {
        heterogeneousPhi = static_cast< const TypedDagNode< std::vector< double > > * >( newP );
    }
    
    if ( oldP == rootMean ) {
        rootMean = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == rootStdDev ) {
        rootStdDev = static_cast< const TypedDagNode< double > * >( newP );
    }
    
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::setSigma( const TypedDagNode< double >* x) {
    homogeneousSigma = x;
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::setMean( const TypedDagNode< double >* x){
    homogeneousMean = x;
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::setPhi( const TypedDagNode< double >* x){
    homogeneousPhi = x;
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::setSigma( const TypedDagNode< std::vector< double > >* x){
    heterogeneousSigma = x;
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::setMean( const TypedDagNode< std::vector< double > >* x){
    heterogeneousMean = x;
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::setPhi( const TypedDagNode< std::vector< double > >* x){
    heterogeneousPhi = x;
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::setRootMean( const TypedDagNode< double >* x){
    rootMean = x;
}


void BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess::setRootStdDev( const TypedDagNode< double >* x){
    rootStdDev = x;
}





