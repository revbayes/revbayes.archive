#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>
#include "AbstractPhyloCTMCSiteHomogeneous.h" // too broad an include, but this is where the computeInternalNodeLikelihood... funcs are declared
#include "AscertainmentBiasCorrection.h"
#include "RbException.h"
using namespace RevBayesCore;
double lnSumOfNumbersInLnForm(double lnX, double lnY);
double lnSumRootSiteProb(const double *per_mixture_Likelihoods, const size_t * patternCounts,
                                                 const size_t numPatterns, const size_t numSiteRates,
                                                 const double * rootFreq,
                                                 const size_t numStates,
                                                 const double p_inv,
                                                 const std::vector<bool> & siteInvariant,
                                                 const std::vector<size_t> & invariantSiteIndex );
double lnSumAscCorrectionsForPatterns(const double *per_mixture_Likelihoods, const size_t * patternCounts,
                                                 const size_t numPatterns, const size_t numSiteRates,
                                                 const double * rootFreq,
                                                 const size_t numStates,
                                                 const double p_inv,
                                                 const std::vector<bool> & siteInvariant,
                                                 const std::vector<size_t> & invariantSiteIndex );
double lnSumAscCorrectionsForPatterns2Nodes(const double *p_left,
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
                                                 const std::vector<size_t> & invariantSiteIndex);
double lnSumAscCorrectionsForPatterns3Nodes(const double *p_left,
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
                                                 const std::vector<size_t> & invariantSiteIndex);

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

    std::vector<bool> proxyGapNode;
    std::vector<unsigned long> proxyData;
    this->fillProxyTip(proxyGapNode, proxyData, nPatterns, gap_node, char_node);

    computeTipNodeLikelihood(&(this->partialLikelihoods[0]),
                              nSiteRates,
                              nStates,
                              numProxyPatterns,
                              siteOffset,
                              mixtureOffset,
                              tpMats,
                              proxyGapNode,
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
    std::vector<bool> proxyInvariants;
    std::vector<unsigned long> proxyInvariantSiteIndex;
    std::vector<size_t> proxyPatternCounts;
    this->fillProxyInvariants(proxyInvariants, proxyInvariantSiteIndex, proxyPatternCounts, patternCounts, nPatterns, siteInvariant, invariantSiteIndex);

    const double lnProbConstant = this->calcAscBiasTempFromProxies2Node(&(this->partialLikelihoods[0]),
                                                                      &(aR->partialLikelihoods[0]),
                                                                      nSiteRates,
                                                                      rootFreq,
                                                                      nStates,
                                                                      &(proxyPatternCounts[0]),
                                                                      numProxyPatterns,
                                                                      siteOffset,
                                                                      mixtureOffset,
                                                                      p_inv,
                                                                      proxyInvariants,
                                                                      proxyInvariantSiteIndex);
    return this->calcMatrixAscBias(lnProbConstant, patternCounts, nPatterns);
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
    std::vector<bool> proxyInvariants;
    std::vector<unsigned long> proxyInvariantSiteIndex;
    std::vector<size_t> proxyPatternCounts;
    this->fillProxyInvariants(proxyInvariants, proxyInvariantSiteIndex, proxyPatternCounts, patternCounts, nPatterns, siteInvariant, invariantSiteIndex);
    
    const double lnProbConstant = this->calcAscBiasTempFromProxies3Node(&(this->partialLikelihoods[0]),
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
                                                                      proxyInvariants,
                                                                      proxyInvariantSiteIndex);
    /* If the ln probability of a constant pattern is less than -40, then the correction is lost in rounding error...*/
    return this->calcMatrixAscBias(lnProbConstant, patternCounts, nPatterns);
}
void RevBayesCore::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct::fillProxyTip(std::vector<bool> & proxyGapNode,
                                                                                        std::vector<unsigned long> & proxyData,
                                                                                        const size_t nPatterns,
                                                                                        const std::vector<bool> &gap_node,
                                                                                        const std::vector<unsigned long> &char_node) const {
    const size_t numProxyPatterns = this->GetNumProxyPatterns(nPatterns);
    proxyGapNode.assign(numProxyPatterns, false);
    proxyData.resize(numProxyPatterns);
    for (unsigned long i = 0; i < numProxyPatterns; ++i) {
        proxyData[i] = i;
    }
}

