#include "PhyloCTMCSiteHomogeneousRestriction.h"

#ifdef RESTRICTION_SSE_ENABLED
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
const size_t REALS_PER_SIMD_REGISTER = sizeof(__m128d)/sizeof(double);
#endif

RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode<Tree> *t, bool c, size_t nSites, bool amb, RestrictionAscertainmentBias::Coding ty) :
    PhyloCTMCSiteHomogeneousConditional<RestrictionState>(  t, 2, c, nSites, amb, AscertainmentBias::Coding(ty))
{
#ifdef RESTRICTION_SSE_ENABLED
    probNodeOffset = num_site_rates*4;
    activeProbabilityOffset = num_nodes*probNodeOffset;
    transitionProbabilities = (double*)_mm_malloc(2*activeProbabilityOffset * sizeof(double), REALS_PER_SIMD_REGISTER*sizeof(double));
#endif
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
#ifdef RESTRICTION_SSE_ENABLED
    double sumPartialProbs = sumUncorrectedRootLikelihood();
#else
    double sumPartialProbs = PhyloCTMCSiteHomogeneous<RestrictionState>::sumRootLikelihood();
#endif
    
    if(coding == RestrictionAscertainmentBias::ALL)
        return sumPartialProbs;
    
    // get the root node
    const TopologyNode &root = this->tau->getValue().getRoot();

    // get the index of the root node
    size_t node_index = root.getIndex();
    
    const std::vector<double> &f = this->getRootFrequencies();
    
    std::vector<double>::const_iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[node_index] * activeCorrectionOffset  + node_index*correctionNodeOffset;
    
    std::vector<double> perMaskCorrections = std::vector<double>(numCorrectionMasks, 0.0);
    
    // iterate over each correction mask
    for(size_t mask = 0; mask < numCorrectionMasks; mask++)
    {
        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
        {
            double prob = 0.0;
            
            // iterate over ancestral (non-autapomorphic) states
            for(size_t a = 0; a < this->num_chars; a++)
            {
                size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset + a*correctionOffset;

                std::vector<double>::const_iterator             u = p_node   + offset;

                // iterate over combinations of autapomorphic states
                for(size_t c = 0; c < numCorrectionPatterns; c++)
                {
                    // constant site pattern likelihoods
                    std::vector<double>::const_iterator         uc = u  + c*this->num_chars;

                    if( ((coding & RestrictionAscertainmentBias::NOABSENCESITES)      && a == 0 && c == 0) ||
                        ((coding & RestrictionAscertainmentBias::NOPRESENCESITES)     && a == 1 && c == 0) ||
                        ((coding & RestrictionAscertainmentBias::NOSINGLETONPRESENCE) && a == 0 && c == 1 && maskObservationCounts[mask] > 1) ||
                        ((coding & RestrictionAscertainmentBias::NOSINGLETONABSENCE)  && a == 1 && c == 1 && maskObservationCounts[mask] > 2)
                        )
                    {
                        // iterate over initial states
                        for(size_t ci = 0; ci < this->num_chars; ci++)
                        {
                            prob += uc[ci];
                        }
                    }
                }
            }
            
            // impose a per-mixture boundary
            if(prob <= 0.0 || prob >= 1.0)
            {
                prob = RbConstants::Double::nan;
            }

            perMaskCorrections[mask] += prob;

            // add corrections for invariant sites
            double prob_invariant = getPInv();
            if(prob_invariant > 0.0)
            {
                prob *= (1.0 - prob_invariant);
                
                if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
                    prob += f[0]*prob_invariant;
                
                if(coding & RestrictionAscertainmentBias::NOPRESENCESITES)
                    prob += f[1]*prob_invariant;
            }
        
            perMaskMixtureCorrections[mask*num_site_rates + mixture] = 1.0 - prob;
        }

        // add corrections for invariant sites
        double prob_invariant = getPInv();
        if(prob_invariant > 0.0)
        {
            perMaskCorrections[mask] *= (1.0 - prob_invariant);

            if(coding & RestrictionAscertainmentBias::NOABSENCESITES)
                perMaskCorrections[mask] += f[0] * prob_invariant * this->num_site_rates;

            if(coding & RestrictionAscertainmentBias::NOPRESENCESITES)
                perMaskCorrections[mask] += f[1] * prob_invariant * this->num_site_rates;
        }

        // normalize the log-probability
        perMaskCorrections[mask] /= this->num_site_rates;

        // impose a per-mask boundary
        if(perMaskCorrections[mask] <= 0.0 || perMaskCorrections[mask] >= 1.0)
            perMaskCorrections[mask] = RbConstants::Double::nan;

        perMaskCorrections[mask] = log(1.0 - perMaskCorrections[mask]);

        // apply the correction for this correction mask
        sumPartialProbs -= perMaskCorrections[mask]*correctionMaskCounts[mask];
    }

    return sumPartialProbs;
}

#ifdef RESTRICTION_SSE_ENABLED

RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::PhyloCTMCSiteHomogeneousRestriction(const PhyloCTMCSiteHomogeneousRestriction &n) : PhyloCTMCSiteHomogeneousConditional<RestrictionState>(n),
        numSIMDBlocks(n.numSIMDBlocks), per_mixture_Likelihoods(NULL), transitionProbabilities(n.transitionProbabilities), activeProbabilityOffset(n.activeProbabilityOffset), probNodeOffset(n.probNodeOffset)
{
    if ( inMcmcMode == true )
    {
        per_mixture_Likelihoods = (double*)_mm_malloc(pattern_block_size * sizeof(double), REALS_PER_SIMD_REGISTER*sizeof(double));
        transitionProbabilities = (double*)_mm_malloc(2*activeProbabilityOffset * sizeof(double), REALS_PER_SIMD_REGISTER*sizeof(double));
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::resizeLikelihoodVectors( void )
{
    PhyloCTMCSiteHomogeneousConditional<RestrictionState>::resizeLikelihoodVectors();

    numSIMDBlocks               = size_t((pattern_block_size - 1)/REALS_PER_SIMD_REGISTER) + 1;
    siteOffset                  = 2*REALS_PER_SIMD_REGISTER;
    mixtureOffset               = numSIMDBlocks*siteOffset;
    nodeOffset                  = num_site_rates*mixtureOffset;
    activeLikelihoodOffset      = num_nodes*nodeOffset;

    _mm_free(transitionProbabilities);

    probNodeOffset = num_site_rates*4;
    activeProbabilityOffset = num_nodes*probNodeOffset;
    transitionProbabilities = (double*)_mm_malloc(2*activeProbabilityOffset * sizeof(double), REALS_PER_SIMD_REGISTER*sizeof(double));

    perNodeSiteLogScalingFactors = std::vector<std::vector< std::vector<double> > >(2, std::vector<std::vector<double> >(num_nodes, std::vector<double>(numSIMDBlocks*REALS_PER_SIMD_REGISTER, 0.0) ) );

    transition_prob_matrices = std::vector<TransitionProbabilityMatrix>(num_site_rates, TransitionProbabilityMatrix(num_chars) );

    // only do this if we are in MCMC mode. This will safe memory
    if ( inMcmcMode == true )
    {

        // we resize the partial likelihood vectors to the new dimensions
        _mm_free(partialLikelihoods);
        _mm_free(per_mixture_Likelihoods);

        partialLikelihoods = (double*)_mm_malloc(2*activeLikelihoodOffset * sizeof(double), REALS_PER_SIMD_REGISTER*sizeof(double));
        per_mixture_Likelihoods = (double*)_mm_malloc(pattern_block_size * sizeof(double), REALS_PER_SIMD_REGISTER*sizeof(double));

        // reinitialize likelihood vectors
        for (size_t i = 0; i < 2*activeLikelihoodOffset; i++)
        {
            partialLikelihoods[i] = 0.0;
        }

        const Tree& tau = this->tau->getValue();

        for(size_t i=0; i < tau.getNumberOfTips(); i++)
        {
            setTipData(tau.getNode(i), i);
        }
    }

    if ( useMarginalLikelihoods == true )
    {
        // we resize the partial likelihood vectors to the new dimensions
        delete [] marginalLikelihoods;

        marginalLikelihoods = new double[activeLikelihoodOffset];

        // reinitialize likelihood vectors
        for (size_t i = 0; i < activeLikelihoodOffset; i++)
        {
            marginalLikelihoods[i] = 0.0;
        }

    }
}

inline void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeRootLikelihood( size_t root, size_t left, size_t right)
{
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();

    __m128d mrf0 = _mm_load1_pd (&f[0]);
    __m128d mrf1 = _mm_load1_pd (&f[1]);

    // get the pointers to the partial likelihoods of the left and right subtree
          double* p        = this->partialLikelihoods + this->activeLikelihood[root]   * this->activeLikelihoodOffset + root   * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]   * this->activeLikelihoodOffset + left   * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]  * this->activeLikelihoodOffset + right  * this->nodeOffset;

    double *          p_mixture          = p;
    const double *    p_mixture_left     = p_left;
    const double *    p_mixture_right    = p_right;

    const double* t_left = transitionProbabilities + this->activeLikelihood[left] * activeProbabilityOffset + left * probNodeOffset;
    const double* t_right = transitionProbabilities + this->activeLikelihood[right] * activeProbabilityOffset + right * probNodeOffset;

    const double* t_mixture_left   = t_left;
    const double* t_mixture_right  = t_right;

    __m128d          m1, m2, m3;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        double *          p_site_mixture          = p_mixture;
        const double *    p_site_mixture_left     = p_mixture_left;
        const double *    p_site_mixture_right    = p_mixture_right;

        // iterate over all sites
        for (size_t site = 0; site < this->numSIMDBlocks; ++site)
        {
            __m128d * m_site_mixture       = (__m128d *)p_site_mixture;
            __m128d * m_site_mixture_left  = (__m128d *)p_site_mixture_left;
            __m128d * m_site_mixture_right = (__m128d *)p_site_mixture_right;

            m1 = _mm_load1_pd (t_mixture_left);
            m1 = _mm_mul_pd (m1, m_site_mixture_left[0]);

            m2 = _mm_load1_pd (t_mixture_left+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_left[1]);

            m3 = _mm_add_pd (m1, m2);

            m1 = _mm_load1_pd (t_mixture_right);
            m1 = _mm_mul_pd (m1, m_site_mixture_right[0]);

            m2 = _mm_load1_pd (t_mixture_right+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_right[1]);

            m1 = _mm_add_pd (m1, m2);
            m_site_mixture[0] = _mm_mul_pd (m1, m3);

            m_site_mixture[0] = _mm_mul_pd (mrf0, m_site_mixture[0]);

            m1 = _mm_load1_pd (t_mixture_left+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_left[0]);

            m2 = _mm_load1_pd (t_mixture_left+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_left[1]);

            m3 = _mm_add_pd (m1, m2);


            m1 = _mm_load1_pd (t_mixture_right+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_right[0]);

            m2 = _mm_load1_pd (t_mixture_right+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_right[1]);

            m1 = _mm_add_pd (m1, m2);
            m_site_mixture[1] = _mm_mul_pd (m1, m3);

            m_site_mixture[1] = _mm_mul_pd (mrf1, m_site_mixture[1]);

            p_site_mixture += siteOffset; p_site_mixture_left += siteOffset; p_site_mixture_right += siteOffset;
        } // end-for over all sites (=patterns)

        p_mixture += mixtureOffset; p_mixture_left += mixtureOffset; p_mixture_right += mixtureOffset;
        t_mixture_left += 4; t_mixture_right += 4;
    } // end-for over all mixtures (=rate categories)

    if(coding != AscertainmentBias::ALL)
    {
        computeRootCorrection(root, left, right);
    }
}


