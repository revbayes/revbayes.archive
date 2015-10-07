#ifndef PhyloCTMCSiteHomogeneousRestriction_H
#define PhyloCTMCSiteHomogeneousRestriction_H

#include "PhyloCTMCSiteHomogeneousConditional.h"
#include "StandardState.h"

namespace RevBayesCore {

    struct RestrictionCoding {

        enum { ALL               = 0x00,
               NOABSENCESITES    = 0x01,
               NOPRESENCESITES   = 0x02,
               VARIABLE          = 0x03,
               NOSINGLETONGAINS  = 0x04,
               NOSINGLETONLOSSES = 0x08,
               NOSINGLETONS      = 0x0C,
               INFORMATIVE       = 0x0F
              };
    };

    class PhyloCTMCSiteHomogeneousRestriction : public PhyloCTMCSiteHomogeneousConditional<StandardState> {

    public:
        PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode< Tree > *t, bool c, size_t nSites, bool amb, int cod = 0);
        virtual                                            ~PhyloCTMCSiteHomogeneousRestriction(void);                                                                   //!< Virtual destructor

        // public member functions
        PhyloCTMCSiteHomogeneousRestriction*                clone(void) const;

    protected:
        double                                              sumRootLikelihood( void );
        bool                                                isSitePatternCompatible(std::map<std::string, size_t>);
    };

}

#endif
