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

    massNodeOffset = this->numSiteRates*numCorrectionMasks;
    activeMassOffset = this->num_nodes*massNodeOffset;
    perMaskMixtureCorrections = std::vector<double>(2*activeMassOffset, 0.0);

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
        death_rate(n.death_rate),
        massNodeOffset(n.massNodeOffset),
        activeMassOffset(n.activeMassOffset)
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

    massNodeOffset = numSiteRates*numCorrectionMasks;
    activeMassOffset = num_nodes*massNodeOffset;
    perMaskMixtureCorrections = std::vector<double>(2*activeMassOffset, 0.0);

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

            p_site_mixture[n] = p_site_mixture_left[n] * p_site_mixture_right[n];

            if(p_site_mixture[n] == 0)
            {
                for(size_t c = 0; c < n; c++)
                {
                    p_site_mixture[n + 1] += p_site_mixture_left[c] * p_site_mixture_right[c] * f[c];
                }

                p_site_mixture[n + 1] *= integrationFactors[mixture];
            }

            if( !RbSettings::userSettings().getUseScaling() )
            {
                p_site_mixture[n + 1] += ( p_site_mixture_left[n] > 0 ) * p_site_mixture_right[n + 1] + ( p_site_mixture_right[n] > 0 ) * p_site_mixture_left[n + 1];
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

            p_site_mixture[n] = p_site_mixture_left[n] * p_site_mixture_right[n] * p_site_mixture_middle[n];

            if(p_site_mixture[n] == 0)
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
        const double* tp_begin = transitionProbMatrices[mixture].theMatrix;

        // get the pointers to the likelihood for this mixture category
        double*          p_site_mixture          = p_node;
        const double*    p_site_mixture_left     = p_left;
        const double*    p_site_mixture_right    = p_right;
        // compute the per site probabilities
        for (size_t site = 0; site < pattern_block_size ; ++site)
        {
            const double* tp_a = tp_begin;

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
                        p_site_mixture[from] += p_site_mixture_left[to] * p_site_mixture_right[to] * tp_a[to] * survival[mixture];
                    }
                }

                tp_a += this->numChars;
            }

            p_site_mixture[n + 1] = 0.0;

            if(p_site_mixture[n] == 0)
            {
                for(size_t c = 0; c < n; c++)
                {
                    p_site_mixture[n + 1] += p_site_mixture_left[c] * p_site_mixture_right[c] * f[c];
                }

                p_site_mixture[n + 1] *= integrationFactors[mixture];
            }

            if( !RbSettings::userSettings().getUseScaling() )
            {
                p_site_mixture[n + 1] += ( p_site_mixture_left[n] > 0 ) * p_site_mixture_right[n + 1] + ( p_site_mixture_right[n] > 0 ) * p_site_mixture_left[n + 1];
            }

            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset; p_site_mixture_right+=siteOffset; p_site_mixture+=siteOffset;

        } // end-for over all sites (=patterns)

        p_left+=mixtureOffset; p_right+=mixtureOffset; p_node+=mixtureOffset;
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
        const double* tp_begin = transitionProbMatrices[mixture].theMatrix;

        // get the pointers to the likelihood for this mixture category
        double*          p_site_mixture          = p_node;
        const double*    p_site_mixture_left     = p_left;
        const double*    p_site_mixture_middle   = p_middle;
        const double*    p_site_mixture_right    = p_right;
        // compute the per site probabilities
        for (size_t site = 0; site < pattern_block_size ; ++site)
        {
            const double* tp_a = tp_begin;

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
                        p_site_mixture[from] += p_site_mixture_left[to] * p_site_mixture_right[to] * p_site_mixture_middle[to] * tp_a[to] * survival[mixture];
                    }
                }

                tp_a += this->numChars;
            }

            p_site_mixture[n + 1] = 0.0;

            if(p_site_mixture[n] == 0)
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

        p_left+=mixtureOffset; p_middle+=mixtureOffset; p_right+=mixtureOffset; p_node+=mixtureOffset;

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
        const double* tp_begin = transitionProbMatrices[mixture].theMatrix;

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
                    if(org_val == n)
                    {
                        p_site_mixture[c] = 1.0 - survival[mixture];
                    }
                    else
                    {
                        p_site_mixture[c] = tp_begin[c*this->numChars + org_val] * survival[mixture];
                    }
                }

                p_site_mixture[n] = (org_val == n);
                p_site_mixture[n + 1] = (org_val == n) ? 0.0 : f[org_val] * integrationFactors[mixture];


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

    std::vector<double>::iterator c_node = perMaskMixtureCorrections.begin() + this->activeLikelihood[nodeIndex]*activeMassOffset + nodeIndex*massNodeOffset;

    std::vector<double> partialNodeCorrections(4, 0.0);
    partialNodeCorrections[0] = 0;
    partialNodeCorrections[3] = 0;

    std::vector<double>::iterator c_mask_node   = c_node;
    std::vector<double>::iterator p_mask_node   = p_node;

    // iterate over all mixture categories
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        maskNodeObservationCounts[mask][nodeIndex] = !correctionMaskMatrix[mask][nodeIndex];

        bool gap = correctionMaskMatrix[mask][nodeIndex];

        // Probabilities
        // given node state 1
        partialNodeCorrections[1] = !gap;
        partialNodeCorrections[2] = !gap;

        std::vector<double>::iterator u = p_mask_node;

        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
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
                        double Pij = cj == 0 ? 1.0 - survival_prob : survival_prob;

                        // Probability of constant state c at this tip and node state cj
                        // given ancestral state ci
                        uC_i[c] += ((c == cj) && !gap) * Pij;

                        // Probability of invert singleton state c at this tip and node state cj
                        // given ancestral state ci
                        uI_i[c] += ((c != cj) && !gap) * Pij;
                    }
                }
            }

            c_mask_node[mixture] = computeIntegratedNodeCorrection(partialNodeCorrections, nodeIndex, mask, mixture);

            u += correctionMixtureOffset;
        }

        p_mask_node += correctionMaskOffset; c_mask_node += numSiteRates;
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    std::vector<double>::iterator c_node   = perMaskMixtureCorrections.begin() + this->activeLikelihood[nodeIndex]*activeMassOffset + nodeIndex*massNodeOffset;
    std::vector<double>::iterator c_left   = perMaskMixtureCorrections.begin() + this->activeLikelihood[left]*activeMassOffset + left*massNodeOffset;
    std::vector<double>::iterator c_right  = perMaskMixtureCorrections.begin() + this->activeLikelihood[right]*activeMassOffset + right*massNodeOffset;
    std::vector<double>::iterator c_middle = perMaskMixtureCorrections.begin() + this->activeLikelihood[middle]*activeMassOffset + middle*massNodeOffset;

    std::vector<double> partialNodeCorrections(4, 0.0);

    std::vector<double>::iterator c_mask_node   = c_node;
    std::vector<double>::iterator c_mask_left   = c_left;
    std::vector<double>::iterator c_mask_right  = c_right;
    std::vector<double>::iterator c_mask_middle = c_middle;

    std::vector<double>::iterator        p_mask_node   = p_node;
    std::vector<double>::const_iterator  p_mask_left   = p_left;
    std::vector<double>::const_iterator  p_mask_right  = p_right;
    std::vector<double>::const_iterator  p_mask_middle = p_middle;

    // iterate over all mixture categories
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        maskNodeObservationCounts[mask][nodeIndex] = maskNodeObservationCounts[mask][left] + maskNodeObservationCounts[mask][right] + maskNodeObservationCounts[mask][middle];

        std::vector<double>::iterator                 u = p_mask_node;
        std::vector<double>::const_iterator         u_l = p_mask_left;
        std::vector<double>::const_iterator         u_r = p_mask_right;
        std::vector<double>::const_iterator         u_m = p_mask_middle;

        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
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
                        double C = lC_j[c] * rC_j[c] * mC_j[c];

                        uC_i[c] += Pij * C;

                        // probability of invert singleton state c descending from
                        // given ancestral state ci and node state cj
                        double I = lI_j[c] * rC_j[c] * mC_j[c]
                                 + lC_j[c] * rI_j[c] * mC_j[c]
                                 + lC_j[c] * rC_j[c] * mI_j[c];

                        uI_i[c] += Pij * I;

                        if(cj == 1)
                        {
                            partialNodeCorrections[c*2] = C;
                            partialNodeCorrections[c*2 + 1] = I;
                        }
                    }
                }
            }

            c_mask_node[mixture] = computeIntegratedNodeCorrection(partialNodeCorrections, nodeIndex, mask, mixture);
            //c_mask_node[mixture] += c_mask_left[mixture] + c_mask_right[mixture] + c_mask_middle[mixture];

            u += correctionMixtureOffset; u_l += correctionMixtureOffset; u_r += correctionMixtureOffset; u_m += correctionMixtureOffset;
        }

        p_mask_node += correctionMaskOffset; p_mask_left += correctionMaskOffset; p_mask_right += correctionMaskOffset; p_mask_middle += correctionMaskOffset;
        c_mask_node += numSiteRates; c_mask_left += numSiteRates; c_mask_right += numSiteRates; c_mask_middle += numSiteRates;
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    std::vector<double>::iterator c_node   = perMaskMixtureCorrections.begin() + this->activeLikelihood[nodeIndex]*activeMassOffset + nodeIndex*massNodeOffset;
    std::vector<double>::iterator c_left   = perMaskMixtureCorrections.begin() + this->activeLikelihood[left]*activeMassOffset + left*massNodeOffset;
    std::vector<double>::iterator c_right  = perMaskMixtureCorrections.begin() + this->activeLikelihood[right]*activeMassOffset + right*massNodeOffset;

    std::vector<double> partialNodeCorrections(4, 0.0);

    std::vector<double>::iterator c_mask_node   = c_node;
    std::vector<double>::iterator c_mask_left   = c_left;
    std::vector<double>::iterator c_mask_right  = c_right;

    std::vector<double>::iterator        p_mask_node   = p_node;
    std::vector<double>::const_iterator  p_mask_left   = p_left;
    std::vector<double>::const_iterator  p_mask_right  = p_right;

    // iterate over all mixture categories
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        std::vector<double>::iterator                 u = p_mask_node;
        std::vector<double>::const_iterator         u_l = p_mask_left;
        std::vector<double>::const_iterator         u_r = p_mask_right;

        maskNodeObservationCounts[mask][nodeIndex] = maskNodeObservationCounts[mask][left] + maskNodeObservationCounts[mask][right];

        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
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
                        double C = lC_j[c] * rC_j[c];

                        uC_i[c] += Pij * C;

                        // probability of invert singleton state c descending from
                        // given ancestral state ci and node state cj
                        double I = lI_j[c] * rC_j[c]
                                 + lC_j[c] * rI_j[c];

                        uI_i[c] += Pij * I;

                        if(cj == 1)
                        {
                            partialNodeCorrections[c*2] = C;
                            partialNodeCorrections[c*2 + 1] = I;
                        }
                    }
                }
            }

            c_mask_node[mixture] = computeIntegratedNodeCorrection(partialNodeCorrections, nodeIndex, mask, mixture);
            //c_mask_node[mixture] += c_mask_left[mixture] + c_mask_right[mixture];

            u += correctionMixtureOffset; u_l += correctionMixtureOffset; u_r += correctionMixtureOffset;
        }

        p_mask_node += correctionMaskOffset; p_mask_left += correctionMaskOffset; p_mask_right += correctionMaskOffset;
        c_mask_node += numSiteRates; c_mask_left += numSiteRates; c_mask_right += numSiteRates;
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeRootCorrection( size_t root, size_t left, size_t right, size_t middle)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;

    std::vector<double>::iterator c_node   = perMaskMixtureCorrections.begin() + this->activeLikelihood[root]*activeMassOffset + root*massNodeOffset;
    std::vector<double>::iterator c_left   = perMaskMixtureCorrections.begin() + this->activeLikelihood[left]*activeMassOffset + left*massNodeOffset;
    std::vector<double>::iterator c_right  = perMaskMixtureCorrections.begin() + this->activeLikelihood[right]*activeMassOffset + right*massNodeOffset;
    std::vector<double>::iterator c_middle = perMaskMixtureCorrections.begin() + this->activeLikelihood[middle]*activeMassOffset + middle*massNodeOffset;

    std::vector<double> partialNodeCorrections(4, 0.0);

    std::vector<double>::iterator c_mask_node   = c_node;
    std::vector<double>::iterator c_mask_left   = c_left;
    std::vector<double>::iterator c_mask_right  = c_right;
    std::vector<double>::iterator c_mask_middle = c_middle;

    std::vector<double>::const_iterator  p_mask_left   = p_left;
    std::vector<double>::const_iterator  p_mask_right  = p_right;
    std::vector<double>::const_iterator  p_mask_middle = p_middle;

    // iterate over all mixture categories
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        std::vector<double>::const_iterator         u_l = p_mask_left;
        std::vector<double>::const_iterator         u_r = p_mask_right;
        std::vector<double>::const_iterator         u_m = p_mask_middle;

        maskNodeObservationCounts[mask][root] = maskNodeObservationCounts[mask][left] + maskNodeObservationCounts[mask][right] + maskNodeObservationCounts[mask][middle];

        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
            std::vector<double>::const_iterator         lC_i = u_l + 2; // ci = 1
            std::vector<double>::const_iterator         lI_i = lC_i + correctionOffset;

            std::vector<double>::const_iterator         rC_i = u_r + 2;
            std::vector<double>::const_iterator         rI_i = rC_i + correctionOffset;

            std::vector<double>::const_iterator         mC_i = u_m + 2;
            std::vector<double>::const_iterator         mI_i = mC_i + correctionOffset;

            for(size_t c = 0; c < 2; c++)
            {
                // probability of constant state c descending from this node
                // given node state 1
                partialNodeCorrections[c*2] = lC_i[c] * rC_i[c] * mC_i[c];

                // probability of invert singleton state c descending from this node
                // given node state 1
                partialNodeCorrections[c*2 + 1] = lI_i[c] * rC_i[c] * mC_i[c]
                                                + lC_i[c] * rI_i[c] * mC_i[c]
                                                + lC_i[c] * rC_i[c] * mI_i[c];
            }

            c_mask_node[mixture] = computeIntegratedNodeCorrection(partialNodeCorrections, root, mask, mixture);
            //c_mask_node[mixture] += c_mask_left[mixture] + c_mask_right[mixture] + c_mask_middle[mixture];

            u_l += correctionMixtureOffset; u_r += correctionMixtureOffset; u_m += correctionMixtureOffset;
        }

        p_mask_left += correctionMaskOffset; p_mask_right += correctionMaskOffset; p_mask_middle += correctionMaskOffset;
        c_mask_node += numSiteRates; c_mask_left += numSiteRates; c_mask_right += numSiteRates; c_mask_middle += numSiteRates;
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeRootCorrection( size_t root, size_t left, size_t right)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;

    std::vector<double>::iterator c_node   = perMaskMixtureCorrections.begin() + this->activeLikelihood[root]*activeMassOffset + root*massNodeOffset;
    std::vector<double>::iterator c_left   = perMaskMixtureCorrections.begin() + this->activeLikelihood[left]*activeMassOffset + left*massNodeOffset;
    std::vector<double>::iterator c_right  = perMaskMixtureCorrections.begin() + this->activeLikelihood[right]*activeMassOffset + right*massNodeOffset;

    std::vector<double> partialNodeCorrections(4, 0.0);

    std::vector<double>::iterator c_mask_node   = c_node;
    std::vector<double>::iterator c_mask_left   = c_left;
    std::vector<double>::iterator c_mask_right  = c_right;

    std::vector<double>::const_iterator  p_mask_left   = p_left;
    std::vector<double>::const_iterator  p_mask_right  = p_right;

    // iterate over all mixture categories
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        std::vector<double>::const_iterator         u_l = p_mask_left;
        std::vector<double>::const_iterator         u_r = p_mask_right;

        maskNodeObservationCounts[mask][root] = maskNodeObservationCounts[mask][left] + maskNodeObservationCounts[mask][right];

        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
            std::vector<double>::const_iterator         lC_i = u_l + 2; // ci = 1
            std::vector<double>::const_iterator         lI_i = lC_i + correctionOffset;

            std::vector<double>::const_iterator         rC_i = u_r + 2;
            std::vector<double>::const_iterator         rI_i = rC_i + correctionOffset;

            for(size_t c = 0; c < 2; c++)
            {
                // probability of constant state c descending from this node
                // given node state 1
                partialNodeCorrections[c*2] = lC_i[c] * rC_i[c];

                // probability of invert singleton state c descending from this node
                // given node state 1
                partialNodeCorrections[c*2 + 1] = lI_i[c] * rC_i[c]
                                                + lC_i[c] * rI_i[c];
            }

            c_mask_node[mixture] = computeIntegratedNodeCorrection(partialNodeCorrections, root, mask, mixture);
            //c_mask_node[mixture] += c_mask_left[mixture] + c_mask_right[mixture];

            u_l += correctionMixtureOffset; u_r += correctionMixtureOffset;
        }

        p_mask_left += correctionMaskOffset; p_mask_right += correctionMaskOffset;
        c_mask_node += numSiteRates; c_mask_left += numSiteRates; c_mask_right += numSiteRates;
    }
}