inline void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();

    __m128d mrf0 = _mm_load1_pd (&f[0]);
    __m128d mrf1 = _mm_load1_pd (&f[1]);

    // get the pointers to the partial likelihoods of the left and right subtree
          double* p        = this->partialLikelihoods + this->activeLikelihood[root]   * this->activeLikelihoodOffset + root   * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]   * this->activeLikelihoodOffset + left   * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]  * this->activeLikelihoodOffset + right  * this->nodeOffset;
    const double* p_middle = this->partialLikelihoods + this->activeLikelihood[middle] * this->activeLikelihoodOffset + middle * this->nodeOffset;

    double *          p_mixture          = p;
    const double *    p_mixture_left     = p_left;
    const double *    p_mixture_right    = p_right;
    const double *    p_mixture_middle   = p_middle;

    const double* t_left   = transitionProbabilities + this->activeLikelihood[left] * activeProbabilityOffset + left * probNodeOffset;
    const double* t_right  = transitionProbabilities + this->activeLikelihood[right] * activeProbabilityOffset + right * probNodeOffset;
    const double* t_middle = transitionProbabilities + this->activeLikelihood[middle] * activeProbabilityOffset + middle * probNodeOffset;

    const double* t_mixture_left   = t_left;
    const double* t_mixture_right  = t_right;
    const double* t_mixture_middle = t_middle;

    __m128d          m1, m2, m3;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        double *          p_site_mixture          = p_mixture;
        const double *    p_site_mixture_left     = p_mixture_left;
        const double *    p_site_mixture_right    = p_mixture_right;
        const double *    p_site_mixture_middle   = p_mixture_middle;

        // iterate over all sites
        for (size_t site = 0; site < this->numSIMDBlocks; ++site)
        {
            __m128d * m_site_mixture        = (__m128d *)p_site_mixture;
            __m128d * m_site_mixture_left   = (__m128d *)p_site_mixture_left;
            __m128d * m_site_mixture_right  = (__m128d *)p_site_mixture_right;
            __m128d * m_site_mixture_middle = (__m128d *)p_site_mixture_middle;

            m1 = _mm_load1_pd (t_mixture_left);
            m1 = _mm_mul_pd (m1, m_site_mixture_left[0]);

            m2 = _mm_load1_pd (t_mixture_left+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_left[1]);

            m3 = _mm_add_pd (m1, m2);


            m1 = _mm_load1_pd (t_mixture_right);
            m1 = _mm_mul_pd (m1, m_site_mixture_right[0]);

            m2 = _mm_load1_pd (t_mixture_right+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_right[1]);

            m1 = _mm_add_pd (m1, m2);
            m3 = _mm_mul_pd (m1, m3);


            m1 = _mm_load1_pd (t_mixture_middle);
            m1 = _mm_mul_pd (m1, m_site_mixture_middle[0]);

            m2 = _mm_load1_pd (t_mixture_middle+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_middle[1]);

            m1 = _mm_add_pd (m1, m2);
            m_site_mixture[0] = _mm_mul_pd (m1, m3);

            m_site_mixture[0] = _mm_mul_pd (mrf0, m_site_mixture[0]);

            m1 = _mm_load1_pd (t_mixture_left+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_left[0]);

            m2 = _mm_load1_pd (t_mixture_left+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_left[1]);

            m3 = _mm_add_pd (m1, m2);


            m1 = _mm_load1_pd (t_mixture_right+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_right[0]);

            m2 = _mm_load1_pd (t_mixture_right+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_right[1]);

            m1 = _mm_add_pd (m1, m2);
            m3 = _mm_mul_pd (m1, m3);


            m1 = _mm_load1_pd (t_mixture_middle+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_middle[0]);

            m2 = _mm_load1_pd (t_mixture_middle+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_middle[1]);

            m1 = _mm_add_pd (m1, m2);
            m_site_mixture[1] = _mm_mul_pd (m1, m3);

            m_site_mixture[1] = _mm_mul_pd (mrf1, m_site_mixture[1]);

            p_site_mixture += siteOffset; p_site_mixture_left += siteOffset; p_site_mixture_right += siteOffset; p_site_mixture_middle += siteOffset;
        } // end-for over all sites (=patterns)

        p_mixture += mixtureOffset; p_mixture_left += mixtureOffset; p_mixture_right += mixtureOffset; p_site_mixture_middle += mixtureOffset;
        t_mixture_left += 4; t_mixture_right += 4; t_mixture_middle += 4;
    } // end-for over all mixtures (=rate categories)

    if(coding != AscertainmentBias::ALL)
    {
        computeRootCorrection(root, left, right, middle);
    }
}


inline void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left      = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right     = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node      = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    double *          p_mixture          = p_node;
    const double *    p_mixture_left     = p_left;
    const double *    p_mixture_right    = p_right;

          double* t_node   = transitionProbabilities + this->activeLikelihood[nodeIndex] * activeProbabilityOffset + nodeIndex * probNodeOffset;
    const double* t_left   = transitionProbabilities + this->activeLikelihood[left] * activeProbabilityOffset + left * probNodeOffset;
    const double* t_right  = transitionProbabilities + this->activeLikelihood[right] * activeProbabilityOffset + right * probNodeOffset;

          double* t_mixture        = t_node;
    const double* t_mixture_left   = t_left;
    const double* t_mixture_right  = t_right;

    __m128d          m1, m2, m3;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        memcpy(t_mixture, this->transition_prob_matrices[mixture].theMatrix, 4*sizeof(double));

        double *          p_site_mixture          = p_mixture;
        const double *    p_site_mixture_left     = p_mixture_left;
        const double *    p_site_mixture_right    = p_mixture_right;

        // compute the per site probabilities
        for (size_t site = 0; site < this->numSIMDBlocks ; ++site)
        {
            __m128d * m_site_mixture       = (__m128d *)p_site_mixture;
            __m128d * m_site_mixture_left  = (__m128d *)p_site_mixture_left;
            __m128d * m_site_mixture_right = (__m128d *)p_site_mixture_right;

            m1 = _mm_load1_pd (t_mixture_left);
            m1 = _mm_mul_pd (m1, m_site_mixture_left[0]);

            m2 = _mm_load1_pd (t_mixture_left+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_left[1]);

            m3 = _mm_add_pd (m1, m2);

            m1 = _mm_load1_pd (t_mixture_right);
            m1 = _mm_mul_pd (m1, m_site_mixture_right[0]);

            m2 = _mm_load1_pd (t_mixture_right+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_right[1]);

            m1 = _mm_add_pd (m1, m2);
            m_site_mixture[0] = _mm_mul_pd (m1, m3);

            m1 = _mm_load1_pd (t_mixture_left+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_left[0]);

            m2 = _mm_load1_pd (t_mixture_left+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_left[1]);

            m3 = _mm_add_pd (m1, m2);


            m1 = _mm_load1_pd (t_mixture_right+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_right[0]);

            m2 = _mm_load1_pd (t_mixture_right+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_right[1]);

            m1 = _mm_add_pd (m1, m2);
            m_site_mixture[1] = _mm_mul_pd (m1, m3);

            p_site_mixture += siteOffset; p_site_mixture_left += siteOffset; p_site_mixture_right += siteOffset;
        } // end-for over all sites (=patterns)

        p_mixture += mixtureOffset; p_mixture_left += mixtureOffset; p_mixture_right += mixtureOffset;
        t_mixture += 4; t_mixture_left += 4; t_mixture_right += 4;
    } // end-for over all mixtures (=rate-categories)

    if(coding != AscertainmentBias::ALL)
    {
        computeInternalNodeCorrection(node, nodeIndex, left, right);
    }
}