void RevBayesCore::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct::fillProxyInvariants(std::vector<bool> & proxyInvariants,
                                                                                        std::vector<unsigned long> & proxyInvariantSiteIndex,
                                                                                        std::vector<size_t> & proxyPatternCounts,
                                                                                        const size_t * patternCounts,
                                                                                        const size_t nPatterns,
                                                                                        const std::vector<bool> &siteInvariant,
                                                                                        const std::vector<unsigned long> &invariantSiteIndex) const {
    const size_t numProxyPatterns = this->GetNumProxyPatterns(nPatterns);
    proxyInvariants.assign(numProxyPatterns, true);
    proxyInvariantSiteIndex.resize(numProxyPatterns);
    for (unsigned long i = 0; i < numProxyPatterns; ++i) {
        proxyInvariantSiteIndex[i] = i;
    }
    proxyPatternCounts.assign(numProxyPatterns, 1);
}
double RevBayesCore::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct::calcMatrixAscBias(double lnProbConstantFromLnSum,
                                                                                               const size_t * patternCounts,
                                                                                               const size_t nPatterns) const {
    /* If the ln probability of a constant pattern is less than -40, then the correction is lost in rounding error...*/
    if (lnProbConstantFromLnSum < -40) {
        return 0.0;
    }
    const double probConstant = exp(lnProbConstantFromLnSum);
    const double lnProbVar = log(1.0 - probConstant);
    size_t sumPatWts = 0;
    for (size_t j = 0; j < nPatterns; ++j) {
        sumPatWts += patternCounts[j];
    }
    const double lnAscProb = sumPatWts*lnProbVar;
    return lnAscProb;
}

double RevBayesCore::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct::calcAscBiasTempFromProxies2Node(const double *p_left,
                                                      const double *p_right,
                                                      const size_t numSiteRates,
                                                      const double * rootFreq,
                                                      const size_t numStates,
                                                      const size_t * patternCounts,
                                                      const size_t numProxyPatterns,
                                                      const size_t siteOffset,
                                                      const size_t mixtureOffset,
                                                      const double p_inv,
                                                      const std::vector<bool> & proxyInvariants,
                                                      const std::vector<size_t> & proxyInvariantSiteIndex) const {
    return lnSumRootPatternProbabilities2Nodes(p_left, p_right,
                                                                      numSiteRates,
                                                                      rootFreq,
                                                                      numStates,
                                                                      patternCounts,
                                                                      numProxyPatterns,
                                                                      siteOffset,
                                                                      mixtureOffset,
                                                                      p_inv,
                                                                      proxyInvariants,
                                                                      proxyInvariantSiteIndex);
}
double RevBayesCore::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct::calcAscBiasTempFromProxies3Node(const double *p_left,
                                                      const double *p_right,
                                                      const double *p_middle,
                                                      const size_t numSiteRates,
                                                      const double * rootFreq,
                                                      const size_t numStates,
                                                      const size_t * patternCounts,
                                                      const size_t numProxyPatterns,
                                                      const size_t siteOffset,
                                                      const size_t mixtureOffset,
                                                      const double p_inv,
                                                      const std::vector<bool> & proxyInvariants,
                                                      const std::vector<size_t> & proxyInvariantSiteIndex) const {
    return lnSumRootPatternProbabilities3Nodes(p_left, p_right, p_middle,
                                                                      numSiteRates,
                                                                      rootFreq,
                                                                      numStates,
                                                                      patternCounts,
                                                                      numProxyPatterns,
                                                                      siteOffset,
                                                                      mixtureOffset,
                                                                      p_inv,
                                                                      proxyInvariants,
                                                                      proxyInvariantSiteIndex);
}

double lnSumRootSiteProb(const double *per_mixture_Likelihoods, const size_t * patternCounts,
                                                 const size_t numPatterns, const size_t numSiteRates,
                                                 const double * rootFreq,
                                                 const size_t numStates,
                                                 const double p_inv,
                                                 const std::vector<bool> & siteInvariant,
                                                 const std::vector<size_t> & invariantSiteIndex ) {
    double lnProbSum = 0.0;
    double siteLnProb = 0.0;
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
            //std::cerr << "site = " << site << "    siteLnProb = " << siteLnProb << "    lnProbSum = " << lnProbSum  << "    patternCounts = " << patternCounts[site] << std::endl;
        }
    }
    return lnProbSum;
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
    std::vector<double> per_mixture_Likelihoods;
    fillRootSiteLikelihoodVector(per_mixture_Likelihoods, p_left, p_right, numSiteRates, rootFreq, numStates, numPatterns, siteOffset, mixtureOffset);
    return lnSumRootSiteProb(&(per_mixture_Likelihoods[0]), patternCounts, numPatterns, numSiteRates, rootFreq, numStates, p_inv, siteInvariant, invariantSiteIndex);
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
    std::vector<double> per_mixture_Likelihoods;
    fillRootSiteLikelihoodVector(per_mixture_Likelihoods, p_left, p_right, p_middle, numSiteRates, rootFreq, numStates, numPatterns, siteOffset, mixtureOffset);
    return lnSumRootSiteProb(&(per_mixture_Likelihoods[0]), patternCounts, numPatterns, numSiteRates, rootFreq, numStates, p_inv, siteInvariant, invariantSiteIndex);
}