double RevBayesCore::PhyloCTMCSiteHomogeneousDollo::computeIntegratedNodeCorrection( std::vector<double> partials, size_t nodeIndex, size_t mask, size_t mixture )
{
    double prob = 0.0;

    if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
        prob += partials[0];

    if(coding & RestrictionAscertainmentBias::NOSINGLETONPRESENCE)
        prob += partials[1];

    // if there is only one observed tip, then don't double-count singleton gains
    if((coding & RestrictionAscertainmentBias::NOPRESENCESITES) && maskNodeObservationCounts[mask][nodeIndex] == maskObservationCounts[mask] && maskObservationCounts[mask] > 1)
        prob += partials[2];

    // if there are only two observed tips, then don't double-count singleton gains
    // if there is only one observed tip, then don't double-count absence sites
    // if the number of observed tips from this node is 1 less than the total number of observed tips, then add the constant prob
    if((coding & RestrictionAscertainmentBias::NOSINGLETONABSENCE) && maskObservationCounts[mask] > 2)
    {
        if(maskNodeObservationCounts[mask][nodeIndex] == maskObservationCounts[mask])
            prob += partials[3];
        else if(maskNodeObservationCounts[mask][nodeIndex] == maskObservationCounts[mask] - 1)
            prob += partials[2];
    }

    // impose a per-mixture boundary
    if(prob <= 0.0 || prob > 1.0)
        prob = RbConstants::Double::nan;

    prob = integrationFactors[mixture]*(1.0 - prob);

    return prob;
}

