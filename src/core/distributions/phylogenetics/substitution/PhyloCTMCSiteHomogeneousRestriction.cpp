#include "PhyloCTMCSiteHomogeneousRestriction.h"
#include "DiscreteCharacterState.h"
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

bool RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::isSitePatternCompatible( std::map<std::string, size_t> charCounts )
{
    std::map<std::string, size_t>::iterator zero = charCounts.find("0");
    std::map<std::string, size_t>::iterator one  = charCounts.find("1");

    bool compatible = true;
    
    if( charCounts.size() == 1 )
    {
        if(zero != charCounts.end() && (coding & RestrictionCoding::NOABSENCESITES) )
        {
            compatible = false;
        }
        else if(one != charCounts.end()  && (coding & RestrictionCoding::NOPRESENCESITES) )
        {
            compatible = false;
        }
    }
    else
    {
        if(zero != charCounts.end() && zero->second == 1 && (coding & RestrictionCoding::NOSINGLETONLOSSES) )
        {
            compatible = false;
        }
        else if(one != charCounts.end() && one->second == 1 && (coding & RestrictionCoding::NOSINGLETONGAINS) )
        {
            compatible = false;
        }
    }
    
    return compatible;
}

double RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::sumRootLikelihood( void )
{
    double sumPartialProbs = PhyloCTMCSiteHomogeneous<StandardState>::sumRootLikelihood();
    
    if(coding == RestrictionCoding::ALL)
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
        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {    
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::const_iterator         u_i = p_node   + offset;
            
            std::vector<double> logCorrections;
            
            double max = RbConstants::Double::neginf;
            
            double prob = 0.0;
            
            for(size_t ci = 0; ci < this->numChars; ci++)
            {
                // constant site pattern likelihoods
                std::vector<double>::const_iterator         uC_i = u_i  + ci*this->numChars;
                // invert singleton likelihoods
                std::vector<double>::const_iterator         uI_i = uC_i + correctionOffset;
                
                for(size_t c = 0; c < this->numChars; c++)
                {
                    double tmp = 0.0;
                    
                    // c is the character state of the correction pattern
                    if(c == 0)
                    {
                        if(coding & RestrictionCoding::NOABSENCESITES)
                            tmp += uC_i[c];
                        
                        if(coding & RestrictionCoding::NOSINGLETONGAINS)
                            tmp += uI_i[c];
                    }
                    
                    if(c == 1)
                    {
                        // if there is only one observed tip, then don't double-count singleton gains
                        if((coding & RestrictionCoding::NOPRESENCESITES) && maskObservationCounts[mask] > 1)
                            tmp += uC_i[c];
                    
                        // if there are only two observed tips, then don't double-count singleton gains
                        // if there is only one observed tip, then don't double-count absence sites
                        if((coding & RestrictionCoding::NOSINGLETONLOSSES) && maskObservationCounts[mask] > 2)
                            tmp += uI_i[c];
                    }
                    
                    if(tmp == 0.0)
                        continue;
                    
                    if(this->useScaling)
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
            
            if(this->useScaling)
            {
                // use the log-exp-sum to get the sum of the corrections
                prob = exp(RbMath::log_sum_exp(logCorrections, max));
            }
        
            // add corrections for invariant sites
            double p_inv = this->pInv->getValue();
            if(p_inv > 0.0)
            {
                prob *= (1.0 - p_inv);
                
                if(coding & RestrictionCoding::NOABSENCESITES)
                    prob += f[0]*p_inv;
                
                if(coding & RestrictionCoding::NOPRESENCESITES)
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
        perMaskCorrections[mask] = log(perMaskCorrections[mask]) - log(this->numSiteRates);
        
        // apply the correction for this correction mask
        sumPartialProbs -= perMaskCorrections[mask]*correctionMaskCounts[mask];
    }

    return sumPartialProbs;
}