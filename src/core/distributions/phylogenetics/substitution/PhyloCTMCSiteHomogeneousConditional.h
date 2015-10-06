#ifndef PhyloCTMCSiteHomogeneousConditional_H
#define PhyloCTMCSiteHomogeneousConditional_H

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DistributionBinomial.h"
#include "DistributionNegativeBinomial.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "RlUserInterface.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"

namespace RevBayesCore {

    struct CodingBias {

        enum { ALL               = 0x00,
               VARIABLE          = 0x01,
               NOSINGLETONS      = 0x02,
               INFORMATIVE       = 0x03
              };
    };

    template<class charType>
    class PhyloCTMCSiteHomogeneousConditional : public PhyloCTMCSiteHomogeneous<charType> {

    public:
        PhyloCTMCSiteHomogeneousConditional(const TypedDagNode< Tree > *t, size_t nChars, bool c, size_t nSites, bool amb, int cod = 0);
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


    protected:
        int                                                 type;
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
        
        void                                                simulateConditional( const TopologyNode &node, std::vector<charType> &taxa, size_t rateIndex, std::map<std::string, size_t>& charCounts);

    private:
        virtual double                                      sumRootLikelihood( void );
        void                                                compress(void);


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
RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::PhyloCTMCSiteHomogeneousConditional(const TypedDagNode<Tree> *t, size_t nChars, bool c, size_t nSites, bool amb, int ty) :
    PhyloCTMCSiteHomogeneous<charType>(  t, nChars, c, nSites, amb ), type(ty), N(nSites), numCorrectionMasks(0)
{

    numCorrectionMasks      = (type != CodingBias::ALL);
    
    correctionOffset        = nChars*nChars;
    correctionMaskOffset    = correctionOffset*2;
    correctionMixtureOffset = numCorrectionMasks*correctionMaskOffset;
    correctionNodeOffset    = this->numSiteRates*correctionMixtureOffset;
    activeCorrectionOffset  = this->numNodes*correctionNodeOffset;

    perMaskCorrections    = std::vector<double>(numCorrectionMasks, 0.0);
    perMixtureCorrections = std::vector<std::vector<double> >(this->numSiteRates, std::vector<double>(numCorrectionMasks, 0.0) );
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
    type(n.type),
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
    perMixtureCorrections(n.perMixtureCorrections)
{
    if ( this->inMcmcMode == true)
        correctionLikelihoods = n.correctionLikelihoods;
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::compress( void )
{

    this->charMatrix.clear();
    this->gapMatrix.clear();
    this->patternCounts.clear();
    this->numPatterns = 0;

    // resize the matrices
    size_t tips = this->tau->getValue().getNumberOfTips();
    this->charMatrix.resize(tips);
    this->gapMatrix.resize(tips);

    correctionMaskCounts.clear();
    maskObservationCounts.clear();
    correctionMaskMatrix.clear();

    // check whether there are ambiguous characters (besides gaps)
    bool ambiguousCharacters = false;
    // find the unique site patterns and compute their respective frequencies
    std::vector<TopologyNode*> nodes = this->tau->getValue().getNodes();

    // create a vector with the correct site indices
    // some of the sites may have been excluded
    std::vector<size_t> siteIndices;
    size_t siteIndex = 0;
    size_t incompatible = 0;

    std::map<std::string, size_t> maskIndices;

    if(type != CodingBias::ALL)
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

    for (size_t i = 0; i < this->numSites; ++i)
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

                // if we treat unknown characters as gaps and this is an unknown character then we change it
                // because we might then have a pattern more
                if ( this->treatAmbiguousAsGaps && (c.isAmbiguous() || c.isMissingState()) )
                {
                    c.setGapState( true );
                }
                else if ( this->treatUnknownAsGap && (c.getNumberOfStates() == c.getNumberObservedStates() || c.isMissingState()) )
                {
                    c.setGapState( true );
                }
                else if ( !c.isGapState() && (c.isAmbiguous() || c.isMissingState()) )
                {
                    ambiguousCharacters = true;
                }

                if(c.isGapState())
                {
                    if(type != CodingBias::ALL)
                        mask += "-";
                    numGap++;
                }
                else
                {
                    if(type != CodingBias::ALL)
                        mask += " ";
                    
                    charCounts[c.getStringValue()]++;
                }

                if(type != CodingBias::ALL)
                    maskData.push_back(c.isGapState());
            }
        }
        
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

        if( (charCounts.size() == 1 && (type & CodingBias::VARIABLE) ) ||
            (singleton && (type & CodingBias::NOSINGLETONS) )
          )
        {
            incompatible++;
        }
        else
        {
            siteIndices.push_back(siteIndex);

            if(type != CodingBias::ALL)
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
        this->numSites = siteIndices.size();
        this->sitePattern.clear();
        this->sitePattern.resize(this->numSites);
    }

    // readjust the number of correction sites to account for masked sites
    if(type > 0)
    {
        numCorrectionMasks = correctionMaskCounts.size();
    }

    // set the global variable if we use ambiguous characters
    this->usingAmbiguousCharacters = ambiguousCharacters;

    std::vector<size_t> indexOfSitePattern;

    // compress the character matrix if we're asked to
    if ( this->compressed )
    {
        // find the unique site patterns and compute their respective frequencies
        std::vector<bool> unique(this->numSites, true);
        std::map<std::string,size_t> patterns;
        for (size_t site = 0; site < this->numSites; ++site)
        {
            // create the site pattern
            std::string pattern = "";
            for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
            {
                if ( (*it)->isTip() )
                {
                    AbstractDiscreteTaxonData& taxon = this->value->getTaxonData( (*it)->getName() );
                    CharacterState &c = taxon.getCharacter(siteIndices[site]);
                    pattern += c.getStringValue();
                }
            }
            // check if we have already seen this site pattern
            std::map<std::string, size_t>::const_iterator index = patterns.find( pattern );
            if ( index != patterns.end() )
            {
                // we have already seen this pattern
                // increase the frequency counter
                this->patternCounts[ index->second ]++;

                // obviously this site isn't unique nor the first encounter
                unique[site] = false;

                // remember which pattern this site uses
                this->sitePattern[site] = index->second;
            }
            else
            {
                // create a new pattern frequency counter for this pattern
                this->patternCounts.push_back(1);

                // insert this pattern with the corresponding index in the map
                patterns.insert( std::pair<std::string,size_t>(pattern,this->numPatterns) );

                // remember which pattern this site uses
                this->sitePattern[site] = this->numPatterns;

                // increase the pattern counter
                this->numPatterns++;

                // add the index of the site to our pattern-index vector
                indexOfSitePattern.push_back( site );

                // flag that this site is unique (or the first occurence of this pattern)
                unique[site] = true;
            }
        }
    }
    else
    {
        // we do not compress
        this->numPatterns = this->numSites;
        this->patternCounts = std::vector<size_t>(this->numSites,1);
        for(size_t i = 0; i < this->numSites; i++)
        {
            indexOfSitePattern[i] = i;
        }
    }


    // compute which block of the data this process needs to compute
    this->pattern_block_start = size_t(floor( (double(this->pid)   / this->numProcesses ) * this->numPatterns) );
    this->pattern_block_end   = size_t(floor( (double(this->pid+1) / this->numProcesses ) * this->numPatterns) );
    this->pattern_block_size  = this->pattern_block_end - this->pattern_block_start;


    // allocate and fill the cells of the matrices
    for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        if ( (*it)->isTip() )
        {
            size_t nodeIndex = (*it)->getIndex();
            AbstractDiscreteTaxonData& taxon = this->value->getTaxonData( (*it)->getName() );

            // resize the column
            this->charMatrix[nodeIndex].resize(this->pattern_block_size);
            this->gapMatrix[nodeIndex].resize(this->pattern_block_size);
            for (size_t patternIndex = this->pattern_block_start; patternIndex < this->pattern_block_end; ++patternIndex)
            {
                charType &c = static_cast<charType &>( taxon.getCharacter(siteIndices[indexOfSitePattern[patternIndex]]) );
                this->gapMatrix[nodeIndex][patternIndex] = c.isGapState();

                if ( ambiguousCharacters )
                {
                    // we use the actual state
                    this->charMatrix[nodeIndex][patternIndex] = c.getState();
                }
                else
                {
                    // we use the index of the state
                    this->charMatrix[nodeIndex][patternIndex] = c.getStateIndex();
                }
            }
        }
    }

