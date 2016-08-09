#ifndef PhyloCTMCSiteHomogeneousNucleotideIntegrated_H
#define PhyloCTMCSiteHomogeneousNucleotideIntegrated_H

#include "AbstractPhyloCTMCSiteHomogeneousIntegrated.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType>
    class PhyloCTMCSiteHomogeneousNucleotideIntegrated : public AbstractPhyloCTMCSiteHomogeneousIntegrated<charType> {
        
    public:
        PhyloCTMCSiteHomogeneousNucleotideIntegrated(const TypedDagNode< Tree > *t, bool c, size_t nSites, bool amb);
        virtual                                            ~PhyloCTMCSiteHomogeneousNucleotideIntegrated(void);                                                                   //!< Virtual destructor
        
        // public member functions
        PhyloCTMCSiteHomogeneousNucleotideIntegrated*                 clone(void) const;                                                                          //!< Create an independent clone
        
    protected:
        
        void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
        void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r,  size_t m);
        void                                                computeRootLikelihood( size_t root, size_t left, size_t right);
        void                                                computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle);
        void                                                computeTipLikelihood(const TopologyNode &node, size_t nIdx);
        
        
    private:
        
        void                                                normalizeSiteProbabilities(double *p_node);
        void                                                resetSiteProbabilities(double *p_node);
    };
    
}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RateMatrix_GTR.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "RbMathLogic.h"

#include <cmath>
#include <cstring>
#if defined (SSE_ENABLED)
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#elif defined ( AVX_ENABLED )
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <immintrin.h>
#endif

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::PhyloCTMCSiteHomogeneousNucleotideIntegrated(const TypedDagNode<Tree> *t, bool c, size_t nSites, bool amb) : AbstractPhyloCTMCSiteHomogeneousIntegrated<charType>(  t, 4, 1, c, nSites, amb )
{
    
}

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::~PhyloCTMCSiteHomogeneousNucleotideIntegrated( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>* RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::clone( void ) const
{
    
    return new PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>( *this );
}



template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::computeRootLikelihood( size_t root, size_t left, size_t right)
{
    
    // reset the likelihood
    this->lnProb = 0.0;
    
    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();
    
    double  f0  = f[0];
    double  f1  = f[1];
    double  f2  = f[2];
    double  f3  = f[3];
    
    // get the pointers to the partial likelihoods of the left and right subtree
    double* p        = this->partialLikelihoods + this->activeLikelihood[root]  *this->activeLikelihoodOffset + root   * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]  *this->activeLikelihoodOffset + left   * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right] *this->activeLikelihoodOffset + right  * this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    double*   p_mixture          = p;
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        double*   p_site          = p_mixture;
        const double*   p_site_left     = p_mixture_left;
        const double*   p_site_right    = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            
            p_site[0] = p_site_left[0] * p_site_right[0] * f0;
            p_site[1] = p_site_left[1] * p_site_right[1] * f1;
            p_site[2] = p_site_left[2] * p_site_right[2] * f2;
            p_site[3] = p_site_left[3] * p_site_right[3] * f3;
            
            // increment the pointers to the next site
            p_site+=this->siteOffset; p_site_left+=this->siteOffset; p_site_right+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->site_rate_offset; p_mixture_left+=this->site_rate_offset; p_mixture_right+=this->site_rate_offset;
        
    } // end-for over all mixtures (=rate categories)
    
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{
    
    // reset the likelihood
    this->lnProb = 0.0;
    
    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();
    
    double  f0  = f[0];
    double  f1  = f[1];
    double  f2  = f[2];
    double  f3  = f[3];
    
    // get the pointers to the partial likelihoods of the left and right subtree
    double* p        = this->partialLikelihoods + this->activeLikelihood[root]  *this->activeLikelihoodOffset + root   * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]  *this->activeLikelihoodOffset + left   * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right] *this->activeLikelihoodOffset + right  * this->nodeOffset;
    const double* p_middle = this->partialLikelihoods + this->activeLikelihood[middle]*this->activeLikelihoodOffset + middle * this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    double*   p_mixture          = p;
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    const double*   p_mixture_middle   = p_middle;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        double*   p_site          = p_mixture;
        const double*   p_site_left     = p_mixture_left;
        const double*   p_site_right    = p_mixture_right;
        const double*   p_site_middle   = p_mixture_middle;
        // iterate over all sites
        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            p_site[0] = p_site_left[0] * p_site_right[0] * p_site_middle[0] * f0;
            p_site[1] = p_site_left[1] * p_site_right[1] * p_site_middle[1] * f1;
            p_site[2] = p_site_left[2] * p_site_right[2] * p_site_middle[2] * f2;
            p_site[3] = p_site_left[3] * p_site_right[3] * p_site_middle[3] * f3;
            
            // increment the pointers to the next site
            p_site+=this->siteOffset; p_site_left+=this->siteOffset; p_site_right+=this->siteOffset; p_site_middle+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->site_rate_offset; p_mixture_left+=this->site_rate_offset; p_mixture_right+=this->site_rate_offset; p_mixture_middle+=this->site_rate_offset;
        
    } // end-for over all mixtures (=rate categories)
    
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    
    
