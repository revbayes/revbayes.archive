#ifndef PhyloCTMCSiteHomogeneousConditional_H
#define PhyloCTMCSiteHomogeneousConditional_H

#include "PhyloCTMCSiteHomogeneous.h"
#include "DistributionBinomial.h"
#include "DistributionNegativeBinomial.h"
#include "RbMathFunctions.h"
#include "RlUserInterface.h"
#include "TransitionProbabilityMatrix.h"

namespace RevBayesCore {

    struct AscertainmentBias {
        
        enum Coding { ALL               = 0x00,
                      VARIABLE          = 0x01,
                      NOSINGLETONS      = 0x02,
                      INFORMATIVE       = 0x03
                    };
    };

    template<class charType>
    class PhyloCTMCSiteHomogeneousConditional : public PhyloCTMCSiteHomogeneous<charType> {

    public:
        PhyloCTMCSiteHomogeneousConditional(const TypedDagNode< Tree > *t, size_t nChars, bool c, size_t nSites, bool amb, AscertainmentBias::Coding cod = AscertainmentBias::ALL);
        PhyloCTMCSiteHomogeneousConditional(const PhyloCTMCSiteHomogeneousConditional &n);
        virtual                                            ~PhyloCTMCSiteHomogeneousConditional(void);                                                                   //!< Virtual destructor

        // public member functions
        PhyloCTMCSiteHomogeneousConditional*                clone(void) const;                                                                        //!< Create an independent clone
        virtual void                                        redrawValue(void);

    protected:

        virtual void                                        computeRootLikelihood(size_t root, size_t l, size_t r);
        virtual void                                        computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
        virtual void                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
        virtual void                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
        virtual void                                        computeTipLikelihood(const TopologyNode &node, size_t nIdx);

