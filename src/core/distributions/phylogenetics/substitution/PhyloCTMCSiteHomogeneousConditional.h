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
        void                                                redrawValue(void);

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
        std::vector<double>                                 perMaskCorrections;
        std::vector<std::vector<double> >                   perMixtureCorrections;
        
        std::vector< std::vector< std::vector<double> > >   perNodeCorrectionLogScalingFactors;
        
        void                                                simulateConditional( const TopologyNode &node, std::vector<charType> &taxa, size_t rateIndex, std::map<std::string, size_t>& charCounts);
        virtual double                                      sumRootLikelihood( void );
        virtual bool                                        isSitePatternCompatible( std::map<std::string, size_t> );
        std::vector<size_t>                                 getIncludedSiteIndices( void );
        
    private:
        void                                                scaleCorrection(size_t i);
        void                                                scaleCorrection(size_t i, size_t l, size_t r);
        void                                                scaleCorrection(size_t i, size_t l, size_t r, size_t m);

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
        
        correctionOffset        = nChars*nChars;
        correctionMaskOffset    = correctionOffset*2;
        correctionMixtureOffset = numCorrectionMasks*correctionMaskOffset;
        correctionNodeOffset    = this->num_site_rates*correctionMixtureOffset;
        activeCorrectionOffset  = this->num_nodes*correctionNodeOffset;
        
        if(this->use_scaling)
            perNodeCorrectionLogScalingFactors = std::vector<std::vector< std::vector<double> > >(2, std::vector<std::vector<double> >(this->num_nodes, std::vector<double>(this->num_chars, 0.0) ) );
    
        perMaskCorrections    = std::vector<double>(numCorrectionMasks, 0.0);
        perMixtureCorrections = std::vector<std::vector<double> >(this->num_site_rates, std::vector<double>(numCorrectionMasks, 0.0) );
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
    perMaskCorrections(n.perMaskCorrections),
    perMixtureCorrections(n.perMixtureCorrections),
    perNodeCorrectionLogScalingFactors(n.perNodeCorrectionLogScalingFactors)
{
    if ( this->inMcmcMode == true)
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

        std::map<std::string, size_t> charCounts;
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
                    
                    charCounts[c.getStringValue()]++;
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

    // warn if we have to exclude some incompatible characters
    if(incompatible > 0)
    {
        std::stringstream ss;
        ss << "WARNING: There are " << incompatible << " characters incompatible with the specified coding bias. These characters will be excluded.";
        RBOUT(ss.str());

        // resize our datset to account for the newly excluded characters
        this->num_sites = siteIndices.size();
        this->site_pattern.clear();
        this->site_pattern.resize(this->num_sites);
    }

    // readjust the number of correction sites to account for masked sites
    if(coding != AscertainmentBias::ALL)
    {
        numCorrectionMasks = correctionMaskCounts.size();
    }

    return siteIndices;
}

