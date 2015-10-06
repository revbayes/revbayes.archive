#ifndef PhyloCTMCSiteHomogeneousRestriction_H
#define PhyloCTMCSiteHomogeneousRestriction_H

#include "PhyloCTMCSiteHomogeneousConditional.h"
#include "DistributionBinomial.h"
#include "DistributionNegativeBinomial.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "RlUserInterface.h"
#include "StandardState.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"

namespace RevBayesCore {

    struct RestrictionCoding {

        enum { ALL               = 0x00,
               NOABSENCESITES    = 0x01,
               NOPRESENCESITES   = 0x02,
               VARIABLE          = 0x03,
               NOSINGLETONGAINS  = 0x04,
               NOSINGLETONLOSSES = 0x08,
               NOSINGLETONS      = 0x0C,
               INFORMATIVE       = 0x0F
              };
    };

    class PhyloCTMCSiteHomogeneousRestriction : public PhyloCTMCSiteHomogeneousConditional<StandardState> {

    public:
        PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode< Tree > *t, bool c, size_t nSites, bool amb, int cod = 0);
        virtual                                            ~PhyloCTMCSiteHomogeneousRestriction(void);                                                                   //!< Virtual destructor

        // public member functions
        PhyloCTMCSiteHomogeneousRestriction*                clone(void) const;                                                                        //!< Create an independent clone
        void                                                redrawValue(void);

    protected:

        virtual void                                        computeRootCorrection(size_t root, size_t l, size_t r);
        virtual void                                        computeRootCorrection(size_t root, size_t l, size_t r, size_t m);

    private:
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

RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode<Tree> *t, bool c, size_t nSites, bool amb, int ty) :
    PhyloCTMCSiteHomogeneousConditional<StandardState>(  t, 2, c, nSites, amb, ty)
{
}



RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::~PhyloCTMCSiteHomogeneousRestriction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}



RevBayesCore::PhyloCTMCSiteHomogeneousRestriction* RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::clone( void ) const {

    return new PhyloCTMCSiteHomogeneousRestriction( *this );
}


