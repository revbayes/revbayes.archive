#include "PhyloCTMCSiteHomogeneousDollo.h"

RevBayesCore::PhyloCTMCSiteHomogeneousDollo::PhyloCTMCSiteHomogeneousDollo(const TypedDagNode<Tree> *t, size_t nc, bool c, size_t nSites, bool amb, RestrictionAscertainmentBias::Coding ty, bool norm) :
    PhyloCTMCSiteHomogeneousConditional<StandardState>(  t, nc, c, nSites, amb, AscertainmentBias::Coding(ty)), integrationFactors(0), normalize(norm)
{
    if(!(coding & RestrictionAscertainmentBias::NOABSENCESITES))
    {
        throw(RbException("Dollo model requires coding=noabsencesites"));
    }

    correctionOffset        = 4;
    correctionMaskOffset    = correctionOffset*2;
    correctionMixtureOffset = numCorrectionMasks*correctionMaskOffset;
    correctionNodeOffset    = this->numSiteRates*correctionMixtureOffset;
    activeCorrectionOffset  = this->num_nodes*correctionNodeOffset;

    correctionLikelihoods = std::vector<double>(activeCorrectionOffset*2, 0.0);

    perMaskMixtureCorrections = std::vector<double>(numSiteRates*num_nodes*numCorrectionMasks, 0.0);

    death_rate = NULL;
    this->addParameter(death_rate);

    survival = std::vector<double>(this->numSiteRates,0.0);
    integrationFactors = std::vector<double>(this->numSiteRates,0.0);
    maskNodeObservationCounts = std::vector<std::vector<size_t> >(numCorrectionMasks, std::vector<size_t>(num_nodes, 0) );

}

RevBayesCore::PhyloCTMCSiteHomogeneousDollo::PhyloCTMCSiteHomogeneousDollo(const PhyloCTMCSiteHomogeneousDollo& n) :
        PhyloCTMCSiteHomogeneousConditional<StandardState>(n),
        integrationFactors(n.integrationFactors),
        maskNodeObservationCounts(n.maskNodeObservationCounts),
        survival(n.survival),
        normalize(n.normalize),
        death_rate(n.death_rate)
{

}

RevBayesCore::PhyloCTMCSiteHomogeneousDollo* RevBayesCore::PhyloCTMCSiteHomogeneousDollo::clone( void ) const {

    return new PhyloCTMCSiteHomogeneousDollo( *this );
}