template<class charType>
bool RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::isSitePatternCompatible( std::map<std::string, size_t> charCounts )
{
    bool singleton = false;
    if(charCounts.size() == 2)
    {
        for(std::map<std::string, size_t>::iterator it = charCounts.begin(); it != charCounts.end(); it++)
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
        
        scaleCorrection(root, left, right);
    }
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{

    PhyloCTMCSiteHomogeneous<charType>::computeRootLikelihood(root, left, right, middle);

    if(coding != AscertainmentBias::ALL)
    {
        computeRootCorrection(root, left, right, middle);
        
        scaleCorrection(root, left, right, middle);
    }
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{

    PhyloCTMCSiteHomogeneous<charType>::computeInternalNodeLikelihood(node, nodeIndex, left, right);

    if(coding != AscertainmentBias::ALL)
    {
        computeInternalNodeCorrection(node, nodeIndex, left, right);
        
        scaleCorrection(nodeIndex, left, right);
    }
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{

    PhyloCTMCSiteHomogeneous<charType>::computeInternalNodeLikelihood(node, nodeIndex, left, right, middle);

    if(coding != AscertainmentBias::ALL)
    {
        computeInternalNodeCorrection(node, nodeIndex, left, right, middle);
        
        scaleCorrection(nodeIndex, left, right, middle);
    }
}




template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
{

    PhyloCTMCSiteHomogeneous<charType>::computeTipLikelihood(node, nodeIndex);

    if(coding != AscertainmentBias::ALL)
    {
        computeTipCorrection(node, nodeIndex);
        
        scaleCorrection(nodeIndex);
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeTipCorrection(const TopologyNode &node, size_t nodeIndex)
{
    std::vector<double>::iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*this->activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator         u      = p_node + offset;

            bool gap = correctionMaskMatrix[mask][nodeIndex];

            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                std::vector<double>::iterator         uC = u  + ci*this->num_chars;
                std::vector<double>::iterator         uI = uC + correctionOffset;
                                    
                for(size_t c = 0; c < this->num_chars; c++)
                {
                                
                    // Probability of constant state c this tip
                    // when the state at this tip is ci
                    uC[c] = (c == ci) && !gap;
                    
                    // Probability of invert singleton state c this tip
                    // when the state at this tip is ci
                    uI[c] = (c != ci) && !gap;
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

    const TopologyNode &left_node   = this->tau->getValue().getNode(left);
    const TopologyNode &right_node  = this->tau->getValue().getNode(right);
    const TopologyNode &middle_node = this->tau->getValue().getNode(middle);

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        //get the transition probabilities for each branch
        this->updateTransitionProbabilities( left, left_node.getBranchLength() );
        TransitionProbabilityMatrix    pij = this->transition_prob_matrices[mixture];

        this->updateTransitionProbabilities( right, right_node.getBranchLength() );
        TransitionProbabilityMatrix    pik = this->transition_prob_matrices[mixture];

        this->updateTransitionProbabilities( middle, middle_node.getBranchLength() );
        TransitionProbabilityMatrix    pil = this->transition_prob_matrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator               u_i = p_node   + offset;
            std::vector<double>::const_iterator         u_j = p_left   + offset;
            std::vector<double>::const_iterator         u_k = p_right  + offset;
            std::vector<double>::const_iterator         u_l = p_middle + offset;
            
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                std::vector<double>::iterator         uC_i = u_i  + ci*this->num_chars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;
                
                for(size_t c = 0; c < this->num_chars; c++)
                {
                                
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;
                                        
                    for(size_t cj = 0; cj < this->num_chars; cj++)
                    {
                        std::vector<double>::const_iterator         uC_j = u_j  + cj*this->num_chars;
                        std::vector<double>::const_iterator         uI_j = uC_j + correctionOffset;
                                        
                        for(size_t ck = 0; ck < this->num_chars; ck++)
                        {
                            std::vector<double>::const_iterator         uC_k = u_k  + ck*this->num_chars;
                            std::vector<double>::const_iterator         uI_k = uC_k + correctionOffset;
                            
                            for(size_t cl = 0; cl < this->num_chars; cl++)
                            {
                                std::vector<double>::const_iterator         uC_l = u_l  + cl*this->num_chars;
                                std::vector<double>::const_iterator         uI_l = uC_l + correctionOffset;
                            
                                double Pij = pij[ci][cj];
                                double Pik = pik[ci][ck];
                                double Pil = pil[ci][cl];
                                
                                // probability of constant state c descending from this node
                                // when the state at this node is ci, with children states cj, ck, and cl
                                uC_i[c] += Pij*uC_j[c] * Pik*uC_k[c] * Pil*uC_l[c];
                                
                                // probability of invert singleton state c descending from
                                // when the state at this node is ci, with children states cj, ck, and cl
                                uI_i[c] += Pij*uI_j[c] * Pik*uC_k[c] * Pil*uC_l[c]
                                         + Pij*uC_j[c] * Pik*uI_k[c] * Pil*uC_l[c]
                                         + Pij*uC_j[c] * Pik*uC_k[c] * Pil*uI_l[c];
                            }
                        }
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

    const TopologyNode &left_node  = this->tau->getValue().getNode(left);
    const TopologyNode &right_node = this->tau->getValue().getNode(right);

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        //get the transition probabilities for each branch
        this->updateTransitionProbabilities( left, left_node.getBranchLength() );
        TransitionProbabilityMatrix    pij = this->transition_prob_matrices[mixture];

        this->updateTransitionProbabilities( right, right_node.getBranchLength() );
        TransitionProbabilityMatrix    pik = this->transition_prob_matrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator               u_i = p_node  + offset;
            std::vector<double>::const_iterator         u_j = p_left  + offset;
            std::vector<double>::const_iterator         u_k = p_right + offset;

            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                std::vector<double>::iterator         uC_i = u_i  + ci*this->num_chars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;
                
                for(size_t c = 0; c < this->num_chars; c++)
                {
                    
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;
                    
                    for(size_t cj = 0; cj < this->num_chars; cj++)
                    {
                        std::vector<double>::const_iterator         uC_j = u_j  + cj*this->num_chars;
                        std::vector<double>::const_iterator         uI_j = uC_j + correctionOffset;
                                        
                        for(size_t ck = 0; ck < this->num_chars; ck++)
                        {
                            std::vector<double>::const_iterator         uC_k = u_k  + ck*this->num_chars;
                            std::vector<double>::const_iterator         uI_k = uC_k + correctionOffset;
                            
                            double Pij = pij[ci][cj];
                            double Pik = pik[ci][ck];
                            
                            // probability of constant state c descending from this node
                            // when the state at this node is ci, with children states cj and ck
                            uC_i[c] += Pij*uC_j[c] * Pik*uC_k[c];
                            
                            // probability of invert singleton state c descending from this node
                            // when the state at this node is ci, with children states cj and ck
                            uI_i[c] += Pij*uC_j[c] * Pik*uI_k[c] + Pij*uI_j[c] * Pik*uC_k[c];
                        }
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

    const TopologyNode &left_node   = this->tau->getValue().getNode(left);
    const TopologyNode &right_node  = this->tau->getValue().getNode(right);
    const TopologyNode &middle_node = this->tau->getValue().getNode(middle);

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        //get the transition probabilities for each branch
        this->updateTransitionProbabilities( left, left_node.getBranchLength() );
        TransitionProbabilityMatrix    pij = this->transition_prob_matrices[mixture];

        this->updateTransitionProbabilities( right, right_node.getBranchLength() );
        TransitionProbabilityMatrix    pik = this->transition_prob_matrices[mixture];

        this->updateTransitionProbabilities( middle, middle_node.getBranchLength() );
        TransitionProbabilityMatrix    pil = this->transition_prob_matrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator               u_i = p_node   + offset;
            std::vector<double>::const_iterator         u_j = p_left   + offset;
            std::vector<double>::const_iterator         u_k = p_right  + offset;
            std::vector<double>::const_iterator         u_l = p_middle + offset;
            
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                std::vector<double>::iterator         uC_i = u_i  + ci*this->num_chars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;
                                
                for(size_t c = 0; c < this->num_chars; c++)
                {  
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;
                                        
                    for(size_t cj = 0; cj < this->num_chars; cj++)
                    {
                        std::vector<double>::const_iterator         uC_j = u_j  + cj*this->num_chars;
                        std::vector<double>::const_iterator         uI_j = uC_j + correctionOffset;
                                        
                        for(size_t ck = 0; ck < this->num_chars; ck++)
                        {
                            std::vector<double>::const_iterator         uC_k = u_k  + ck*this->num_chars;
                            std::vector<double>::const_iterator         uI_k = uC_k + correctionOffset;
                            
                            for(size_t cl = 0; cl < this->num_chars; cl++)
                            {
                                std::vector<double>::const_iterator         uC_l = u_l  + cl*this->num_chars;
                                std::vector<double>::const_iterator         uI_l = uC_l + correctionOffset;
                            
                                double Pij = pij[ci][cj];
                                double Pik = pik[ci][ck];
                                double Pil = pil[ci][cl];
                                
                                // probability of constant state c descending from this node
                                // when the state at this node is ci, with children states cj, ck and cl
                                uC_i[c] += f[ci] * ( Pij*uC_j[c] * Pik*uC_k[c] * Pil*uC_l[c] );
                                
                                // probability of invert singleton state c descending from this node
                                // when the state at this node is ci, with children states cj, ck and cl
                                uI_i[c] += f[ci] * ( Pij*uI_j[c] * Pik*uC_k[c] * Pil*uC_l[c]
                                                   + Pij*uC_j[c] * Pik*uI_k[c] * Pil*uC_l[c]
                                                   + Pij*uC_j[c] * Pik*uC_k[c] * Pil*uI_l[c] );
                            }
                        }
                    }
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

    const TopologyNode &left_node  = this->tau->getValue().getNode(left);
    const TopologyNode &right_node = this->tau->getValue().getNode(right);
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        //get the transition probabilities for each branch
        this->updateTransitionProbabilities( left, left_node.getBranchLength() );
        TransitionProbabilityMatrix    pij = this->transition_prob_matrices[mixture];
        
        this->updateTransitionProbabilities( right, right_node.getBranchLength() );
        TransitionProbabilityMatrix    pik = this->transition_prob_matrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator               u_i = p_node  + offset;
            std::vector<double>::const_iterator         u_j = p_left  + offset;
            std::vector<double>::const_iterator         u_k = p_right + offset;

            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                std::vector<double>::iterator         uC_i = u_i  + ci*this->num_chars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;
                                
                for(size_t c = 0; c < this->num_chars; c++)
                {     
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;
                                        
                    for(size_t cj = 0; cj < this->num_chars; cj++)
                    {
                        std::vector<double>::const_iterator         uC_j = u_j  + cj*this->num_chars;
                        std::vector<double>::const_iterator         uI_j = uC_j + correctionOffset;
                                        
                        for(size_t ck = 0; ck < this->num_chars; ck++)
                        {
                            std::vector<double>::const_iterator         uC_k = u_k  + ck*this->num_chars;
                            std::vector<double>::const_iterator         uI_k = uC_k + correctionOffset;
                            
                            double Pij = pij[ci][cj];
                            double Pik = pik[ci][ck];
                            
                            // probability of constant state c descending from this node
                            // when the state at this node is ci, with children states cj and ck
                            uC_i[c] += f[ci] * (Pij*uC_j[c] * Pik*uC_k[c]);
                            
                            // probability of invert singleton state c descending from this node
                            // when the state at this node is ci, with children states cj and ck
                            uI_i[c] += f[ci] * (Pij*uC_j[c] * Pik*uI_k[c] + Pij*uI_j[c] * Pik*uC_k[c]);
                        }
                    }
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
    
    perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);
    
    // iterate over each correction mask
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
        {
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::const_iterator         u_i = p_node   + offset;
            
            // create a vector for the log correction probs
            std::vector<double> logCorrections;
            
            double max = RbConstants::Double::neginf;
            
            double prob = 0.0;
            
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                // constant site pattern likelihoods
                std::vector<double>::const_iterator         uC_i = u_i  + ci*this->num_chars;
                // invert singleton likelihoods
                std::vector<double>::const_iterator         uI_i = uC_i + correctionOffset; 
                                
                for(size_t c = 0; c < this->num_chars; c++)
                {  
                    double tmp = 0.0;
                    
                    // probability of constant state c at tips descending from this node
                    if(coding & AscertainmentBias::VARIABLE)
                        tmp += uC_i[c];
                    
                    // probability of invert singleton state c at tips descending from this node.
                    // if there is only one observed tip, then don't double-count constant site patterns
                    if((coding & AscertainmentBias::NOSINGLETONS) && maskObservationCounts[mask] > 1)
                    {
                        // if there are only two observed tips, then don't double-count variable site patterns
                        double p = maskObservationCounts[mask] > 2 ? 0.5 : 1.0;
                        
                        tmp += uI_i[c] / p;
                    }
                    
                    // rescale the correction
                    if(this->use_scaling)
                    {
                        tmp = log(tmp) + perNodeCorrectionLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][c];
                    
                        max = std::max(tmp, max);
                        
                        logCorrections.push_back(tmp);
                    }
                    else
                    {
                        prob += tmp;
                    }
                    
                }
            }
            
            if(this->use_scaling)
            {
                // use the log-exp-sum to get the sum of the corrections
                prob = exp(RbMath::log_sum_exp(logCorrections, max));
            }
        
            // add corrections for invariant sites
            double p_inv = this->p_inv->getValue();
            if(p_inv > 0.0)
            {
                prob *= (1.0 - p_inv);
        
                if(coding & AscertainmentBias::VARIABLE)
                {
                    prob += p_inv;
                }
            }
            
            // invert the probability
            prob = 1.0 - prob;
            
            // correct rounding errors
            if(prob < 0)
                prob = 0;
        
            perMixtureCorrections[mixture][mask] = prob;
            
            perMaskCorrections[mask] += prob;
        }
        
        // normalize the log-probability
        perMaskCorrections[mask] = log(perMaskCorrections[mask]) - log(this->num_site_rates);
        
        // apply the correction for this correction mask
        sumPartialProbs -= perMaskCorrections[mask]*correctionMaskCounts[mask];
    }

    return sumPartialProbs;
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::scaleCorrection( size_t nodeIndex)
{

    std::vector<double>::iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    if ( this->use_scaling == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {   
        for(size_t c = 0; c < this->num_chars; c++)
        {
            double max = 0.0;
            
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                for (size_t mask = 0; mask < numCorrectionMasks; ++mask)
                {
                    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                    {
                        size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;
    
                        std::vector<double>::const_iterator   u_i  = p_node  + offset;
                        
                        std::vector<double>::const_iterator   uC_i = u_i  + ci*this->num_chars;
                        std::vector<double>::const_iterator   uI_i = uC_i + correctionOffset;
        
                        max = std::max(max, std::max(uC_i[c], uI_i[c]));
                    }
                }
            }

            perNodeCorrectionLogScalingFactors[this->activeLikelihood[nodeIndex] ][nodeIndex ][c] = 
            log(max);
    
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                // compute the per site probabilities
                for (size_t mask = 0; mask < numCorrectionMasks; ++mask)
                {
                    // compute the per site probabilities
                    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                    {
                        size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;
    
                        std::vector<double>::iterator   u_i  = p_node  + offset;
                                                
                        std::vector<double>::iterator   uC_i = u_i  + ci*this->num_chars;
                        std::vector<double>::iterator   uI_i = uC_i + correctionOffset;
        
                        uC_i[c] /= max;
                        uI_i[c] /= max;
                    }
                }
            }
        }
    }
    else if ( this->use_scaling == true )
    {
        // iterate over all character states
        for(size_t c = 0; c < this->num_chars; c++)
        {
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[nodeIndex] ][nodeIndex ][c] = 0;               
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::scaleCorrection( size_t nodeIndex, size_t left, size_t right )
{

    std::vector<double>::iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    if ( this->use_scaling == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {   
        for(size_t c = 0; c < this->num_chars; c++)
        {
            double max = 0.0;
            
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                for (size_t mask = 0; mask < numCorrectionMasks; ++mask)
                {
                    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                    {
                        size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;
    
                        std::vector<double>::const_iterator   u_i  = p_node  + offset;
                        
                        std::vector<double>::const_iterator   uC_i = u_i  + ci*this->num_chars;
                        std::vector<double>::const_iterator   uI_i = uC_i + correctionOffset;
        
                        max = std::max(max, std::max(uC_i[c], uI_i[c]));
                    }
                }
            }

            perNodeCorrectionLogScalingFactors[this->activeLikelihood[nodeIndex] ][nodeIndex ][c] = 
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[left]      ][left      ][c] + 
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[right]     ][right     ][c] +
            log(max);
    
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                // compute the per site probabilities
                for (size_t mask = 0; mask < numCorrectionMasks; ++mask)
                {
                    // compute the per site probabilities
                    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                    {
                        size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;
    
                        std::vector<double>::iterator   u_i  = p_node  + offset;
                                                
                        std::vector<double>::iterator   uC_i = u_i  + ci*this->num_chars;
                        std::vector<double>::iterator   uI_i = uC_i + correctionOffset;
        
                        uC_i[c] /= max;
                        uI_i[c] /= max;
                    }
                }
            }
        }
    }
    else if ( this->use_scaling == true )
    {
        // iterate over all character states
        for(size_t c = 0; c < this->num_chars; c++)
        {
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[nodeIndex] ][nodeIndex ][c] = 
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[left]      ][left      ][c] +
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[right]     ][right     ][c];               
        }
    }
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::scaleCorrection( size_t nodeIndex, size_t left, size_t right, size_t middle )
{

    std::vector<double>::iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    if ( this->use_scaling == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {   
        for(size_t c = 0; c < this->num_chars; c++)
        {
            double max = 0.0;
            
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                for (size_t mask = 0; mask < numCorrectionMasks; ++mask)
                {
                    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                    {
                        size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;
    
                        std::vector<double>::const_iterator   u_i  = p_node  + offset;
                        
                        std::vector<double>::const_iterator   uC_i = u_i  + ci*this->num_chars;
                        std::vector<double>::const_iterator   uI_i = uC_i + correctionOffset;
        
                        max = std::max(max, std::max(uC_i[c], uI_i[c]));
                    }
                }
            }

            perNodeCorrectionLogScalingFactors[this->activeLikelihood[nodeIndex] ][nodeIndex ][c] = 
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[left]      ][left      ][c] + 
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[right]     ][right     ][c] +
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[middle]    ][middle    ][c] +
            log(max);
    
            for(size_t ci = 0; ci < this->num_chars; ci++)
            {
                // compute the per site probabilities
                for (size_t mask = 0; mask < numCorrectionMasks; ++mask)
                {
                    // compute the per site probabilities
                    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                    {
                        size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;
    
                        std::vector<double>::iterator   u_i  = p_node  + offset;
                                                
                        std::vector<double>::iterator   uC_i = u_i  + ci*this->num_chars;
                        std::vector<double>::iterator   uI_i = uC_i + correctionOffset;
        
                        uC_i[c] /= max;
                        uI_i[c] /= max;
                    }
                }
            }
        }
    }
    else if ( this->use_scaling == true )
    {
        // iterate over all character states
        for(size_t c = 0; c < this->num_chars; c++)
        {
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[nodeIndex] ][nodeIndex ][c] = 
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[left]      ][left      ][c] +
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[right]     ][right     ][c] +
            perNodeCorrectionLogScalingFactors[this->activeLikelihood[middle]    ][middle    ][c];               
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::resizeLikelihoodVectors( void ) {

    RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::resizeLikelihoodVectors();
    
    if(coding != AscertainmentBias::ALL)
    {
        correctionOffset        = this->num_chars*this->num_chars;
        correctionMaskOffset    = correctionOffset*2;
        correctionMixtureOffset = numCorrectionMasks*correctionMaskOffset;
        correctionNodeOffset    = this->num_site_rates*correctionMixtureOffset;
        activeCorrectionOffset  = this->num_nodes*correctionNodeOffset;
    
        if ( this->inMcmcMode == true)
            correctionLikelihoods = std::vector<double>(activeCorrectionOffset*2, 0.0);
        
        if(this->use_scaling)
            perNodeCorrectionLogScalingFactors = std::vector<std::vector< std::vector<double> > >(2, std::vector<std::vector<double> >(this->num_nodes, std::vector<double>(this->num_chars, 0.0) ) );
    
        perMixtureCorrections   = std::vector<std::vector<double> >(this->num_site_rates, std::vector<double>(numCorrectionMasks, 0.0) );
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::redrawValue( void ) {

    if(coding == AscertainmentBias::ALL)
    {
        AbstractPhyloCTMCSiteHomogeneous<charType>::redrawValue();
        
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

    std::vector< DiscreteTaxonData<charType> > taxa = std::vector< DiscreteTaxonData<charType> >(numTips, DiscreteTaxonData<charType>( Taxon("") ) );

    // first sample a total number of characters (M) from the marginal posterior: 
    // M - N | N ~ NegBinomial(N+1, exp(lnCorrection) )
    double M_minus_N = RbStatistics::NegativeBinomial::rv(N + 1, exp(perMaskCorrections[0]), *rng);

    // then sample the observed number of characters (num_sites) from the likelihood:
    // num_sites | M ~ Binomial(M, exp(lnCorrection) )
    this->num_sites = RbStatistics::Binomial::rv( M_minus_N + N, exp(perMaskCorrections[0]), *rng);

    // sample the rate categories in proportion to the total probability (correction) for each mixture.
    double total = 0.0;
    for ( size_t i = 0; i < this->num_site_rates; ++i )
        total += perMixtureCorrections[i][0];

    std::vector<size_t> perSiteRates;
    for ( size_t i = 0; i < this->num_sites; ++i )
    {
        // draw the state
        double u = rng->uniform01()*total;
        size_t rateIndex = 0;

        double tmp = 0.0;
        while(tmp < u){
            tmp += perMixtureCorrections[rateIndex][0];
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

            if ( u > 0.0 && stateIndex < this->num_chars)
            {
                ++c;
            }
            else
            {
                break;
            }

        }

        // recursively simulate the sequences
        std::map<std::string, size_t> charCounts;
        simulateConditional( root, siteData, rateIndex, charCounts);

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

    for (std::vector<bool>::iterator it = this->dirty_nodes.begin(); it != this->dirty_nodes.end(); ++it)
    {
        (*it) = true;
    }

    // flip the active likelihood pointers
    for (size_t index = 0; index < this->changed_nodes.size(); ++index)
    {
        if ( this->changed_nodes[index] == false )
        {
            this->activeLikelihood[index] = (this->activeLikelihood[index] == 0 ? 1 : 0);
            this->changed_nodes[index] = true;
        }
    }

}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::simulateConditional( const TopologyNode &node, std::vector<charType> &data, size_t rateIndex, std::map<std::string, size_t>& charCounts) {

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

        double *freqs = this->transition_prob_matrices[ rateIndex ][ cp ];

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

            if ( u > 0.0 && stateIndex < this->num_chars)
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
            charCounts[c.getStringValue()]++;
        else
            simulateConditional( child, data, rateIndex, charCounts);
    }

}

#endif
