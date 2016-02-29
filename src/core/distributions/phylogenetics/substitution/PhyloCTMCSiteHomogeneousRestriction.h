#ifndef PhyloCTMCSiteHomogeneousRestriction_H
#define PhyloCTMCSiteHomogeneousRestriction_H

#include "PhyloCTMCSiteHomogeneousConditional.h"
#include "RestrictionState.h"

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
            bool                                                isSitePatternCompatible(std::map<std::string, size_t>);
        };

}

#endif
