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
class AscertainmentBiasCorrectionStruct;
double computeRootFilteredLikelihood2Nodes(const double *p_left,
                                   const double *p_right,
                                   const size_t numSiteRates,
                                   const double * rootFreq,
                                   const size_t numStates,
                                   const size_t * patternCounts,
                                   const size_t numPatterns,
                                   const size_t siteOffset,
                                   const size_t nodeIndex,
                                   const size_t mixtureOffset,
                                   const double p_inv,
                                   const std::vector<bool> & siteInvariant,
                                   const std::vector<size_t> & invariantSiteIndex,
                                   const AscertainmentBiasCorrectionStruct *ascLeft,
                                   const AscertainmentBiasCorrectionStruct *ascRight,
                                   double & uncorrectedLnProb,
                                   double & ascBiasLnProb
                                   );
double computeRootFilteredLikelihood3Nodes(const double *p_left,
                                   const double *p_right,
                                   const double *p_middle,
                                   const size_t numSiteRates,
                                   const double * rootFreq,
                                   const size_t numStates,
                                   const size_t * patternCounts,
                                   const size_t numPatterns,
                                   const size_t siteOffset,
                                   const size_t nodeIndex,
                                   const size_t mixtureOffset,
                                   const double p_inv,
                                   const std::vector<bool> & siteInvariant,
                                   const std::vector<size_t> & invariantSiteIndex,
                                   const AscertainmentBiasCorrectionStruct *ascLeft,
                                   const AscertainmentBiasCorrectionStruct *ascRight,
                                   const AscertainmentBiasCorrectionStruct *ascMiddle,
                                   double & uncorrectedLnProb,
                                   double & ascBiasLnProb);
void computeInternalNodeFilteredLikelihood(double * p_node,
                                            AscertainmentBiasCorrectionStruct *ascNode,
                                    const double *p_left,
                                    const double *p_right,
                                    const size_t numSiteRates,
                                    const size_t numStates,
                                    const size_t numPatterns,
                                    const size_t siteOffset,
                                    const size_t nodeIndex,
                                    const size_t mixtureOffset,
                                    const double ** tpMats,
                                   const AscertainmentBiasCorrectionStruct *ascLeft,
                                   const AscertainmentBiasCorrectionStruct *ascRight);
void computeTipNodeFilteredLikelihood(double * p_node,
                                            AscertainmentBiasCorrectionStruct *ascNode,
                               const size_t numSiteRates,
                               const size_t numStates,
                               const size_t numPatterns,
                               const size_t siteOffset,
                               const size_t nodeIndex,
                               const size_t mixtureOffset,
                               const double ** tpMats,
                               const std::vector<bool> &gap_node,
                               const std::vector<unsigned long> &char_node,
                               const bool usingAmbiguousCharacters);
    
    template<class charType, class treeType>
    class FilteredPhyloCTMCSiteHomogeneous : public AbstractPhyloCTMCSiteHomogeneous<charType, treeType> {
        
    public:
        FilteredPhyloCTMCSiteHomogeneous(const TypedDagNode< treeType > *t, size_t nChars, bool compressed, size_t nSites);
        virtual                                            ~FilteredPhyloCTMCSiteHomogeneous(void);                                                                   //!< Virtual destructor
        
        // public member functions
        FilteredPhyloCTMCSiteHomogeneous*                           clone(void) const;                                                                          //!< Create an independent clone
        
        
    protected:
        AscertainmentBiasCorrectionStruct * getAscBiasStruct(size_t nodeIndex) {
            const size_t actInd = this->activeLikelihood[nodeIndex];
            const size_t ascActStride = this->numNodes;
            return this->ascBiasCorrStructs[ascActStride*actInd + nodeIndex];
        }
        const AscertainmentBiasCorrectionStruct * getAscBiasStruct(size_t nodeIndex) const {
            const size_t actInd = this->activeLikelihood[nodeIndex];
            const size_t ascActStride = this->numNodes;
            return const_cast<const AscertainmentBiasCorrectionStruct *>(this->ascBiasCorrStructs[ascActStride*actInd + nodeIndex]);
        }
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r);
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
        void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
        void                                                computeTipLikelihood(const TopologyNode &node, size_t nIdx);
        
        
    private:        
        std::vector<AscertainmentBiasCorrectionStruct *> ascBiasCorrStructs;
        double uncorrectedLnProb;
        double ascBiasLnProb;
    };
    std::vector<AscertainmentBiasCorrectionStruct *> allocAscBiasCorrStructs(const size_t numCopies, const size_t numNodes, const size_t numStates, const size_t numMixtures=0);
    void freeAscBiasCorrStructs(std::vector<AscertainmentBiasCorrectionStruct *> &);
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
RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::FilteredPhyloCTMCSiteHomogeneous(const TypedDagNode<treeType> *t, size_t nChars, bool compressed, size_t nSites) : AbstractPhyloCTMCSiteHomogeneous<charType, treeType>(  t, nChars, 1, compressed, nSites ) {
    this->ascBiasCorrStructs = allocAscBiasCorrStructs(numActiveLikelihoods, this->numNodes, nChars);
}


