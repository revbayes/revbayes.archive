#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>
#include "AbstractPhyloCTMCSiteHomogeneous.h" // too broad an include, but this is where the computeInternalNodeLikelihood... funcs are declared
#include "AscertainmentBiasCorrection.h"
#include "RbException.h"
using namespace RevBayesCore;
RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::VariableOnlyAscertainmentBiasCorrectionStruct(const size_t ns, const size_t nm)
    :numStates(ns),
    numMixtures(nm){
    partialLikelihoods.reserve(this->numStates * this->numStates * this->numMixtures);
}
RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::~VariableOnlyAscertainmentBiasCorrectionStruct(){}
void RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::computeInternalAscBias(const AscertainmentBiasCorrectionStruct * ascLeft,
                                                                                         const AscertainmentBiasCorrectionStruct * ascRight,
                                                                                         size_t nSiteRates,
                                                                                         size_t nStates,
                                                                                         size_t nPatterns,
                                                                                         const double ** tpMats) {
    const VariableOnlyAscertainmentBiasCorrectionStruct * aL = dynamic_cast<const VariableOnlyAscertainmentBiasCorrectionStruct*>(ascLeft);
    const VariableOnlyAscertainmentBiasCorrectionStruct * aR = dynamic_cast<const VariableOnlyAscertainmentBiasCorrectionStruct*>(ascRight);
    if (aL == 0 || aR == 0) {
        assert(aL && aR);
        throw RbException("Expected left and right ascertainment bias correction structs to be VariableOnlyAscertainmentBiasCorrectionStruct *");
    }
    assert(nStates == this->numStates);
    assert(this->numMixtures == 0 || nSiteRates == this->numMixtures);
    assert(nStates == aL->numStates);
    assert(nSiteRates == aL->numMixtures);
    assert(nStates == aR->numStates);
    assert(nSiteRates == aR->numMixtures);
    const unsigned plLen = this->numStates * this->numStates * nSiteRates;
    assert(plLen == aL->partialLikelihoods.size());
    assert(plLen == aR->partialLikelihoods.size());
    if (this->partialLikelihoods.size() < plLen) {
        this->numMixtures = nSiteRates;
        this->partialLikelihoods.resize(plLen);
    }
    const size_t mixtureOffset =  nPatterns*nStates;
    const size_t siteOffset =  nStates;
    computeInternalNodeLikelihood(&(this->partialLikelihoods[0]),
                                  &(aL->partialLikelihoods[0]),
                                  &(aR->partialLikelihoods[0]),
                                  nSiteRates,
                                  nStates,
                                  nStates,
                                  siteOffset,
                                  mixtureOffset,
                                  tpMats);
}
void RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::computeTipAscBias(size_t nSiteRates,
                                                                                    size_t nStates,
                                                                                    size_t nPatterns,
                                                                                    const double ** tpMats,
                                                                                    const std::vector<bool> &gap_node,
                                                                                    const std::vector<unsigned long> &char_node,
                                                                                    bool usingAmbiguousCharacters) {
    assert(nStates == this->numStates);
    assert(this->numMixtures == 0 || nSiteRates == this->numMixtures);
    const unsigned plLen = this->numStates * this->numStates * nSiteRates;
    if (this->partialLikelihoods.size() < plLen) {
        this->numMixtures = nSiteRates;
        this->partialLikelihoods.resize(plLen);
    }
    const size_t mixtureOffset =  nPatterns*nStates;
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
                              nStates,
                              siteOffset,
                              mixtureOffset,
                              tpMats,
                              noMissingGapNode,
                              proxyData,
                              false);
}
double RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::computeAscBiasLnProbCorrection2Node(const AscertainmentBiasCorrectionStruct * ascRight, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const {
    std::cerr << "2node asc" << std::endl;
    double lnAscProb = 0.0;
    return lnAscProb;
}
double RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::computeAscBiasLnProbCorrection3Node(const AscertainmentBiasCorrectionStruct * ascRight, const AscertainmentBiasCorrectionStruct * ascMiddle, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const {
    std::cerr << "3node asc" << std::endl;
    double lnAscProb = 0.0;
    return lnAscProb;
}
