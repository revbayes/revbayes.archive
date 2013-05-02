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



#ifndef SimpleSiteHomogeneousGtrCharEvoModel_H
#define SimpleSiteHomogeneousGtrCharEvoModel_H

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
    class SimpleSiteHomogeneousCharEvoModel : public AbstractSiteHomogeneousCharEvoModel<charType, treeType> {
        
    public:
        SimpleSiteHomogeneousCharEvoModel(const TypedDagNode<treeType> *t, const TypedDagNode<RateMatrix> *rm, const TypedDagNode<double> *ra, bool c, size_t nSites);
        SimpleSiteHomogeneousCharEvoModel(const SimpleSiteHomogeneousCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~SimpleSiteHomogeneousCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        SimpleSiteHomogeneousCharEvoModel*                  clone(void) const;                                                                          //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
        const std::vector<double>&                          getRootFrequencies(void);
        
    private:
        CharacterData<charType>*                            simulate(const TopologyNode& node);
        
        // members
        const TypedDagNode<double>*                         rateMultiplier;
        const TypedDagNode<RateMatrix>*                     rateMatrix;
        
    };
    
}


#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType, class treeType>
RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>::SimpleSiteHomogeneousCharEvoModel(const TypedDagNode<treeType> *t, const TypedDagNode<RateMatrix> *rm, const TypedDagNode<double> *ra, bool c, size_t nSites) : AbstractSiteHomogeneousCharEvoModel<charType, treeType>(  t, c, nSites ), rateMatrix( rm ), rateMultiplier( ra ) {
    // add the parameters to the parents list
    this->addParameter( rateMatrix );
    this->addParameter( rateMultiplier );
    
    delete this->value;
    this->value = simulate(this->tau->getValue().getRoot());
    
}


template<class charType, class treeType>
RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>::SimpleSiteHomogeneousCharEvoModel(const SimpleSiteHomogeneousCharEvoModel &n) : AbstractSiteHomogeneousCharEvoModel<charType, treeType>( n ), rateMatrix( n.rateMatrix ), rateMultiplier( n.rateMultiplier ) {
    // parameters are automatically copied
    
}


template<class charType, class treeType>
RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>::~SimpleSiteHomogeneousCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>* RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>::clone( void ) const {

    return new SimpleSiteHomogeneousCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>::getRootFrequencies( void ) {
    
    return rateMatrix->getValue().getStationaryFrequencies();
}


template<class charType, class treeType>
void RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen) {
    
    const RateMatrix &rm = rateMatrix->getValue();
    rm.calculateTransitionProbabilities( brlen * rateMultiplier->getValue(), this->transitionProbMatrix );
    
}




template<class charType, class treeType>
RevBayesCore::CharacterData<charType>* RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>::simulate(const TopologyNode &node) {

    return new CharacterData<charType>();
}



template<class charType, class treeType>
void RevBayesCore::SimpleSiteHomogeneousCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {

    if (oldP == rateMatrix) 
    {
        rateMatrix = static_cast<const TypedDagNode<RateMatrix>* >( newP );
    }
    else if (oldP == rateMultiplier) 
    {
        rateMultiplier = static_cast<const TypedDagNode<double>* >( newP );
    }
    else 
    {
        AbstractSiteHomogeneousCharEvoModel<charType, treeType>::swapParameter(oldP,newP);
    }
    
}


#endif
