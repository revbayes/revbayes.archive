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



#ifndef SimpleSiteHeterogeneousMixtureCharEvoModel_H
#define SimpleSiteHeterogeneousMixtureCharEvoModel_H

#include "AbstractSiteHomogeneousMixtureCharEvoModel.h"
#include "CharacterData.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class SimpleSiteHeterogeneousMixtureCharEvoModel : public AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType> {
        
    public:
        SimpleSiteHeterogeneousMixtureCharEvoModel(const TypedDagNode<treeType> *t, const TypedDagNode<std::vector<double> > *sr, const TypedDagNode<RateMatrix> *rm, bool c, size_t nSites);
        SimpleSiteHeterogeneousMixtureCharEvoModel(const SimpleSiteHeterogeneousMixtureCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~SimpleSiteHeterogeneousMixtureCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        SimpleSiteHeterogeneousMixtureCharEvoModel*         clone(void) const;                                                                          //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
        const std::vector<double>&                          getRootFrequencies(void);
 
    private:
        CharacterData<charType>*                            simulate(const TopologyNode& node);
        
        // members
        const TypedDagNode< RateMatrix >*                   rateMatrix;
        const TypedDagNode< std::vector< double > >*        siteRates;
        
    };
    
}


#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType, class treeType>
RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::SimpleSiteHeterogeneousMixtureCharEvoModel(const TypedDagNode<treeType> *t, const TypedDagNode<std::vector<double> > *sr, const TypedDagNode<RateMatrix> *rm, bool c, size_t nSites) : AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>(  t, sr->getValue().size(), c, nSites ), rateMatrix( rm ), siteRates( sr ) {
    // add the parameters to the parents list
    this->addParameter( rateMatrix );
    this->addParameter( siteRates );
    
    delete this->value;
    this->value = simulate(this->tau->getValue().getRoot());
}


template<class charType, class treeType>
RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::SimpleSiteHeterogeneousMixtureCharEvoModel(const SimpleSiteHeterogeneousMixtureCharEvoModel &n) : AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>( n ), rateMatrix( n.rateMatrix ), siteRates( n.siteRates ) {
    // parameters are automatically copied
    
}


template<class charType, class treeType>
RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::~SimpleSiteHeterogeneousMixtureCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>* RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::clone( void ) const {
    
    return new SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::getRootFrequencies( void ) {
    
    return rateMatrix->getValue().getStationaryFrequencies();
}


template<class charType, class treeType>
void RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen) {
    
    const RateMatrix &rm = rateMatrix->getValue();
    const std::vector<double> &r = this->siteRates->getValue();
    for (size_t i = 0; i < this->numSiteRates; ++i)
    {
        rm.calculateTransitionProbabilities( brlen * r[i], this->transitionProbMatrices[i] );
    }
    
}




template<class charType, class treeType>
RevBayesCore::CharacterData<charType>* RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::simulate(const TopologyNode &node) {
    
    return new CharacterData<charType>();
}



template<class charType, class treeType>
void RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == rateMatrix) 
    {
        rateMatrix = static_cast<const TypedDagNode< RateMatrix >* >( newP );
    }
    else if (oldP == rateMatrix) 
    {
        siteRates = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else 
    {
        AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>::swapParameter(oldP,newP);
    }
    
}


#endif
