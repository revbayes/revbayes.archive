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



#ifndef BranchHeterogeneousCharEvoModel_H
#define BranchHeterogeneousCharEvoModel_H

#include "AbstractSiteHomogeneousCharEvoModel.h"
#include "CharacterData.h"
#include "DnaState.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class BranchHeterogeneousCharEvoModel : public AbstractSiteHomogeneousCharEvoModel<charType, treeType> {
        
    public:
        BranchHeterogeneousCharEvoModel(const TypedDagNode<treeType> *t, const  TypedDagNode< RbVector<RateMatrix> > * rm, const TypedDagNode< std::vector< double > >* rf, bool c, size_t nSites);
        BranchHeterogeneousCharEvoModel(const BranchHeterogeneousCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~BranchHeterogeneousCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        BranchHeterogeneousCharEvoModel*                    clone(void) const;                                                                          //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
        const std::vector<double>&                          getRootFrequencies(void);
       
        void                                                touchSpecialization(DagNode *toucher);

    private:
        
        // members
        const  TypedDagNode< RbVector<RateMatrix> >         *rateMatrix;
        const TypedDagNode< std::vector< double > >*        rootFrequencies;
        
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
RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>::BranchHeterogeneousCharEvoModel(const TypedDagNode<treeType> *t, const  TypedDagNode< RbVector<RateMatrix> > * rm, const TypedDagNode< std::vector< double > >* rf, bool c, size_t nSites) : AbstractSiteHomogeneousCharEvoModel<charType, treeType>(  t, c, nSites ), rateMatrix( rm ), rootFrequencies(rf) {
    // add the parameters to the parents list
    this->addParameter( rateMatrix );
    this->addParameter( rootFrequencies );
    
    this->redrawValue();
}


template<class charType, class treeType>
RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>::BranchHeterogeneousCharEvoModel(const BranchHeterogeneousCharEvoModel &n) : AbstractSiteHomogeneousCharEvoModel<charType, treeType>( n ), rateMatrix( n.rateMatrix ), rootFrequencies(n.rootFrequencies) {
    // parameters are automatically copied
    
}


template<class charType, class treeType>
RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>::~BranchHeterogeneousCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>* RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>::clone( void ) const {
    
    return new BranchHeterogeneousCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>::getRootFrequencies( void ) {
    
    return rootFrequencies->getValue();
}


template<class charType, class treeType>
void RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen) {
    
    const RateMatrix &rm = rateMatrix->getValue()[nodeIdx];
    rm.calculateTransitionProbabilities( brlen , this->transitionProbMatrix );
    
}



template<class charType, class treeType>
void RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == rateMatrix) 
    {
        rateMatrix = static_cast<const TypedDagNode< RbVector<RateMatrix> > * >( newP );
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
void RevBayesCore::BranchHeterogeneousCharEvoModel<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
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
    
    if ( !found ) {
        // delegate to base class
        this->AbstractSiteHomogeneousCharEvoModel<charType, treeType>::touchSpecialization( affecter );
    }
    
    
}


#endif