void RevBayesCore::fillRootSiteLikelihoodVector(std::vector<double> & per_mixture_Likelihoods,
                                  const double *p_mixture_left,
                                  const double *p_mixture_right,
                                  const size_t numSiteRates,
                                  const double * rootFreq,
                                  const size_t numStates,
                                  const size_t numPatterns,
                                  const size_t siteOffset,
                                  const size_t mixtureOffset) {
    per_mixture_Likelihoods.assign(numPatterns,0.0);
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        const double* p_site_mixture_left = p_mixture_left;
        const double* p_site_mixture_right = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < numPatterns; ++site) {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            for (size_t i = 0; i < numStates; ++i) {
                const double stContrib = p_site_left_j[i] * p_site_right_j[i] * rootFreq[i];
                tmp += stContrib;
                //std::cerr << "    mixture = " << mixture << " site = " << site << " state = " << i << " rootFreq = " << rootFreq[i] << "    stContrib = " << stContrib << "    tmp = " << tmp << std::endl;
            }
            per_mixture_Likelihoods[site] += tmp;
            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset;
            p_site_mixture_right+=siteOffset;
        }
        p_mixture_left+=mixtureOffset;
        p_mixture_right+=mixtureOffset;
    }
}

void RevBayesCore::fillRootSiteLikelihoodVector(std::vector<double> & per_mixture_Likelihoods,
                                  const double *p_mixture_left,
                                  const double *p_mixture_right,
                                  const double *p_mixture_middle,
                                  const size_t numSiteRates,
                                  const double * rootFreq,
                                  const size_t numStates,
                                  const size_t numPatterns,
                                  const size_t siteOffset,
                                  const size_t mixtureOffset) {
    per_mixture_Likelihoods.assign(numPatterns,0.0);
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
}

/* if np is the "real" number of patterns in the data numPatterns = numStates*np
 */
double lnSumAscCorrectionsForPatterns2Nodes(const double *p_left,
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
    assert(numPatterns % numStates == 0); 
    std::vector<double> per_mixture_Likelihoods;
    fillRootSiteLikelihoodVector(per_mixture_Likelihoods, p_left, p_right, numSiteRates, rootFreq, numStates, numPatterns, siteOffset, mixtureOffset);
    return lnSumAscCorrectionsForPatterns(&(per_mixture_Likelihoods[0]), patternCounts, numPatterns, numSiteRates, rootFreq, numStates, p_inv, siteInvariant, invariantSiteIndex);
}

double lnSumAscCorrectionsForPatterns(const double *per_mixture_Likelihoods, const size_t * patternCounts,
                                                 const size_t numPatterns, const size_t numSiteRates,
                                                 const double * rootFreq,
                                                 const size_t numStates,
                                                 const double p_inv,
                                                 const std::vector<bool> & siteInvariant,
                                                 const std::vector<size_t> & invariantSiteIndex ) {
    // sum the log-likelihoods for all sites together
    double oneMinusPInv = 1.0 - p_inv;
    const size_t numBins = numPatterns / numStates;
    double lnCorrectionSum = 0.0;
    for (size_t b = 0; b < numBins; ++b) {
        double binProb = 0.0;
        for (size_t stateIndex = 0; stateIndex < numStates; ++stateIndex) {
            if (stateIndex > 0) {
                assert(patternCounts[b * numStates] == patternCounts[b * numStates + stateIndex]);
            }
            size_t site = b * numStates + stateIndex;
            binProb += per_mixture_Likelihoods[site];
            //std::cerr << " bin = " << b << " stateIndex = " << stateIndex <<  "  per_mixture_Likelihoods[site] = "<< per_mixture_Likelihoods[site] << "   binProb = " << binProb << "    lnCorrectionSum = " << lnCorrectionSum << std::endl;
        }
        binProb *= (oneMinusPInv/numSiteRates);
        binProb += p_inv;
        const double lnCorrectionPattern = patternCounts[b * numStates]*log(1- binProb);
        lnCorrectionSum += lnCorrectionPattern;
        //std::cerr << " bin = " << b << "   binProb = " << binProb << "    lnCorrectionSum = " << lnCorrectionSum << std::endl;
    }
    return lnCorrectionSum;
}