    // reset the vector if a site is invariant
    this->siteInvariant.resize( this->pattern_block_size );
    this->invariantSiteIndex.resize( this->pattern_block_size );
    size_t length = this->charMatrix.size();
    for (size_t i=0; i<this->pattern_block_size; ++i)
    {
        bool inv = true;
        unsigned long c = this->charMatrix[0][i];

        this->invariantSiteIndex[i] = c;

        for (size_t j=1; j<length; ++j)
        {
            if ( c != this->charMatrix[j][i] )
            {
                inv = false;
                break;
            }
        }

        this->siteInvariant[i] = inv;
    }

    // finally we resize the partial likelihood vectors to the new pattern counts
    this->resizeLikelihoodVectors();

}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeRootLikelihood( size_t root, size_t left, size_t right)
{

    PhyloCTMCSiteHomogeneous<charType>::computeRootLikelihood(root, left, right);

    computeRootCorrection(root, left, right);
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{

    PhyloCTMCSiteHomogeneous<charType>::computeRootLikelihood(root, left, right, middle);

    computeRootCorrection(root, left, right, middle);
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{

    PhyloCTMCSiteHomogeneous<charType>::computeInternalNodeLikelihood(node, nodeIndex, left, right);

    computeInternalNodeCorrection(node, nodeIndex, left, right);
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{

    PhyloCTMCSiteHomogeneous<charType>::computeInternalNodeLikelihood(node, nodeIndex, left, right, middle);

    computeInternalNodeCorrection(node, nodeIndex, left, right, middle);
}




template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
{

    PhyloCTMCSiteHomogeneous<charType>::computeTipLikelihood(node, nodeIndex);

    computeTipCorrection(node, nodeIndex);

}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeTipCorrection(const TopologyNode &node, size_t nodeIndex)
{
    if(type == CodingBias::ALL)
        return;

    std::vector<double>::iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*this->activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator         u      = p_node + offset;

            bool gap = correctionMaskMatrix[mask][nodeIndex];

            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                std::vector<double>::iterator         uC = u  + ci*this->numChars;
                std::vector<double>::iterator         uI = uC + correctionOffset;
                                    
                for(size_t c = 0; c < this->numChars; c++)
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
    if(type == CodingBias::ALL)
        return;

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    const TopologyNode &left_node   = this->tau->getValue().getNode(left);
    const TopologyNode &right_node  = this->tau->getValue().getNode(right);
    const TopologyNode &middle_node = this->tau->getValue().getNode(middle);

    // iterate over all mixture categories

    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        this->updateTransitionProbabilities( left, left_node.getBranchLength() );
        TransitionProbabilityMatrix    pij = this->transitionProbMatrices[mixture];

        this->updateTransitionProbabilities( right, right_node.getBranchLength() );
        TransitionProbabilityMatrix    pik = this->transitionProbMatrices[mixture];

        this->updateTransitionProbabilities( middle, middle_node.getBranchLength() );
        TransitionProbabilityMatrix    pil = this->transitionProbMatrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator               u_i = p_node   + offset;
            std::vector<double>::const_iterator         u_j = p_left   + offset;
            std::vector<double>::const_iterator         u_k = p_right  + offset;
            std::vector<double>::const_iterator         u_l = p_middle + offset;
            
            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                std::vector<double>::iterator         uC_i = u_i  + ci*this->numChars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;
                
                for(size_t c = 0; c < this->numChars; c++)
                {
                                
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;
                                        
                    for(size_t cj = 0; cj < this->numChars; cj++)
                    {
                        std::vector<double>::const_iterator         uC_j = u_j  + cj*this->numChars;
                        std::vector<double>::const_iterator         uI_j = uC_j + correctionOffset;
                                        
                        for(size_t ck = 0; ck < this->numChars; ck++)
                        {
                            std::vector<double>::const_iterator         uC_k = u_k  + ck*this->numChars;
                            std::vector<double>::const_iterator         uI_k = uC_k + correctionOffset;
                            
                            for(size_t cl = 0; cl < this->numChars; cl++)
                            {
                                std::vector<double>::const_iterator         uC_l = u_l  + cl*this->numChars;
                                std::vector<double>::const_iterator         uI_l = uC_l + correctionOffset;
                            
                                double Pij = pij[ci][cj];
                                double Pik = pik[ci][ck];
                                double Pil = pil[ci][cl];
                                
                                // probability of constant state c descending from this node
                                // when the state at this node is ci, with children states cj, ck, and cl
                                uC_i[c]   += Pij*uC_j[c] * Pik*uC_k[c] * Pil*uC_l[c];
                                
                                // probability of invert singleton state c descending from
                                // when the state at this node is ci, with children states cj, ck, and cl
                                uI_i[c]  += Pij*uI_j[c] * Pik*uC_k[c] * Pil*uC_l[c]
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

    if(type == CodingBias::ALL)
        return;

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    const TopologyNode &left_node  = this->tau->getValue().getNode(left);
    const TopologyNode &right_node = this->tau->getValue().getNode(right);

    // iterate over all mixture categories

    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        this->updateTransitionProbabilities( left, left_node.getBranchLength() );
        TransitionProbabilityMatrix    pij = this->transitionProbMatrices[mixture];

        this->updateTransitionProbabilities( right, right_node.getBranchLength() );
        TransitionProbabilityMatrix    pik = this->transitionProbMatrices[mixture];

        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator               u_i = p_node  + offset;
            std::vector<double>::const_iterator         u_j = p_left  + offset;
            std::vector<double>::const_iterator         u_k = p_right + offset;

            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                std::vector<double>::iterator         uC_i = u_i  + ci*this->numChars;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;
                
                for(size_t c = 0; c < this->numChars; c++)
                {
                    
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;
                    
                    for(size_t cj = 0; cj < this->numChars; cj++)
                    {
                        std::vector<double>::const_iterator         uC_j = u_j  + cj*this->numChars;
                        std::vector<double>::const_iterator         uI_j = uC_j + correctionOffset;
                                        
                        for(size_t ck = 0; ck < this->numChars; ck++)
                        {
                            std::vector<double>::const_iterator         uC_k = u_k  + ck*this->numChars;
                            std::vector<double>::const_iterator         uI_k = uC_k + correctionOffset;
                            
                            double Pij = pij[ci][cj];
                            double Pik = pik[ci][ck];
                            
                            // probability of constant state c descending from this node
                            // when the state at this node is ci, with children states cj and ck
                            uC_i[c]   += Pij*uC_j[c] * Pik*uC_k[c];
                            
                            // probability of invert singleton state c descending from
                            // when the state at this node is ci, with children states cj and ck
                            uI_i[c]   += Pij*uC_j[c] * Pik*uI_k[c] + Pij*uI_j[c] * Pik*uC_k[c];
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
    if(type != CodingBias::ALL){

        perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);

        const std::vector<double> &f                    = this->getRootFrequencies();

        // get the pointers to the partial likelihoods for this node and the two descendant subtrees
        std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
        std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
        std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;

        const TopologyNode &left_node   = this->tau->getValue().getNode(left);
        const TopologyNode &right_node  = this->tau->getValue().getNode(right);
        const TopologyNode &middle_node = this->tau->getValue().getNode(middle);

        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
            perMixtureCorrections[mixture] = std::vector<double>(numCorrectionMasks, 0.0);

            //get the 1->1 transition probabilities for each branch
            this->updateTransitionProbabilities( left, left_node.getBranchLength() );
            TransitionProbabilityMatrix    pij = this->transitionProbMatrices[mixture];

            this->updateTransitionProbabilities( right, right_node.getBranchLength() );
            TransitionProbabilityMatrix    pik = this->transitionProbMatrices[mixture];

            this->updateTransitionProbabilities( middle, middle_node.getBranchLength() );
            TransitionProbabilityMatrix    pil = this->transitionProbMatrices[mixture];

            for(size_t mask = 0; mask < numCorrectionMasks; mask++){

                size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

                std::vector<double>::const_iterator         u_j = p_left   + offset;
                std::vector<double>::const_iterator         u_k = p_right  + offset;
                std::vector<double>::const_iterator         u_l = p_middle + offset;

                double prob = 0.0;
                
                for(size_t ci = 0; ci < this->numChars; ci++)
                {
                    for(size_t c = 0; c < this->numChars; c++)
                    {  
                        for(size_t cj = 0; cj < this->numChars; cj++)
                        {
                            std::vector<double>::const_iterator         uC_j = u_j  + cj*this->numChars;
                            std::vector<double>::const_iterator         uI_j = uC_j + correctionOffset;
                                            
                            for(size_t ck = 0; ck < this->numChars; ck++)
                            {
                                std::vector<double>::const_iterator         uC_k = u_k  + ck*this->numChars;
                                std::vector<double>::const_iterator         uI_k = uC_k + correctionOffset;
                                
                                for(size_t cl = 0; cl < this->numChars; cl++)
                                {
                                    std::vector<double>::const_iterator         uC_l = u_l  + cl*this->numChars;
                                    std::vector<double>::const_iterator         uI_l = uC_l + correctionOffset;
                                
                                    double Pij = pij[ci][cj];
                                    double Pik = pik[ci][ck];
                                    double Pil = pil[ci][cl];
                                    
                                    // probability of constant state c at tips descending from this node
                                    if(type & CodingBias::VARIABLE)
                                        prob   += Pij*uC_j[c] * Pik*uC_k[c] * Pil*uC_l[c];
                                    
                                    // probability of invert singleton state c at tips descending from this node
                                    // if there is only one observed tip, then don't double-count constant site patterns
                                    if((type & CodingBias::NOSINGLETONS) && maskObservationCounts[mask] > 1)
                                    {
                                        // if there are only two observed tips, then don't double-count variable site patterns
                                        double p = maskObservationCounts[mask] > 2 ? 0.5 : 1.0;
                                        
                                        p *= Pij*uI_j[c] * Pik*uC_k[c] * Pil*uC_l[c]
                                           + Pij*uC_j[c] * Pik*uI_k[c] * Pil*uC_l[c]
                                           + Pij*uC_j[c] * Pik*uC_k[c] * Pil*uI_l[c];
                                        
                                        p *= f[ci];
                                        
                                        prob += p;
                                    }
                                }
                            }
                        }
                    }
                }

                // add corrections for invariant sites
                double p_inv = this->pInv->getValue();
                if(p_inv > 0.0)
                {
                    prob *= (1.0 - p_inv);
        
                    if(type & CodingBias::VARIABLE)
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
            }
        }

        for (size_t mask = 0; mask < numCorrectionMasks; ++mask)
        {
            // sum the likelihoods for all mixtures together
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
                perMaskCorrections[mask] += perMixtureCorrections[mixture][mask];

            // normalize the log-probability
            perMaskCorrections[mask] = log(perMaskCorrections[mask]) - log(this->numSiteRates);
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::computeRootCorrection( size_t root, size_t left, size_t right)
{
    if(type != CodingBias::ALL){
        
        perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);
            
        const std::vector<double> &f                    = this->getRootFrequencies();

        // get the pointers to the partial likelihoods for this node and the two descendant subtrees
        std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
        std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;

        const TopologyNode &left_node  = this->tau->getValue().getNode(left);
        const TopologyNode &right_node = this->tau->getValue().getNode(right);

        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
            perMixtureCorrections[mixture] = std::vector<double>(numCorrectionMasks, 0.0);

            //get the 1->1 transition probabilities for each branch
            this->updateTransitionProbabilities( left, left_node.getBranchLength() );
            TransitionProbabilityMatrix    pij = this->transitionProbMatrices[mixture];

            this->updateTransitionProbabilities( right, right_node.getBranchLength() );
            TransitionProbabilityMatrix    pik = this->transitionProbMatrices[mixture];

            for(size_t mask = 0; mask < numCorrectionMasks; mask++){

                size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

                std::vector<double>::const_iterator         u_j = p_left  + offset;
                std::vector<double>::const_iterator         u_k = p_right + offset;

                double prob = 0.0;
                
                for(size_t ci = 0; ci < this->numChars; ci++)
                {
                    for(size_t c = 0; c < this->numChars; c++)
                    {     
                        for(size_t cj = 0; cj < this->numChars; cj++)
                        {
                            std::vector<double>::const_iterator         uC_j = u_j  + cj*this->numChars;
                            std::vector<double>::const_iterator         uI_j = uC_j + correctionOffset;
                                            
                            for(size_t ck = 0; ck < this->numChars; ck++)
                            {
                                std::vector<double>::const_iterator         uC_k = u_k  + ck*this->numChars;
                                std::vector<double>::const_iterator         uI_k = uC_k + correctionOffset;
                                
                                double Pij = pij[ci][cj];
                                double Pik = pik[ci][ck];
                                
                                // probability of constant state c at tips descending from this node.=
                                if(type & CodingBias::VARIABLE)
                                    prob   += f[ci] * (Pij*uC_j[c] * Pik*uC_k[c]);
                                
                                // probability of invert singleton state c at tips descending from this node.
                                // if there is only one observed tip, then don't double-count constant site patterns
                                if((type & CodingBias::NOSINGLETONS) && maskObservationCounts[mask] > 1)
                                {
                                    // if there are only two observed tips, then don't double-count variable site patterns
                                    double p = maskObservationCounts[mask] > 2 ? 0.5 : 1.0;
                                    
                                    prob   += f[ci] * (Pij*uC_j[c] * Pik*uI_k[c] + Pij*uI_j[c] * Pik*uC_k[c]) / p;
                                }
                            }
                        }
                    }
                }
                
                // add corrections for invariant sites
                double p_inv = this->pInv->getValue();
                if(p_inv > 0.0)
                {
                    prob *= (1.0 - p_inv);
        
                    if(type & CodingBias::VARIABLE)
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
            }
        }

        for (size_t mask = 0; mask < numCorrectionMasks; ++mask)
        {
            // sum the likelihoods for all mixtures together
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
                perMaskCorrections[mask] += perMixtureCorrections[mixture][mask];

            // normalize the log-probability
            perMaskCorrections[mask] = log(perMaskCorrections[mask]) - log(this->numSiteRates);
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::resizeLikelihoodVectors( void ) {

    RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::resizeLikelihoodVectors();
    
    correctionOffset        = this->numChars*this->numChars;
    correctionMaskOffset    = correctionOffset*2;
    correctionMixtureOffset = numCorrectionMasks*correctionMaskOffset;
    correctionNodeOffset    = this->numSiteRates*correctionMixtureOffset;
    activeCorrectionOffset  = this->numNodes*correctionNodeOffset;

    if ( this->inMcmcMode == true)
        correctionLikelihoods = std::vector<double>(activeCorrectionOffset*2, 0.0);

    perMixtureCorrections   = std::vector<std::vector<double> >(this->numSiteRates, std::vector<double>(numCorrectionMasks, 0.0) );
}

template<class charType>
double RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::sumRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = this->tau->getValue().getRoot();

    // get the index of the root node
    size_t nodeIndex = root.getIndex();

    // get the pointers to the partial likelihoods of the left and right subtree
    double*   p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex] * this->activeLikelihoodOffset  + nodeIndex*this->nodeOffset;

    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(this->pattern_block_size,0.0);

    // get pointer the likelihood
    double*   p_mixture     = p_node;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {

        // get pointers to the likelihood for this mixture category
        double*   p_site_mixture     = p_mixture;
        // iterate over all sites

        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            // get the pointers to the likelihoods for this site and mixture category
            double* p_site_j   = p_site_mixture;
            // iterate over all starting states
            for (size_t i=0; i<this->numChars; ++i)
            {
                // add the probability of starting from this state
                tmp += *p_site_j;

                // increment pointers
                ++p_site_j;
            }
            // add the likelihood for this mixture category
            per_mixture_Likelihoods[site] += tmp;

            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset;

        } // end-for over all sites (=patterns)

        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset;

    } // end-for over all mixtures (=rate categories)

    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();

    double p_inv = this->pInv->getValue();
    double oneMinusPInv = 1.0 - p_inv;
    std::vector< size_t >::const_iterator patterns = this->patternCounts.begin();
    if ( p_inv > 0.0 )
    {
        for (size_t site = 0; site < this->pattern_block_size; ++site, ++patterns)
        {

            if ( this->useScaling == true )
            {

                if ( this->siteInvariant[site] )
                {
                    sumPartialProbs += log( p_inv * f[ this->invariantSiteIndex[site] ] * exp(this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site]) + oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                else
                {
                    sumPartialProbs += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                sumPartialProbs -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] * *patterns;

            }
            else // no scaling
            {

                if ( this->siteInvariant[site] )
                {
                    sumPartialProbs += log( p_inv * f[ this->invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                else
                {
                    sumPartialProbs += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }

            }
        }
    }
    else
    {

        for (size_t site = 0; site < this->pattern_block_size; ++site, ++patterns)
        {

            sumPartialProbs += log( per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;

            if ( this->useScaling == true )
            {

                sumPartialProbs -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] * *patterns;
            }

        }


    }


#ifdef RB_MPI

    if ( !this->processActive )
    {
        // send from the workers the log-likelihood to the master
        MPI::COMM_WORLD.Send(&sumPartialProbs, 1, MPI::DOUBLE, this->activePID, 0);
    }

    if ( this->processActive )
    {
        for (size_t i=this->activePID+1; i<this->activePID+this->numProcesses; ++i)
        {
            double tmp = 0;
            MPI::COMM_WORLD.Recv(&tmp, 1, MPI::DOUBLE, (int)i, 0);
            sumPartialProbs += tmp;
        }
    }

    if ( this->processActive )
    {
        for (size_t i=this->activePID+1; i<this->activePID+this->numProcesses; ++i)
        {
            MPI::COMM_WORLD.Send(&sumPartialProbs, 1, MPI::DOUBLE, (int)i, 0);
        }
    }
    else
    {
        MPI::COMM_WORLD.Recv(&sumPartialProbs, 1, MPI::DOUBLE, this->activePID, 0);
    }

#endif

    for(size_t i = 0; i < numCorrectionMasks; i++)
    {
        sumPartialProbs -= perMaskCorrections[i]*correctionMaskCounts[i];
    }

    return sumPartialProbs;
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditional<charType>::redrawValue( void ) {

    //this->computeLnProbability();

    // delete the old value first
    delete this->value;

    // create a new character data object
    this->value = new HomologousDiscreteCharacterData<charType>();

    size_t numTips = this->tau->getValue().getNumberOfTips();
    size_t numNodes = this->tau->getValue().getNumberOfNodes();

    RandomNumberGenerator* rng = GLOBAL_RNG;

    const std::vector< double > &pi = this->getRootFrequencies();

    const TopologyNode &root = this->tau->getValue().getRoot();
    size_t rootIndex = this->tau->getValue().getRoot().getIndex();

    std::vector< DiscreteTaxonData<charType> > taxa = std::vector< DiscreteTaxonData<charType> >(numTips, DiscreteTaxonData<charType>("") );

    if(type != CodingBias::ALL){
        // if we are using a correction, first sample a total number of characters (M)
        // from the marginal posterior: M - N | N ~ NegBinomial(N+1, exp(lnCorrection) )
        double M_minus_N = RbStatistics::NegativeBinomial::rv(N + 1, exp(perMaskCorrections[0]), *rng);

        // then sample the observed number of characters (numSites)
        // from the likelihood: numSites | M ~ Binomial(M, exp(lnCorrection) )
        this->numSites = RbStatistics::Binomial::rv( M_minus_N + N, exp(perMaskCorrections[0]), *rng);
    }

    // if we are using a correction, then sample the rate categories in proportion to the
    // total probability (correction) for each mixture.
    // otherwise, just sample each category with equal weight
    double total = 0.0;
    if(type == CodingBias::ALL)
    {
        total = 1.0;
    }
    else
    {
        for ( size_t i = 0; i < this->numSiteRates; ++i )
            total += perMixtureCorrections[i][0];
    }

    std::vector<size_t> perSiteRates;
    for ( size_t i = 0; i < this->numSites; ++i )
    {
        // draw the state
        double u = rng->uniform01()*total;
        size_t rateIndex = 0;

        if(type == CodingBias::ALL)
        {
            rateIndex = size_t(u*this->numSiteRates);
        }
        else
        {
            double tmp = 0.0;
            while(tmp < u){
                tmp += perMixtureCorrections[rateIndex][0];
                if(tmp < u)
                    rateIndex++;
            }
        }

        perSiteRates.push_back( rateIndex );
    }

    // then sample site-patterns using rejection sampling,
    // rejecting those that match the unobservable ones.
    for ( size_t i = 0; i < this->numSites; i++ )
    {
        size_t rateIndex = perSiteRates[i];

        std::vector<charType> siteData(numNodes, charType());

        if(rng->uniform01() < pi[1])
            siteData[rootIndex].setState("1");
        else
            siteData[rootIndex].setState("0");

        // recursively simulate the sequences
        std::map<std::string, size_t> charCounts;
        simulateConditional( root, siteData, rateIndex, charCounts);
        
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

        if((type & CodingBias::VARIABLE) && charCounts.size() == 1)
        {
            i--;
            continue;
        }
        else if((type & CodingBias::NOSINGLETONS) && singleton)
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
        taxa[i].setTaxonName(this->tau->getValue().getNode(i).getName());
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
            charCounts[c.getStringValue()]++;
        else
            simulateConditional( child, data, rateIndex, charCounts);
    }

}

#endif