bool RevBayesCore::PhyloCTMCSiteHomogeneousDollo::isSitePatternCompatible( std::map<size_t, size_t> charCounts )
{
    size_t absent = 0;
    size_t total = 0;

    for(std::map<size_t, size_t>::iterator it = charCounts.begin(); it != charCounts.end(); it++)
    {
        total += it->second;
        if(it->first == 0)
        {
            absent += it->second;
        }
    }

    bool compatible = true;

    if( charCounts.size() == 1 )
    {
        if(absent > 0 && (coding & RestrictionAscertainmentBias::NOABSENCESITES) )
        {
            compatible = false;
        }
        else if(coding & RestrictionAscertainmentBias::NOPRESENCESITES)
        {
            compatible = false;
        }
    }
    else
    {
        if(absent == 1 && (coding & RestrictionAscertainmentBias::NOSINGLETONABSENCE) )
        {
            compatible = false;
        }
        else if(charCounts.size() == 2 && total - absent == 1 && (coding & RestrictionAscertainmentBias::NOSINGLETONPRESENCE) )
        {
            compatible = false;
        }
    }

    return true;
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::resizeLikelihoodVectors( void )
{
    RevBayesCore::PhyloCTMCSiteHomogeneous<StandardState>::resizeLikelihoodVectors();

    perMaskMixtureCorrections = std::vector<double>(numSiteRates*num_nodes*numCorrectionMasks, 0.0);

    maskNodeObservationCounts = std::vector<std::vector<size_t> >(numCorrectionMasks, std::vector<size_t>(num_nodes, 0) );

    correctionOffset        = 4;
    correctionMaskOffset    = correctionOffset*2;
    correctionMixtureOffset = numCorrectionMasks*correctionMaskOffset;
    correctionNodeOffset    = this->numSiteRates*correctionMixtureOffset;
    activeCorrectionOffset  = this->num_nodes*correctionNodeOffset;

    correctionLikelihoods = std::vector<double>(activeCorrectionOffset*2, 0.0);

    integrationFactors = std::vector<double>(numSiteRates, 0.0);
    survival = std::vector<double>(this->numSiteRates, 0.0);

    // set the offsets for easier iteration through the likelihood vector
    siteOffset                  =  numChars + 2;
    mixtureOffset               =  pattern_block_size*siteOffset;
    nodeOffset                  =  numSiteRates*mixtureOffset;
    activeLikelihoodOffset      =  num_nodes*nodeOffset;

    // only do this if we are in MCMC mode. This will safe memory
    if ( inMcmcMode == true )
    {
        // we resize the partial likelihood vectors to the new dimensions
        delete [] partialLikelihoods;

        partialLikelihoods = new double[2*activeLikelihoodOffset];

        // reinitialize likelihood vectors
        for (size_t i = 0; i < 2*activeLikelihoodOffset; i++)
        {
            partialLikelihoods[i] = 0.0;
        }
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::updateTransitionProbabilities(size_t nodeIdx, double brlen)
{
    // first, get the rate matrix for this branch
    const RateGenerator *rg = NULL;
    if ( this->branchHeterogeneousSubstitutionMatrices == true )
    {
        rg = &this->heterogeneousRateMatrices->getValue()[nodeIdx];
    }
    else if(homogeneousRateMatrix != NULL)
    {
        rg = dynamic_cast<const RateMatrix *>(&this->homogeneousRateMatrix->getValue());
    }

    const RateMatrix *rm = dynamic_cast<const RateMatrix *>(rg);
    if ( rm == NULL && rg != NULL)
    {
        throw RbException("Dollo model cannot use RateGenerators that are not RateMatrices");
    }

    // second, get the clock rate for the branch
    double rate = 1.0;
    if ( this->branchHeterogeneousClockRates == true )
    {
        rate = this->heterogeneous_clock_rates->getValue()[nodeIdx];
    }
    else if(this->homogeneous_clock_rate != NULL)
    {
        rate = this->homogeneous_clock_rate->getValue();
    }

    // third, get the death rate
    double death = 1.0;
    if(death_rate != NULL)
    {
        death = death_rate->getValue();
    }

    // and finally compute the per site rate transition probability matrix
    const TopologyNode* node = tau->getValue().getNodes()[nodeIdx];

    double endAge = node->getAge();

    // if the tree is not a time tree, then the age will be not a number
    if ( RbMath::isFinite(endAge) == false )
    {
        // we assume by default that the end is at time 0
        endAge = 0.0;
    }
    double startAge = endAge + node->getBranchLength();

    double mu = death * rate;
    double beta = rate;

    if(rm != NULL && normalize)
    {
        double avg = rm->averageRate();

        beta *= avg / (avg + death);
        mu /= (avg + death);
    }

    if ( this->rateVariationAcrossSites == true )
    {
        const std::vector<double> &r = this->siteRates->getValue();
        for (size_t i = 0; i < this->numSiteRates; ++i)
        {
            double scaled_mu = r[i] * mu;

            if(node->isRoot())
            {
                integrationFactors[i] = 1.0/scaled_mu;
            }
            else
            {
                survival[i] = exp( - scaled_mu * (startAge - endAge) );
                integrationFactors[i] = (1.0 - survival[i])/scaled_mu;

                if(rm != NULL)
                {
                    rm->calculateTransitionProbabilities( startAge, endAge,  r[i] * beta, this->transitionProbMatrices[i] );
                }
                else
                    this->transitionProbMatrices[i][0][0] = 1.0;
            }
        }
    }
    else
    {
        if(node->isRoot())
        {
            integrationFactors[0] = 1.0/mu;
        }
        else
        {
            survival[0] = exp( - mu * (startAge - endAge) );
            integrationFactors[0] = (1.0 - survival[0])/mu;
            if(rm != NULL)
            {
                rm->calculateTransitionProbabilities( startAge, endAge, beta, this->transitionProbMatrices[0] );
            }
            else
                this->transitionProbMatrices[0][0][0] = 1.0;
        }
    }
}

std::vector<double> RevBayesCore::PhyloCTMCSiteHomogeneousDollo::getStationaryFrequencies( size_t nodeIdx ) const
{
    const TopologyNode* node = tau->getValue().getNodes()[nodeIdx];

    if ( node->isRoot() )
    {
        return getRootFrequencies();
    }

    // first, get the rate matrix for this branch
    const RateGenerator *rg = NULL;
    if ( this->branchHeterogeneousSubstitutionMatrices == true )
    {
        rg = &this->heterogeneousRateMatrices->getValue()[nodeIdx];
    }
    else if(this->homogeneousRateMatrix != NULL)
    {
        rg = &this->homogeneousRateMatrix->getValue();
    }

    if(rg != NULL)
    {
        const RateMatrix *rm = dynamic_cast<const RateMatrix *>(rg);
        if ( rm != NULL )
        {
            return rm->getStationaryFrequencies();
        }
        else
        {
            throw RbException("Dollo model cannot use RateGenerators that are not RateMatrices");
        }
    }
    else
    {
        return std::vector<double>(1,1.0);
    }

}

std::vector<double> RevBayesCore::PhyloCTMCSiteHomogeneousDollo::getRootFrequencies( void ) const
{
    if(this->rootFrequencies != NULL && this->branchHeterogeneousSubstitutionMatrices == false && this->homogeneousRateMatrix == NULL)
    {
        throw RbException("Cannot specify root frequencies in Dollo model without first specifying a rate matrix");
    }
    else if ( this->branchHeterogeneousSubstitutionMatrices == true || this->rootFrequencies != NULL )
    {
        if(this->rootFrequencies == NULL)
        {
            throw RbException("Using branch-heterogeneous rate matrices, but no root frequencies have been specified");
        }

        std::vector<double> rf = rootFrequencies->getValue();

        if(rf.size() != this->numChars)
        {
            std::stringstream ss;
            ss << "Number of root frequencies (" << rf.size() << ") does not match the number of character states (" << this->numChars << ")";
            throw RbException(ss.str());
        }

        return rootFrequencies->getValue();
    }
    else if(this->homogeneousRateMatrix != NULL)
    {
        const RateMatrix *rm = dynamic_cast<const RateMatrix *>(&this->homogeneousRateMatrix->getValue());
        if ( rm != NULL )
        {
            if(rm->size() != this->numChars)
            {
                std::stringstream ss;
                ss << "Rate matrix size (" << rm->size() << ") does not match the number of character states (" << this->numChars << ")";
                throw RbException(ss.str());
            }

            return rm->getStationaryFrequencies();
        }
        else
        {
            throw RbException("Dollo model cannot use RateGenerators that are not RateMatrices");
        }

    }
    else
    {
        return std::vector<double>(1,1.0);
    }

}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::setDeathRate(const TypedDagNode< double > *r)
{

    // remove the old parameter first
    if ( death_rate != NULL )
    {
        this->removeParameter( death_rate );
        death_rate = NULL;
    }

    // set the value
    death_rate = r;

    // add the new parameter
    this->addParameter( death_rate );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }

}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeRootLikelihood( size_t root, size_t left, size_t right)
{
    // compute the transition probability matrix
    updateTransitionProbabilities( root, 0 );

    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();

    // get the pointers to the partial likelihoods of the left and right subtree
          double* p        = partialLikelihoods + activeLikelihood[root]  * activeLikelihoodOffset + root  * nodeOffset;
    const double* p_left   = partialLikelihoods + activeLikelihood[left]  * activeLikelihoodOffset + left  * nodeOffset;
    const double* p_right  = partialLikelihoods + activeLikelihood[right] * activeLikelihoodOffset + right * nodeOffset;

    // get pointers the likelihood for both subtrees
          double*   p_mixture          = p;
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;

    size_t n = numChars;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
    {
        // get pointers to the likelihood for this mixture category
              double*   p_site_mixture          = p_mixture;
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < pattern_block_size; ++site)
        {
            p_site_mixture[n + 1] = 0.0;

            bool integrate = ( p_site_mixture_left[n] == 0 || p_site_mixture_right[n] == 0 );

            if(integrate)
            {
                for(size_t c = 0; c < n; c++)
                {
                    p_site_mixture[n + 1] += p_site_mixture_left[c] * p_site_mixture_right[c] * f[c];
                }

                p_site_mixture[n + 1] *= integrationFactors[mixture];
            }

            if( !RbSettings::userSettings().getUseScaling() )
            {
                p_site_mixture[n + 1] += ( p_site_mixture_left[n] > 0 ) * p_site_mixture_right[n + 1] + ( p_site_mixture_left[n] > 0 ) * p_site_mixture_left[n + 1];
            }

            // increment the pointers to the next site
            p_site_mixture+=siteOffset; p_site_mixture_left+=siteOffset; p_site_mixture_right+=siteOffset;

        } // end-for over all sites (=patterns)

        // increment the pointers to the next mixture category
        p_mixture+=mixtureOffset; p_mixture_left+=mixtureOffset; p_mixture_right+=mixtureOffset;

    } // end-for over all mixtures (=rate categories)

    computeRootCorrection(root, left, right);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{
    // compute the transition probability matrix
    updateTransitionProbabilities( root, 0 );

    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();


    // get the pointers to the partial likelihoods of the left and right subtree
          double* p        = partialLikelihoods + activeLikelihood[root]   * activeLikelihoodOffset + root   * nodeOffset;
    const double* p_left   = partialLikelihoods + activeLikelihood[left]   * activeLikelihoodOffset + left   * nodeOffset;
    const double* p_right  = partialLikelihoods + activeLikelihood[right]  * activeLikelihoodOffset + right  * nodeOffset;
    const double* p_middle = partialLikelihoods + activeLikelihood[middle] * activeLikelihoodOffset + middle * nodeOffset;

    // get pointers the likelihood for both subtrees
          double*   p_mixture          = p;
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    const double*   p_mixture_middle   = p_middle;

    size_t n = numChars;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
    {
        // get pointers to the likelihood for this mixture category
              double*   p_site_mixture          = p_mixture;
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        const double*   p_site_mixture_middle   = p_mixture_middle;
        // iterate over all sites
        for (size_t site = 0; site < pattern_block_size; ++site)
        {
            p_site_mixture[n + 1] = 0.0;

            bool integrate = ( p_site_mixture_left[n] == 0 || p_site_mixture_right[n] == 0 || p_site_mixture_middle[n] == 0 );

            if(integrate)
            {
                for(size_t c = 0; c < n; c++)
                {
                    p_site_mixture[n + 1] += p_site_mixture_left[c] * p_site_mixture_right[c] * p_site_mixture_middle[c] * f[c];
                }

                p_site_mixture[n + 1] *= integrationFactors[mixture];
            }

            if( !RbSettings::userSettings().getUseScaling() )
            {
                p_site_mixture[n + 1] += ( p_site_mixture_left[n] > 0 )   * ( p_site_mixture_middle[n] > 0 ) * p_site_mixture_right[n + 1]
                                          + ( p_site_mixture_left[n] > 0 )   * ( p_site_mixture_right[n] > 0 )  * p_site_mixture_middle[n + 1]
                                          + ( p_site_mixture_middle[n] > 0 ) * ( p_site_mixture_right[n] > 0 )  * p_site_mixture_left[n + 1];
            }

            // increment the pointers to the next site
            p_site_mixture+=siteOffset; p_site_mixture_left+=siteOffset; p_site_mixture_right+=siteOffset; p_site_mixture_middle+=siteOffset;

        } // end-for over all sites (=patterns)

        // increment the pointers to the next mixture category
        p_mixture+=mixtureOffset; p_mixture_left+=mixtureOffset; p_mixture_right+=mixtureOffset; p_mixture_middle+=mixtureOffset;

    } // end-for over all mixtures (=rate categories)

    computeRootCorrection(root, left, right, middle);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    // compute the transition probability matrix
    updateTransitionProbabilities( nodeIndex, node.getBranchLength() );

    // get the root frequencies
    const std::vector<double> &f = getStationaryFrequencies(nodeIndex);

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left  = partialLikelihoods + activeLikelihood[left]*activeLikelihoodOffset + left*nodeOffset;
    const double*   p_right = partialLikelihoods + activeLikelihood[right]*activeLikelihoodOffset + right*nodeOffset;
    double*         p_node  = partialLikelihoods + activeLikelihood[nodeIndex]*activeLikelihoodOffset + nodeIndex*nodeOffset;

    size_t n = numChars;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        TransitionProbabilityMatrix pij = transitionProbMatrices[mixture];

        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        // compute the per site probabilities
        for (size_t site = 0; site < pattern_block_size ; ++site)
        {
            p_site_mixture[n] = p_site_mixture_left[n] * p_site_mixture_right[n];

            for(size_t from = 0; from < n; from++)
            {
                p_site_mixture[from] = 0.0;

                for(size_t to = 0; to < n + 1; to++)
                {
                    if(to == n)
                    {
                        p_site_mixture[from] += p_site_mixture_left[to] * p_site_mixture_right[to] * (1.0 - survival[mixture]);
                    }
                    else
                    {
                        p_site_mixture[from] += p_site_mixture_left[to] * p_site_mixture_right[to] * pij[from][to] * survival[mixture];
                    }
                }
            }

            bool integrate = ( p_site_mixture_left[n] == 0 || p_site_mixture_right[n] == 0 );

            p_site_mixture[n + 1] = 0.0;

            if(integrate)
            {
                for(size_t c = 0; c < n; c++)
                {
                    p_site_mixture[n + 1] += p_site_mixture_left[c] * p_site_mixture_right[c] * f[c];
                }

                p_site_mixture[n + 1] *= integrationFactors[mixture];
            }

            if( !RbSettings::userSettings().getUseScaling() )
            {
                p_site_mixture[n + 1] += ( p_site_mixture_left[n] > 0 ) * p_site_mixture_right[n + 1] + ( p_site_mixture_left[n] > 0 ) * p_site_mixture_left[n + 1];
            }

            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset; p_site_mixture_right+=siteOffset; p_site_mixture+=siteOffset;

        } // end-for over all sites (=patterns)

    } // end-for over all mixtures (=rate-categories)

    computeInternalNodeCorrection(node, nodeIndex, left, right);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{

    // compute the transition probability matrix
    updateTransitionProbabilities( nodeIndex, node.getBranchLength() );

    // get the root frequencies
    const std::vector<double> &f = getStationaryFrequencies(nodeIndex);

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left      = partialLikelihoods + activeLikelihood[left]*activeLikelihoodOffset + left*nodeOffset;
    const double*   p_middle    = partialLikelihoods + activeLikelihood[middle]*activeLikelihoodOffset + middle*nodeOffset;
    const double*   p_right     = partialLikelihoods + activeLikelihood[right]*activeLikelihoodOffset + right*nodeOffset;
    double*         p_node      = partialLikelihoods + activeLikelihood[nodeIndex]*activeLikelihoodOffset + nodeIndex*nodeOffset;

    size_t n = numChars;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        TransitionProbabilityMatrix pij = transitionProbMatrices[mixture];

        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_middle   = p_middle + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        // compute the per site probabilities
        for (size_t site = 0; site < pattern_block_size ; ++site)
        {
            p_site_mixture[n] = p_site_mixture_left[n] * p_site_mixture_right[n] * p_site_mixture_middle[n];

            for(size_t from = 0; from < n; from++)
            {
                p_site_mixture[from] = 0.0;

                for(size_t to = 0; to < n + 1; to++)
                {
                    if(to == n)
                    {
                        p_site_mixture[from] += p_site_mixture_left[to] * p_site_mixture_right[to] * p_site_mixture_middle[to] * (1.0 - survival[mixture]);
                    }
                    else
                    {
                        p_site_mixture[from] += p_site_mixture_left[to] * p_site_mixture_right[to] * p_site_mixture_middle[to] * pij[from][to] * survival[mixture];
                    }
                }
            }

            bool integrate = ( p_site_mixture_left[n] == 0 || p_site_mixture_right[n] == 0 || p_site_mixture_middle[n] == 0 );

            p_site_mixture[n + 1] = 0.0;

            if(integrate)
            {
                for(size_t c = 0; c < n; c++)
                {
                    p_site_mixture[n + 1] += p_site_mixture_left[c] * p_site_mixture_right[c] * p_site_mixture_middle[c] * f[c];
                }

                p_site_mixture[n + 1] *= integrationFactors[mixture];
            }

            if( !RbSettings::userSettings().getUseScaling() )
            {
                p_site_mixture[n + 1] += ( p_site_mixture_left[n] > 0 )   * ( p_site_mixture_middle[n] > 0 ) * p_site_mixture_right[n + 1]
                                          + ( p_site_mixture_left[n] > 0 )   * ( p_site_mixture_right[n] > 0 )  * p_site_mixture_middle[n + 1]
                                          + ( p_site_mixture_middle[n] > 0 ) * ( p_site_mixture_right[n] > 0 )  * p_site_mixture_left[n + 1];
            }

            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset; p_site_mixture_middle+=siteOffset; p_site_mixture_right+=siteOffset; p_site_mixture+=siteOffset;

        } // end-for over all sites (=patterns)

    } // end-for over all mixtures (=rate-categories)

    computeInternalNodeCorrection(node, nodeIndex, left, right, middle);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
{

    double* p_node = partialLikelihoods + activeLikelihood[nodeIndex]*activeLikelihoodOffset + nodeIndex*nodeOffset;

    const std::vector<bool> &gap_node = gapMatrix[nodeIndex];
    const std::vector<unsigned long> &char_node = charMatrix[nodeIndex];

    // compute the transition probabilities
    updateTransitionProbabilities( nodeIndex, node.getBranchLength() );

    // get the root frequencies
    const std::vector<double> &f = getStationaryFrequencies(nodeIndex);

    double*   p_mixture      = p_node;

    size_t n = numChars;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        TransitionProbabilityMatrix pij = transitionProbMatrices[mixture];

        // get the pointer to the likelihoods for this site and mixture category
        double*     p_site_mixture      = p_mixture;

        // iterate over all sites
        for (size_t site = 0; site != pattern_block_size; ++site)
        {

            // is this site a gap?
            if ( gap_node[site] )
            {
                for(size_t c = 0; c < n + 1; c++)
                {
                    p_site_mixture[c] = 1.0;
                }

                p_site_mixture[n + 1] = integrationFactors[mixture];
            }
            else // we have observed a character
            {

                // get the original character
                // shift the characters so that a zero state is the (n-1)th state
                unsigned long org_val = char_node[site] == 0 ? n : char_node[site] - 1;

                // store the branch likelihoods and integrated node likelihood
                for(size_t c = 0; c < n; c++)
                {
                    p_site_mixture[c] = pij[c][org_val];
                }

                p_site_mixture[n] = org_val == n;
                p_site_mixture[n + 1] = org_val == n ? 0.0 : f[org_val] * integrationFactors[mixture];


            } // end-if a gap state

            // increment the pointers to next site
            p_site_mixture+=siteOffset;

        } // end-for over all sites/patterns in the sequence

        // increment the pointers to next mixture category
        p_mixture+=mixtureOffset;

    } // end-for over all mixture categories

    computeTipCorrection(node, nodeIndex);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeTipCorrection(const TopologyNode &node, size_t nodeIndex)
{
    std::vector<double>::iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*this->activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator         u      = p_node + offset;

            bool gap = correctionMaskMatrix[mask][nodeIndex];

            for(size_t ci = 0; ci < 2; ci++)
            {
                std::vector<double>::iterator         uC = u + ci*2;
                std::vector<double>::iterator         uI = uC + correctionOffset;

                double survival_prob = ci == 0 ? 0.0 : survival[mixture];

                // Probability of constant state 0 this tip
                // given ancestral state ci
                uC[0] = 1.0 - survival_prob;

                // Probability of invert singleton state 0 this tip
                // given ancestral state ci
                uI[0] = !gap * survival_prob;

                // Probability of constant state 1 this tip
                // given ancestral state ci
                uC[1] = !gap * survival_prob;

                // Probability of invert singleton state 1 this tip
                // given ancestral state ci
                uI[1] = 1.0 - survival_prob;
            }
        }
    }

    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        maskNodeObservationCounts[mask][nodeIndex] = !correctionMaskMatrix[mask][nodeIndex];
    }

    computeIntegratedNodeCorrection(nodeIndex);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator                 u = p_node   + offset;
            std::vector<double>::const_iterator         u_l = p_left   + offset;
            std::vector<double>::const_iterator         u_r = p_right  + offset;
            std::vector<double>::const_iterator         u_m = p_middle + offset;

            for(size_t ci = 0; ci < 2; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*2;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;

                double survival_prob = ci == 0 ? 0.0 : survival[mixture];

                for(size_t c = 0; c < 2; c++)
                {
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;

                    for(size_t cj = 0; cj < 2; cj++)
                    {
                        std::vector<double>::const_iterator         lC_j = u_l  + cj*2;
                        std::vector<double>::const_iterator         lI_j = lC_j + correctionOffset;

                        std::vector<double>::const_iterator         rC_j = u_r  + cj*2;
                        std::vector<double>::const_iterator         rI_j = rC_j + correctionOffset;

                        std::vector<double>::const_iterator         mC_j = u_m  + cj*2;
                        std::vector<double>::const_iterator         mI_j = mC_j + correctionOffset;

                        double Pij = cj == 0 ? 1.0 - survival_prob : survival_prob;

                        // probability of constant state c descending from this node
                        // given ancestral state ci and node state cj
                        uC_i[c] += Pij * lC_j[0] * rC_j[0] * mC_j[0];

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

    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        maskNodeObservationCounts[mask][nodeIndex] = maskNodeObservationCounts[mask][left] + maskNodeObservationCounts[mask][right] + maskNodeObservationCounts[mask][middle];
    }

    computeIntegratedNodeCorrection(nodeIndex);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        for(size_t mask = 0; mask < numCorrectionMasks; mask++){

            size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset;

            std::vector<double>::iterator                 u = p_node   + offset;
            std::vector<double>::const_iterator         u_l = p_left   + offset;
            std::vector<double>::const_iterator         u_r = p_right  + offset;

            for(size_t ci = 0; ci < 2; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*2;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;

                double survival_prob = ci == 0 ? 0.0 : survival[mixture];

                for(size_t c = 0; c < 2; c++)
                {
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;

                    for(size_t cj = 0; cj < 2; cj++)
                    {
                        std::vector<double>::const_iterator         lC_j = u_l  + cj*2;
                        std::vector<double>::const_iterator         lI_j = lC_j + correctionOffset;

                        std::vector<double>::const_iterator         rC_j = u_r  + cj*2;
                        std::vector<double>::const_iterator         rI_j = rC_j + correctionOffset;

                        double Pij = cj == 0 ? 1.0 - survival_prob : survival_prob;

                        // probability of constant state c descending from this node
                        // given ancestral state ci and node state cj
                        uC_i[c] += Pij * lC_j[0] * rC_j[0];

                        // probability of invert singleton state c descending from
                        // given ancestral state ci and node state cj
                        uI_i[c] += Pij * lI_j[c] * rC_j[c]
                                 + Pij * lC_j[c] * rI_j[c];
                    }
                }
            }
        }
    }

    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        maskNodeObservationCounts[mask][nodeIndex] = maskNodeObservationCounts[mask][left] + maskNodeObservationCounts[mask][right];
    }

    computeIntegratedNodeCorrection(nodeIndex);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeRootCorrection( size_t root, size_t left, size_t right, size_t middle)
{
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

            for(size_t ci = 1; ci < 2; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*2;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;

                std::vector<double>::const_iterator         lC_i = u_l  + ci*2;
                std::vector<double>::const_iterator         lI_i = lC_i + correctionOffset;

                std::vector<double>::const_iterator         rC_i = u_r  + ci*2;
                std::vector<double>::const_iterator         rI_i = rC_i + correctionOffset;

                std::vector<double>::const_iterator         mC_i = u_m  + ci*2;
                std::vector<double>::const_iterator         mI_i = mC_i + correctionOffset;

                for(size_t c = 0; c < 2; c++)
                {
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;

                    // probability of constant state c descending from this node
                    // given ancestral state ci
                    uC_i[c] += lC_i[c] * rC_i[c] * mC_i[c];

                    // probability of invert singleton state c descending from this node
                    // given ancestral state ci
                    uI_i[c] += lI_i[c] * rC_i[c] * mC_i[c]
                             + lC_i[c] * rI_i[c] * mC_i[c]
                             + lC_i[c] * rC_i[c] * mI_i[c];
                }
            }
        }
    }

    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        maskNodeObservationCounts[mask][root] = maskNodeObservationCounts[mask][left] + maskNodeObservationCounts[mask][right] + maskNodeObservationCounts[mask][middle];
    }

    computeIntegratedNodeCorrection(root);
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeRootCorrection( size_t root, size_t left, size_t right)
{
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

            for(size_t ci = 1; ci < 2; ci++)
            {
                std::vector<double>::iterator         uC_i = u  + ci*2;
                std::vector<double>::iterator         uI_i = uC_i + correctionOffset;

                std::vector<double>::const_iterator         lC_i = u_l  + ci*2;
                std::vector<double>::const_iterator         lI_i = lC_i + correctionOffset;

                std::vector<double>::const_iterator         rC_i = u_r  + ci*2;
                std::vector<double>::const_iterator         rI_i = rC_i + correctionOffset;

                for(size_t c = 0; c < 2; c++)
                {
                    uC_i[c] = 0.0;
                    uI_i[c] = 0.0;

                    // probability of constant state c descending from this node
                    // given ancestral state ci
                    uC_i[c] += lC_i[c] * rC_i[c];

                    // probability of invert singleton state c descending from this node
                    // given ancestral state ci
                    uI_i[c] += lI_i[c] * rC_i[c]
                             + lC_i[c] * rI_i[c];
                }
            }
        }
    }

    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        maskNodeObservationCounts[mask][root] = maskNodeObservationCounts[mask][left] + maskNodeObservationCounts[mask][right];
    }

    computeIntegratedNodeCorrection(root);
}


