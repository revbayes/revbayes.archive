#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>
#include "AbstractPhyloCTMCSiteHomogeneous.h" // too broad an include, but this is where the computeInternalNodeLikelihood... funcs are declared
#include "AscertainmentBiasCorrection.h"
#include "RbException.h"
using namespace RevBayesCore;
double lnSumOfNumbersInLnForm(double lnX, double lnY);
RevBayesCore::VariableOnlyAscBiasCorrection::VariableOnlyAscBiasCorrection(const size_t ns, const size_t nm)
    :numStates(ns),
    numMixtures(nm) {
    partialLikelihoods.reserve(this->numStates * this->numStates * this->numMixtures);
}
RevBayesCore::VariableOnlyAscBiasCorrection::~VariableOnlyAscBiasCorrection() {

}
RevBayesCore::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct(const size_t ns, const size_t nm)
    :VariableOnlyAscBiasCorrection(ns, nm) {
}
RevBayesCore::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct::~VariableOnlyNoMissingAscertainmentBiasCorrectionStruct() {
}
void RevBayesCore::VariableOnlyAscBiasCorrection::computeInternalAscBias(const AscertainmentBiasCorrectionStruct * ascLeft,
                                                                         const AscertainmentBiasCorrectionStruct * ascRight,
                                                                         size_t nSiteRates,
                                                                         size_t nStates,
                                                                         size_t nPatterns,
                                                                         const double ** tpMats) {
    const VariableOnlyAscBiasCorrection * aL = dynamic_cast<const VariableOnlyAscBiasCorrection*>(ascLeft);
    const VariableOnlyAscBiasCorrection * aR = dynamic_cast<const VariableOnlyAscBiasCorrection*>(ascRight);
    if (aL == 0 || aR == 0) {
        assert(aL && aR);
        throw RbException("Expected left and right ascertainment bias correction structs to be VariableOnlyAscBiasCorrection *");
    }
    assert(nStates == this->numStates);
    assert(this->numMixtures == 0 || nSiteRates == this->numMixtures);
    assert(nStates == aL->numStates);
    assert(nSiteRates == aL->numMixtures);
    assert(nStates == aR->numStates);
    assert(nSiteRates == aR->numMixtures);
    const size_t numProxyPatterns = this->GetNumProxyPatterns(nPatterns);
    const unsigned plLen = this->numStates * numProxyPatterns * nSiteRates;
    assert(plLen == aL->partialLikelihoods.size());
    assert(plLen == aR->partialLikelihoods.size());
    if (this->partialLikelihoods.size() < plLen) {
        this->numMixtures = nSiteRates;
        this->partialLikelihoods.resize(plLen);
    }
    const size_t mixtureOffset =  numProxyPatterns*nStates;
    const size_t siteOffset =  nStates;
    computeInternalNodeLikelihood(&(this->partialLikelihoods[0]),
                                  &(aL->partialLikelihoods[0]),
                                  &(aR->partialLikelihoods[0]),
                                  nSiteRates,
                                  nStates,
                                  numProxyPatterns,
                                  siteOffset,
                                  mixtureOffset,
                                  tpMats);
}
void RevBayesCore::VariableOnlyAscBiasCorrection::computeTipAscBias(size_t nSiteRates,
                                                                    size_t nStates,
                                                                    size_t nPatterns,
                                                                    const double ** tpMats,
                                                                    const std::vector<bool> &gap_node,
                                                                    const std::vector<unsigned long> &char_node,
                                                                    bool usingAmbiguousCharacters) {
    assert(nStates == this->numStates);
    assert(this->numMixtures == 0 || nSiteRates == this->numMixtures);
    const size_t numProxyPatterns = this->GetNumProxyPatterns(nPatterns);
    const unsigned plLen = this->numStates * numProxyPatterns * nSiteRates;
    if (this->partialLikelihoods.size() < plLen) {
        this->numMixtures = nSiteRates;
        this->partialLikelihoods.resize(plLen);
    }
    const size_t mixtureOffset =  numProxyPatterns*nStates;
    const size_t siteOffset =  nStates;
    const std::vector<bool> noMissingGapNode(numStates*numStates, false);
    std::vector<unsigned long> proxyData;
    proxyData.reserve(nStates);
    for (unsigned long i = 0; i < nStates; ++i) {
        proxyData[i] = i;
    }
    computeTipNodeLikelihood(&(this->partialLikelihoods[0]),
                              nSiteRates,
                              nStates,
                              numProxyPatterns,
                              siteOffset,
                              mixtureOffset,
                              tpMats,
                              noMissingGapNode,
                              proxyData,
                              false);
}
double RevBayesCore::VariableOnlyAscBiasCorrection::computeAscBiasLnProbCorrection2Node(const AscertainmentBiasCorrectionStruct * ascRight,
                                                                                        const size_t nSiteRates,
                                                                                        const double *rootFreq,
                                                                                        const size_t nStates,
                                                                                        const size_t * patternCounts,
                                                                                        const size_t nPatterns,
                                                                                        const double p_inv,
                                                                                        const std::vector<bool> & siteInvariant,
                                                                                        const std::vector<size_t> & invariantSiteIndex) const {
    const VariableOnlyAscBiasCorrection * aR = dynamic_cast<const VariableOnlyAscBiasCorrection*>(ascRight);
    if (aR == 0) {
        assert(aR);
        throw RbException("Expected left and right ascertainment bias correction structs to be VariableOnlyAscBiasCorrection *");
    }
    assert(nStates == this->numStates);
    assert(this->numMixtures == 0 || nSiteRates == this->numMixtures);
    assert(nStates == aR->numStates);
    assert(nSiteRates == aR->numMixtures);
    const size_t numProxyPatterns = this->GetNumProxyPatterns(nPatterns);
    const unsigned plLen = this->numStates * numProxyPatterns * nSiteRates;
    assert(plLen == aR->partialLikelihoods.size());
    if (this->partialLikelihoods.size() < plLen) {
         throw RbException("root partialLikelihood is of the wrong size...");
    }
    const size_t mixtureOffset =  numProxyPatterns*nStates;
    const size_t siteOffset =  nStates;
    const std::vector<bool> allConstSiteInvariant(numProxyPatterns, true);
    std::vector<size_t> proxyInvariantSiteIndex(numProxyPatterns, 0);
    for (unsigned long i = 0; i < numProxyPatterns; ++i) {
        proxyInvariantSiteIndex[i] = i;
    }
    const std::vector<size_t> proxyPatternCounts(numProxyPatterns, 1);
    
    const double lnProbConstant = lnSumRootPatternProbabilities2Nodes(&(this->partialLikelihoods[0]),
                                                                      &(aR->partialLikelihoods[0]),
                                                                      nSiteRates,
                                                                      rootFreq,
                                                                      nStates,
                                                                      &(proxyPatternCounts[0]),
                                                                      numProxyPatterns,
                                                                      siteOffset,
                                                                      mixtureOffset,
                                                                      p_inv,
                                                                      allConstSiteInvariant,
                                                                      proxyInvariantSiteIndex);
    /* If the ln probability of a constant pattern is less than -40, then the correction is lost in rounding error...*/
    if (lnProbConstant < -40) {
        return 0.0;
    }
    const double probConstant = exp(lnProbConstant);
    const double lnProbVar = log(1.0 - probConstant);
    size_t sumPatWts = 0;
    for (size_t j = 0; j < nPatterns; ++j) {
        sumPatWts += patternCounts[j];
    }
    const double lnAscProb = sumPatWts*lnProbVar;
    return lnAscProb;
}
double RevBayesCore::VariableOnlyAscBiasCorrection::computeAscBiasLnProbCorrection3Node(const AscertainmentBiasCorrectionStruct * ascRight,
                                                                                        const AscertainmentBiasCorrectionStruct * ascMiddle,
                                                                                        const size_t nSiteRates,
                                                                                        const double *rootFreq,
                                                                                        const size_t nStates,
                                                                                        const size_t * patternCounts,
                                                                                        const size_t nPatterns,
                                                                                        const double p_inv,
                                                                                        const std::vector<bool> & siteInvariant,
                                                                                        const std::vector<size_t> & invariantSiteIndex) const {
    assert(false);
    throw RbException("RevBayesCore::VariableOnlyAscBiasCorrection::computeAscBiasLnProbCorrection3Node Not implemented yet");
    const VariableOnlyAscBiasCorrection * aR = dynamic_cast<const VariableOnlyAscBiasCorrection*>(ascRight);
    const VariableOnlyAscBiasCorrection * aM = dynamic_cast<const VariableOnlyAscBiasCorrection*>(ascMiddle);
    if (aR == 0 || aM == 0) {
        assert(aR && aM);
        throw RbException("Expected right and middle ascertainment bias correction structs to be VariableOnlyAscBiasCorrection *");
    }
    assert(nStates == this->numStates);
    assert(this->numMixtures == 0 || nSiteRates == this->numMixtures);
    const size_t numProxyPatterns = this->GetNumProxyPatterns(nPatterns);
    assert(nStates == aR->numStates);
    assert(nSiteRates == aR->numMixtures);
    assert(nStates == aM->numStates);
    assert(nSiteRates == aM->numMixtures);
    const unsigned plLen = this->numStates * numProxyPatterns * nSiteRates;
    assert(plLen == aR->partialLikelihoods.size());
    assert(plLen == aM->partialLikelihoods.size());
    if (this->partialLikelihoods.size() < plLen) {
         throw RbException("root partialLikelihood is of the wrong size...");
    }
    const size_t mixtureOffset =  numProxyPatterns*nStates;
    const size_t siteOffset =  nStates;
    const std::vector<bool> allConstSiteInvariant(numStates, true);
    std::vector<size_t> proxyInvariantSiteIndex(numProxyPatterns, 0);
    for (unsigned long i = 0; i < numProxyPatterns; ++i) {
        proxyInvariantSiteIndex[i] = i;
    }
    const std::vector<size_t> proxyPatternCounts(numProxyPatterns, 1);
    
    const double lnProbConstant = lnSumRootPatternProbabilities3Nodes(&(this->partialLikelihoods[0]),
                                                                      &(aR->partialLikelihoods[0]),
                                                                      &(aM->partialLikelihoods[0]),
                                                                      nSiteRates,
                                                                      rootFreq,
                                                                      nStates,
                                                                      &(proxyPatternCounts[0]),
                                                                      numProxyPatterns,
                                                                      siteOffset,
                                                                      mixtureOffset,
                                                                      p_inv,
                                                                      allConstSiteInvariant,
                                                                      proxyInvariantSiteIndex);
    /* If the ln probability of a constant pattern is less than -40, then the correction is lost in rounding error...*/
    if (lnProbConstant < -40) {
        return 0.0;
    }
    const double probConstant = exp(lnProbConstant);
    const double lnProbVar = log(1.0 - probConstant);
    size_t sumPatWts = 0;
    for (size_t j = 0; j < nPatterns; ++j) {
        sumPatWts += patternCounts[j];
    }
    const double lnAscProb = sumPatWts*lnProbVar;
    return lnAscProb;
}


