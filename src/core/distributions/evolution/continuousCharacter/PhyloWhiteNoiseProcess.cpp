#include "PhyloWhiteNoiseProcess.h"
#include "DistributionGamma.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <cmath>
#include <iostream>



using namespace RevBayesCore;



// constructor(s)
PhyloWhiteNoiseProcess::PhyloWhiteNoiseProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s): TypedDistribution< RbVector< double > >( new RbVector< double >(t->getValue().getNumberOfNodes() - 1, 0.0 ) ),
        tau( t ), 
        sigma( s )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( sigma );
    
    simulate();
}


PhyloWhiteNoiseProcess* PhyloWhiteNoiseProcess::clone(void) const {
    return new PhyloWhiteNoiseProcess( *this );
}


double PhyloWhiteNoiseProcess::computeLnProbability(void) {
  
    return recursiveLnProb(tau->getValue().getRoot());
}

double PhyloWhiteNoiseProcess::recursiveLnProb(const TopologyNode &from)   {

    double lnProb = 0.0;
    if (! from.isRoot())   {
        // compute the variance
        double mean = 1.0;
        double stdev = sigma->getValue() / sqrt(from.getBranchLength());
        double alpha = mean * mean / (stdev * stdev);
        double beta = mean / (stdev * stdev);
        double v = (*value)[from.getIndex()];
        lnProb += log( RbStatistics::Gamma::lnPdf(alpha,beta,v) );
    }
    
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = from.getChild(i);
        lnProb += recursiveLnProb(child);
            
    }
    
    return lnProb;
}

void PhyloWhiteNoiseProcess::redrawValue(void)
{
    simulate();
}


/** Swap a parameter of the distribution */
void PhyloWhiteNoiseProcess::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    
    if ( oldP == tau )
    {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma )
    {
        sigma = static_cast< const TypedDagNode<double> * >( newP );
    }
    
}


void PhyloWhiteNoiseProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
    
}


void PhyloWhiteNoiseProcess::recursiveSimulate(const TopologyNode& from) {
    
    if (! from.isRoot())    {
        
    // get the index
    size_t index = from.getIndex();
    
    // compute the variance along the branch
    double mean = 1.0;
    double stdev = sigma->getValue() / sqrt(from.getBranchLength());
    double alpha = mean * mean / (stdev * stdev);
    double beta = mean / (stdev * stdev);
    
    // simulate a new Val
    RandomNumberGenerator* rng = GLOBAL_RNG;
    double v = RbStatistics::Gamma::rv( alpha,beta, *rng);
    
    // we store this val here
    (*value)[index] = v;
    
    }
    
    // simulate the val for each child (if any)
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = from.getChild(i);
        recursiveSimulate(child);
    }
}



/*
 void PhyloWhiteNoiseProcess::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
 // only delegate when the toucher was the root val
 //    if ( affecter == rootVal )
 //        this->dagNode->getAffectedNodes( affected );
 }
 
 
 void PhyloWhiteNoiseProcess::keepSpecialization( DagNode* affecter ) {
 // only do this when the toucher was the root val
 //    if ( affecter == rootVal )
 //        this->dagNode->keepAffected();
 }
 */

/*
void PhyloWhiteNoiseProcess::restoreSpecialization( DagNode *restorer ) {
    // only do this when the toucher was our parameters
//    if ( restorer == rootVal ) {
        // get the index of the root
//        size_t rootIndex = tau->getValue().getRoot().getIndex();
//        (*value)[rootIndex] = rootVal->getValue();
        
        // delegate a restore to our children
//        this->dagNode->restoreAffected();
//    }
}

*/


//void PhyloWhiteNoiseProcess::touchSpecialization( DagNode *toucher, bool touchAll ) {
    // if the root val has changed, then we need to change the corresponding value in our vector and downpass the touch
/*
 if ( rootVal == toucher ) {
        
        // get the index of the root
        size_t rootIndex = tau->getValue().getRoot().getIndex();
        (*value)[rootIndex] = rootVal->getValue();
        
        // we notify our DAG node which element has changed
        dagNode->addTouchedElementIndex( rootIndex );
        
        // delegate a touch to our children
        this->dagNode->touchAffected();
    }
*/
//}