void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeIntegratedNodeCorrection( size_t nodeIndex )
{
    std::vector<double>::const_iterator p_node = correctionLikelihoods.begin() + activeLikelihood[nodeIndex] * activeCorrectionOffset  + nodeIndex*correctionNodeOffset;

    // iterate over each correction mask
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        std::vector<double>::iterator c_node = perMaskMixtureCorrections.begin() + mask*num_nodes*numSiteRates + nodeIndex*numSiteRates;

        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
        {
            // constant site pattern likelihoods
            std::vector<double>::const_iterator         uC_i = p_node + 2; // ci = 1
            // invert singleton likelihoods
            std::vector<double>::const_iterator         uI_i = uC_i + correctionOffset;

            std::vector<double> logCorrections;

            double prob = 0.0;

            if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
                prob += uC_i[0];

            if(coding & RestrictionAscertainmentBias::NOSINGLETONPRESENCE)
                prob += uI_i[0];

            // if there is only one observed tip, then don't double-count singleton gains
            if((coding & RestrictionAscertainmentBias::NOPRESENCESITES) && maskNodeObservationCounts[mask][nodeIndex] == maskObservationCounts[mask] && maskObservationCounts[mask] > 1)
                prob += uC_i[1];

            // if there are only two observed tips, then don't double-count singleton gains
            // if there is only one observed tip, then don't double-count absence sites
            // if the number of observed tips from this node is 1 less than the total number of observed tips, then add the constant prob
            if((coding & RestrictionAscertainmentBias::NOSINGLETONABSENCE) && maskObservationCounts[mask] > 2)
            {
                if(maskNodeObservationCounts[mask][nodeIndex] == maskObservationCounts[mask])
                    prob += uI_i[1];
                else if(maskNodeObservationCounts[mask][nodeIndex] == maskObservationCounts[mask] - 1)
                    prob += uC_i[1];
            }

            prob = integrationFactors[mixture]*(1.0 - prob);

            if(prob <= 0.0)
                prob = 0.0;

            c_node[mixture] = prob;
        }
    }

}