double RevBayesCore::PhyloCTMCSiteHomogeneousDollo::sumRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = tau->getValue().getRoot();

    // get the index of the root node
    size_t root_index = root.getIndex();

    const double*   p_root  = this->partialLikelihoods + this->activeLikelihood[root_index] * this->activeLikelihoodOffset + root_index*nodeOffset;

    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(pattern_block_size,0.0);

    const double*   p_site_root = p_root;

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
            const double*   p_site_mixture_root = p_site_root;

            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                per_mixture_Likelihoods[site] += p_site_mixture_root[numChars + 1];

                p_site_mixture_root += mixtureOffset;
            } // end-for over all sites (=patterns)

            p_site_root += siteOffset;
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
        for (size_t node = 0; node < num_nodes; ++node)
        {
            std::vector<double>::iterator c_node_mixture   = perMaskMixtureCorrections.begin() + this->activeLikelihood[node]*activeMassOffset + node*massNodeOffset;

            // iterate over all mixture categories
            for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
            {
                perMaskCorrections[mask] += c_node_mixture[mixture];
            }
        }
        
        if(perMaskCorrections[mask] <= 0.0)
            perMaskCorrections[mask] = RbConstants::Double::nan;

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
        double prob = log(p_node[numChars + 1]) - logScalingFactor;

        max = std::max(prob, max);

        weights.push_back( prob );

        p_node += mixtureOffset;
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
    if(num_with_descendants == 1)
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

    if ( RbSettings::userSettings().getUseScaling() == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 && nodeIndex < num_nodes -1)
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

    if ( RbSettings::userSettings().getUseScaling() == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 && nodeIndex < num_nodes -1)
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

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::redrawValue( void ) {

    // delete the old value first
    delete this->value;

    // create a new character data object
    this->value = new HomologousDiscreteCharacterData<StandardState>();

    size_t numTips = this->tau->getValue().getNumberOfTips();
    size_t num_nodes = this->tau->getValue().getNumberOfNodes();

    RandomNumberGenerator* rng = GLOBAL_RNG;

    const TopologyNode &root = this->tau->getValue().getRoot();
    size_t rootIndex = this->tau->getValue().getRoot().getIndex();

    updateCorrections(root, rootIndex);

    std::vector< DiscreteTaxonData<StandardState> > taxa = std::vector< DiscreteTaxonData<StandardState> >(numTips, DiscreteTaxonData<StandardState>( Taxon("") ) );

    std::vector<size_t> rateIndices;
    std::vector<size_t> birthNodes;

    double total = 0.0;
    for ( size_t i = 0; i < this->num_nodes; ++i )
        for ( size_t j = 0; j < this->numSiteRates; ++j )
            total += perMaskMixtureCorrections[i*massNodeOffset + j];

    for ( size_t i = 0; i < num_sites; i++ )
    {
        double u = rng->uniform01()*total;
        double tmp = 0.0;

        // simulate a birth and rate index for this character
        // by sampling nodes in proportion to the per node survival probs
        size_t birthNode = 0;
        size_t rateIndex = 0;
        while(tmp < u)
        {
            tmp += perMaskMixtureCorrections[birthNode*massNodeOffset + rateIndex];

            if(tmp < u)
            {
                rateIndex++;
                if(rateIndex == numSiteRates)
                {
                    rateIndex = 0;
                    birthNode++;
                }
            }
        }

        rateIndices.push_back(rateIndex);
        birthNodes.push_back(birthNode);
    }

    const std::vector< double > &freqs = this->getRootFrequencies();

    // then sample site-patterns using rejection sampling,
    // rejecting those that match the unobservable ones.
    for ( size_t i = 0; i < this->num_sites; i++ )
    {
        size_t rateIndex = rateIndices[i];
        size_t birthIndex = birthNodes[i];

        std::vector<StandardState> siteData(num_nodes, StandardState());

        // create the character
        StandardState &c = siteData[birthIndex];
        c.setStateByIndex(1);
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

        const TopologyNode& birthNode = tau->getValue().getNode(birthIndex);

        // recursively simulate the sequences
        std::map<size_t, size_t> charCounts;
        simulate( birthNode, siteData, rateIndex, charCounts);

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

void RevBayesCore::PhyloCTMCSiteHomogeneousDollo::simulate( const TopologyNode &node, std::vector<StandardState> &data, size_t rateIndex, std::map<size_t, size_t>& charCounts) {

    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();

    // get the sequence of this node
    size_t nodeIndex = node.getIndex();
    StandardState &parentState = data[ nodeIndex ];

    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);

        // update the transition probability matrix
        this->updateTransitionProbabilities( child.getIndex(), child.getBranchLength() );

        double u = rng->uniform01();

        if(u < survival[rateIndex])
        {
            unsigned long cp = parentState.getStateIndex() - 1;

            double *freqs = this->transitionProbMatrices[ rateIndex ][ cp ];

            // create the character
            StandardState &c = data[ child.getIndex() ];
            c.setStateByIndex(1);
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

}
