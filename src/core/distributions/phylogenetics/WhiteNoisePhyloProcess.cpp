#include "WhiteNoisePhyloProcess.h"
#include "DistributionGamma.h"
#include "GtrRateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <iostream>



using namespace RevBayesCore;



// constructor(s)
WhiteNoisePhyloProcess::WhiteNoisePhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s): TypedDistribution< RbVector< double > >( new RbVector< double >(t->getValue().getNumberOfNodes() - 1, 0.0 ) ),
        tau( t ), 
        sigma( s )
{
    simulate();
}


WhiteNoisePhyloProcess* WhiteNoisePhyloProcess::clone(void) const {
    return new WhiteNoisePhyloProcess( *this );
}


double WhiteNoisePhyloProcess::computeLnProbability(void) {
  
    return recursiveLnProb(tau->getValue().getRoot());
}

double WhiteNoisePhyloProcess::recursiveLnProb(const TopologyNode &from)   {

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

void WhiteNoisePhyloProcess::redrawValue(void) {
    simulate();
    std::cerr << "WhiteNoise:\t\t" << *value << std::endl;
}


/** Get the parameters of the distribution */
std::set<const DagNode*> WhiteNoisePhyloProcess::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( tau );
    parameters.insert( sigma );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void WhiteNoisePhyloProcess::swapParameter( const DagNode *oldP, const DagNode *newP )
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


void WhiteNoisePhyloProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
    
}


void WhiteNoisePhyloProcess::recursiveSimulate(const TopologyNode& from) {
    
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
 void WhiteNoisePhyloProcess::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
 // only delegate when the toucher was the root val
 //    if ( affecter == rootVal )
 //        this->dagNode->getAffectedNodes( affected );
 }
 
 
 void WhiteNoisePhyloProcess::keepSpecialization( DagNode* affecter ) {
 // only do this when the toucher was the root val
 //    if ( affecter == rootVal )
 //        this->dagNode->keepAffected();
 }
 */

/*
void WhiteNoisePhyloProcess::restoreSpecialization( DagNode *restorer ) {
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


//void WhiteNoisePhyloProcess::touchSpecialization( DagNode *toucher ) {
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


