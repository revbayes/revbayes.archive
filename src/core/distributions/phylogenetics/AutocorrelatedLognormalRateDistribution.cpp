#include "AutocorrelatedLognormalRateDistribution.h"
#include "ConstantNode.h"
#include "DistributionLognormal.h"
#include "GtrRateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <cmath>
#include <iostream>



using namespace RevBayesCore;



// constructor(s)
AutocorrelatedLognormalRateDistribution::AutocorrelatedLognormalRateDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* rr): TypedDistribution< RbVector< double > >( new RbVector< double >(t->getValue().getNumberOfNodes()-1, 0.0 ) ),
    tau( t ),
    sigma( s ),
    rootRate( rr ),
    scaleValue( new ConstantNode<double>(" ", new double(1.0) ))
{
    
    simulate();
}

// constructor for when rescaling the tree to relative rates or other value
AutocorrelatedLognormalRateDistribution::AutocorrelatedLognormalRateDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* rr, const TypedDagNode< double >* sv): TypedDistribution< RbVector< double > >( new RbVector< double >(t->getValue().getNumberOfNodes()-1, 0.0 ) ),
    tau( t ),
    sigma( s ),
    rootRate( rr ),
    scaleValue( sv )
{
    
    simulate();
}



AutocorrelatedLognormalRateDistribution* AutocorrelatedLognormalRateDistribution::clone(void) const {
    return new AutocorrelatedLognormalRateDistribution( *this );
}


double AutocorrelatedLognormalRateDistribution::computeLnProbability(void) {
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
	
    
    double lnProb = 0.0;
	double scale = scaleValue->getValue();
	
    
    double parentRate = rootRate->getValue();

	parentRate = log( parentRate );
	size_t numChildren = root.getNumberOfChildren();
	for (size_t i = 0; i < numChildren; ++i) {
		const TopologyNode& child = root.getChild(i);
		lnProb += recursiveLnProb(child);
		
		// compute the variance
		double variance = sigma->getValue() * child.getBranchLength() * scale;
		
		size_t childIndex = child.getIndex();
		double childRate = (*value)[childIndex];
		
		// the mean of the LN dist is parentRate = exp[mu + (variance / 2)],
		// where mu is the location param of the LN dist (see Kishino & Thorne 2001)
		double mu = parentRate - (variance * 0.5);
		double stDev = sqrt(variance);
		lnProb += RbStatistics::Lognormal::lnPdf(mu, stDev, childRate);
	} 
    
    return lnProb;
}


void AutocorrelatedLognormalRateDistribution::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    // only delegate when the toucher was the root rate
    if ( affecter == rootRate )
        this->dagNode->getAffectedNodes( affected );
}


void AutocorrelatedLognormalRateDistribution::keepSpecialization( DagNode* affecter ) {
    // only do this when the toucher was the root rate
    if ( affecter == rootRate )
        this->dagNode->keepAffected();
}



double AutocorrelatedLognormalRateDistribution::recursiveLnProb( const TopologyNode& n ) {
    
    // get the index
    size_t nodeIndex = n.getIndex();
    
    double lnProb = 0.0;
    size_t numChildren = n.getNumberOfChildren();
	double scale = scaleValue->getValue();
	size_t rootIndex = tau->getValue().getRoot().getIndex();
    
    if ( numChildren > 0 ) {
        double parentRate;
		if(nodeIndex == rootIndex){
			parentRate = log(rootRate->getValue());
		}
		else{
			parentRate = log( (*value)[nodeIndex] );
		}
        
        for (size_t i = 0; i < numChildren; ++i) {
            const TopologyNode& child = n.getChild(i);
            lnProb += recursiveLnProb(child);
            
            // compute the variance
            double variance = sigma->getValue() * child.getBranchLength() * scale;
            
            size_t childIndex = child.getIndex();
            double childRate = (*value)[childIndex];

			// the mean of the LN dist is parentRate = exp[mu + (variance / 2)],
			// where mu is the location param of the LN dist (see Kishino & Thorne 2001)
			double mu = parentRate - (variance * 0.5);
			double stDev = sqrt(variance);
            lnProb += RbStatistics::Lognormal::lnPdf(mu, stDev, childRate);
        } 
    }
    return lnProb;
    
}


void AutocorrelatedLognormalRateDistribution::redrawValue(void) {
    simulate();
    std::cerr << "ACLN:\t\t" << *value << std::endl;
}


void AutocorrelatedLognormalRateDistribution::restoreSpecialization( DagNode *restorer ) {
    // only do this when the toucher was our parameters
//    if ( restorer == rootRate ) {
//        // get the index of the root
//        size_t rootIndex = tau->getValue().getRoot().getIndex();
//        (*value)[rootIndex] = rootRate->getValue();
//        
//        // delegate a restore to our children
//        this->dagNode->restoreAffected();
//    }
}


/** Get the parameters of the distribution */
std::set<const DagNode*> AutocorrelatedLognormalRateDistribution::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( tau );
    parameters.insert( sigma );
    parameters.insert( rootRate );
    parameters.insert( scaleValue );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void AutocorrelatedLognormalRateDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == rootRate ) {
        rootRate = static_cast< const TypedDagNode< double > * >( newP );
    }

    if ( oldP == scaleValue ) {
        scaleValue = static_cast< const TypedDagNode< double > * >( newP );
    }
}


void AutocorrelatedLognormalRateDistribution::simulate() {
    

    // get the initial rate
    double parentRate = rootRate->getValue();
        
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    
    
    size_t numChildren = root.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = root.getChild(i);
        recursiveSimulate(child,parentRate);
    }
    
}




void AutocorrelatedLognormalRateDistribution::recursiveSimulate(const TopologyNode& node, double parentRate) {
    
    // get the index
    size_t nodeIndex = node.getIndex();
    
    // compute the variance along the branch
	double scale = scaleValue->getValue();
    double variance = sigma->getValue() * node.getBranchLength() * scale;
	double mu = log(parentRate) - (variance * 0.5);
	double stDev = sqrt(variance);
    
    // simulate a new rate
    RandomNumberGenerator* rng = GLOBAL_RNG;
    double nodeRate = RbStatistics::Lognormal::rv( mu, stDev, *rng );
    
    // we store this rate here
    (*value)[nodeIndex] = nodeRate;
    
    // simulate the rate for each child (if any)
    size_t numChildren = node.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = node.getChild(i);
        recursiveSimulate(child,nodeRate);
    }
}



void AutocorrelatedLognormalRateDistribution::touchSpecialization( DagNode *toucher ) {
    // if the root rate has changed, then we need to change the corresponding value in our vector and downpass the touch
//    if ( rootRate == toucher ) {
//        
//        // get the index of the root
//        //size_t rootIndex = tau->getValue().getRoot().getIndex();
//        //(*value)[rootIndex] = rootRate->getValue();
//        
//        // we notify our DAG node which element has changed
//        dagNode->addTouchedElementIndex( rootIndex );
//        
//        // delegate a touch to our children
//        this->dagNode->touchAffected();
//    }
}