double RevBayesCore::lnSumRootPatternProbabilities2Nodes(const double *p_left,
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
    double lnProbSum = 0.0;
    double siteLnProb = 0.0;
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(numPatterns,0.0);
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    //std::cerr << "computeRootLikelihood2Nodes numStates = " << numStates << " numPatterns = " << numPatterns << " p_inv = " << p_inv << "\n";
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < numPatterns; ++site) {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            for (size_t i = 0; i < numStates; ++i) {
                const double stContrib = p_site_left_j[i] * p_site_right_j[i] * rootFreq[i];
                tmp += stContrib;
                //std::cerr << "mixture = " << mixture << " site = " << site << " state = " << i << " rootFreq = " << rootFreq[i] << "    stContrib = " << stContrib << "    tmp = " << tmp << std::endl;
            }
            per_mixture_Likelihoods[site] += tmp;
            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset;
            p_site_mixture_right+=siteOffset;
        }
        p_mixture_left+=mixtureOffset;
        p_mixture_right+=mixtureOffset;
    }
    // sum the log-likelihoods for all sites together
    double oneMinusPInv = 1.0 - p_inv;
    if ( p_inv > 0.0 ) {
        for (size_t site = 0; site < numPatterns; ++site) {
            if ( siteInvariant[site] ) {
                siteLnProb = log( p_inv * rootFreq[ invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
            } else {
                siteLnProb = log( oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
            }
            if (site == 0) {
                lnProbSum = lnProbSum;
            } else {
                lnProbSum = lnSumOfNumbersInLnForm(siteLnProb, lnProbSum);
            }
//            std::cerr << "site = " << site << "    siteLnProb = " << siteLnProb << "    lnProbSum = " << lnProbSum << std::endl;
        }
    } else {
        for (size_t site = 0; site < numPatterns; ++site) {
            siteLnProb = log( per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
            if (site == 0) {
                lnProbSum = siteLnProb;
            } else {
                lnProbSum = lnSumOfNumbersInLnForm(siteLnProb, lnProbSum);
            }
            std::cerr << "site = " << site << "    siteLnProb = " << siteLnProb << "    lnProbSum = " << lnProbSum  << "    patternCounts = " << patternCounts[site] << std::endl;
        }
    }
    return lnProbSum;
}


double RevBayesCore::lnSumRootPatternProbabilities3Nodes(const double *p_left,
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
    double lnProbSum = 0.0;
    double siteLnProb = 0.0;
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(numPatterns,0.0);
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    const double*   p_mixture_middle   = p_middle;
    //std::cerr << "computeRootLikelihood3Nodes numStates = " << numStates << " numPatterns = " << numPatterns << " p_inv = " << p_inv << "\n";
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
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
            siteLnProb = log(prob) * patternCounts[site];
            if (site == 0) {
                lnProbSum = siteLnProb;
            } else {
                lnProbSum = lnSumOfNumbersInLnForm(siteLnProb, lnProbSum);
            }
        }
    } else {
        for (size_t site = 0; site < numPatterns; ++site) {
            siteLnProb = log( per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
            if (site == 0) {
                lnProbSum = siteLnProb;
            } else {
                lnProbSum = lnSumOfNumbersInLnForm(siteLnProb, lnProbSum);
            }

        }
    }
    return lnProbSum;
}

/* returns ln(X + Y) when given ln(X) and ln(Y) 
 *  let the larger and smaller of X, and Y be L and S resp:
 *   then lnD = lnL - lnS and lnS = lnL - lnD
 *    ln(L + S) = ln(exp[lnL] + exp[lnS])
                = ln(exp[lnL] + exp[lnL - lnD])
                = ln(exp[lnL] *(1 + exp[-lnD]))
                = ln(exp[lnL]) + ln(1 + exp[-lnD])
                = lnL + ln(1 + exp[-lnD])

 */
double lnSumOfNumbersInLnForm(double lnX, double lnY) {
    double larger, smaller;
    if (lnX > lnY) {
        larger = lnX;
        smaller = lnY;
    } else {
        larger = lnY;
        smaller = lnX;
    }
    const double lnDiff = larger - smaller;
    return larger + log(1.0 + exp(-lnDiff));
}