template<class charType, class treeType>
RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::~FilteredPhyloCTMCSiteHomogeneous( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
    freeAscBiasCorrStructs(this->ascBiasCorrStructs);
}


template<class charType, class treeType>
RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>* RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::clone( void ) const {
    
    return new FilteredPhyloCTMCSiteHomogeneous<charType, treeType>( *this );
}



template<class charType, class treeType>
void RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right) {
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    const AscertainmentBiasCorrectionStruct * ascLeft = this->getAscBiasStruct(left);
    const AscertainmentBiasCorrectionStruct * ascRight = this->getAscBiasStruct(right);
    this->lnProb = computeRootFilteredLikelihood2Nodes(p_left,
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
                                                    this->invariantSiteIndex,
                                                    ascLeft,
                                                    ascRight,
                                                    this->uncorrectedLnProb,
                                                    this->ascBiasLnProb);
}


template<class charType, class treeType>
void RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle) {
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    const double* p_middle = this->partialLikelihoods + this->activeLikelihood[middle]*this->activeLikelihoodOffset + middle*this->nodeOffset;
    const AscertainmentBiasCorrectionStruct * ascLeft = this->getAscBiasStruct(left);
    const AscertainmentBiasCorrectionStruct * ascRight = this->getAscBiasStruct(right);
    const AscertainmentBiasCorrectionStruct * ascMiddle = this->getAscBiasStruct(middle);
    this->lnProb = computeRootFilteredLikelihood3Nodes(p_left,
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
                                                    this->invariantSiteIndex,
                                                    ascLeft,
                                                    ascRight,
                                                    ascMiddle,
                                                    this->uncorrectedLnProb,
                                                    this->ascBiasLnProb);
}


template<class charType, class treeType>
void RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right) {
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    const AscertainmentBiasCorrectionStruct * ascLeft = this->getAscBiasStruct(left);
    const AscertainmentBiasCorrectionStruct * ascRight = this->getAscBiasStruct(right);
    AscertainmentBiasCorrectionStruct * ascNode = this->getAscBiasStruct(nodeIndex);
    std::vector<const double *> tpMats(this->numSiteRates, NULL);
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture) {
        tpMats[mixture] = this->transitionProbMatrices[mixture].theMatrix;
    }
    computeInternalNodeFilteredLikelihood(p_node,
                                   ascNode,
                                              p_left,
                                              p_right,
                                              this->numSiteRates,
                                              this->numChars,
                                              this->numPatterns,
                                              this->siteOffset,
                                              this->mixtureOffset,
                                              &(tpMats[0]),
                                    ascLeft,
                                    ascRight);
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
    AscertainmentBiasCorrectionStruct * ascNode = this->getAscBiasStruct(nodeIndex);
    computeTipNodeFilteredLikelihood(p_node,
                                      ascNode,
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
