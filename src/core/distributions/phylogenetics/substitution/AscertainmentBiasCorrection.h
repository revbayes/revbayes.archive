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

class VariableOnlyAscBiasCorrection : public AscertainmentBiasCorrectionStruct {
    public:
        VariableOnlyAscBiasCorrection(const size_t numStates,const size_t numMixtures=0);
        virtual ~VariableOnlyAscBiasCorrection();
        virtual void computeInternalAscBias(const AscertainmentBiasCorrectionStruct * ascLeft, const AscertainmentBiasCorrectionStruct * ascRight, size_t numSiteRates, size_t numStates, size_t numPatterns, const double ** tpMats);
        virtual void computeTipAscBias(size_t numSiteRates, size_t numStates, size_t numPatterns, const double ** tpMats,  const std::vector<bool> &gap_node, const std::vector<unsigned long> &char_node, bool usingAmbiguousCharacters);
        virtual double computeAscBiasLnProbCorrection2Node(const AscertainmentBiasCorrectionStruct * ascRight, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const size_t numPatterns, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const;
        virtual double computeAscBiasLnProbCorrection3Node(const AscertainmentBiasCorrectionStruct * ascRight, const AscertainmentBiasCorrectionStruct * ascMiddle, const size_t numSiteRates, const double *rootFreq, const size_t numStates, const size_t * patternCounts, const size_t numPatterns, const double p_inv,const std::vector<bool> &  siteInvariant, const std::vector<size_t> & invariantSiteIndex) const;
        virtual size_t GetNumProxyPatterns(const size_t numPatterns) const = 0;
        virtual void fillProxyTip(std::vector<bool> & proxyGapNode, std::vector<unsigned long> & proxyData, 
                                        const size_t nPatterns, const std::vector<bool> &gap_node, const std::vector<unsigned long> &char_node) const = 0;
        virtual void fillProxyInvariants(std::vector<bool> & proxyInv, std::vector<unsigned long> & proxyInvSiteInd, std::vector<size_t> & proxyPatCount, 
                                         const size_t * patternCounts, const size_t nPatterns, const std::vector<bool> &inv, const std::vector<unsigned long> &invSiteInd) const = 0;
        virtual double calcMatrixAscBias(double lnProbConstantFromLnSum, const size_t * patternCounts, const size_t nPatterns) const = 0;
    protected:
        virtual double calcAscBiasTempFromProxies2Node(const double *p_left,
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
                                                      const std::vector<size_t> & invariantSiteIndex) const =0 ;
        virtual double calcAscBiasTempFromProxies3Node(const double *p_left,
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
                                                      const std::vector<size_t> & invariantSiteIndex) const =0;
        const size_t numStates;
        mutable size_t numMixtures; /* must be mutable in case we have to realloc in computeAscBias... */
        std::vector<double> partialLikelihoods;
};

class VariableOnlyNoMissingAscertainmentBiasCorrectionStruct : public VariableOnlyAscBiasCorrection {
    public:
        VariableOnlyNoMissingAscertainmentBiasCorrectionStruct(const size_t numStates,const size_t numMixtures=0);
        virtual ~VariableOnlyNoMissingAscertainmentBiasCorrectionStruct();
        virtual size_t GetNumProxyPatterns(const size_t numPatterns) const {
            return this->numStates;
        }
        virtual void fillProxyTip(std::vector<bool> & proxyGapNode, std::vector<unsigned long> & proxyData, 
                                        const size_t nPatterns, const std::vector<bool> &gap_node, const std::vector<unsigned long> &char_node) const;
        virtual void fillProxyInvariants(std::vector<bool> & proxyInv, std::vector<unsigned long> & proxyInvSiteInd, std::vector<size_t> & proxyPatCount, 
                                         const size_t * patternCounts, const size_t nPatterns, const std::vector<bool> &inv, const std::vector<unsigned long> &invSiteInd) const;
        virtual double calcMatrixAscBias(double lnProbConstantFromLnSum, const size_t * patternCounts, const size_t nPatterns) const;

    protected:
        virtual double calcAscBiasTempFromProxies2Node(const double *p_left,
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
                                                      const std::vector<size_t> & invariantSiteIndex) const ;
        virtual double calcAscBiasTempFromProxies3Node(const double *p_left,
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
                                                      const std::vector<size_t> & invariantSiteIndex) const ;
};

class MissingAwareVariableOnlyAscertainmentBiasCorrection : public VariableOnlyAscBiasCorrection {
    public:
        MissingAwareVariableOnlyAscertainmentBiasCorrection(const size_t numStates,const size_t numMixtures=0);
        virtual ~MissingAwareVariableOnlyAscertainmentBiasCorrection();
        virtual size_t GetNumProxyPatterns(const size_t numPatterns) const {
            return this->numStates * numPatterns;
        }
        virtual void fillProxyTip(std::vector<bool> & proxyGapNode, std::vector<unsigned long> & proxyData, 
                                        const size_t nPatterns, const std::vector<bool> &gap_node, const std::vector<unsigned long> &char_node) const;
        virtual void fillProxyInvariants(std::vector<bool> & proxyInv, std::vector<unsigned long> & proxyInvSiteInd, std::vector<size_t> & proxyPatCount, 
                                         const size_t * patternCounts, const size_t nPatterns, const std::vector<bool> &inv, const std::vector<unsigned long> &invSiteInd) const;
        virtual double calcMatrixAscBias(double lnProbConstantFromLnSum, const size_t * patternCounts, const size_t nPatterns) const;

    protected:
        virtual double calcAscBiasTempFromProxies2Node(const double *p_left,
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
                                                      const std::vector<size_t> & invariantSiteIndex) const ;
        virtual double calcAscBiasTempFromProxies3Node(const double *p_left,
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
                                                      const std::vector<size_t> & invariantSiteIndex) const ;
};
}
#endif
