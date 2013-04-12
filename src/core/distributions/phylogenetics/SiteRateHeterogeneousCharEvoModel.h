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



#ifndef SiteRateHeterogeneousGtrCharEvoModel_H
#define SiteRateHeterogeneousGtrCharEvoModel_H

#include "AbstractHeterogeneousCharEvoModel.h"
#include "CharacterData.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class SiteRateHeterogeneousCharEvoModel : public AbstractHeterogeneousCharEvoModel<charType, treeType> {
        
    public:
        SiteRateHeterogeneousCharEvoModel(const TypedDagNode<treeType> *t, const TypedDagNode<RateMatrix> *rm, const std::vector <const TypedDagNode < double > * > &sr, size_t nSites);
        SiteRateHeterogeneousCharEvoModel(const SiteRateHeterogeneousCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~SiteRateHeterogeneousCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        SiteRateHeterogeneousCharEvoModel*                  clone(void) const;                                                                          //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        const std::vector<TransitionProbabilityMatrix *>&   computeTransitionProbabilities(const TopologyNode &node);
        const TransitionProbabilityMatrix&                  computeTransitionProbabilities(const TopologyNode &node, size_t site);
        const std::vector< std::vector< double > *>&        getRootFrequencies(void);
        const std::vector< double >&                        getRootFrequencies(size_t site);
        CharacterData<charType>*                            simulate(const TopologyNode& node);
        
    private:
        // members
        const TypedDagNode<RateMatrix>*                     rateMatrix;     
        std::vector< const TypedDagNode< double > * >       siteRates;
        
        std::vector< std::vector< double > *>               rootFrequencies;
        std::vector< TransitionProbabilityMatrix *>         transitionProbabilities;
        TransitionProbabilityMatrix                         transitionProbabilitySpace;
    };
    
}


#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType, class treeType>
RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::SiteRateHeterogeneousCharEvoModel(const TypedDagNode<treeType> *t, const TypedDagNode<RateMatrix> *rm, const std::vector<const TypedDagNode < double > * > &sr, size_t nSites) : AbstractHeterogeneousCharEvoModel<charType, treeType>(  t, nSites ), rateMatrix( rm ), transitionProbabilities( rm->getValue().getNumberOfStates() ) , siteRates( sr ), transitionProbabilitySpace( charType().getNumberOfStates() )  {
    // add the parameters to the parents list
    this->addParameter( rateMatrix );
    for (std::vector<const TypedDagNode<double> * >::iterator it = siteRates.begin(); it != siteRates.end(); ++it) {
        this->addParameter( *it );
    }
    
    delete this->value;
    this->value = simulate(this->tau->getValue().getRoot());
}


template<class charType, class treeType>
RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::SiteRateHeterogeneousCharEvoModel(const SiteRateHeterogeneousCharEvoModel &n) : AbstractHeterogeneousCharEvoModel<charType, treeType>( n ), rateMatrix( n.rateMatrix ), transitionProbabilities( n.transitionProbabilities ), siteRates( n.siteRates ), transitionProbabilitySpace( charType().getNumberOfStates() ) {
    // parameters are automatically copied
    
}


template<class charType, class treeType>
RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::~SiteRateHeterogeneousCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>* RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::clone( void ) const {
    return new SiteRateHeterogeneousCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
const std::vector< RevBayesCore::TransitionProbabilityMatrix *>& RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::computeTransitionProbabilities( const TopologyNode &n ) {
    // get the transition probability matrices per site for the given node
    transitionProbabilities.clear();
    
    std::vector< const TypedDagNode<double> * >::iterator end = siteRates.end();
    std::map<double,TransitionProbabilityMatrix*> tpms;
    for (std::vector< const TypedDagNode<double> * >::iterator it = siteRates.begin(); it != end; ++it) {
        double rate = (*it)->getValue();
        std::map<double, TransitionProbabilityMatrix*>::iterator xxx = tpms.find( rate );
        if ( xxx == tpms.end() ) {
            // create new transition probability matrix
            TransitionProbabilityMatrix *tp = new TransitionProbabilityMatrix( charType().getNumberOfStates() );
            
            // compute
            rateMatrix->getValue().calculateTransitionProbabilities( n.getBranchLength() * (*it)->getValue(), *tp);

            // add it to the vector of matrices
            transitionProbabilities.push_back( tp );
            
            // add it to our lookup map
            tpms.insert( std::pair<double,TransitionProbabilityMatrix*>(rate,tp) );
        }
        else {
            // just add the pre-existing transition probability matrix
            transitionProbabilities.push_back( xxx->second );
        }
        
    }
    
    
    return transitionProbabilities;
}


template<class charType, class treeType>
const RevBayesCore::TransitionProbabilityMatrix& RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::computeTransitionProbabilities( const TopologyNode &n, size_t site ) {
    
    double rate = siteRates[site]->getValue();
    
    // compute
    rateMatrix->getValue().calculateTransitionProbabilities( n.getBranchLength() * rate, transitionProbabilitySpace);
    
    
    return transitionProbabilitySpace;
}


template<class charType, class treeType>
const std::vector< std::vector<double> *>& RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::getRootFrequencies( void ) {
    // get the different root frequency classes
    rootFrequencies.clear();
    
    // create base frequencies
    std::vector<double> *freqs = new std::vector<double>( rateMatrix->getValue().getStationaryFrequencies() );    
    rootFrequencies = std::vector<std::vector<double> *>( siteRates.size(),freqs );
    
    return rootFrequencies;
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::getRootFrequencies( size_t site ) {
    
    return rateMatrix->getValue().getStationaryFrequencies();
}


template<class charType, class treeType>
RevBayesCore::CharacterData<charType>* RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::simulate(const TopologyNode &node) {
    return new CharacterData<charType>();
}



template<class charType, class treeType>
void RevBayesCore::SiteRateHeterogeneousCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    size_t rateNum = siteRates.size();
    for (unsigned int i = 0 ; i < rateNum ; i++) {
        if (oldP == siteRates[i]) {
            siteRates[i] = static_cast<const TypedDagNode< double > * >( newP );
        }
        
    }
    if (oldP == rateMatrix) {
        rateMatrix = static_cast<const TypedDagNode<RateMatrix>* >( newP );
    }
    else {
        AbstractHeterogeneousCharEvoModel<charType, treeType>::swapParameter(oldP,newP);
    }
}


#endif
