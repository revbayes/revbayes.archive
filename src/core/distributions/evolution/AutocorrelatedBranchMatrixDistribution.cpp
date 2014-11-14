#include "AutocorrelatedBranchMatrixDistribution.h"
#include "ConstantNode.h"
#include "DistributionDirichlet.h"
#include "GtrRateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include <iostream>



using namespace RevBayesCore;



// constructor(s)
AutocorrelatedBranchMatrixDistribution::AutocorrelatedBranchMatrixDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* p, const TypedDagNode< RbVector< double > >* rf, const TypedDagNode< RbVector< double > >* er, const TypedDagNode< double >* a): TypedDistribution< RbVector< RateMatrix > >( new RbVector<RateMatrix>(t->getValue().getNumberOfNodes()-1, RateMatrix_GTR(rf->getValue().size()) ) ),
    tau( t ),
    changeProbability( p ),
    rootFrequencies( rf ),
    exchangeabilityRates( er ),
    alpha( a )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( changeProbability );
    addParameter( rootFrequencies );
    addParameter( exchangeabilityRates );
    addParameter( alpha );
    
    delete value;
    value = simulate();
}




AutocorrelatedBranchMatrixDistribution* AutocorrelatedBranchMatrixDistribution::clone(void) const {
    return new AutocorrelatedBranchMatrixDistribution( *this );
}


double AutocorrelatedBranchMatrixDistribution::computeLnProbability(void) {
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    size_t rootIndex = root.getIndex();
    
    
    double lnProb = 0.0;
    size_t numChildren = root.getNumberOfChildren();
    std::vector<double> parent = rootFrequencies->getValue();
    
    std::vector<double>::iterator end = parent.end();
    for (std::vector<double>::iterator it = parent.begin(); it != end; ++it) {
        (*it) *= alpha->getValue();
    }
    
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = root.getChild(i);
        lnProb += recursiveLnProb(child);
        
        size_t childIndex = child.getIndex();
//        RateMatrix& rm = (*value)[childIndex];
        
        // compare if the child has a different matrix
        if ( matrixIndex[rootIndex] == matrixIndex[childIndex] ) {
            // no change -> just the probability of no change
            lnProb += log( 1.0 - changeProbability->getValue() );
        } else {
            // change:
            
            // probability of change
            lnProb += log( changeProbability->getValue() );
            
            const std::vector<double>& descendant = (*value)[childIndex].getStationaryFrequencies();
//            const std::vector<double>& descendant = uniqueMatrices[ matrixIndex[childIndex] ].getStationaryFrequencies();
            
            // probability of new descendant values
            double p = RbStatistics::Dirichlet::lnPdf(parent, descendant);
            lnProb += p;
        }
    }
    
    return lnProb;
}





double AutocorrelatedBranchMatrixDistribution::recursiveLnProb( const TopologyNode& n ) {
    
    // get the index
    size_t nodeIndex = n.getIndex();
    
    double lnProb = 0.0;
    size_t numChildren = n.getNumberOfChildren();
    
    if ( numChildren > 0 ) {
        std::vector<double> parent = (*value)[nodeIndex].getStationaryFrequencies();

        std::vector<double>::iterator end = parent.end();
        for (std::vector<double>::iterator it = parent.begin(); it != end; ++it) {
            (*it) *= alpha->getValue();
        }

        for (size_t i = 0; i < numChildren; ++i) {
            const TopologyNode& child = n.getChild(i);
            lnProb += recursiveLnProb(child);
            
            size_t childIndex = child.getIndex();
            //        RateMatrix& rm = (*value)[childIndex];
            
            // compare if the child has a different matrix
            if ( matrixIndex[nodeIndex] == matrixIndex[childIndex] ) {
                // no change -> just the probability of no change
                lnProb += log( 1.0 - changeProbability->getValue() );
            } else {
                // change:
                
                // probability of change
                lnProb += log( changeProbability->getValue() );
                
                const std::vector<double>& descendant = (*value)[childIndex].getStationaryFrequencies();
        //            const std::vector<double>& descendant = uniqueMatrices[ matrixIndex[childIndex] ].getStationaryFrequencies();
                
                // probability of new descendant values
                double p = RbStatistics::Dirichlet::lnPdf(parent, descendant);
                lnProb += p;
            }
        } 
    }
    
    return lnProb;
    
}


void AutocorrelatedBranchMatrixDistribution::redrawValue(void) {
    delete value;
    value = simulate();
}


/** Swap a parameter of the distribution */
void AutocorrelatedBranchMatrixDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    
    if ( oldP == tau )
    {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == changeProbability )
    {
        changeProbability = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == rootFrequencies )
    {
        rootFrequencies = static_cast< const TypedDagNode< RbVector< double > > * >( newP );
    }
    
    if ( oldP == exchangeabilityRates )
    {
        exchangeabilityRates = static_cast< const TypedDagNode< RbVector< double > > * >( newP );
    }
    
    if ( oldP == alpha )
    {
        alpha = static_cast< const TypedDagNode<double> * >( newP );
    }
}