double RevBayesCore::PhyloCTMCSiteHomogeneousDollo::sumRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = tau->getValue().getRoot();

    // get the index of the root node
    size_t root_index = root.getIndex();

    double*   p_root  = this->partialLikelihoods + this->activeLikelihood[root_index] * this->activeLikelihoodOffset + root_index*nodeOffset;

    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(pattern_block_size,0.0);

    // iterate over all mixture categories
    for (size_t site = 0; site < pattern_block_size; ++site)
    {
        if ( RbSettings::userSettings().getUseScaling() )
        {
            std::vector<double> integrated_likelihoods;

            double max = getScaledNodeWeights(root, site, integrated_likelihoods);

            per_mixture_Likelihoods[site] = RbMath::log_sum_exp(integrated_likelihoods, max);
        }
        else
        {
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                double*   p_node  = p_root + mixture*mixtureOffset + site*siteOffset;

                per_mixture_Likelihoods[site] += p_node[numChars + 1];

            } // end-for over all sites (=patterns)
        }

    } // end-for over all mixtures (=rate categories)

    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;

    std::vector< size_t >::const_iterator patterns = this->patternCounts.begin();
    for (size_t site = 0; site < pattern_block_size; ++site, ++patterns)
    {
        if ( RbSettings::userSettings().getUseScaling() )
        {
            sumPartialProbs += ( per_mixture_Likelihoods[site] - log(this->numSiteRates ) ) * *patterns;
        }
        else
        {
            sumPartialProbs += log( per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
        }

    }


#ifdef RB_MPI

    // we only need to send message if there is more than one process
    if ( num_processes > 1 )
    {

        // send the likelihood from the helpers to the master
        if ( process_active == false )
        {
            // send from the workers the log-likelihood to the master
            MPI::COMM_WORLD.Send(&sumPartialProbs, 1, MPI::DOUBLE, active_PID, 0);
        }

        // receive the likelihoods from the helpers
        if ( process_active == true )
        {
            for (size_t i=active_PID+1; i<active_PID+num_processes; ++i)
            {
                double tmp = 0;
                MPI::COMM_WORLD.Recv(&tmp, 1, MPI::DOUBLE, int(i), 0);
                sumPartialProbs += tmp;
            }
        }

        // now send back the combined likelihood to the helpers
        if ( process_active == true )
        {
            for (size_t i=active_PID+1; i<active_PID+num_processes; ++i)
            {
                MPI::COMM_WORLD.Send(&sumPartialProbs, 1, MPI::DOUBLE, int(i), 0);
            }
        }
        else
        {
            MPI::COMM_WORLD.Recv(&sumPartialProbs, 1, MPI::DOUBLE, active_PID, 0);
        }
    
    }

#endif

    std::vector<double> perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);
    
    // iterate over each correction mask
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        for(size_t node_index = 0; node_index < num_nodes; node_index++)
        {
            std::vector<double>::iterator c_node = perMaskMixtureCorrections.begin() + mask*num_nodes*numSiteRates + node_index*numSiteRates;
            
            // iterate over all mixture categories
            for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
            {
                perMaskCorrections[mask] += c_node[mixture];
            }
        }
        
        if(perMaskCorrections[mask] <= 0.0)
            perMaskCorrections[mask] = RbConstants::Double::inf;
        
        perMaskCorrections[mask] = log(perMaskCorrections[mask]) - log(numSiteRates);

        // apply the correction for this correction mask
        sumPartialProbs -= perMaskCorrections[mask]*correctionMaskCounts[mask];
    }

    // num_sites is poisson
    sumPartialProbs -= log(num_sites);

    return sumPartialProbs;
}