double lnSumAscCorrectionsForPatterns3Nodes(const double *p_left,
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
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods;
    fillRootSiteLikelihoodVector(per_mixture_Likelihoods, p_left, p_right, p_middle, numSiteRates, rootFreq, numStates, numPatterns, siteOffset, mixtureOffset);
    return lnSumAscCorrectionsForPatterns(&(per_mixture_Likelihoods[0]), patternCounts, numPatterns, numSiteRates, rootFreq, numStates, p_inv, siteInvariant, invariantSiteIndex);
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






RevBayesCore::MissingAwareVariableOnlyAscertainmentBiasCorrection::MissingAwareVariableOnlyAscertainmentBiasCorrection(const size_t ns, const size_t nm)
    :VariableOnlyAscBiasCorrection(ns, nm) {
}

RevBayesCore::MissingAwareVariableOnlyAscertainmentBiasCorrection::~MissingAwareVariableOnlyAscertainmentBiasCorrection() {
}

void RevBayesCore::MissingAwareVariableOnlyAscertainmentBiasCorrection::fillProxyTip(std::vector<bool> & proxyGapNode,
                                                                                        std::vector<unsigned long> & proxyData,
                                                                                        const size_t nPatterns,
                                                                                        const std::vector<bool> &gap_node,
                                                                                        const std::vector<unsigned long> &char_node) const {
    const size_t numProxyPatterns = this->GetNumProxyPatterns(nPatterns);
    proxyGapNode.resize(numProxyPatterns);
    proxyData.resize(numProxyPatterns);
    for (unsigned long i = 0; i < nPatterns; ++i) {
        for (unsigned long j = 0; j < this->numStates; ++j) {
            proxyGapNode[this->numStates*i + j] = gap_node[i];
            proxyData[this->numStates*i + j] = j;
        }
    }
}

void RevBayesCore::MissingAwareVariableOnlyAscertainmentBiasCorrection::fillProxyInvariants(std::vector<bool> & proxyInvariants,
                                                                                        std::vector<unsigned long> & proxyInvariantSiteIndex,
                                                                                        std::vector<size_t> & proxyPatternCounts,
                                                                                        const size_t * patternCounts,
                                                                                        const size_t nPatterns,
                                                                                        const std::vector<bool> &siteInvariant,
                                                                                        const std::vector<unsigned long> &invariantSiteIndex) const {
    const size_t numProxyPatterns = this->GetNumProxyPatterns(nPatterns);
    proxyInvariants.assign(numProxyPatterns, true);
    proxyInvariantSiteIndex.resize(numProxyPatterns);
    proxyPatternCounts.resize(numProxyPatterns);
    for (unsigned long i = 0; i < nPatterns; ++i) {
        for (unsigned long j = 0; j < this->numStates; ++j) {
            proxyInvariantSiteIndex[this->numStates*i +j] = i;
            proxyPatternCounts[this->numStates*i +j] = patternCounts[i];
        }
    }
    
}
double RevBayesCore::MissingAwareVariableOnlyAscertainmentBiasCorrection::calcMatrixAscBias(double lnProbConstantFromLnSum,
                                                                                               const size_t * patternCounts,
                                                                                               const size_t nPatterns) const {
    return lnProbConstantFromLnSum;
}

double RevBayesCore::MissingAwareVariableOnlyAscertainmentBiasCorrection::calcAscBiasTempFromProxies2Node(const double *p_left,
                                                      const double *p_right,
                                                      const size_t numSiteRates,
                                                      const double * rootFreq,
                                                      const size_t numStates,
                                                      const size_t * patternCounts,
                                                      const size_t numProxyPatterns,
                                                      const size_t siteOffset,
                                                      const size_t mixtureOffset,
                                                      const double p_inv,
                                                      const std::vector<bool> & proxyInvariants,
                                                      const std::vector<size_t> & proxyInvariantSiteIndex) const {
    return lnSumAscCorrectionsForPatterns2Nodes(p_left, p_right,
                                                                      numSiteRates,
                                                                      rootFreq,
                                                                      numStates,
                                                                      patternCounts,
                                                                      numProxyPatterns,
                                                                      siteOffset,
                                                                      mixtureOffset,
                                                                      p_inv,
                                                                      proxyInvariants,
                                                                      proxyInvariantSiteIndex);
}
double RevBayesCore::MissingAwareVariableOnlyAscertainmentBiasCorrection::calcAscBiasTempFromProxies3Node(const double *p_left,
                                                      const double *p_right,
                                                      const double *p_middle,
                                                      const size_t numSiteRates,
                                                      const double * rootFreq,
                                                      const size_t numStates,
                                                      const size_t * patternCounts,
                                                      const size_t numProxyPatterns,
                                                      const size_t siteOffset,
                                                      const size_t mixtureOffset,
                                                      const double p_inv,
                                                      const std::vector<bool> & proxyInvariants,
                                                      const std::vector<size_t> & proxyInvariantSiteIndex) const {
    return lnSumAscCorrectionsForPatterns3Nodes(p_left, p_right, p_middle,
                                                                      numSiteRates,
                                                                      rootFreq,
                                                                      numStates,
                                                                      patternCounts,
                                                                      numProxyPatterns,
                                                                      siteOffset,
                                                                      mixtureOffset,
                                                                      p_inv,
                                                                      proxyInvariants,
                                                                      proxyInvariantSiteIndex);
}