        virtual void                                        computeRootCorrection(size_t root, size_t l, size_t r);
        virtual void                                        computeRootCorrection(size_t root, size_t l, size_t r, size_t m);
        virtual void                                        computeInternalNodeCorrection(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
        virtual void                                        computeInternalNodeCorrection(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
        virtual void                                        computeTipCorrection(const TopologyNode &node, size_t nIdx);

        virtual void                                        resizeLikelihoodVectors(void);

        int                                                 coding;
        size_t                                              N;

        size_t                                              numCorrectionMasks;
        size_t                                              activeCorrectionOffset;
        size_t                                              correctionNodeOffset;
        size_t                                              correctionMixtureOffset;
        size_t                                              correctionMaskOffset;
        size_t                                              correctionOffset;

        std::vector<double>                                 correctionLikelihoods;

        std::vector<std::vector<bool> >                     correctionMaskMatrix;
        std::vector<size_t>                                 correctionMaskCounts;
        std::vector<size_t>                                 maskObservationCounts;
        std::vector<double>                                 perMaskMixtureCorrections;
        
        virtual double                                      sumRootLikelihood( void );
        virtual bool                                        isSitePatternCompatible( std::map<size_t, size_t> );
        std::vector<size_t>                                 getIncludedSiteIndices( void );
        void                                                updateCorrections( const TopologyNode& node, size_t nodeIndex );

    private:
        virtual void                                        simulate( const TopologyNode &node, std::vector<charType> &taxa, size_t rateIndex, std::map<size_t, size_t>& charCounts);
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

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::PhyloCTMCSiteHomogeneousConditional(const TypedDagNode<Tree> *t, size_t nChars, bool c, size_t nSites, bool amb, AscertainmentBias::Coding ty) :
    PhyloCTMCSiteHomogeneous<charType>(  t, nChars, c, nSites, amb ), coding(ty), N(nSites), numCorrectionMasks(0)
{
    if(coding != AscertainmentBias::ALL)
    {
        numCorrectionMasks      = 1;
        correctionMaskMatrix    = std::vector<std::vector<bool> >(1, std::vector<bool>(this->num_nodes,0) );
        
        correctionOffset        = this->numChars*this->numChars;
        correctionMaskOffset    = correctionOffset*2;
        correctionMixtureOffset = numCorrectionMasks*correctionMaskOffset;
        correctionNodeOffset    = this->numSiteRates*correctionMixtureOffset;
        activeCorrectionOffset  = this->num_nodes*correctionNodeOffset;

        correctionLikelihoods = std::vector<double>(activeCorrectionOffset*2, 0.0);

        perMaskMixtureCorrections = std::vector<double>(this->numSiteRates*numCorrectionMasks, 0.0);
    }
}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::~PhyloCTMCSiteHomogeneousConditional( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>* RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::clone( void ) const {

    return new PhyloCTMCSiteHomogeneousConditional<charType>( *this );
}

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::PhyloCTMCSiteHomogeneousConditional(const PhyloCTMCSiteHomogeneousConditional &n) :
    PhyloCTMCSiteHomogeneous< charType>( n ),
    coding(n.coding),
    N(n.N),
    numCorrectionMasks(n.numCorrectionMasks),
    activeCorrectionOffset(n.activeCorrectionOffset),
    correctionNodeOffset(n.activeCorrectionOffset),
    correctionMixtureOffset(n.activeCorrectionOffset),
    correctionMaskOffset(n.correctionMaskOffset),
    correctionOffset(n.correctionOffset),
    correctionMaskMatrix(n.correctionMaskMatrix),
    correctionMaskCounts(n.correctionMaskCounts),
    maskObservationCounts(n.maskObservationCounts),
    perMaskMixtureCorrections(n.perMaskMixtureCorrections)
{
    correctionLikelihoods = n.correctionLikelihoods;
}

template<class charType>
std::vector<size_t> RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::getIncludedSiteIndices( void )
{
    correctionMaskCounts.clear();
    maskObservationCounts.clear();
    correctionMaskMatrix.clear();

    // find the unique site patterns and compute their respective frequencies
    std::vector<TopologyNode*> nodes = this->tau->getValue().getNodes();
    size_t tips = this->tau->getValue().getNumberOfTips();

    // create a vector with the correct site indices
    // some of the sites may have been excluded
    std::vector<size_t> siteIndices;
    size_t siteIndex = 0;
    size_t incompatible = 0;

    std::map<std::string, size_t> maskIndices;

    if(coding != AscertainmentBias::ALL)
    {
        // if we are using a correction, add a correction mask with 0 gaps.
        // it is required when simulating data, but not necessarily used
        // in computing the likelihood (e.g. if all sites have at least one gap)

        std::string gapless(tips, ' ');
        std::vector<bool> mask(tips, false);

        maskIndices[gapless] = 0;

        correctionMaskCounts.push_back(0);
        maskObservationCounts.push_back(tips);
        correctionMaskMatrix.push_back(mask);
    }

    this->num_sites = this->value->getNumberOfIncludedCharacters();

    for (size_t i = 0; i < this->num_sites; ++i)
    {
        while ( this->value->isCharacterExcluded(siteIndex) )
        {
            siteIndex++;
            if ( siteIndex >= this->value->getNumberOfCharacters()  )
            {
                throw RbException( "The character matrix cannot set to this variable because it does not have enough included characters." );
            }
        }

        std::map<size_t, size_t> charCounts;
        size_t numGap = 0;

        std::string mask = "";
        std::vector<bool> maskData;
        for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            if ( (*it)->isTip() )
            {
                AbstractDiscreteTaxonData& taxon = this->value->getTaxonData( (*it)->getName() );
                DiscreteCharacterState &c = taxon.getCharacter(siteIndex);

                bool gap = false;
                // if we treat unknown characters as gaps and this is an unknown character then we change it
                // because we might then have a pattern more
                if ( this->treatAmbiguousAsGaps && (c.isAmbiguous() || c.isMissingState()) )
                {
                    gap = true;
                }
                else if ( this->treatUnknownAsGap && (c.getNumberOfStates() == c.getNumberObservedStates() || c.isMissingState()) )
                {
                    gap = true;
                }

                if(gap)
                {
                    if(coding != AscertainmentBias::ALL)
                        mask += "-";
                    numGap++;
                }
                else
                {
                    if(coding != AscertainmentBias::ALL)
                        mask += " ";
                    
                    charCounts[c.getStateIndex()]++;
                }

                if(coding != AscertainmentBias::ALL)
                    maskData.push_back(gap);
            }
        }

        if( !isSitePatternCompatible(charCounts) )
        {
            incompatible++;
        }
        else
        {
            siteIndices.push_back(siteIndex);

            if(coding != AscertainmentBias::ALL)
            {
                // increase the count for this mask
                std::map<std::string, size_t>::iterator it = maskIndices.find(mask);
                if(it != maskIndices.end())
                {
                    correctionMaskCounts[it->second]++;
                }
                else
                {
                    maskIndices[mask] = correctionMaskCounts.size();

                    correctionMaskCounts.push_back(1);
                    maskObservationCounts.push_back(tips - numGap);
                    correctionMaskMatrix.push_back(maskData);
                }
            }
        }

        siteIndex++;
    }

    // test if there were additional sites that we did not use
    while ( siteIndex < this->value->getNumberOfCharacters() )
    {
        if ( !this->value->isCharacterExcluded(siteIndex)  )
        {
            throw RbException( "The character matrix cannot set to this variable because it has too many included characters." );
        }
        siteIndex++;
    }

    // warn if we have to exclude some incompatible characters
    if(incompatible > 0)
    {
        std::stringstream ss;
        ss << "WARNING: There are " << incompatible << " characters incompatible with the specified coding bias. These characters will be excluded.";
        RBOUT(ss.str());

        // resize our datset to account for the newly excluded characters
        this->num_sites = siteIndices.size();
        this->sitePattern.clear();
        this->sitePattern.resize(this->num_sites);
    }

    // readjust the number of correction sites to account for masked sites
    if(coding != AscertainmentBias::ALL)
    {
        numCorrectionMasks = correctionMaskCounts.size();
    }

    return siteIndices;
}

template<class charType>
bool RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::isSitePatternCompatible( std::map<size_t, size_t> charCounts )
{
    bool singleton = false;
    if(charCounts.size() == 2)
    {
        for(std::map<size_t, size_t>::iterator it = charCounts.begin(); it != charCounts.end(); it++)
        {
            if(it->second == 1)
            {
                singleton = true;
                break;
            }
        }
    }

    if( (charCounts.size() == 1 && (coding & AscertainmentBias::VARIABLE)) || (singleton && (coding & AscertainmentBias::NOSINGLETONS)) )
    {
        return false;
    }
    
    return true;
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeRootLikelihood( size_t root, size_t left, size_t right)
{

    PhyloCTMCSiteHomogeneous<charType>::computeRootLikelihood(root, left, right);

    if(coding != AscertainmentBias::ALL)
    {
        computeRootCorrection(root, left, right);
    }
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{

    PhyloCTMCSiteHomogeneous<charType>::computeRootLikelihood(root, left, right, middle);

    if(coding != AscertainmentBias::ALL)
    {
        computeRootCorrection(root, left, right, middle);
    }
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{

    PhyloCTMCSiteHomogeneous<charType>::computeInternalNodeLikelihood(node, nodeIndex, left, right);

    if(coding != AscertainmentBias::ALL)
    {
        computeInternalNodeCorrection(node, nodeIndex, left, right);
    }
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{

    PhyloCTMCSiteHomogeneous<charType>::computeInternalNodeLikelihood(node, nodeIndex, left, right, middle);

    if(coding != AscertainmentBias::ALL)
    {
        computeInternalNodeCorrection(node, nodeIndex, left, right, middle);
    }
}




template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
{

    PhyloCTMCSiteHomogeneous<charType>::computeTipLikelihood(node, nodeIndex);

    if(coding != AscertainmentBias::ALL)
    {
        computeTipCorrection(node, nodeIndex);
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeTipCorrection(const TopologyNode &node, size_t nodeIndex)
{
    std::vector<double>::iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*this->activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        TransitionProbabilityMatrix    pij = this->transitionProbMatrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator         u      = p_node + offset;

            bool gap = correctionMaskMatrix[mask][nodeIndex];

            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*this->numChars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;

                for(size_t c = 0; c < this->numChars; c++)
                {
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;

                    for(size_t cj = 0; cj < this->numChars; cj++)
                    {
                        // Probability of constant state c at this tip and node state cj
                        // given ancestral state ci
                        uC_i[c] += ((c == cj) && !gap) * pij[ci][cj];

                        // Probability of invert singleton state c at this tip and node state cj
                        // given ancestral state ci
                        uI_i[c] += ((c != cj) && !gap) * pij[ci][cj];
                    }
                }
            }
        }

    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        TransitionProbabilityMatrix    pij = this->transitionProbMatrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator                 u = p_node   + offset;
            std::vector<double>::const_iterator         u_l = p_left   + offset;
            std::vector<double>::const_iterator         u_r = p_right  + offset;
            std::vector<double>::const_iterator         u_m = p_middle + offset;
            
            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*this->numChars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;
                
                for(size_t c = 0; c < this->numChars; c++)
                {
                                
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;
                                        
                    for(size_t cj = 0; cj < this->numChars; cj++)
                    {
                        std::vector<double>::const_iterator         lC_j = u_l  + cj*this->numChars;
                        std::vector<double>::const_iterator         lI_j = lC_j + correctionOffset;

                        std::vector<double>::const_iterator         rC_j = u_r  + cj*this->numChars;
                        std::vector<double>::const_iterator         rI_j = rC_j + correctionOffset;

                        std::vector<double>::const_iterator         mC_j = u_m  + cj*this->numChars;
                        std::vector<double>::const_iterator         mI_j = mC_j + correctionOffset;

                        double Pij = pij[ci][cj];
                                
                        // probability of constant state c descending from this node
                        // given ancestral state ci and node state cj
                        uC_i[c] += Pij * lC_j[c] * rC_j[c] * mC_j[c];

                        // probability of invert singleton state c descending from
                        // given ancestral state ci and node state cj
                        uI_i[c] += Pij * lI_j[c] * rC_j[c] * mC_j[c]
                                 + Pij * lC_j[c] * rI_j[c] * mC_j[c]
                                 + Pij * lC_j[c] * rC_j[c] * mI_j[c];
                    }
                }
            }
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        TransitionProbabilityMatrix    pij = this->transitionProbMatrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator                 u = p_node   + offset;
            std::vector<double>::const_iterator         u_l = p_left   + offset;
            std::vector<double>::const_iterator         u_r = p_right  + offset;

            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*this->numChars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;
                
                for(size_t c = 0; c < this->numChars; c++)
                {

                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;

                    for(size_t cj = 0; cj < this->numChars; cj++)
                    {
                        std::vector<double>::const_iterator         lC_j = u_l  + cj*this->numChars;
                        std::vector<double>::const_iterator         lI_j = lC_j + correctionOffset;

                        std::vector<double>::const_iterator         rC_j = u_r  + cj*this->numChars;
                        std::vector<double>::const_iterator         rI_j = rC_j + correctionOffset;

                        double Pij = pij[ci][cj];

                        // probability of constant state c descending from this node
                        // given ancestral state ci and node state cj
                        uC_i[c] += Pij * lC_j[c] * rC_j[c];

                        // probability of invert singleton state c descending from
                        // given ancestral state ci and node state cj
                        uI_i[c] += Pij * lI_j[c] * rC_j[c]
                                 + Pij * lC_j[c] * rI_j[c];
                    }
                }
            }
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeRootCorrection( size_t root, size_t left, size_t right, size_t middle)
{
    const std::vector<double> &f = this->getRootFrequencies();

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[root]*activeCorrectionOffset + root*correctionNodeOffset;
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator                 u = p_node   + offset;
            std::vector<double>::const_iterator         u_l = p_left   + offset;
            std::vector<double>::const_iterator         u_r = p_right  + offset;
            std::vector<double>::const_iterator         u_m = p_middle + offset;
            
            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*this->numChars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;

                std::vector<double>::const_iterator         lC_i = u_l  + ci*this->numChars;
                std::vector<double>::const_iterator         lI_i = lC_i + correctionOffset;

                std::vector<double>::const_iterator         rC_i = u_r  + ci*this->numChars;
                std::vector<double>::const_iterator         rI_i = rC_i + correctionOffset;

                std::vector<double>::const_iterator         mC_i = u_m  + ci*this->numChars;
                std::vector<double>::const_iterator         mI_i = mC_i + correctionOffset;

                for(size_t c = 0; c < this->numChars; c++)
                {
                                
                    // probability of constant state c descending from this node
                    // given ancestral state ci
                    uC_i[c] = f[ci] * lC_i[c] * rC_i[c] * mC_i[c];

                    // probability of invert singleton state c descending from
                    // given ancestral state ci
                    uI_i[c] = f[ci] * lI_i[c] * rC_i[c] * mC_i[c]
                            + f[ci] * lC_i[c] * rI_i[c] * mC_i[c]
                            + f[ci] * lC_i[c] * rC_i[c] * mI_i[c];
                }
            }
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeRootCorrection( size_t root, size_t left, size_t right)
{
    const std::vector<double> &f = this->getRootFrequencies();

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[root]*activeCorrectionOffset + root*correctionNodeOffset;
    std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator                 u = p_node   + offset;
            std::vector<double>::const_iterator         u_l = p_left   + offset;
            std::vector<double>::const_iterator         u_r = p_right  + offset;

            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*this->numChars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;

                std::vector<double>::const_iterator         lC_i = u_l  + ci*this->numChars;
                std::vector<double>::const_iterator         lI_i = lC_i + correctionOffset;

                std::vector<double>::const_iterator         rC_i = u_r  + ci*this->numChars;
                std::vector<double>::const_iterator         rI_i = rC_i + correctionOffset;

                for(size_t c = 0; c < this->numChars; c++)
                {
                                
                    // probability of constant state c descending from this node
                    // given ancestral state ci
                    uC_i[c] = f[ci] * lC_i[c] * rC_i[c];

                    // probability of invert singleton state c descending from
                    // given ancestral state ci
                    uI_i[c] = f[ci] * lI_i[c] * rC_i[c]
                            + f[ci] * lC_i[c] * rI_i[c];
                }
            }
        }
    }
}

template<class charType>
double RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::sumRootLikelihood( void )
{
    double sumPartialProbs = PhyloCTMCSiteHomogeneous<charType>::sumRootLikelihood();
    
    if(coding == AscertainmentBias::ALL)
        return sumPartialProbs;
    
    
    // get the root node
    const TopologyNode &root = this->tau->getValue().getRoot();

    // get the index of the root node
    size_t nodeIndex = root.getIndex();
    
    std::vector<double>::const_iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex] * activeCorrectionOffset  + nodeIndex*correctionNodeOffset;
    
    std::vector<double> perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);
    
    // iterate over each correction mask
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::const_iterator         u_i = p_node   + offset;
            
            double prob = 0.0;
            
            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                // constant site pattern likelihoods
                std::vector<double>::const_iterator         uC_i = u_i  + ci*this->numChars;
                // invert singleton likelihoods
                std::vector<double>::const_iterator         uI_i = uC_i + correctionOffset; 

                for(size_t c = 0; c < this->numChars; c++)
                {  
                    // probability of constant state c at tips descending from this node
                    if(coding & AscertainmentBias::VARIABLE)
                        prob += uC_i[c];

                    // probability of invert singleton state c at tips descending from this node.
                    // if there is only one observed tip, then don't double-count constant site patterns
                    if((coding & AscertainmentBias::NOSINGLETONS) && maskObservationCounts[mask] > 1)
                    {
                        // if there are only two observed tips, then don't double-count variable site patterns
                        double p = maskObservationCounts[mask] > 2 ? 0.5 : 1.0;
                        
                        prob += uI_i[c] / p;
                    }
                }
            }