double RevBayesCore::PhyloCTMCSiteHomogeneousDollo::getScaledNodeWeights(const TopologyNode &node, size_t pattern, std::vector<double>& weights)
{
    //reset the per site likelihood
    double max = RbConstants::Double::neginf;

    size_t nodeIndex = node.getIndex();

    const double* p_node  = partialLikelihoods + activeLikelihood[nodeIndex] * activeLikelihoodOffset  + nodeIndex*nodeOffset + pattern*siteOffset;

    double logScalingFactor = perNodeSiteLogScalingFactors[activeLikelihood[nodeIndex]][nodeIndex][pattern];

    //otherwise, it is an ancestral node so we add the integrated likelihood
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
    {
        size_t offset = mixture*mixtureOffset;

        // get the pointers to the partial likelihoods of the left and right subtree
        const double* p_mixture        = p_node  + offset;

        double prob = log(p_mixture[numChars + 1]) + logScalingFactor;

        max = std::max(prob, max);

        weights.push_back( prob );
    }

    if(node.isTip())
        return max;

    std::vector<TopologyNode*> children = node.getChildren();

    TopologyNode* child;
    size_t num_with_descendants = 0;

    for(size_t i = 0; i < children.size(); i++)
    {
        size_t child_index = children[i]->getIndex();
        const double* p_child  = partialLikelihoods + activeLikelihood[child_index]  * activeLikelihoodOffset + child_index*nodeOffset  + pattern*siteOffset;

        // does this child have descendants?
        if(p_child[numChars] == 0)
        {
            child = children[i];
            num_with_descendants++;
        }
    }

    //if more than one child has observed descendants, then this is the last ancestral node
    if(num_with_descendants > 1)
    {
        max = std::max(max, getScaledNodeWeights(*child, pattern, weights));
    }

    return max;
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::scale( size_t nodeIndex)
{

    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    if ( RbSettings::userSettings().getUseScaling() == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {

            // the max probability
            double max = 0.0;

            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<numChars; ++i)
                {
                    if ( p_site_mixture[i] > max )
                    {
                        max = p_site_mixture[i];
                    }
                }

            }

            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = -log(max);


            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<numChars+1; ++i)
                {
                    p_site_mixture[i] /= max;
                }

            }

        }
    }
    else if ( RbSettings::userSettings().getUseScaling() == true )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = 0;
        }

    }
}


