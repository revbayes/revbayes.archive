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



#ifndef BranchRateTimeBHGtrCharEvoModel_H
#define BranchRateTimeBHGtrCharEvoModel_H

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
    class BranchRateTimeBHCharEvoModel : public AbstractSiteHomogeneousCharEvoModel<charType, treeType> {
        
    public:
        BranchRateTimeBHCharEvoModel(const TypedDagNode<treeType> *t, const  TypedDagNode< RbVector<RateMatrix> > * rm, const TypedDagNode< std::vector< double > >* rf, const TypedDagNode< std::vector< double> > *br, bool c, size_t nSites);
        BranchRateTimeBHCharEvoModel(const BranchRateTimeBHCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~BranchRateTimeBHCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        BranchRateTimeBHCharEvoModel*                       clone(void) const;                                                                          //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
        const std::vector<double>&                          getRootFrequencies(void);
		
        void                                                touchSpecialization(DagNode *toucher);
		
    private:
        CharacterData<charType>*                            simulate(const TopologyNode& node);
        
        // members
        const TypedDagNode< RbVector<RateMatrix> > *        rateMatrix;
        const TypedDagNode< std::vector< double > >*        rootFrequencies;
		const TypedDagNode< std::vector< double > >*        branchRates;

        
    };
    
}


#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>
#include <set>

template<class charType, class treeType>
RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::BranchRateTimeBHCharEvoModel(const TypedDagNode<treeType> *t, 
																								   const TypedDagNode< RbVector<RateMatrix> > * rm, 
																								   const TypedDagNode< std::vector< double > >* rf,
																								   const TypedDagNode< std::vector< double > > *br,
																								   bool c, size_t nSites) : AbstractSiteHomogeneousCharEvoModel<charType, treeType>(  t, c, nSites ), rateMatrix( rm ), rootFrequencies(rf), branchRates(br) {
    // add the parameters to the parents list
    this->addParameter( rateMatrix );
    this->addParameter( rootFrequencies );
	this->addParameter( branchRates );

    delete this->value;
    this->value = simulate(this->tau->getValue().getRoot());

}


template<class charType, class treeType>
RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::BranchRateTimeBHCharEvoModel(const BranchRateTimeBHCharEvoModel &n) : AbstractSiteHomogeneousCharEvoModel<charType, treeType>( n ), rateMatrix( n.rateMatrix ), rootFrequencies(n.rootFrequencies), branchRates(n.branchRates) {
    // parameters are automatically copied
    
}


template<class charType, class treeType>
RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::~BranchRateTimeBHCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>* RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::clone( void ) const {
 
    return new BranchRateTimeBHCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::getRootFrequencies( void ) {
    
    return rootFrequencies->getValue();
}


template<class charType, class treeType>
void RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen) {
    
    const RateMatrix &rm = rateMatrix->getValue()[nodeIdx];
    rm.calculateTransitionProbabilities( brlen * branchRates->getValue()[nodeIdx] , this->transitionProbMatrix );
    
}


template<class charType, class treeType>
RevBayesCore::CharacterData<charType>* RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::simulate(const TopologyNode &node) {
    
    return new CharacterData<charType>();
}



template<class charType, class treeType>
void RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == rateMatrix) 
    {
        rateMatrix = static_cast<const TypedDagNode< RbVector<RateMatrix> > * >( newP );
    }
	if (oldP == branchRates) 
    {
        branchRates = static_cast<const TypedDagNode< std::vector< double >  > * >( newP );
    }
	if (oldP == rootFrequencies) 
    {
        rootFrequencies = static_cast<const TypedDagNode< std::vector< double > > * >( newP );
    }
    else 
    {
        AbstractSiteHomogeneousCharEvoModel<charType, treeType>::swapParameter(oldP,newP);
    }
}



template<class charType, class treeType>
void RevBayesCore::BranchRateTimeBHCharEvoModel<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    bool found = false;
    if ( affecter == rootFrequencies ) 
    {
        found = true;
        const TopologyNode& root = this->tau->getValue().getRoot();
        this->recursivelyFlagNodeDirty( root );
    }
    else if ( affecter == rateMatrix ) 
    {
        const std::set<size_t>& indices = rateMatrix->getTouchedElementIndices();
        if ( indices.size() > 0 ) 
        {
            found = true;
            std::set<size_t>::const_iterator end = indices.end();
            for (std::set<size_t>::const_iterator it = indices.begin(); it != end; ++it) 
            {
                // get the node with the touched index
                const TopologyNode &n = this->tau->getValue().getNode( *it );
                // flag all nodes dirty above this one (and including it)
                this->recursivelyFlagNodeDirty( n );
            }
        }
    }
	else if ( affecter == branchRates ) 
    {
		const std::set< size_t > &touchedNodeIndices = branchRates->getTouchedElementIndices();
		found = touchedNodeIndices.size() > 0;
		std::set< size_t >::const_iterator end = touchedNodeIndices.end();
		for (std::set< size_t >::const_iterator it = touchedNodeIndices.begin() ; it != end ; ++it) 
        {
			const TopologyNode &node = this->tau->getValue().getNode( *it );
			this->recursivelyFlagNodeDirty( node );
		}
	}
    
    if ( !found ) 
    {
        // delegate to base class
        this->AbstractSiteHomogeneousCharEvoModel<charType, treeType>::touchSpecialization( affecter );
    }
    
    
}


#endif
