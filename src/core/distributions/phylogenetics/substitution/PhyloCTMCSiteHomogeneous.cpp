#include <cstdlib>
#include <vector>
#include <cmath>
#include "AbstractPhyloCTMCSiteHomogeneous.h"

double RevBayesCore::homogeneousComputeRootLikelihood(const double *p_left,
                                                      const double *p_right,
                                                      const size_t numSiteRates,
                                                      const double * rootFreq,
                                                      const size_t numStates,
                                                      const size_t * patternCounts,
                                                      const size_t numPatterns,
                                                      const size_t siteOffset,
                                                      const size_t mixtureOffset,
                                                      const double p_inv,
                                                      const std::vector<bool> & siteInvariant,
                                                      const std::vector<size_t> & invariantSiteIndex) {
    double lnProb = 0.0;
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(numPatterns,0.0);
    // get pointers the likelihood for both subtrees
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < numPatterns; ++site) {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            for (size_t i = 0; i < numStates; ++i) {
                tmp += p_site_left_j[i] * p_site_right_j[i] * rootFreq[i];
            }
            per_mixture_Likelihoods[site] += tmp;
            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset;
            p_site_mixture_right+=siteOffset;
        }
        p_mixture_left+=mixtureOffset; p_mixture_right+=mixtureOffset;
    }
    // sum the log-likelihoods for all sites together
    double oneMinusPInv = 1.0 - p_inv;
    if ( p_inv > 0.0 ) {
        for (size_t site = 0; site < numPatterns; ++site) {
            if ( siteInvariant[site] ) {
                lnProb += log( p_inv * rootFreq[ invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
            } else {
                lnProb += log( oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
            }
        }
    } else {
        for (size_t site = 0; site < numPatterns; ++site) {
            lnProb += log( per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
        }
    }
    return lnProb;
}
double RevBayesCore::homogeneousComputeRootLikelihood(const double *p_left,
                                                      const double *p_right,
                                                      const double *p_middle,
                                                      const size_t numSiteRates,
                                                      const double * rootFreq,
                                                      const size_t numStates,
                                                      const size_t * patternCounts,
                                                      const size_t numPatterns,
                                                      const size_t siteOffset,
                                                      const size_t mixtureOffset,
                                                      const double p_inv,
                                                      const std::vector<bool> & siteInvariant,
                                                      const std::vector<size_t> & invariantSiteIndex) {
    double lnProb = 0.0;
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(numPatterns,0.0);
    // get pointers the likelihood for both subtrees
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    const double*   p_mixture_middle   = p_middle;
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        const double*   p_site_mixture_middle   = p_mixture_middle;
        for (size_t site = 0; site < numPatterns; ++site) {
            double tmp = 0.0;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            const double* p_site_middle_j = p_site_mixture_middle;
            // iterate over all starting states
            for (size_t i = 0; i < numStates; ++i) {
                tmp += p_site_left_j[i] * p_site_right_j[i] * p_site_middle_j[i] * rootFreq[i];
            }
            per_mixture_Likelihoods[site] += tmp;
            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset;
            p_site_mixture_right+=siteOffset;
            p_site_mixture_middle+=siteOffset;
        } // end-for over all sites (=patterns)
        p_mixture_left+=mixtureOffset;
        p_mixture_right+=mixtureOffset;
        p_mixture_middle+=mixtureOffset;
    } // end-for over all mixtures (=rate categories)
    // sum the log-likelihoods for all sites together
    double oneMinusPInv = 1.0 - p_inv;
    if ( p_inv > 0.0 ) {
        for (size_t site = 0; site < numPatterns; ++site) {
            double prob;
            if ( siteInvariant[site] ) {
                prob = p_inv * rootFreq[ invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates;
            } else {
                prob = oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates;
            }
            lnProb += log(prob) * patternCounts[site];
        }
    } else {
        for (size_t site = 0; site < numPatterns; ++site) {
            lnProb += log( per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
        }
    }
    return lnProb;
}

void RevBayesCore::homogeneousComputeInternalNodeLikelihoood(double * p_node,
                                                             const double *p_left,
                                                             const double *p_right,
                                                             const size_t numSiteRates,
                                                             const size_t numStates,
                                                             const size_t numPatterns,
                                                             const size_t siteOffset,
                                                             const size_t mixtureOffset,
                                                             const double ** tpMats) {
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = tpMats[mixture];
        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        // compute the per site probabilities
        for (size_t site = 0; site < numPatterns ; ++site) {
            const double*       tp_a    = tp_begin;
            for (size_t c1 = 0; c1 < numStates; ++c1) {
                double sum = 0.0;
                for (size_t c2 = 0; c2 < numStates; ++c2 ) {
                    sum += p_site_mixture_left[c2] * p_site_mixture_right[c2] * tp_a[c2];
                } // end-for over all distination character 
                p_site_mixture[c1] = sum;
                tp_a+=numStates;
            } // end-for over all initial characters
            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset; p_site_mixture_right+=siteOffset; p_site_mixture+=siteOffset;
        } // end-for over all sites (=patterns)
    } // end-for over all mixtures (=rate-categories)
}

void RevBayesCore::homogeneousComputeTipNodeLikelihoood(double * p_node,
                                                             const size_t numSiteRates,
                                                             const size_t numStates,
                                                             const size_t numPatterns,
                                                             const size_t siteOffset,
                                                             const size_t mixtureOffset,
                                                             const double ** tpMats,
                                                             const std::vector<bool> &gap_node,
                                                             const std::vector<unsigned long> &char_node,
                                                             const bool usingAmbiguousCharacters) {
    double*   p_mixture      = p_node;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        // the transition probability matrix for this mixture category
        const double* tp_begin = tpMats[mixture];
        // get the pointer to the likelihoods for this site and mixture category
        double*     p_site_mixture      = p_mixture;
        // iterate over all sites
        for (size_t site = 0; site != numPatterns; ++site) {
            // is this site a gap?
            if ( gap_node[site] ) {
                // since this is a gap we need to assume that the actual state could have been any state
                // iterate over all initial states for the transitions
                for (size_t c1 = 0; c1 < numStates; ++c1) {
                    // store the likelihood
                    p_site_mixture[c1] = 1.0;
                }
            } else { // we have observed a character
                // get the original character
                unsigned long org_val = char_node[site];
                // iterate over all possible initial states
                for (size_t c1 = 0; c1 < numStates; ++c1) {
                    if ( usingAmbiguousCharacters ) {
                        // compute the likelihood that we had a transition from state c1 to the observed state org_val
                        // note, the observed state could be ambiguous!
                        unsigned long val = org_val;
                        // get the pointer to the transition probabilities for the terminal states
                        const double* d  = tp_begin+(numStates*c1);
                        double tmp = 0.0;
                        while ( val != 0 ) // there are still observed states left
                        {
                            // check whether we observed this state
                            if ( (val & 1) == 1 ) {
                                // add the probability
                                tmp += *d;
                            }
                            // remove this state from the observed states
                            val >>= 1;
                            // increment the pointer to the next transition probability
                            ++d;
                        } // end-while over all observed states for this character
                        // store the likelihood
                        p_site_mixture[c1] = tmp;
                    } else // no ambiguous characters in use
                    {
                        // store the likelihood
                        p_site_mixture[c1] = tp_begin[c1*numStates+org_val];
                    }
                } // end-for over all possible initial character for the branch
            } // end-if a gap state
            // increment the pointers to next site
            p_site_mixture+=siteOffset; 
        } // end-for over all sites/patterns in the sequence
        // increment the pointers to next mixture category
        p_mixture+=mixtureOffset;
    } // end-for over all mixture categories
}