void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::scale( size_t nodeIndex, size_t left, size_t right )
{

    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    if ( RbSettings::userSettings().getUseScaling() == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {

            // the max probability
            double max = 0.0;

            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<numChars; ++i)
                {
                    if ( p_site_mixture[i] > max )
                    {
                        max = p_site_mixture[i];
                    }
                }

            }

            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] - log(max);


            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<numChars+1; ++i)
                {
                    p_site_mixture[i] /= max;
                }

            }

        }
    }
    else if ( RbSettings::userSettings().getUseScaling() == true )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site];
        }

    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::scale( size_t nodeIndex, size_t left, size_t right, size_t middle )
{

    double* p_node   = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    if ( RbSettings::userSettings().getUseScaling() == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {

            // the max probability
            double max = 0.0;

            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<numChars; ++i)
                {
                    if ( p_site_mixture[i] > max )
                    {
                        max = p_site_mixture[i];
                    }
                }

            }

            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[middle]][middle][site] - log(max);


            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<numChars+1; ++i)
                {
                    p_site_mixture[i] /= max;
                }

            }

        }
    }
    else if ( RbSettings::userSettings().getUseScaling() == true )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[middle]][middle][site];
        }

    }
}

/** Swap a parameter of the distribution */
void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == death_rate)
    {
        death_rate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else
    {
        PhyloCTMCSiteHomogeneousConditional<StandardState>::swapParameterInternal(oldP,newP);
    }

}
