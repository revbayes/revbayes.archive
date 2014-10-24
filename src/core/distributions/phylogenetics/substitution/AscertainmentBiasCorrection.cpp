#include <cstdlib>
#include <cmath>
#include <AscertainmentBiasCorrection.h>
using namespace RevBayesCore;
RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::VariableOnlyAscertainmentBiasCorrectionStruct(const size_t ns, const size_t nm)
    :numStates(ns),
    numMixtures(nm){
}
RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::~VariableOnlyAscertainmentBiasCorrectionStruct(){}
void RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::computeInternalAscBias(const AscertainmentBiasCorrectionStruct * ascLeft, const AscertainmentBiasCorrectionStruct * ascRight, size_t numSiteRates, size_t numStates, size_t numPatterns, const double ** tpMats) {
}
void RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::computeTipAscBias(size_t numSiteRates, size_t numStates, size_t numPatterns, const double ** tpMats,  const std::vector<bool> &gap_node, const std::vector<unsigned long> &char_node, bool usingAmbiguousCharacters) {
}
double RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::computeAscBiasLnProbCorrection2Node(const AscertainmentBiasCorrectionStruct * ascRight, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const {
    double lnAscProb = 0.0;
    return lnAscProb;
}
double RevBayesCore::VariableOnlyAscertainmentBiasCorrectionStruct::computeAscBiasLnProbCorrection3Node(const AscertainmentBiasCorrectionStruct * ascRight, const AscertainmentBiasCorrectionStruct * ascMiddle, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const {
    double lnAscProb = 0.0;
    return lnAscProb;
}
