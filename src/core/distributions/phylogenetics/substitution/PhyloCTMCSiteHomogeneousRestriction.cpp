#include "PhyloCTMCSiteHomogeneousRestriction.h"

RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode<Tree> *t, bool c, size_t nSites, bool amb, RestrictionAscertainmentBias::Coding ty) :
    PhyloCTMCSiteHomogeneousConditional<RestrictionState>(  t, 2, c, nSites, amb, AscertainmentBias::Coding(ty))
{
}

RevBayesCore::PhyloCTMCSiteHomogeneousRestriction* RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::clone( void ) const {

    return new PhyloCTMCSiteHomogeneousRestriction( *this );
}

bool RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::isSitePatternCompatible( std::map<std::string, size_t> charCounts )
{
    std::map<std::string, size_t>::iterator zero = charCounts.find("0");
    std::map<std::string, size_t>::iterator one  = charCounts.find("1");

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
    
    perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);
    
    // iterate over each correction mask
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
        {    
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::const_iterator         u_i = p_node   + offset;
            
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
                    
                    // c is the character state of the correction pattern
                    if(c == 0)
                    {
                        if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
                            tmp += uC_i[c];
                        
                        if(coding & RestrictionAscertainmentBias::NOSINGLETONPRESENCE)
                            tmp += uI_i[c];
                    }
                    
                    if(c == 1)
                    {
                        // if there is only one observed tip, then don't double-count singleton gains
                        if((coding & RestrictionAscertainmentBias::NOPRESENCESITES) && maskObservationCounts[mask] > 1)
                            tmp += uC_i[c];
                    
                        // if there are only two observed tips, then don't double-count singleton gains
                        // if there is only one observed tip, then don't double-count absence sites
                        if((coding & RestrictionAscertainmentBias::NOSINGLETONABSENCE) && maskObservationCounts[mask] > 2)
                            tmp += uI_i[c];
                    }
                    
                    if(tmp == 0.0)
                        continue;
                    
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
            
            if (this->use_scaling == true)
            {
                // use the log-exp-sum to get the sum of the corrections
                prob = exp(RbMath::log_sum_exp(logCorrections, max));
            }
        
            // add corrections for invariant sites
            double p_inv = this->p_inv->getValue();
            if(p_inv > 0.0)
            {
                prob *= (1.0 - p_inv);
                
                if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
                    prob += f[0]*p_inv;
                
                if(coding & RestrictionAscertainmentBias::NOPRESENCESITES)
                    prob += f[1]*p_inv;
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