RbVector< RateMatrix >* AutocorrelatedBranchMatrixDistribution::simulate() {
    
    size_t numNodes = tau->getValue().getNumberOfNodes()-1;
    RbVector< RateMatrix >* newValues = new RbVector< RateMatrix >( numNodes,RateMatrix_GTR( rootFrequencies->getValue().size() ));
    matrixIndex = std::vector<size_t>(numNodes+1, 0);
    
    // create the initial rate-matrix
    RateMatrix_GTR rm = RateMatrix_GTR( rootFrequencies->getValue().size() );
    RbPhylogenetics::Gtr::computeRateMatrix(exchangeabilityRates->getValue(), rootFrequencies->getValue(), &rm);
    
    uniqueMatrices.clear();
    uniqueMatrices.push_back( rm );
    uniqueBaseFrequencies.clear();
    uniqueBaseFrequencies.push_back( rootFrequencies->getValue() );
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    
    size_t numChildren = root.getNumberOfChildren();
    const std::vector<double>& parent = rootFrequencies->getValue();
    std::vector<double> scaledParent = parent;
    
    std::vector<double>::iterator end = scaledParent.end();
    for (std::vector<double>::iterator it = scaledParent.begin(); it != end; ++it) {
        (*it) *= alpha->getValue();
    }
    
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = root.getChild(i);
        recursiveSimulate(child,newValues,scaledParent);
    }
    
    
    return newValues;
}




void AutocorrelatedBranchMatrixDistribution::recursiveSimulate(const TopologyNode& node, RbVector< RateMatrix > *values, const std::vector< double > &scaledParent) {
    
    // get the index
    size_t nodeIndex = node.getIndex();
    
    // first we simulate our value
    RandomNumberGenerator* rng = GLOBAL_RNG;
    // do we keep our parents values?
    double u = rng->uniform01();
    if ( u < changeProbability->getValue() ) {
        // change
        
        // draw a new value for the base frequencies
        std::vector<double> newParent = RbStatistics::Dirichlet::rv(scaledParent, *rng);
        std::vector<double> newScaledParent = newParent;
        
        // compute the new scaled parent
        std::vector<double>::iterator end = newScaledParent.end();
        for (std::vector<double>::iterator it = newScaledParent.begin(); it != end; ++it) {
            (*it) *= alpha->getValue();
        }
        
        RateMatrix_GTR rm = RateMatrix_GTR( newParent.size() );
        RbPhylogenetics::Gtr::computeRateMatrix( exchangeabilityRates->getValue(), newParent, &rm );
        
        uniqueBaseFrequencies.push_back( newParent );
        uniqueMatrices.push_back( rm );
        matrixIndex[nodeIndex] = uniqueMatrices.size()-1;
        // @Sebastian: Fix this!!!
//        values->insert(nodeIndex, rm);
        
        size_t numChildren = node.getNumberOfChildren();
        if ( numChildren > 0 ) {
            
            for (size_t i = 0; i < numChildren; ++i) {
                const TopologyNode& child = node.getChild(i);
                recursiveSimulate(child,values,newScaledParent);
            }
        }
        
    }
    else {
        // no change
        size_t parentIndex = node.getParent().getIndex();
        // @Sebastian: Fix this!!!
//        values->insert(nodeIndex, uniqueMatrices[ matrixIndex[ parentIndex ] ]);
        
        size_t numChildren = node.getNumberOfChildren();
        if ( numChildren > 0 ) {
            
            for (size_t i = 0; i < numChildren; ++i) {
                const TopologyNode& child = node.getChild(i);
                recursiveSimulate(child,values,scaledParent);
            }
        }
    }
    
}



void AutocorrelatedBranchMatrixDistribution::touchSpecialization( DagNode *toucher ) {
    // if the exchangeability rates have changed, then all rate-matrices need to be recomputed
    if ( exchangeabilityRates == toucher ) {
        
        const std::vector< double >& er = exchangeabilityRates->getValue();
        RbIterator< RateMatrix > it_rm = value->begin();
        std::vector< size_t >::iterator it_idx = matrixIndex.begin();
        RbIterator< RateMatrix > end = value->end();
        for (; it_rm != end; ++it_idx, ++it_rm) {
            const std::vector< double >& bf = uniqueBaseFrequencies[*it_idx];
            RbPhylogenetics::Gtr::computeRateMatrix(er,bf,static_cast<RateMatrix_GTR*>( &(*it_rm) ));
        }
    }
}