inline void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left      = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_middle    = this->partialLikelihoods + this->activeLikelihood[middle]*this->activeLikelihoodOffset + middle*this->nodeOffset;
    const double*   p_right     = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node      = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    double *          p_mixture          = p_node;
    const double *    p_mixture_left     = p_left;
    const double *    p_mixture_right    = p_right;
    const double *    p_mixture_middle   = p_middle;

          double* t_node   = transitionProbabilities + this->activeLikelihood[nodeIndex] * activeProbabilityOffset + nodeIndex * probNodeOffset;
    const double* t_left   = transitionProbabilities + this->activeLikelihood[left] * activeProbabilityOffset + left * probNodeOffset;
    const double* t_right  = transitionProbabilities + this->activeLikelihood[right] * activeProbabilityOffset + right * probNodeOffset;
    const double* t_middle = transitionProbabilities + this->activeLikelihood[middle] * activeProbabilityOffset + middle * probNodeOffset;

          double* t_mixture        = t_node;
    const double* t_mixture_left   = t_left;
    const double* t_mixture_right  = t_right;
    const double* t_mixture_middle = t_middle;

    __m128d          m1, m2, m3;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        memcpy(t_mixture, this->transition_prob_matrices[mixture].theMatrix, 4*sizeof(double));

        double *          p_site_mixture          = p_mixture;
        const double *    p_site_mixture_left     = p_mixture_left;
        const double *    p_site_mixture_right    = p_mixture_right;
        const double *    p_site_mixture_middle   = p_mixture_middle;

        // compute the per site probabilities
        for (size_t site = 0; site < this->numSIMDBlocks ; ++site)
        {
            __m128d * m_site_mixture        = (__m128d *)p_site_mixture;
            __m128d * m_site_mixture_left   = (__m128d *)p_site_mixture_left;
            __m128d * m_site_mixture_right  = (__m128d *)p_site_mixture_right;
            __m128d * m_site_mixture_middle = (__m128d *)p_site_mixture_middle;

            m1 = _mm_load1_pd (t_mixture_left);
            m1 = _mm_mul_pd (m1, m_site_mixture_left[0]);

            m2 = _mm_load1_pd (t_mixture_left+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_left[1]);

            m3 = _mm_add_pd (m1, m2);


            m1 = _mm_load1_pd (t_mixture_right);
            m1 = _mm_mul_pd (m1, m_site_mixture_right[0]);

            m2 = _mm_load1_pd (t_mixture_right+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_right[1]);

            m1 = _mm_add_pd (m1, m2);
            m3 = _mm_mul_pd (m1, m3);


            m1 = _mm_load1_pd (t_mixture_middle);
            m1 = _mm_mul_pd (m1, m_site_mixture_middle[0]);

            m2 = _mm_load1_pd (t_mixture_middle+1);
            m2 = _mm_mul_pd (m2, m_site_mixture_middle[1]);

            m1 = _mm_add_pd (m1, m2);
            m_site_mixture[0] = _mm_mul_pd (m1, m3);

            m1 = _mm_load1_pd (t_mixture_left+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_left[0]);

            m2 = _mm_load1_pd (t_mixture_left+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_left[1]);

            m3 = _mm_add_pd (m1, m2);


            m1 = _mm_load1_pd (t_mixture_right+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_right[0]);

            m2 = _mm_load1_pd (t_mixture_right+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_right[1]);

            m1 = _mm_add_pd (m1, m2);
            m3 = _mm_mul_pd (m1, m3);


            m1 = _mm_load1_pd (t_mixture_middle+2);
            m1 = _mm_mul_pd (m1, m_site_mixture_middle[0]);

            m2 = _mm_load1_pd (t_mixture_middle+3);
            m2 = _mm_mul_pd (m2, m_site_mixture_middle[1]);

            m1 = _mm_add_pd (m1, m2);
            m_site_mixture[1] = _mm_mul_pd (m1, m3);

            p_site_mixture += siteOffset; p_site_mixture_left += siteOffset; p_site_mixture_right += siteOffset; p_site_mixture_middle += siteOffset;
        } // end-for over all sites (=patterns)

        p_mixture += mixtureOffset; p_mixture_left += mixtureOffset; p_mixture_right += mixtureOffset; p_mixture_middle += mixtureOffset;
        t_mixture += 4; t_mixture_left += 4; t_mixture_right += 4; t_mixture_middle += 4;
    } // end-for over all mixtures (=rate-categories)

    if(coding != AscertainmentBias::ALL)
    {
        computeInternalNodeCorrection(node, nodeIndex, left, right, middle);
    }
}