            perMaskCorrections[mask] += prob;
        

            double mixprob = prob;

            // add corrections for invariant sites
            double p_inv = this->pInv->getValue();
            if(p_inv > 0.0)
            {
                mixprob *= (1.0 - p_inv);
        
                if(coding & AscertainmentBias::VARIABLE)
                {
                    mixprob += p_inv;
                }
            }
            
            // invert the probability
            mixprob = 1.0 - mixprob;
            
            // correct rounding errors
            if(mixprob <= 0.0)
                mixprob = 0.0;
        
            perMaskMixtureCorrections[mask*this->numSiteRates + mixture] = mixprob;
        }
        
        // add corrections for invariant sites
        double p_inv = this->pInv->getValue();
        if(p_inv > 0.0)
        {
            perMaskCorrections[mask] *= (1.0 - p_inv);

            if(coding & AscertainmentBias::VARIABLE)
            {
                perMaskCorrections[mask] += p_inv * this->numSiteRates;
            }
        }

        // normalize and invert the log-probability
        perMaskCorrections[mask] = 1.0 - perMaskCorrections[mask]/this->numSiteRates;

        if(perMaskCorrections[mask] <= 0.0)
            perMaskCorrections[mask] = RbConstants::Double::inf;

        perMaskCorrections[mask] = log(perMaskCorrections[mask]);
        