# if defined ( SSE_ENABLED )
    
    double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double* p_node   = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    //    __m128d* p_left   = (__m128d *) this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    //    __m128d* p_right  = (__m128d *) this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    //    __m128d* p_node   = (__m128d *) this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
# elif defined ( AVX_ENABLED )
    
    double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double* p_node   = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    double* tmp_ac = new double[4];
    double* tmp_gt = new double[4];
    //    double tmp_ac[4];
    //    double tmp_gt[4];
    
    
# else
    
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
# endif

    // reset the probabilities
    resetSiteProbabilities( p_node );

    size_t num_clock_rate_cats  = (this->integrate_over_clock_rates ? this->heterogeneous_clock_rates->getValue().size() : 1);
    size_t num_rate_matrix_cats  = (this->integrate_over_subst_matrices ? this->heterogeneousRateMatrices->getValue().size() : 1);
    
    // iterate over all mixture categories
    for (size_t site_rate_index = 0; site_rate_index < this->num_site_rates; ++site_rate_index)
    {
        
        // get the pointers to the likelihood for this mixture category
        size_t offset = site_rate_index*this->site_rate_offset;
        
        for (size_t rate_matrix_index=0; rate_matrix_index<num_rate_matrix_cats; ++rate_matrix_index)
        {
            
            for (size_t clock_rate_index=0; clock_rate_index<num_clock_rate_cats; ++clock_rate_index)
            {
                
                // compute the transition probability matrix
                this->updateTransitionProbabilities( nodeIndex, node.getBranchLength(), site_rate_index, rate_matrix_index, clock_rate_index );

                // the transition probability matrix for this mixture category
                const double* tp_begin = this->transition_prob_matrix.theMatrix;
                
# if defined ( SSE_ENABLED )
                
                double*          p_site          = p_node + offset;
                const double*    p_site_left     = p_left + offset;
                const double*    p_site_right    = p_right + offset;
                
                __m128d tp_a_ac = _mm_load_pd(tp_begin);
                __m128d tp_a_gt = _mm_load_pd(tp_begin+2);
                __m128d tp_c_ac = _mm_load_pd(tp_begin+4);
                __m128d tp_c_gt = _mm_load_pd(tp_begin+6);
                __m128d tp_g_ac = _mm_load_pd(tp_begin+8);
                __m128d tp_g_gt = _mm_load_pd(tp_begin+10);
                __m128d tp_t_ac = _mm_load_pd(tp_begin+12);
                __m128d tp_t_gt = _mm_load_pd(tp_begin+14);
                
# elif defined ( AVX_ENABLED )
                
                double*          p_site          = p_node + offset;
                const double*    p_site_left     = p_left + offset;
                const double*    p_site_right    = p_right + offset;
                
                __m256d tp_a = _mm256_load_pd(tp_begin);
                __m256d tp_c = _mm256_load_pd(tp_begin+4);
                __m256d tp_g = _mm256_load_pd(tp_begin+8);
                __m256d tp_t = _mm256_load_pd(tp_begin+12);
                
# else
                
                double*          p_site          = p_node + offset;
                const double*    p_site_left     = p_left + offset;
                const double*    p_site_right    = p_right + offset;
                
# endif
                
                // compute the per site probabilities
                for (size_t site = 0; site < this->pattern_block_size ; ++site)
                {
                    
# if defined ( SSE_ENABLED )
                    
                    __m128d a01 = _mm_load_pd(p_site_left);
                    __m128d a23 = _mm_load_pd(p_site_left+2);
                    
                    __m128d b01 = _mm_load_pd(p_site_right);
                    __m128d b23 = _mm_load_pd(p_site_right+2);
                    
                    __m128d p01 = _mm_mul_pd(a01,b01);
                    __m128d p23 = _mm_mul_pd(a23,b23);
                    
                    __m128d a_ac = _mm_mul_pd(p01, tp_a_ac   );
                    __m128d a_gt = _mm_mul_pd(p23, tp_a_gt );
                    __m128d a_acgt = _mm_hadd_pd(a_ac,a_gt);
                    
                    __m128d c_ac = _mm_mul_pd(p01, tp_c_ac );
                    __m128d c_gt = _mm_mul_pd(p23, tp_c_gt );
                    __m128d c_acgt = _mm_hadd_pd(c_ac,c_gt);
                    
                    __m128d ac = _mm_hadd_pd(a_acgt,c_acgt);
                    
                    __m128d old_prob_ac = _mm_load_pd(p_site);
                    __m128d ac_sum = _mm_add_pd(ac,old_prob_ac);
                    _mm_store_pd(p_site,ac_sum);
                    
                    
                    __m128d g_ac = _mm_mul_pd(p01, tp_g_ac  );
                    __m128d g_gt = _mm_mul_pd(p23, tp_g_gt );
                    __m128d g_acgt = _mm_hadd_pd(g_ac,g_gt);
                    
                    __m128d t_ac = _mm_mul_pd(p01, tp_t_ac );
                    __m128d t_gt = _mm_mul_pd(p23, tp_t_gt );
                    __m128d t_acgt = _mm_hadd_pd(t_ac,t_gt);
                    
                    __m128d gt = _mm_hadd_pd(g_acgt,t_acgt);
                    
                    __m128d old_prob_gt = _mm_load_pd(p_site+2);
                    __m128d gt_sum = _mm_add_pd(gt,old_prob_gt);
                    _mm_store_pd(p_site+2,gt_sum);
                    
# elif defined ( AVX_ENABLED )
                    
                    __m256d a = _mm256_load_pd(p_site_left);
                    __m256d b = _mm256_load_pd(p_site_right);
                    __m256d p = _mm256_mul_pd(a,b);
                    
                    __m256d a_acgt = _mm256_mul_pd(p, tp_a );
                    __m256d c_acgt = _mm256_mul_pd(p, tp_c );
                    __m256d g_acgt = _mm256_mul_pd(p, tp_g );
                    __m256d t_acgt = _mm256_mul_pd(p, tp_t );
                    
                    __m256d ac   = _mm256_hadd_pd(a_acgt,c_acgt);
                    __m256d gt   = _mm256_hadd_pd(g_acgt,t_acgt);
                    
                    
                    _mm256_store_pd(tmp_ac,ac);
                    _mm256_store_pd(tmp_gt,gt);
                    
                    p_site[0] += tmp_ac[0] + tmp_ac[2];
                    p_site[1] += tmp_ac[1] + tmp_ac[3];
                    p_site[2] += tmp_gt[0] + tmp_gt[2];
                    p_site[3] += tmp_gt[1] + tmp_gt[3];
                    
# else
                    
                    double p0 = p_site_left[0] * p_site_right[0];
                    double p1 = p_site_left[1] * p_site_right[1];
                    double p2 = p_site_left[2] * p_site_right[2];
                    double p3 = p_site_left[3] * p_site_right[3];
                    
                    double sum = p0 * tp_begin[0];
                    sum += p1 * tp_begin[1];
                    sum += p2 * tp_begin[2];
                    sum += p3 * tp_begin[3];
                    
                    p_site[0] += sum;
                    
                    sum = p0 * tp_begin[4];
                    sum += p1 * tp_begin[5];
                    sum += p2 * tp_begin[6];
                    sum += p3 * tp_begin[7];
                    
                    p_site[1] += sum;
                    
                    sum = p0 * tp_begin[8];
                    sum += p1 * tp_begin[9];
                    sum += p2 * tp_begin[10];
                    sum += p3 * tp_begin[11];
                    
                    p_site[2] += sum;
                    
                    sum = p0 * tp_begin[12];
                    sum += p1 * tp_begin[13];
                    sum += p2 * tp_begin[14];
                    sum += p3 * tp_begin[15];
                    
                    p_site[3] += sum;
                    
# endif
                    
                    // increment the pointers to the next site
                    p_site_left+=this->siteOffset; p_site_right+=this->siteOffset; p_site+=this->siteOffset;
                    
                    
                } // end-for over all sites (=patterns)
                
            }
            
        }
        
    } // end-for over all mixtures (=rate-categories)
    
    
    // normalize the probabilities
    normalizeSiteProbabilities( p_node );
    
