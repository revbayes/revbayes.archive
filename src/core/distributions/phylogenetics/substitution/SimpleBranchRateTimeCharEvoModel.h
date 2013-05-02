/**
 * @file
 * This file contains the declaration of the random variable class for a character state evolving along a tree.
 * This class is derived from the stochastic node and each instance will represent a random variable.
 *
 * @brief Declaration of the character state evolution along a tree class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id:$
 */



#ifndef SimpleBranchRateTimeCharEvoModel_H
#define SimpleBranchRateTimeCharEvoModel_H

#include "AbstractSiteHomogeneousCharEvoModel.h"
#include "CharacterData.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class SimpleBranchRateTimeCharEvoModel : public AbstractSiteHomogeneousCharEvoModel<charType, treeType> {
        
    public:
        SimpleBranchRateTimeCharEvoModel(const TypedDagNode< treeType > *t, const TypedDagNode< RateMatrix > *rm, const TypedDagNode< std::vector< double> > *br, bool c, size_t nSites);
        SimpleBranchRateTimeCharEvoModel(const SimpleBranchRateTimeCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~SimpleBranchRateTimeCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        SimpleBranchRateTimeCharEvoModel*                   clone(void) const;                                                                          //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
        const std::vector<double>&                          getRootFrequencies(void);
        
        void                                                touchSpecialization(DagNode *toucher);
		
    private:
        
        // members
        const TypedDagNode< RateMatrix >*					rateMatrix;
        const TypedDagNode< std::vector<double> >*          branchRates;
        
    };
    
}


#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType, class treeType>
RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>::SimpleBranchRateTimeCharEvoModel(const TypedDagNode<treeType> *t, 
																										   const TypedDagNode< RateMatrix > *rm, const TypedDagNode< std::vector< double > > *br, 
																										   bool c, size_t nSites) : 
																										   AbstractSiteHomogeneousCharEvoModel<charType, treeType>(  t, c, nSites ), 
																										   rateMatrix( rm ), branchRates(br) {
    // add the parameters to the parents list
    this->addParameter( branchRates );
	this->addParameter( rateMatrix );

    
    this->redrawValue();
}


template<class charType, class treeType>
RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>::SimpleBranchRateTimeCharEvoModel(const SimpleBranchRateTimeCharEvoModel &n) : AbstractSiteHomogeneousCharEvoModel<charType, treeType>( n ), 
																											rateMatrix( n.rateMatrix ), branchRates(n.branchRates) {
    // parameters are automatically copied
    
}


template<class charType, class treeType>
RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>::~SimpleBranchRateTimeCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>* RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>::clone( void ) const {
    return new SimpleBranchRateTimeCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>::getRootFrequencies( void ) {
    
    return rateMatrix->getValue().getStationaryFrequencies();
}


template<class charType, class treeType>
void RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen) {
    
    const RateMatrix &rm = rateMatrix->getValue();
    rm.calculateTransitionProbabilities( brlen * branchRates->getValue()[nodeIdx], this->transitionProbMatrix );
    
}



template<class charType, class treeType>
void RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == branchRates) 
    {
        branchRates = static_cast<const TypedDagNode< std::vector< double>  > * >( newP );
    }
	else if (oldP == rateMatrix) 
    {
        rateMatrix = static_cast< const TypedDagNode< RateMatrix > *>( newP );
        
    }
    else 
    {
        AbstractSiteHomogeneousCharEvoModel<charType, treeType>::swapParameter(oldP,newP);
    }
}



template<class charType, class treeType>
void RevBayesCore::SimpleBranchRateTimeCharEvoModel<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    bool found = false;
    if ( affecter == branchRates ) {
        const std::set< size_t > &touchedNodeIndices = branchRates->getTouchedElementIndices();
        found = touchedNodeIndices.size() > 0;
        std::set< size_t >::const_iterator end = touchedNodeIndices.end();
        for (std::set< size_t >::const_iterator it = touchedNodeIndices.begin() ; it != end ; ++it) {
            const TopologyNode &node = this->tau->getValue().getNode( *it );
            this->recursivelyFlagNodeDirty( node );
        }
    }
    
    if ( !found ) {
        // delegate to base class
        this->AbstractSiteHomogeneousCharEvoModel<charType, treeType>::touchSpecialization( affecter );
    }
    
    
}


#endif
