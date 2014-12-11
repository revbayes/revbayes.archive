#include "AutocorrelatedLognormalRateBranchwiseVarDistribution.h"
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
AutocorrelatedLognormalRateBranchwiseVarDistribution::AutocorrelatedLognormalRateBranchwiseVarDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< RbVector< double > > * s, const TypedDagNode< double >* rr): TypedDistribution< RbVector< double > >( new RbVector< double >(t->getValue().getNumberOfNodes(), 0.0 ) ),
tau( t ), 
sigma( s ), 
rootRate( rr ),
scaleValue( new ConstantNode<double>(" ", new double(1.0) ))
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( sigma );
    addParameter( rootRate );
    addParameter( scaleValue );
	
    simulate();
}

AutocorrelatedLognormalRateBranchwiseVarDistribution::AutocorrelatedLognormalRateBranchwiseVarDistribution(const TypedDagNode< TimeTree > *t, TypedDagNode< RbVector< double > > *s, const TypedDagNode< double >* rr, const TypedDagNode< double >* sv): TypedDistribution< RbVector< double > >( new RbVector< double >(t->getValue().getNumberOfNodes(), 0.0 ) ),
tau( t ), 
sigma( s ), 
rootRate( rr ),
scaleValue( sv )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( sigma );
    addParameter( rootRate );
    addParameter( scaleValue );
	
    simulate();
}




AutocorrelatedLognormalRateBranchwiseVarDistribution* AutocorrelatedLognormalRateBranchwiseVarDistribution::clone(void) const {
    return new AutocorrelatedLognormalRateBranchwiseVarDistribution( *this );
}


double AutocorrelatedLognormalRateBranchwiseVarDistribution::computeLnProbability(void) {
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    size_t rootIndex= root.getIndex();
    
    double lnProb = 0.0;
	double scale = scaleValue->getValue();
	
    
    double parentRate = rootRate->getValue();
    // check if the root rate matches the parameter value for the root rate
    // the move could have changed this
    if ( (*value)[rootIndex] != parentRate ) {
        lnProb = RbConstants::Double::neginf;
    } else {
        parentRate = log( parentRate );
        size_t numChildren = root.getNumberOfChildren();
        for (size_t i = 0; i < numChildren; ++i) {
            const TopologyNode& child = root.getChild(i);
            lnProb += recursiveLnProb(child);

            size_t childIndex = child.getIndex();
            // compute the variance
            double variance = sigma->getValue()[childIndex] * child.getBranchLength() * scale;
            
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


void AutocorrelatedLognormalRateBranchwiseVarDistribution::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    // only delegate when the toucher was the root rate
    if ( affecter == rootRate )
        this->dagNode->getAffectedNodes( affected );
}


void AutocorrelatedLognormalRateBranchwiseVarDistribution::keepSpecialization( DagNode* affecter ) {
    // only do this when the toucher was the root rate
    if ( affecter == rootRate )
        this->dagNode->keepAffected();
}



double AutocorrelatedLognormalRateBranchwiseVarDistribution::recursiveLnProb( const TopologyNode& n ) {
    
    // get the index
    size_t nodeIndex = n.getIndex();
    
    double lnProb = 0.0;
    size_t numChildren = n.getNumberOfChildren();
	double scale = scaleValue->getValue();
    
    if ( numChildren > 0 ) {
        double parentRate = log( (*value)[nodeIndex] );
        
        for (size_t i = 0; i < numChildren; ++i) {
            const TopologyNode& child = n.getChild(i);
            lnProb += recursiveLnProb(child);
            
            size_t childIndex = child.getIndex();
            // compute the variance
            double variance = sigma->getValue()[childIndex] * child.getBranchLength() * scale;
            
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


void AutocorrelatedLognormalRateBranchwiseVarDistribution::redrawValue(void) {
    simulate();
    std::cerr << "ACLN:\t\t" << *value << std::endl;
}


void AutocorrelatedLognormalRateBranchwiseVarDistribution::restoreSpecialization( DagNode *restorer ) {
    // only do this when the toucher was our parameters
    if ( restorer == rootRate ) {
        // get the index of the root
        size_t rootIndex = tau->getValue().getRoot().getIndex();
        (*value)[rootIndex] = rootRate->getValue();
        
        // delegate a restore to our children
        this->dagNode->restoreAffected();
    }
}


/** Swap a parameter of the distribution */
void AutocorrelatedLognormalRateBranchwiseVarDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode< RbVector<double> > * >( newP );
    }
    
    if ( oldP == rootRate ) {
        rootRate = static_cast< const TypedDagNode< double > * >( newP );
    }

    if ( oldP == scaleValue ) {
        scaleValue = static_cast< const TypedDagNode< double > * >( newP );
    }
}


void AutocorrelatedLognormalRateBranchwiseVarDistribution::simulate() {
        
    // get the initial rate
    double parentRate = rootRate->getValue();
        
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    
    // get the index of the root
    size_t rootIndex = root.getIndex();
    
    // for savety reasons we store this rate here again
    (*value)[rootIndex] = parentRate;
    
    size_t numChildren = root.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = root.getChild(i);
        recursiveSimulate(child,parentRate);
    }
    
}




void AutocorrelatedLognormalRateBranchwiseVarDistribution::recursiveSimulate(const TopologyNode& node, double parentRate) {
    
    // get the index
    size_t nodeIndex = node.getIndex();
    
    // compute the variance along the branch
	double scale = scaleValue->getValue();
    double variance = sigma->getValue()[nodeIndex] * node.getBranchLength() * scale;
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



void AutocorrelatedLognormalRateBranchwiseVarDistribution::touchSpecialization( DagNode *toucher ) {
    // if the root rate has changed, then we need to change the corresponding value in our vector and downpass the touch
    if ( rootRate == toucher ) {
        
        // get the index of the root
        size_t rootIndex = tau->getValue().getRoot().getIndex();
        (*value)[rootIndex] = rootRate->getValue();
        
        // we notify our DAG node which element has changed
        dagNode->addTouchedElementIndex( rootIndex );
        
        // delegate a touch to our children
        this->dagNode->touchAffected();
    }
}

