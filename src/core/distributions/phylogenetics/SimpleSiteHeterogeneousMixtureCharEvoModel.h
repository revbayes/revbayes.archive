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

#include "AbstractSiteHeterogeneousMixtureCharEvoModelSlow.h"
#include "CharacterData.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class SimpleSiteHeterogeneousMixtureCharEvoModel : public AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType> {
        
    public:
        SimpleSiteHeterogeneousMixtureCharEvoModel(const TypedDagNode<treeType> *t, const std::vector<const TypedDagNode<double> *> &sr, const TypedDagNode<RateMatrix> *rm, bool c, size_t nSites);
        SimpleSiteHeterogeneousMixtureCharEvoModel(const SimpleSiteHeterogeneousMixtureCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~SimpleSiteHeterogeneousMixtureCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        SimpleSiteHeterogeneousMixtureCharEvoModel*         clone(void) const;                                                                          //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        void                                                computeRootLikelihood(size_t rootIndex, size_t left, size_t right, size_t rate);
        void                                                computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right, size_t rate);
        void                                                computeTipLikelihood(const TopologyNode &node, size_t rate);
        
    private:
        CharacterData<charType>*                            simulate(const TopologyNode& node);
        
        // members
        const TypedDagNode<RateMatrix>*                     rateMatrix;
        TransitionProbabilityMatrix                         transitionProbabilities;        
        
    };
    
}


#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType, class treeType>
RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::SimpleSiteHeterogeneousMixtureCharEvoModel(const TypedDagNode<treeType> *t, const std::vector<const TypedDagNode<double> *> &sr, const TypedDagNode<RateMatrix> *rm, bool c, size_t nSites) : AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>(  t, sr, c, nSites ), rateMatrix( rm ), transitionProbabilities( rm->getValue().getNumberOfStates() ) {
    // add the parameters to the parents list
    this->addParameter( rateMatrix );
    
    delete this->value;
    this->value = simulate(this->tau->getValue().getRoot());
}


template<class charType, class treeType>
RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::SimpleSiteHeterogeneousMixtureCharEvoModel(const SimpleSiteHeterogeneousMixtureCharEvoModel &n) : AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>( n ), rateMatrix( n.rateMatrix ), transitionProbabilities( n.transitionProbabilities ) {
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
void RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t rate ) {
    
    // sum the per site probabilities
    const std::vector<double> &f = rateMatrix->getValue().getStationaryFrequencies();
    
    std::vector< double >             &p        = this->rootLikelihoods[rate];
    std::vector<std::vector<double> > &p_left   = this->partialLikelihoods[rate][left];
    std::vector<std::vector<double> > &p_right  = this->partialLikelihoods[rate][right];
    
    std::vector< double >::iterator                     p_site_root     = p.begin();
    std::vector<std::vector<double> >::const_iterator   p_site_left     = p_left.begin();
    std::vector<std::vector<double> >::const_iterator   end             = p_left.end();
    std::vector<std::vector<double> >::const_iterator   p_site_right    = p_right.begin();
    for (; p_site_left != end; ++p_site_left, ++p_site_right, ++p_site_root) {
        
        double tmp = 0.0;
        std::vector<double>::const_iterator f_j             = f.begin();
        std::vector<double>::const_iterator f_end           = f.end();
        std::vector<double>::const_iterator p_site_left_j   = p_site_left->begin();
        std::vector<double>::const_iterator p_site_right_j  = p_site_right->begin();
        for (; f_j != f.end(); ++f_j, ++p_site_left_j, ++p_site_right_j) {
            tmp += *p_site_left_j * *p_site_right_j * *f_j;
        }
        (*p_site_root) = log(tmp);
    }
}