void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::compress( void )
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

    if(type != RestrictionCoding::ALL)
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

        size_t numPresent = 0;
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
                    if(type != RestrictionCoding::ALL)
                        mask += "-";
                    numGap++;
                }
                else
                {
                    if(type != RestrictionCoding::ALL)
                        mask += " ";
                    if(c.getStringValue() == "1")
                        numPresent++;
                }

                if(type != RestrictionCoding::ALL)
                    maskData.push_back(c.isGapState());
            }
        }

        if( ((numPresent == 0)                 && (type & RestrictionCoding::NOABSENCESITES))    ||
            ((numPresent == 1)                 && (type & RestrictionCoding::NOSINGLETONGAINS))  ||
            ((numPresent == tips - numGap)     && (type & RestrictionCoding::NOPRESENCESITES))   ||
            ((numPresent == tips - 1 - numGap) && (type & RestrictionCoding::NOSINGLETONLOSSES))
          )
        {
            incompatible++;
        }
        else
        {
            siteIndices.push_back(siteIndex);

            if(type != RestrictionCoding::ALL)
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
    if(type != RestrictionCoding::ALL)
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
                StandardState &c = static_cast<StandardState &>( taxon.getCharacter(siteIndices[indexOfSitePattern[patternIndex]]) );
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


void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeRootCorrection( size_t root, size_t left, size_t right, size_t middle)
{
    if(type != RestrictionCoding::ALL){

        perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);

        const std::vector<double> &f                   = this->getRootFrequencies();

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
                                
                                if(type & RestrictionCoding::NOABSENCESITES)
                                    prob += f[ci] * Pij*uC_j[0] * Pik*uC_k[0] * Pil*uC_l[0];
                                
                                if(type & RestrictionCoding::NOSINGLETONGAINS)
                                    prob += f[ci] * ( Pij*uI_j[0] * Pik*uC_k[0] * Pil*uC_l[0] +
                                                      Pij*uC_j[0] * Pik*uI_k[0] * Pil*uC_l[0] + 
                                                      Pij*uC_j[0] * Pik*uC_k[0] * Pil*uI_l[0] );
                                
                                // if there is only one observed tip, then don't double-count singleton gains
                                if((type & RestrictionCoding::NOPRESENCESITES) && maskObservationCounts[mask] > 1)
                                    prob += f[ci] * Pij*uC_j[1] * Pik*uC_k[1] * Pil*uC_l[1];
                                
                                // if there are only two observed tips, then don't double-count singleton gains
                                // if there is only one observed tip, then don't double-count absence sites
                                if((type & RestrictionCoding::NOSINGLETONLOSSES) && maskObservationCounts[mask] > 2)
                                    prob += f[ci] * ( Pij*uI_j[1] * Pik*uC_k[1] * Pil*uC_l[1] +
                                                      Pij*uC_j[1] * Pik*uI_k[1] * Pil*uC_l[1] + 
                                                      Pij*uC_j[1] * Pik*uC_k[1] * Pil*uI_l[1] );
                            }
                        }
                    }
                }
                
                // add corrections for invariant sites
                double pInv = this->pInv->getValue();
                if(pInv > 0.0)
                {
                    prob *= (1.0 - pInv);
        
                    if(type & RestrictionCoding::NOABSENCESITES)
                        prob += pInv*f[0];
        
                    if(type & RestrictionCoding::NOPRESENCESITES)
                        prob += pInv*f[1];
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


void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeRootCorrection( size_t root, size_t left, size_t right)
{
    perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);

    if(type != RestrictionCoding::ALL){

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
                            
                            if(type & RestrictionCoding::NOABSENCESITES)
                                prob += f[ci] * Pij*uC_j[0] * Pik*uC_k[0];
                            
                            if(type & RestrictionCoding::NOSINGLETONGAINS)
                                prob += f[ci] * ( Pij*uI_j[0] * Pik*uC_k[0] +
                                                  Pij*uC_j[0] * Pik*uI_k[0] );
                            
                            // if there is only one observed tip, then don't double-count singleton gains
                            if((type & RestrictionCoding::NOPRESENCESITES) && maskObservationCounts[mask] > 1)
                                prob += f[ci] * Pij*uC_j[1] * Pik*uC_k[1];
                            
                            // if there are only two observed tips, then don't double-count singleton gains
                            // if there is only one observed tip, then don't double-count absence sites
                            if((type & RestrictionCoding::NOSINGLETONLOSSES) && maskObservationCounts[mask] > 2)
                                prob += f[ci] * ( Pij*uI_j[1] * Pik*uC_k[1] +
                                                  Pij*uC_j[1] * Pik*uI_k[1] );
                        }
                    }
                }
                
                // add corrections for invariant sites
                double p_inv = this->pInv->getValue();
                if(p_inv > 0.0)
                {
                    prob *= (1.0 - p_inv);
        
                    if(type & RestrictionCoding::NOABSENCESITES)
                        prob += p_inv*f[0];
        
                    if(type & RestrictionCoding::NOPRESENCESITES)
                        prob += p_inv*f[1];
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


void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::redrawValue( void ) {

    //this->computeLnProbability();

    // delete the old value first
    delete this->value;

    // create a new character data object
    this->value = new HomologousDiscreteCharacterData<StandardState>();

    size_t numTips = this->tau->getValue().getNumberOfTips();
    size_t numNodes = this->tau->getValue().getNumberOfNodes();

    RandomNumberGenerator* rng = GLOBAL_RNG;

    const std::vector< double > &pi = this->getRootFrequencies();

    const TopologyNode &root = this->tau->getValue().getRoot();
    size_t rootIndex = this->tau->getValue().getRoot().getIndex();

    std::vector< DiscreteTaxonData<StandardState> > taxa = std::vector< DiscreteTaxonData<StandardState> >(numTips, DiscreteTaxonData<StandardState>("") );

    if(type != RestrictionCoding::ALL){
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
    if(type == RestrictionCoding::ALL)
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

        if(type == RestrictionCoding::ALL)
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

        std::vector<StandardState> siteData(numNodes, StandardState());

        if(rng->uniform01() < pi[1])
            siteData[rootIndex].setState("1");
        else
            siteData[rootIndex].setState("0");

        // recursively simulate the sequences
        std::map<std::string, size_t> charCounts;
        simulateConditional( root, siteData, rateIndex, charCounts);

        if((type & RestrictionCoding::NOABSENCESITES) && charCounts["1"] == 0){
            i--;
            continue;
        }else if((type & RestrictionCoding::NOPRESENCESITES) && charCounts["1"] == numTips){
            i--;
            continue;
        }else if((type & RestrictionCoding::NOSINGLETONGAINS) && charCounts["1"] == 1){
            i--;
            continue;
        }else if((type & RestrictionCoding::NOSINGLETONLOSSES) && charCounts["1"] == numTips - 1){
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

#endif
