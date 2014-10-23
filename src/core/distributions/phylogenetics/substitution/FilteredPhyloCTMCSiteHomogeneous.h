#ifndef FilteredPhyloCTMCSiteHomogeneous_H
#define FilteredPhyloCTMCSiteHomogeneous_H

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class FilteredPhyloCTMCSiteHomogeneous : public AbstractPhyloCTMCSiteHomogeneous<charType, treeType> {
        
    public:
        FilteredPhyloCTMCSiteHomogeneous(const TypedDagNode< treeType > *t, size_t nChars, bool c, size_t nSites);
        virtual                                            ~FilteredPhyloCTMCSiteHomogeneous(void);                                                                   //!< Virtual destructor
        
        // public member functions
        FilteredPhyloCTMCSiteHomogeneous*                           clone(void) const;                                                                          //!< Create an independent clone
        
        
    protected:
        
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r);
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
        void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
        void                                                computeTipLikelihood(const TopologyNode &node, size_t nIdx);
        
        
    private:        

    
    
    };
    
}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType, class treeType>
RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::FilteredPhyloCTMCSiteHomogeneous(const TypedDagNode<treeType> *t, size_t nChars, bool c, size_t nSites) : AbstractPhyloCTMCSiteHomogeneous<charType, treeType>(  t, nChars, 1, c, nSites )
{
    
}


template<class charType, class treeType>
RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::~FilteredPhyloCTMCSiteHomogeneous( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>* RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::clone( void ) const {
    
    return new FilteredPhyloCTMCSiteHomogeneous<charType, treeType>( *this );
}



template<class charType, class treeType>
void RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right) {
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    this->lnProb = homogeneousComputeRootLikelihood(p_left,
                                                    p_right,
                                                    this->numSiteRates,
                                                    &(this->getRootFrequencies()[0]),
                                                    this->numChars,
                                                    &(this->patternCounts[0]),
                                                    this->numPatterns,
                                                    this->siteOffset,
                                                    this->mixtureOffset,
                                                    this->pInv->getValue(),
                                                    this->siteInvariant,
                                                    this->invariantSiteIndex);
}


template<class charType, class treeType>
void RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle) {
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    const double* p_middle = this->partialLikelihoods + this->activeLikelihood[middle]*this->activeLikelihoodOffset + middle*this->nodeOffset;
    this->lnProb = homogeneousComputeRootLikelihood(p_left,
                                                    p_right,
                                                    p_middle,
                                                    this->numSiteRates,
                                                    &(this->getRootFrequencies()[0]),
                                                    this->numChars,
                                                    &(this->patternCounts[0]),
                                                    this->numPatterns,
                                                    this->siteOffset,
                                                    this->mixtureOffset,
                                                    this->pInv->getValue(),
                                                    this->siteInvariant,
                                                    this->invariantSiteIndex);
}


template<class charType, class treeType>
void RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right) {
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    std::vector<const double *> tpMats(this->numSiteRates, NULL);
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture) {
        tpMats[mixture] = this->transitionProbMatrices[mixture].theMatrix;
    }
    homogeneousComputeInternalNodeLikelihoood(p_node,
                                              p_left,
                                              p_right,
                                              this->numSiteRates,
                                              this->numChars,
                                              this->numPatterns,
                                              this->siteOffset,
                                              this->mixtureOffset,
                                              &(tpMats[0]));
}

template<class charType, class treeType>
void RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex) {
    // compute the transition probabilities
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    std::vector<const double *> tpMats(this->numSiteRates, NULL);
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture) {
        tpMats[mixture] = this->transitionProbMatrices[mixture].theMatrix;
    }
    homogeneousComputeTipNodeLikelihoood(p_node,
                                           this->numSiteRates,
                                           this->numChars,
                                           this->numPatterns,
                                           this->siteOffset,
                                           this->mixtureOffset,
                                           &(tpMats[0]),
                                          this->gapMatrix[nodeIndex],
                                          this->charMatrix[nodeIndex],
                                          this->usingAmbiguousCharacters);
}


#endif
