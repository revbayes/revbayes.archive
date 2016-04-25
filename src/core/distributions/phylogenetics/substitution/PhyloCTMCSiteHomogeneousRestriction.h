#ifndef PhyloCTMCSiteHomogeneousRestriction_H
#define PhyloCTMCSiteHomogeneousRestriction_H

#include "PhyloCTMCSiteHomogeneousConditional.h"
#include "RestrictionState.h"

#define RESTRICTION_SSE_ENABLED
namespace RevBayesCore {

    struct RestrictionAscertainmentBias {
        
      enum Coding { ALL                 = 0x00,
                    NOABSENCESITES      = 0x01,
                    NOPRESENCESITES     = 0x02,
                    VARIABLE            = 0x03,
                    NOSINGLETONPRESENCE = 0x04,
                    NOSINGLETONABSENCE  = 0x08,
                    NOSINGLETONS        = 0x0C,
                    INFORMATIVE         = 0x0F
                  };
    };

    class PhyloCTMCSiteHomogeneousRestriction : public PhyloCTMCSiteHomogeneousConditional<RestrictionState> {

        public:
            PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode< Tree > *t, bool c, size_t nSites, bool amb, RestrictionAscertainmentBias::Coding cod = RestrictionAscertainmentBias::ALL);

            // public member functions
            PhyloCTMCSiteHomogeneousRestriction*                clone(void) const;

        protected:

            double                                              sumRootLikelihood( void );
            bool                                                isSitePatternCompatible(std::map<size_t, size_t>);

#ifdef RESTRICTION_SSE_ENABLED
            virtual void                                        computeRootLikelihood(size_t root, size_t l, size_t r);
            virtual void                                        computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
            virtual void                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
            virtual void                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
            virtual void                                        computeTipLikelihood(const TopologyNode &node, size_t nIdx);
            virtual void                                        resizeLikelihoodVectors(void);
            double                                              sumUncorrectedRootLikelihood();

            virtual void                                        scale(size_t i);
            virtual void                                        scale(size_t i, size_t l, size_t r);
            virtual void                                        scale(size_t i, size_t l, size_t r, size_t m);

            size_t                                              numSIMDBlocks;
            double*                                             per_mixture_Likelihoods;
        public:
            PhyloCTMCSiteHomogeneousRestriction(const PhyloCTMCSiteHomogeneousRestriction &n);
#endif
        };

}

#endif
