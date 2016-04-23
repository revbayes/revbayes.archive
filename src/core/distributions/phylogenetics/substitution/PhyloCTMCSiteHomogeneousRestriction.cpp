#include "PhyloCTMCSiteHomogeneousRestriction.h"

RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode<Tree> *t, bool c, size_t nSites, bool amb, RestrictionAscertainmentBias::Coding ty) :
    PhyloCTMCSiteHomogeneousConditional<RestrictionState>(  t, 2, c, nSites, amb, AscertainmentBias::Coding(ty))
{
}

RevBayesCore::PhyloCTMCSiteHomogeneousRestriction* RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::clone( void ) const {

    return new PhyloCTMCSiteHomogeneousRestriction( *this );
}

bool RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::isSitePatternCompatible( std::map<size_t, size_t> charCounts )
{
    std::map<size_t, size_t>::iterator zero = charCounts.find(0);
    std::map<size_t, size_t>::iterator one  = charCounts.find(1);

    bool compatible = true;
    
    if( charCounts.size() == 1 )
    {
        if(zero != charCounts.end() && (coding & RestrictionAscertainmentBias::NOABSENCESITES) )
        {
            compatible = false;
        }
        else if(one != charCounts.end()  && (coding & RestrictionAscertainmentBias::NOPRESENCESITES) )
        {
            compatible = false;
        }
    }
    else
    {
        if(zero != charCounts.end() && zero->second == 1 && (coding & RestrictionAscertainmentBias::NOSINGLETONABSENCE) )
        {
            compatible = false;
        }
        else if(one != charCounts.end() && one->second == 1 && (coding & RestrictionAscertainmentBias::NOSINGLETONPRESENCE) )
        {
            compatible = false;
        }
    }
    
    return compatible;
}

double RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::sumRootLikelihood( void )
{
    double sumPartialProbs = PhyloCTMCSiteHomogeneous<RestrictionState>::sumRootLikelihood();
    
    if(coding == RestrictionAscertainmentBias::ALL)
        return sumPartialProbs;
    
    
    // get the root node
    const TopologyNode &root = this->tau->getValue().getRoot();

    // get the index of the root node
    size_t nodeIndex = root.getIndex();
    
    const std::vector<double> &f = this->getRootFrequencies();
    
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
                
                if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
                    prob += uC_i[0];
                        
                if(coding & RestrictionAscertainmentBias::NOSINGLETONPRESENCE)
                    prob += uI_i[0];

                if((coding & RestrictionAscertainmentBias::NOPRESENCESITES) && maskObservationCounts[mask] > 1)
                    prob += uC_i[1];

                // if there are only two observed tips, then don't double-count singleton gains
                // if there is only one observed tip, then don't double-count absence sites
                if((coding & RestrictionAscertainmentBias::NOSINGLETONABSENCE) && maskObservationCounts[mask] > 2)
                    prob += uI_i[1];
            }
            
            perMaskCorrections[mask] += prob;


            // invert the mixture probability
            double mixprob = prob;

            // add corrections for invariant sites
            double p_inv = this->pInv->getValue();
            if(p_inv > 0.0)
            {
                mixprob *= (1.0 - p_inv);
                
                if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
                    mixprob += f[0]*p_inv;
                
                if(coding & RestrictionAscertainmentBias::NOPRESENCESITES)
                    mixprob += f[1]*p_inv;
            }
            
            // correct rounding errors
            if(mixprob <= 0.0)
                mixprob = 0.0;
        
            perMaskMixtureCorrections[mask*numSiteRates + mixture] = mixprob;
        }

        // add corrections for invariant sites
        double p_inv = this->pInv->getValue();
        if(p_inv > 0.0)
        {
            perMaskCorrections[mask] *= (1.0 - p_inv);

            if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
                perMaskCorrections[mask] += f[0] * p_inv * this->numSiteRates;

            if(coding & RestrictionAscertainmentBias::NOPRESENCESITES)
                perMaskCorrections[mask] += f[1] * p_inv * this->numSiteRates;
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
