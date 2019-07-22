#ifndef TimeConstrainedSBN_H
#define TimeConstrainedSBN_H

#include "MatrixReal.h"
#include "Subsplit.h"
#include "SBNParameters.h"
#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {

    class TimeConstrainedSBN : public TypedDistribution<Tree> {

    public:
        TimeConstrainedSBN(const SBNParameters parameters);
		virtual                                            ~TimeConstrainedSBN(void);                                                                    //!< Virtual destructor

        // public member functions
        TimeConstrainedSBN*                        clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        double                                              computeLnProbabilityNodeTimes( void );
        void                                                redrawValue(void);
        virtual void                                        setValue(Tree *v, bool f=false);                                    //!< Set the current value, e.g. attach an observation (clamp)

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

    private:

        // helper functions
        void                                                simulateTree(void);

        // members
        SBNParameters                                       parameters;  // The parameters for this SBN
        std::vector<Taxon>                                  taxa;
    };

}

#endif