void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
{
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );

    double* t_node    = transitionProbabilities + this->activeLikelihood[nodeIndex] * activeProbabilityOffset + nodeIndex * probNodeOffset;

    double* t_mixture = t_node;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        memcpy(t_mixture, this->transition_prob_matrices[mixture].theMatrix, 4*sizeof(double));

        t_mixture += 4;
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::setTipData(const TopologyNode &node, size_t nodeIndex)
{
    for(size_t active = 0; active < 2; active++)
    {
        double* p_node = this->partialLikelihoods + active*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

        const std::vector<bool> &gap_node = this->gap_matrix[nodeIndex];
        const std::vector<unsigned long> &char_node = this->char_matrix[nodeIndex];

        double * p_mixture = p_node;

        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
        {
            double *          p_site_mixture    = p_mixture;

            // iterate over all sites
            for (size_t site = 0; site != this->numSIMDBlocks; ++site)
            {
                for (size_t ss = 0; ss < REALS_PER_SIMD_REGISTER; ss++)
                {
                    size_t pattern = site*REALS_PER_SIMD_REGISTER + ss;
                    size_t index   = ss;

                    if(pattern >= pattern_block_size)
                        break;

                    // is this site a gap?
                    if ( gap_node[pattern] )
                    {
                        // since this is a gap we need to assume that the actual state could have been any state
                        p_site_mixture[index] = 1.0;
                        p_site_mixture[index + REALS_PER_SIMD_REGISTER] = 1.0;
                    }
                    else // we have observed a character
                    {
                        // get the original character
                        unsigned long org_val = char_node[pattern];

                        p_site_mixture[index] = 1.0 - org_val;
                        p_site_mixture[index + REALS_PER_SIMD_REGISTER] = org_val;

                    }
                }

                p_site_mixture += siteOffset;

            } // end-for over all sites/patterns in the sequence

            p_mixture += mixtureOffset;

        } // end-for over all mixture categories
    }

    if(coding != AscertainmentBias::ALL)
    {
        computeTipCorrection(node, nodeIndex);
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeTipCorrection(const TopologyNode &node, size_t nodeIndex)
{
    for(size_t active = 0; active < 2; active++)
    {
        std::vector<double>::iterator p_node = correctionLikelihoods.begin() + active*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

        std::vector<double>::iterator p_mixture_node = p_node;

        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < num_site_rates; ++mixture)
        {
            std::vector<double>::iterator p_mask_mixture_node = p_mixture_node;

            for(size_t mask = 0; mask < numCorrectionMasks; mask++)
            {
                bool gap = correctionMaskMatrix[mask][nodeIndex];

                for(size_t a = 0; a < 2; a++)
                {
                    size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset + a*correctionOffset;

                    std::vector<double>::iterator                 u = p_node   + offset;

                    for(size_t c = 0; c < numCorrectionPatterns; c++)
                    {
                        std::vector<double>::iterator         uc = u  + c*2;

                        if(gap)
                        {
                            uc[0] = uc[1] = gap;
                        }
                        else if(c == 0)
                        {
                            uc[0] = a == 0;
                            uc[1] = a == 1;
                        }
                        else
                        {
                            uc[0] = a == 1;
                            uc[1] = a == 0;
                        }
                    }
                }

                p_mask_mixture_node += correctionMaskOffset;
            }

            p_mixture_node += correctionMixtureOffset;
        }
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    std::vector<double>::const_iterator   p_mixture_left   = p_left;
    std::vector<double>::const_iterator   p_mixture_right  = p_right;
    std::vector<double>::const_iterator   p_mixture_middle = p_middle;
    std::vector<double>::iterator         p_mixture_node   = p_node;

    const double* t_left   = transitionProbabilities + this->activeLikelihood[left] * activeProbabilityOffset + left * probNodeOffset;
    const double* t_right  = transitionProbabilities + this->activeLikelihood[right] * activeProbabilityOffset + right * probNodeOffset;
    const double* t_middle = transitionProbabilities + this->activeLikelihood[middle] * activeProbabilityOffset + middle * probNodeOffset;

    const double* t_mixture_left   = t_left;
    const double* t_mixture_right  = t_right;
    const double* t_mixture_middle = t_middle;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        std::vector<double>::const_iterator   p_mask_mixture_left   = p_mixture_left;
        std::vector<double>::const_iterator   p_mask_mixture_right  = p_mixture_right;
        std::vector<double>::const_iterator   p_mask_mixture_middle = p_mixture_middle;
        std::vector<double>::iterator         p_mask_mixture_node   = p_mixture_node;

        // iterate over correction masks
        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            // iterate over ancestral (non-autapomorphic) states
            for(size_t a = 0; a < 2; a++)
            {
                size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset + a*correctionOffset;

                std::vector<double>::iterator                   u   = p_mask_mixture_node    + offset;
                std::vector<double>::const_iterator             u_l = p_mask_mixture_left    + offset;
                std::vector<double>::const_iterator             u_r = p_mask_mixture_right   + offset;
                std::vector<double>::const_iterator             u_m = p_mask_mixture_middle  + offset;

                // iterate over combinations of autapomorphic states
                for(size_t c = 0; c < numCorrectionPatterns; c++)
                {
                    std::vector<double>::iterator         uc = u  + c*this->num_chars;

                    std::fill(uc, uc + this->num_chars, 0.0);

                    // iterate over partitions of c
                    for(size_t p1 = 0; p1 <= c; p1++)
                    {
                        if( (p1 | c) == c)
                        {
                            size_t p_tmp = p1 ^ c;

                            // iterate over partitions of p_tmp
                            for(size_t p2 = 0; p2 <= p_tmp; p2++)
                            {
                                if( (p2 | p_tmp) == p_tmp)
                                {
                                    size_t p3 = p2 ^ p_tmp;

                                    std::vector<double>::const_iterator         lc = u_l  + p1*this->num_chars;
                                    std::vector<double>::const_iterator         rc = u_r  + p2*this->num_chars;
                                    std::vector<double>::const_iterator         mc = u_m  + p3*this->num_chars;

                                    // iterate over initial states
                                    for(size_t ci = 0; ci < 2; ci++)
                                    {
                                        // iterate over left states
                                        for(size_t cj = 0; cj < 2; cj++)
                                        {
                                            // iterate over right states
                                            for(size_t ck = 0; ck < 2; ck++)
                                            {
                                                // iterate over middle states
                                                for(size_t cl = 0; cl < 2; cl++)
                                                {
                                                    double Pij =   t_mixture_left[2*ci + cj];
                                                    double Pik =  t_mixture_right[2*ci + ck];
                                                    double Pil = t_mixture_middle[2*ci + cl];

                                                    uc[ci] += Pij*lc[cj] * Pik*rc[ck] * Pil*mc[cl];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            p_mask_mixture_node += correctionMaskOffset; p_mask_mixture_left += correctionMaskOffset; p_mask_mixture_right += correctionMaskOffset; p_mask_mixture_middle += correctionMaskOffset;
        }

        p_mixture_node += correctionMixtureOffset; p_mixture_left += correctionMixtureOffset; p_mixture_right += correctionMixtureOffset; p_mixture_middle += correctionMixtureOffset;
        t_mixture_left += 4; t_mixture_right += 4; t_mixture_middle += 4;
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*activeCorrectionOffset + nodeIndex*correctionNodeOffset;

    std::vector<double>::const_iterator   p_mixture_left   = p_left;
    std::vector<double>::const_iterator   p_mixture_right  = p_right;
    std::vector<double>::iterator         p_mixture_node   = p_node;

    const double* t_left   = transitionProbabilities + this->activeLikelihood[left] * activeProbabilityOffset + left * probNodeOffset;
    const double* t_right  = transitionProbabilities + this->activeLikelihood[right] * activeProbabilityOffset + right * probNodeOffset;

    const double* t_mixture_left   = t_left;
    const double* t_mixture_right  = t_right;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        std::vector<double>::const_iterator   p_mask_mixture_left   = p_mixture_left;
        std::vector<double>::const_iterator   p_mask_mixture_right  = p_mixture_right;
        std::vector<double>::iterator         p_mask_mixture_node   = p_mixture_node;

        // iterate over correction masks
        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            // iterate over ancestral (non-autapomorphic) states
            for(size_t a = 0; a < 2; a++)
            {
                size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset + a*correctionOffset;

                std::vector<double>::iterator                   u   = p_mask_mixture_node    + offset;
                std::vector<double>::const_iterator             u_l = p_mask_mixture_left    + offset;
                std::vector<double>::const_iterator             u_r = p_mask_mixture_right   + offset;

                // iterate over combinations of autapomorphic states
                for(size_t c = 0; c < numCorrectionPatterns; c++)
                {
                    std::vector<double>::iterator         uc = u  + c*this->num_chars;

                    std::fill(uc, uc + this->num_chars, 0.0);

                    // iterate over partitions of c
                    for(size_t p1 = 0; p1 <= c; p1++)
                    {
                        if( (p1 | c) == c)
                        {
                            size_t p2 = p1 ^ c;

                            std::vector<double>::const_iterator         lc = u_l  + p1*this->num_chars;
                            std::vector<double>::const_iterator         rc = u_r  + p2*this->num_chars;

                            // iterate over initial states
                            for(size_t ci = 0; ci < 2; ci++)
                            {
                                // iterate over left states
                                for(size_t cj = 0; cj < 2; cj++)
                                {
                                    // iterate over right states
                                    for(size_t ck = 0; ck < 2; ck++)
                                    {
                                        double Pij =   t_mixture_left[2*ci + cj];
                                        double Pik =  t_mixture_right[2*ci + ck];

                                        uc[ci] += Pij*lc[cj] * Pik*rc[ck];
                                    }
                                }
                            }
                        }
                    }
                }
            }

            p_mask_mixture_node += correctionMaskOffset; p_mask_mixture_left += correctionMaskOffset; p_mask_mixture_right += correctionMaskOffset;
        }

        p_mixture_node += correctionMixtureOffset; p_mixture_left += correctionMixtureOffset; p_mixture_right += correctionMixtureOffset;
        t_mixture_left += 4; t_mixture_right += 4;
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeRootCorrection( size_t root, size_t left, size_t right, size_t middle)
{
    const std::vector<double> &f = this->getRootFrequencies();

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*activeCorrectionOffset + middle*correctionNodeOffset;
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[root]*activeCorrectionOffset + root*correctionNodeOffset;

    std::vector<double>::const_iterator   p_mixture_left   = p_left;
    std::vector<double>::const_iterator   p_mixture_right  = p_right;
    std::vector<double>::const_iterator   p_mixture_middle = p_middle;
    std::vector<double>::iterator         p_mixture_node   = p_node;

    const double* t_left   = transitionProbabilities + this->activeLikelihood[left] * activeProbabilityOffset + left * probNodeOffset;
    const double* t_right  = transitionProbabilities + this->activeLikelihood[right] * activeProbabilityOffset + right * probNodeOffset;
    const double* t_middle = transitionProbabilities + this->activeLikelihood[middle] * activeProbabilityOffset + middle * probNodeOffset;

    const double* t_mixture_left   = t_left;
    const double* t_mixture_right  = t_right;
    const double* t_mixture_middle = t_middle;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        std::vector<double>::const_iterator   p_mask_mixture_left   = p_mixture_left;
        std::vector<double>::const_iterator   p_mask_mixture_right  = p_mixture_right;
        std::vector<double>::const_iterator   p_mask_mixture_middle = p_mixture_middle;
        std::vector<double>::iterator         p_mask_mixture_node   = p_mixture_node;

        // iterate over correction masks
        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            // iterate over ancestral (non-autapomorphic) states
            for(size_t a = 0; a < 2; a++)
            {
                size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset + a*correctionOffset;

                std::vector<double>::iterator                   u   = p_mask_mixture_node    + offset;
                std::vector<double>::const_iterator             u_l = p_mask_mixture_left    + offset;
                std::vector<double>::const_iterator             u_r = p_mask_mixture_right   + offset;
                std::vector<double>::const_iterator             u_m = p_mask_mixture_middle  + offset;

                // iterate over combinations of autapomorphic states
                for(size_t c = 0; c < numCorrectionPatterns; c++)
                {
                    std::vector<double>::iterator         uc = u  + c*this->num_chars;

                    std::fill(uc, uc + this->num_chars, 0.0);

                    // iterate over partitions of c
                    for(size_t p1 = 0; p1 <= c; p1++)
                    {
                        if( (p1 | c) == c)
                        {
                            size_t p_tmp = p1 ^ c;

                            // iterate over partitions of p_tmp
                            for(size_t p2 = 0; p2 <= p_tmp; p2++)
                            {
                                if( (p2 | p_tmp) == p_tmp)
                                {
                                    size_t p3 = p2 ^ p_tmp;

                                    std::vector<double>::const_iterator         lc = u_l  + p1*this->num_chars;
                                    std::vector<double>::const_iterator         rc = u_r  + p2*this->num_chars;
                                    std::vector<double>::const_iterator         mc = u_m  + p3*this->num_chars;

                                    // iterate over initial states
                                    for(size_t ci = 0; ci < 2; ci++)
                                    {
                                        // iterate over left states
                                        for(size_t cj = 0; cj < 2; cj++)
                                        {
                                            // iterate over right states
                                            for(size_t ck = 0; ck < 2; ck++)
                                            {
                                                // iterate over middle states
                                                for(size_t cl = 0; cl < 2; cl++)
                                                {
                                                    double Pij =   t_mixture_left[2*ci + cj];
                                                    double Pik =  t_mixture_right[2*ci + ck];
                                                    double Pil = t_mixture_middle[2*ci + cl];

                                                    uc[ci] += Pij*lc[cj] * Pik*rc[ck] * Pil*mc[cl];
                                                }
                                            }
                                        }

                                        uc[ci] *= f[ci];
                                    }
                                }
                            }
                        }
                    }
                }
            }

            p_mask_mixture_node += correctionMaskOffset; p_mask_mixture_left += correctionMaskOffset; p_mask_mixture_right += correctionMaskOffset; p_mask_mixture_middle += correctionMaskOffset;
        }

        p_mixture_node += correctionMixtureOffset; p_mixture_left += correctionMixtureOffset; p_mixture_right += correctionMixtureOffset; p_mixture_middle += correctionMixtureOffset;
        t_mixture_left += 4; t_mixture_right += 4; t_mixture_middle += 4;
    }
}

void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::computeRootCorrection( size_t root, size_t left, size_t right)
{
    const std::vector<double> &f = this->getRootFrequencies();

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    std::vector<double>::const_iterator   p_left   = correctionLikelihoods.begin() + this->activeLikelihood[left]*activeCorrectionOffset + left*correctionNodeOffset;
    std::vector<double>::const_iterator   p_right  = correctionLikelihoods.begin() + this->activeLikelihood[right]*activeCorrectionOffset + right*correctionNodeOffset;
    std::vector<double>::iterator         p_node   = correctionLikelihoods.begin() + this->activeLikelihood[root]*activeCorrectionOffset + root*correctionNodeOffset;

    std::vector<double>::const_iterator   p_mixture_left   = p_left;
    std::vector<double>::const_iterator   p_mixture_right  = p_right;
    std::vector<double>::iterator         p_mixture_node   = p_node;

    const double* t_left   = transitionProbabilities + this->activeLikelihood[left] * activeProbabilityOffset + left * probNodeOffset;
    const double* t_right  = transitionProbabilities + this->activeLikelihood[right] * activeProbabilityOffset + right * probNodeOffset;

    const double* t_mixture_left   = t_left;
    const double* t_mixture_right  = t_right;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        std::vector<double>::const_iterator   p_mask_mixture_left   = p_mixture_left;
        std::vector<double>::const_iterator   p_mask_mixture_right  = p_mixture_right;
        std::vector<double>::iterator         p_mask_mixture_node   = p_mixture_node;

        // iterate over correction masks
        for(size_t mask = 0; mask < numCorrectionMasks; mask++)
        {
            // iterate over ancestral (non-autapomorphic) states
            for(size_t a = 0; a < 2; a++)
            {
                size_t offset = mixture*correctionMixtureOffset + mask*correctionMaskOffset + a*correctionOffset;

                std::vector<double>::iterator                   u   = p_mask_mixture_node    + offset;
                std::vector<double>::const_iterator             u_l = p_mask_mixture_left    + offset;
                std::vector<double>::const_iterator             u_r = p_mask_mixture_right   + offset;

                // iterate over combinations of autapomorphic states
                for(size_t c = 0; c < numCorrectionPatterns; c++)
                {
                    std::vector<double>::iterator         uc = u  + c*this->num_chars;

                    std::fill(uc, uc + this->num_chars, 0.0);

                    // iterate over partitions of c
                    for(size_t p1 = 0; p1 <= c; p1++)
                    {
                        if( (p1 | c) == c)
                        {
                            size_t p2 = p1 ^ c;

                            std::vector<double>::const_iterator         lc = u_l  + p1*this->num_chars;
                            std::vector<double>::const_iterator         rc = u_r  + p2*this->num_chars;

                            // iterate over initial states
                            for(size_t ci = 0; ci < 2; ci++)
                            {
                                // iterate over left states
                                for(size_t cj = 0; cj < 2; cj++)
                                {
                                    // iterate over right states
                                    for(size_t ck = 0; ck < 2; ck++)
                                    {
                                        double Pij =   t_mixture_left[2*ci + cj];
                                        double Pik =  t_mixture_right[2*ci + ck];

                                        uc[ci] += Pij*lc[cj] * Pik*rc[ck];
                                    }
                                }

                                uc[ci] *= f[ci];
                            }
                        }
                    }
                }
            }

            p_mask_mixture_node += correctionMaskOffset; p_mask_mixture_left += correctionMaskOffset; p_mask_mixture_right += correctionMaskOffset;
        }

        p_mixture_node += correctionMixtureOffset; p_mixture_left += correctionMixtureOffset; p_mixture_right += correctionMixtureOffset;
        t_mixture_left += 4; t_mixture_right += 4;
    }
}

double RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::sumUncorrectedRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = tau->getValue().getRoot();

    // get the index of the root node
    size_t node_index = root.getIndex();

    // get the pointers to the partial likelihoods of the left and right subtree
    double*   p_node  = this->partialLikelihoods + this->activeLikelihood[node_index] * this->activeLikelihoodOffset  + node_index*this->nodeOffset;

    for(size_t pattern = 0; pattern < this->pattern_block_size; pattern++)
    {
        per_mixture_Likelihoods[pattern] = 0.0;
    }

    double * p_mixture = p_node;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        double * p_site_mixture = p_mixture;

        __m128d * mTotals = (__m128d *)per_mixture_Likelihoods;

        for (size_t block = 0; block < numSIMDBlocks; block++)
        {
            // get the pointers to the likelihoods for this site and mixture category
            __m128d *   m_site_mixture     = (__m128d *)p_site_mixture;

            *mTotals = _mm_add_pd(*mTotals, _mm_add_pd(m_site_mixture[0], m_site_mixture[1]));

            mTotals++;
            p_site_mixture += siteOffset;
        }

        p_mixture += mixtureOffset;

    } // end-for over all mixtures (=rate categories)

    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();
    double prob_invariant = (p_inv == NULL ? p_inv->getValue() : 0.0);
    double oneMinusPInv = 1.0 - prob_invariant;
    if ( prob_invariant > 0.0 )
    {
        for (size_t site = 0; site < pattern_block_size; site++)
        {
            if ( RbSettings::userSettings().getUseScaling() == true )
            {
                if ( this->site_invariant[site] == true )
                {
                    sumPartialProbs += log( prob_invariant * f[ this->invariant_site_index[site] ] * exp(this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site]) + oneMinusPInv * per_mixture_Likelihoods[site] / this->num_site_rates ) * pattern_counts[site];
                }
                else
                {
                    sumPartialProbs += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->num_site_rates ) * pattern_counts[site];
                }
            }
            else // no scaling
            {

                if ( this->site_invariant[site] == true )
                {
                    sumPartialProbs += log( prob_invariant * f[ this->invariant_site_index[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / this->num_site_rates ) * pattern_counts[site];
                }
                else
                {
                    sumPartialProbs += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->num_site_rates ) * pattern_counts[site];
                }

            }
        }
    }
    else
    {

        for (size_t site = 0; site < pattern_block_size; site++)
        {
            sumPartialProbs += log( per_mixture_Likelihoods[site] / this->num_site_rates ) * pattern_counts[site];

            if ( RbSettings::userSettings().getUseScaling() == true )
            {

                sumPartialProbs -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site] * pattern_counts[site];
            }

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

    return sumPartialProbs;
}

void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::scale( size_t nodeIndex)
{
    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    if ( RbSettings::userSettings().getUseScaling() == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {
        // iterate over all mixture categories
        for (size_t block = 0; block < numSIMDBlocks ; ++block)
        {
            for (size_t ss = 0; ss < REALS_PER_SIMD_REGISTER; ss++)
            {
                size_t site = block*REALS_PER_SIMD_REGISTER + ss;

                if(site >= pattern_block_size)
                    continue;

                // the max probability
                double max = 0.0;

                // compute the per site probabilities
                for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                {
                    // get the pointers to the likelihood for this mixture category
                    size_t offset = mixture*this->mixtureOffset + block*this->siteOffset;

                    double*          p_site_mixture          = p_node + offset + ss;

                    max = std::max(max, *p_site_mixture);
                    max = std::max(max, *(p_site_mixture + REALS_PER_SIMD_REGISTER));
                }

                this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = -log(max);


                // compute the per site probabilities
                for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                {
                    // get the pointers to the likelihood for this mixture category
                    size_t offset = mixture*this->mixtureOffset + block*this->siteOffset;

                    double*          p_site_mixture          = p_node + offset + ss;

                    *p_site_mixture /= max;
                    *(p_site_mixture + REALS_PER_SIMD_REGISTER) /= max;

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


void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::scale( size_t nodeIndex, size_t left, size_t right )
{
    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    if ( RbSettings::userSettings().getUseScaling() == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {
        // iterate over all mixture categories
        for (size_t block = 0; block < numSIMDBlocks ; ++block)
        {
            for (size_t ss = 0; ss < REALS_PER_SIMD_REGISTER; ss++)
            {
                size_t site = block*REALS_PER_SIMD_REGISTER + ss;

                if(site >= pattern_block_size)
                    continue;

                // the max probability
                double max = 0.0;

                // compute the per site probabilities
                for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                {
                    // get the pointers to the likelihood for this mixture category
                    size_t offset = mixture*this->mixtureOffset + block*this->siteOffset;

                    double*          p_site_mixture          = p_node + offset + ss;

                    max = std::max(max, *p_site_mixture);
                    max = std::max(max, *(p_site_mixture + REALS_PER_SIMD_REGISTER));
                }

                this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] - log(max);


                // compute the per site probabilities
                for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                {
                    // get the pointers to the likelihood for this mixture category
                    size_t offset = mixture*this->mixtureOffset + block*this->siteOffset;

                    double*          p_site_mixture          = p_node + offset + ss;

                    *p_site_mixture /= max;
                    *(p_site_mixture + REALS_PER_SIMD_REGISTER) /= max;

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


void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction::scale( size_t nodeIndex, size_t left, size_t right, size_t middle )
{
    double* p_node   = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    if ( RbSettings::userSettings().getUseScaling() == true && nodeIndex % RbSettings::userSettings().getScalingDensity() == 0 )
    {
        // iterate over all mixture categories
        for (size_t block = 0; block < numSIMDBlocks ; ++block)
        {
            for (size_t ss = 0; ss < REALS_PER_SIMD_REGISTER; ss++)
            {
                size_t site = block*REALS_PER_SIMD_REGISTER + ss;

                if(site >= pattern_block_size)
                    continue;

                // the max probability
                double max = 0.0;

                // compute the per site probabilities
                for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                {
                    // get the pointers to the likelihood for this mixture category
                    size_t offset = mixture*this->mixtureOffset + block*this->siteOffset;

                    double*          p_site_mixture          = p_node + offset + ss;

                    max = std::max(max, *p_site_mixture);
                    max = std::max(max, *(p_site_mixture + REALS_PER_SIMD_REGISTER));
                }

                this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[middle]][middle][site] - log(max);


                // compute the per site probabilities
                for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                {
                    // get the pointers to the likelihood for this mixture category
                    size_t offset = mixture*this->mixtureOffset + block*this->siteOffset;

                    double*          p_site_mixture          = p_node + offset + ss;

                    *p_site_mixture /= max;
                    *(p_site_mixture + REALS_PER_SIMD_REGISTER) /= max;

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
#endif