# if defined ( AVX_ENABLED )
    delete[] tmp_ac;
    delete[] tmp_gt;
# endif
    
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
    
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left      = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_middle    = this->partialLikelihoods + this->activeLikelihood[middle]*this->activeLikelihoodOffset + middle*this->nodeOffset;
    const double*   p_right     = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node      = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    
    // reset the site probabilities
    resetSiteProbabilities( p_node );
    
    size_t num_clock_rate_cats  = (this->integrate_over_clock_rates ? this->heterogeneous_clock_rates->getValue().size() : 1);
    size_t num_rate_matrix_cats  = (this->integrate_over_subst_matrices ? this->heterogeneousRateMatrices->getValue().size() : 1);
    
    
    // iterate over all mixture categories
    for (size_t site_rate_index = 0; site_rate_index < this->num_site_rates; ++site_rate_index)
    {
        
        // get the pointers to the likelihood for this mixture category
        size_t offset = site_rate_index*this->site_rate_offset;
        
        for (size_t rate_matrix_index=0; rate_matrix_index<num_rate_matrix_cats; ++rate_matrix_index)
        {
            
            for (size_t clock_rate_index=0; clock_rate_index<num_clock_rate_cats; ++clock_rate_index)
            {
                
                // compute the transition probability matrix
                this->updateTransitionProbabilities( nodeIndex, node.getBranchLength(), site_rate_index, rate_matrix_index, clock_rate_index );
                
                // the transition probability matrix for this mixture category
                const double* tp_begin = this->transition_prob_matrix.theMatrix;
        
#if defined ( SSE_ENABLED )
        
                double*          p_site          = p_node + offset;
                const double*    p_site_left     = p_left + offset;
                const double*    p_site_middle   = p_middle + offset;
                const double*    p_site_right    = p_right + offset;
        
                __m128d tp_a_ac = _mm_load_pd(tp_begin);
                __m128d tp_a_gt = _mm_load_pd(tp_begin+2);
                __m128d tp_c_ac = _mm_load_pd(tp_begin+4);
                __m128d tp_c_gt = _mm_load_pd(tp_begin+6);
                __m128d tp_g_ac = _mm_load_pd(tp_begin+8);
                __m128d tp_g_gt = _mm_load_pd(tp_begin+10);
                __m128d tp_t_ac = _mm_load_pd(tp_begin+12);
                __m128d tp_t_gt = _mm_load_pd(tp_begin+14);
        
#elif defined ( AVX_ENABLED )
        
                double*          p_site          = p_node + offset;
                const double*    p_site_left     = p_left + offset;
                const double*    p_site_right    = p_right + offset;
        
                __m256d tp_a = _mm256_load_pd(tp_begin);
                __m256d tp_c = _mm256_load_pd(tp_begin+4);
                __m256d tp_g = _mm256_load_pd(tp_begin+8);
                __m256d tp_t = _mm256_load_pd(tp_begin+12);
        
#else
        
                double*          p_site          = p_node + offset;
                const double*    p_site_left     = p_left + offset;
                const double*    p_site_middle   = p_middle + offset;
                const double*    p_site_right    = p_right + offset;
        
#endif
        
                // compute the per site probabilities
                for (size_t site = 0; site < this->pattern_block_size ; ++site)
                {
            
#if defined ( SSE_ENABLED )
            
                    __m128d a01 = _mm_load_pd(p_site_left);
                    __m128d a23 = _mm_load_pd(p_site_left+2);
            
                    __m128d b01 = _mm_load_pd(p_site_middle);
                    __m128d b23 = _mm_load_pd(p_site_middle+2);
            
                    __m128d c01 = _mm_load_pd(p_site_right);
                    __m128d c23 = _mm_load_pd(p_site_right+2);
            
                    __m128d tmp_p01 = _mm_mul_pd(a01,b01);
                    __m128d p01 = _mm_mul_pd(tmp_p01,c01);
                    __m128d tmp_p23 = _mm_mul_pd(a23,b23);
                    __m128d p23 = _mm_mul_pd(tmp_p23,c23);
            
                    __m128d a_ac = _mm_mul_pd(p01, tp_a_ac   );
                    __m128d a_gt = _mm_mul_pd(p23, tp_a_gt );
                    __m128d a_acgt = _mm_hadd_pd(a_ac,a_gt);
            
                    __m128d c_ac = _mm_mul_pd(p01, tp_c_ac );
                    __m128d c_gt = _mm_mul_pd(p23, tp_c_gt );
                    __m128d c_acgt = _mm_hadd_pd(c_ac,c_gt);
            
            
//                    *p_site = _mm_hadd_pd(a_acgt,c_acgt);
                    __m128d ac = _mm_hadd_pd(a_acgt,c_acgt);
                    
                    __m128d old_prob_ac = _mm_load_pd(p_site);
                    __m128d ac_sum = _mm_add_pd(ac,old_prob_ac);
                    _mm_store_pd(p_site,ac_sum);
            
                    __m128d g_ac = _mm_mul_pd(p01, tp_g_ac  );
                    __m128d g_gt = _mm_mul_pd(p23, tp_g_gt );
                    __m128d g_acgt = _mm_hadd_pd(g_ac,g_gt);
            
                    __m128d t_ac = _mm_mul_pd(p01, tp_t_ac );
                    __m128d t_gt = _mm_mul_pd(p23, tp_t_gt );
                    __m128d t_acgt = _mm_hadd_pd(t_ac,t_gt);
            
//                    p_site[2] = _mm_hadd_pd(g_acgt,t_acgt);
                    __m128d gt = _mm_hadd_pd(g_acgt,t_acgt);
                    
                    __m128d old_prob_gt = _mm_load_pd(p_site+2);
                    __m128d gt_sum = _mm_add_pd(gt,old_prob_gt);
                    _mm_store_pd(p_site+2,gt_sum);
            
#elif defined ( AVX_ENABLED )
            
                    __m256d a = _mm256_load_pd(p_site_left);
                    __m256d b = _mm256_load_pd(p_site_right);
                    __m256d p = _mm_mul_pd(a,b);
            
                    __m256d a_acgt = _mm256_mul_pd(p, tp_a );
                    __m256d c_acgt = _mm256_mul_pd(p, tp_c );
                    __m256d g_acgt = _mm256_mul_pd(p, tp_g );
                    __m256d t_acgt = _mm256_mul_pd(p, tp_t );
            
                    __m256d ac   = _mm256_hadd_pd(a_acgt,c_acgt);
                    __m256d gt   = _mm256_hadd_pd(g_acgt,t_acgt)
            
                    __m256d acgt = _mm256_hadd_pd(ac,gt);
            
                    _mm256_store_pd(p_site,acgt);
            
#else
            
                    double p0 = p_site_left[0] * p_site_middle[0] * p_site_right[0];
                    double p1 = p_site_left[1] * p_site_middle[1] * p_site_right[1];
                    double p2 = p_site_left[2] * p_site_middle[2] * p_site_right[2];
                    double p3 = p_site_left[3] * p_site_middle[3] * p_site_right[3];
            
                    double sum = p0 * tp_begin[0];
                    sum += p1 * tp_begin[1];
                    sum += p2 * tp_begin[2];
                    sum += p3 * tp_begin[3];
            
                    p_site[0] += sum;
            
                    sum = p0 * tp_begin[4];
                    sum += p1 * tp_begin[5];
                    sum += p2 * tp_begin[6];
                    sum += p3 * tp_begin[7];
            
                    p_site[1] += sum;
            
                    sum = p0 * tp_begin[8];
                    sum += p1 * tp_begin[9];
                    sum += p2 * tp_begin[10];
                    sum += p3 * tp_begin[11];
            
                    p_site[2] += sum;
            
                    sum = p0 * tp_begin[12];
                    sum += p1 * tp_begin[13];
                    sum += p2 * tp_begin[14];
                    sum += p3 * tp_begin[15];
            
                    p_site[3] += sum;
        
#endif
            
                    // increment the pointers to the next site
                    p_site_left+=this->siteOffset; p_site_middle+=this->siteOffset; p_site_right+=this->siteOffset; p_site+=this->siteOffset;
            
            
                } // end-for over all sites (=patterns)
                
            } // end-for over all rate categories
        
        } // end-for over all rate-matrices
        
    } // end-for over all site rates (=rate-categories)
    
    // normalize
    normalizeSiteProbabilities( p_node );
    
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
{
    
    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    const std::vector<bool> &gap_node = this->gap_matrix[nodeIndex];
    const std::vector<unsigned long> &char_node = this->char_matrix[nodeIndex];
    
    // reset the site probabilities
    resetSiteProbabilities( p_node );
    
    size_t num_clock_rate_cats  = (this->integrate_over_clock_rates ? this->heterogeneous_clock_rates->getValue().size() : 1);
    size_t num_rate_matrix_cats  = (this->integrate_over_subst_matrices ? this->heterogeneousRateMatrices->getValue().size() : 1);
    
    
    // iterate over all mixture categories
    for (size_t site_rate_index = 0; site_rate_index < this->num_site_rates; ++site_rate_index)
    {
        
//        // get the pointers to the likelihood for this mixture category
//        size_t offset = site_rate_index*this->site_rate_offset;
        
        for (size_t rate_matrix_index=0; rate_matrix_index<num_rate_matrix_cats; ++rate_matrix_index)
        {
            
            for (size_t clock_rate_index=0; clock_rate_index<num_clock_rate_cats; ++clock_rate_index)
            {
                
                // compute the transition probability matrix
                this->updateTransitionProbabilities( nodeIndex, node.getBranchLength(), site_rate_index, rate_matrix_index, clock_rate_index );
                
                // the transition probability matrix for this mixture category
                const double* tp_begin = this->transition_prob_matrix.theMatrix;
                
                // get the pointer to the likelihoods for this site and mixture category
                double*     p_site      = p_node + site_rate_index*this->site_rate_offset;
        
                // iterate over all sites
                for (size_t site = 0; site < this->pattern_block_size; ++site)
                {
            
                    // is this site a gap?
                    if ( gap_node[site] )
                    {
                        // since this is a gap we need to assume that the actual state could have been any state
                        p_site[0] += 1.0;
                        p_site[1] += 1.0;
                        p_site[2] += 1.0;
                        p_site[3] += 1.0;
                
                    }
                    else // we have observed a character
                    {
                
                        // get the original character
                        unsigned long org_val = char_node[site];
                
                        if ( this->usingAmbiguousCharacters == true )
                        {
                    
                            double p0 = 0.0;
                            double p1 = 0.0;
                            double p2 = 0.0;
                            double p3 = 0.0;
                            
                            if ( (org_val & 1) == 1 )
                            {
                                p0 = tp_begin[0];
                                p1 = tp_begin[4];
                                p2 = tp_begin[8];
                                p3 = tp_begin[12];
                            }
                    
                            if ( (org_val & 2) == 2 )
                            {
                                p0 += tp_begin[1];
                                p1 += tp_begin[5];
                                p2 += tp_begin[9];
                                p3 += tp_begin[13];
                            }
                    
                            if ( (org_val & 4) == 4 )
                            {
                                p0 += tp_begin[2];
                                p1 += tp_begin[6];
                                p2 += tp_begin[10];
                                p3 += tp_begin[14];
                            }
                    
                            if ( (org_val & 8) == 8 )
                            {
                                p0 += tp_begin[3];
                                p1 += tp_begin[7];
                                p2 += tp_begin[11];
                                p3 += tp_begin[15];
                            }
                    
                            p_site[0] += p0;
                            p_site[1] += p1;
                            p_site[2] += p2;
                            p_site[3] += p3;
                    
                        }
                        else // no ambiguous characters in use
                        {
                    
                            // store the likelihood
                            p_site[0] += tp_begin[org_val];
                            p_site[1] += tp_begin[4+org_val];
                            p_site[2] += tp_begin[8+org_val];
                            p_site[3] += tp_begin[12+org_val];
                    
                        }
                
                    } // end-if a gap state
            
            
                    // increment the pointers to next site
                    p_site+=this->siteOffset;
            
                } // end-for over all sites/patterns in the sequence
                
            } // end-for over all clock rate categories
            
        } // end-for over all rate matrix categories
        
    } // end-for over all site rate categories
    
    
    // normalize the per site probabilities
    normalizeSiteProbabilities( p_node );
    
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::normalizeSiteProbabilities(double* p_node)
{

    size_t num_clock_rate_cats   = (this->integrate_over_clock_rates ? this->heterogeneous_clock_rates->getValue().size() : 1);
    size_t num_rate_matrix_cats  = (this->integrate_over_subst_matrices ? this->heterogeneousRateMatrices->getValue().size() : 1);

    // normalize the probabilities
    double normalization_factor = (num_clock_rate_cats * num_rate_matrix_cats);
    for (size_t site_rate_index = 0; site_rate_index < this->num_site_rates; ++site_rate_index)
    {
        
        // get the pointers to the likelihood for this mixture category
        size_t offset = site_rate_index*this->site_rate_offset;
    
        double* p_site = p_node + offset;
    
        // set all the probabilities to zero
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
        
            p_site[0] /= normalization_factor;
            p_site[1] /= normalization_factor;
            p_site[2] /= normalization_factor;
            p_site[3] /= normalization_factor;
        
            p_site+=this->siteOffset;
        
        }
    
    }
    
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousNucleotideIntegrated<charType>::resetSiteProbabilities( double *p_node )
{

    for (size_t site_rate_index = 0; site_rate_index < this->num_site_rates; ++site_rate_index)
    {
        // get the pointers to the likelihood for this mixture category
        size_t offset = site_rate_index*this->site_rate_offset;
    
        double* p_site = p_node + offset;
    
        // set all the probabilities to zero
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
        
            p_site[0] = 0.0;
            p_site[1] = 0.0;
            p_site[2] = 0.0;
            p_site[3] = 0.0;
        
            p_site+=this->siteOffset;
        
        }
    
    }
    
}


#endif
