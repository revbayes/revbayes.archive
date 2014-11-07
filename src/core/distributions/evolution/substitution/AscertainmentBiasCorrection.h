#ifndef AscertainmentBiasCorrection_h
#define AscertainmentBiasCorrection_h
#include <cstdlib>
#include <vector>
namespace RevBayesCore {
class AscertainmentBiasCorrectionStruct {
    public:
        virtual ~AscertainmentBiasCorrectionStruct(){}
        virtual void computeInternalAscBias(const AscertainmentBiasCorrectionStruct * ascLeft, const AscertainmentBiasCorrectionStruct * ascRight, size_t numSiteRates, size_t numStates, size_t numPatterns, const double ** tpMats) = 0;
        virtual void computeTipAscBias(size_t numSiteRates, size_t numStates, size_t numPatterns, const double ** tpMats,  const std::vector<bool> &gap_node, const std::vector<unsigned long> &char_node, bool usingAmbiguousCharacters) = 0;
        virtual double computeAscBiasLnProbCorrection2Node(const AscertainmentBiasCorrectionStruct * ascRight, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const size_t numPatterns, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const = 0;
        virtual double computeAscBiasLnProbCorrection3Node(const AscertainmentBiasCorrectionStruct * ascRight, const AscertainmentBiasCorrectionStruct * ascMiddle, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const size_t numPatterns, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const = 0;
};

class VariableOnlyAscertainmentBiasCorrectionStruct : public AscertainmentBiasCorrectionStruct {
    public:
        VariableOnlyAscertainmentBiasCorrectionStruct(const size_t numStates,const size_t numMixtures=0);
        virtual ~VariableOnlyAscertainmentBiasCorrectionStruct();
        virtual void computeInternalAscBias(const AscertainmentBiasCorrectionStruct * ascLeft, const AscertainmentBiasCorrectionStruct * ascRight, size_t numSiteRates, size_t numStates, size_t numPatterns, const double ** tpMats);
        virtual void computeTipAscBias(size_t numSiteRates, size_t numStates, size_t numPatterns, const double ** tpMats,  const std::vector<bool> &gap_node, const std::vector<unsigned long> &char_node, bool usingAmbiguousCharacters);
        virtual double computeAscBiasLnProbCorrection2Node(const AscertainmentBiasCorrectionStruct * ascRight, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const size_t numPatterns, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const;
        virtual double computeAscBiasLnProbCorrection3Node(const AscertainmentBiasCorrectionStruct * ascRight, const AscertainmentBiasCorrectionStruct * ascMiddle, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const size_t numPatterns, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const;
    private:
        const size_t numStates;
        mutable size_t numMixtures; /* must be mutable in case we have to realloc in computeAscBias... */
        std::vector<double> partialLikelihoods;
};
}
#endif