template<class charType, class treeType>
void RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right, size_t rate) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    // get rate scaling factor
    double rateMultiplier = this->siteRates[rate]->getValue();
    
    // compute the transition probability matrix
    rateMatrix->getValue().calculateTransitionProbabilities( node.getBranchLength() * rateMultiplier, transitionProbabilities);
    
    std::vector<std::vector<double> > &p_left = this->partialLikelihoods[rate][left];
    std::vector<std::vector<double> > &p_right = this->partialLikelihoods[rate][right];
    std::vector<std::vector<double> > &p_node = this->partialLikelihoods[rate][nodeIndex];
    
    // compute the per site probabilities
    std::vector<std::vector<double> >::const_iterator   p_site_left     = p_left.begin();
    std::vector<std::vector<double> >::const_iterator   p_site_right    = p_right.begin();
    std::vector<std::vector<double> >::iterator         p_site          = p_node.begin();
    std::vector<std::vector<double> >::iterator         p_end           = p_node.end();
    for (; p_site != p_end; ++p_site, ++p_site_left, ++p_site_right) {
        
        // iterate over the possible starting states
        std::vector<double>::iterator                       p_a     = p_site->begin();
        std::vector<std::vector<double> >::const_iterator   tp_a    = transitionProbabilities.begin();
        std::vector<std::vector<double> >::const_iterator   tp_end  = transitionProbabilities.end();
        for (; tp_a != tp_end; ++tp_a, ++p_a) {
            
            double sum = 0.0;
            
            // iterate over all possible terminal states
            std::vector<double>::const_iterator p_site_left_d   = p_site_left->begin();
            std::vector<double>::const_iterator p_site_right_d  = p_site_right->begin();
            std::vector<double>::const_iterator tp_a_d          = tp_a->begin();
            std::vector<double>::const_iterator tp_a_end        = tp_a->end();
            for (; tp_a_d != tp_a_end; ++tp_a_d, ++p_site_left_d, ++p_site_right_d ) {
                sum += *p_site_left_d * *p_site_right_d * *tp_a_d;
            }
            *p_a = sum;
            
        }
    }
}


template<class charType, class treeType>
void RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::computeTipLikelihood(const TopologyNode &node, size_t rate) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    std::vector<std::vector<double> > &p_node = this->partialLikelihoods[rate][nodeIndex];
    
    std::vector<bool> &gap_node = this->gapMatrix[nodeIndex];
    
    std::vector<unsigned int> &char_node = this->charMatrix[nodeIndex];
    
    // get rate scaling factor
    double rateMultiplier = this->siteRates[rate]->getValue();
    
    // compute the transition probabilities
    rateMatrix->getValue().calculateTransitionProbabilities( node.getBranchLength() * rateMultiplier, transitionProbabilities );
    
    // compute the per site probabilities
    std::vector<std::vector<double> >::iterator p_site      = p_node.begin();
    std::vector<std::vector<double> >::iterator p_end       = p_node.end();
    std::vector<bool>::const_iterator           gap_site    = gap_node.begin();
    std::vector<unsigned int>::const_iterator   char_site   = char_node.begin();
    for (; p_site != p_end; ++p_site, ++char_site, ++gap_site) {
        
        if ( *gap_site == true ) {
            std::vector<double>::iterator                       p_a     = p_site->begin();
            std::vector<std::vector<double> >::const_iterator   tp_a    = transitionProbabilities.begin();
            std::vector<std::vector<double> >::const_iterator   tp_end  = transitionProbabilities.end();
            for (; tp_a != tp_end; ++tp_a, ++p_a) {
                
                double tmp = 0.0;
                
                std::vector<double>::const_iterator d   = tp_a->begin();
                std::vector<double>::const_iterator end = tp_a->end();
                for (; d != end; ++d) {
                    tmp += *d;
                }
                *p_a = tmp;
            } 
        }
        else {
            unsigned int org_val = *char_site;
            std::vector<double>::iterator                       p_a     = p_site->begin();
            std::vector<std::vector<double> >::const_iterator   tp_a    = transitionProbabilities.begin();
            std::vector<std::vector<double> >::const_iterator   tp_end  = transitionProbabilities.end();
            for (; tp_a != tp_end; ++tp_a, ++p_a) {
                
                double tmp = 0.0;
                
                unsigned int val = org_val;
                std::vector<double>::const_iterator d   = tp_a->begin();
                std::vector<double>::const_iterator end = tp_a->end();
                for (; d != end; ++d) {
                    if ( (val & 1) == 1 ) {
                        tmp += *d;
                    }
                    
                    val >>= 1;
                }
                *p_a = tmp;
            }
        }
    }
    
}




template<class charType, class treeType>
RevBayesCore::CharacterData<charType>* RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::simulate(const TopologyNode &node) {
    return new CharacterData<charType>();
}



template<class charType, class treeType>
void RevBayesCore::SimpleSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == rateMatrix) {
        rateMatrix = static_cast<const TypedDagNode<RateMatrix>* >( newP );
    }
    else {
        AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::swapParameter(oldP,newP);
    }
}


#endif