        // apply the correction for this correction mask
        sumPartialProbs -= perMaskCorrections[mask]*correctionMaskCounts[mask];
    }

    return sumPartialProbs;
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::resizeLikelihoodVectors( void ) {

    RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::resizeLikelihoodVectors();
    
    if(coding != AscertainmentBias::ALL)
    {
        correctionOffset        = this->numChars*this->numChars;
        correctionMaskOffset    = correctionOffset*2;
        correctionMixtureOffset = numCorrectionMasks*correctionMaskOffset;
        correctionNodeOffset    = this->numSiteRates*correctionMixtureOffset;
        activeCorrectionOffset  = this->num_nodes*correctionNodeOffset;
    
        correctionLikelihoods = std::vector<double>(activeCorrectionOffset*2, 0.0);
        
        perMaskMixtureCorrections   = std::vector<double>(this->numSiteRates*numCorrectionMasks, 0.0);
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::updateCorrections( const TopologyNode& node, size_t nodeIndex ) {

    // only necessary if the node is actually dirty
    //if ( dirtyNodes[nodeIndex] )
    //{
    //    dirtyNodes[nodeIndex] = false;

        // start by filling the likelihood vector for the children of the root
        if( node.isTip() )
        {
            this->updateTransitionProbabilities(nodeIndex, node.getBranchLength() );
            computeTipCorrection( node, nodeIndex );
        }
        else if ( node.getNumberOfChildren() == 2 ) // rooted trees have two children for the root
        {
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            updateCorrections( left, leftIndex );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            updateCorrections( right, rightIndex );



            if(node.isRoot())
                computeRootCorrection( nodeIndex, leftIndex, rightIndex );
            else
            {
                this->updateTransitionProbabilities(nodeIndex, node.getBranchLength() );
                computeInternalNodeCorrection( node, nodeIndex, leftIndex, rightIndex );
            }

        }
        else if ( node.getNumberOfChildren() == 3 ) // unrooted trees have three children for the root
        {
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            updateCorrections( left, leftIndex );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            updateCorrections( right, rightIndex );
            const TopologyNode &middle = node.getChild(2);
            size_t middleIndex = middle.getIndex();
            updateCorrections( middle, middleIndex );

            if(node.isRoot())
                computeRootCorrection( nodeIndex, leftIndex, rightIndex, middleIndex );
            else
            {
                this->updateTransitionProbabilities(nodeIndex, node.getBranchLength() );
                computeInternalNodeCorrection( node, nodeIndex, leftIndex, rightIndex, middleIndex );
            }

        }
        else
        {
            throw RbException("Node has an unexpected number of children. Only 2 or 3 are allowed.");
        }
    //}
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::redrawValue( void ) {

    if(coding == AscertainmentBias::ALL)
    {
        PhyloCTMCSiteHomogeneous<charType>::redrawValue();
        
        return;
    }

    // delete the old value first
    delete this->value;

    // create a new character data object
    this->value = new HomologousDiscreteCharacterData<charType>();

    size_t numTips = this->tau->getValue().getNumberOfTips();
    size_t num_nodes = this->tau->getValue().getNumberOfNodes();

    RandomNumberGenerator* rng = GLOBAL_RNG;

    const TopologyNode &root = this->tau->getValue().getRoot();
    size_t rootIndex = this->tau->getValue().getRoot().getIndex();

    updateCorrections(root, rootIndex);

    std::vector< DiscreteTaxonData<charType> > taxa = std::vector< DiscreteTaxonData<charType> >(numTips, DiscreteTaxonData<charType>( Taxon("") ) );

    /*// first sample a total number of characters (M) from the marginal posterior:
    // M - N | N ~ NegBinomial(N+1, exp(lnCorrection) )
    double M_minus_N = RbStatistics::NegativeBinomial::rv(N + 1, exp(perMaskCorrections[0]), *rng);

    // then sample the observed number of characters (num_sites) from the likelihood:
    // num_sites | M ~ Binomial(M, exp(lnCorrection) )
    this->num_sites = RbStatistics::Binomial::rv( M_minus_N + N, exp(perMaskCorrections[0]), *rng);*/

    // sample the rate categories in proportion to the total probability (correction) for each mixture.
    double total = 0.0;
    for ( size_t i = 0; i < this->numSiteRates; ++i )
        total += perMaskMixtureCorrections[i];

    std::vector<size_t> perSiteRates;
    for ( size_t i = 0; i < this->num_sites; ++i )
    {
        // draw the state
        double u = rng->uniform01()*total;
        size_t rateIndex = 0;

        double tmp = 0.0;
        while(tmp < u){
            tmp += perMaskMixtureCorrections[rateIndex];
            if(tmp < u)
                rateIndex++;
        }

        perSiteRates.push_back( rateIndex );
    }
    
    const std::vector< double > &freqs = this->getRootFrequencies();

    // then sample site-patterns using rejection sampling,
    // rejecting those that match the unobservable ones.
    for ( size_t i = 0; i < this->num_sites; i++ )
    {
        size_t rateIndex = perSiteRates[i];

        std::vector<charType> siteData(num_nodes, charType());

        // create the character
        charType &c = siteData[rootIndex];
        c.setToFirstState();
        // draw the state
        double u = rng->uniform01();
        size_t stateIndex = 0;
        while ( true )
        {
            u -= freqs[stateIndex];
            ++stateIndex;

            if ( u > 0.0 && stateIndex < this->numChars)
            {
                ++c;
            }
            else
            {
                break;
            }

        }

        // recursively simulate the sequences
        std::map<size_t, size_t> charCounts;
        simulate( root, siteData, rateIndex, charCounts);

        if( !isSitePatternCompatible(charCounts) )
        {
            i--;
            continue;
        }

        // add the taxon data to the character data
        for (size_t t = 0; t < numTips; ++t)
        {
            taxa[t].addCharacter(siteData[t]);
        }
    }

    // add the taxon data to the character data
    for (size_t i = 0; i < this->tau->getValue().getNumberOfTips(); ++i)
    {
        taxa[i].setTaxon( this->tau->getValue().getNode(i).getTaxon() );
        this->value->addTaxonData( taxa[i] );
    }

    // compress the data and initialize internal variables
    this->reInitialized();

    for (std::vector<bool>::iterator it = this->dirtyNodes.begin(); it != this->dirtyNodes.end(); ++it)
    {
        (*it) = true;
    }

    // flip the active likelihood pointers
    for (size_t index = 0; index < this->changedNodes.size(); ++index)
    {
        if ( this->changedNodes[index] == false )
        {
            this->activeLikelihood[index] = (this->activeLikelihood[index] == 0 ? 1 : 0);
            this->changedNodes[index] = true;
        }
    }

}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::simulate( const TopologyNode &node, std::vector<charType> &data, size_t rateIndex, std::map<size_t, size_t>& charCounts) {

    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();

    // get the sequence of this node
    size_t nodeIndex = node.getIndex();
    charType &parentState = data[ nodeIndex ];

    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);

        // update the transition probability matrix
        this->updateTransitionProbabilities( child.getIndex(), child.getBranchLength() );

        unsigned long cp = parentState.getStateIndex();

        double *freqs = this->transitionProbMatrices[ rateIndex ][ cp ];

        // create the character
        charType &c = data[ child.getIndex() ];
        c.setToFirstState();
        // draw the state
        double u = rng->uniform01();
        size_t stateIndex = 0;
        while ( true )
        {
            u -= *freqs;
            ++stateIndex;

            if ( u > 0.0 && stateIndex < this->numChars)
            {
                ++c;
                ++freqs;
            }
            else
            {
                break;
            }

        }

        if(child.isTip())
            charCounts[c.getStateIndex()]++;
        else
            simulate( child, data, rateIndex, charCounts);
    }

}

#endif
