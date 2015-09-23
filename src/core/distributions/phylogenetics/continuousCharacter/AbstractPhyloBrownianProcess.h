#ifndef AbstractPhyloBrownianProcess_H
#define AbstractPhyloBrownianProcess_H

#include "ContinuousCharacterData.h"
#include "RbVector.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

#include <memory.h>

namespace RevBayesCore {
    
    /**
     * @brief Homogeneous distribution of character state evolution along a tree class (PhyloCTMC).
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-01-23, version 1.0
     */
    class AbstractPhyloBrownianProcess : public TypedDistribution< ContinuousCharacterData > {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractPhyloBrownianProcess(const TypedDagNode<Tree> *t, const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR, const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR, size_t nSites );
        virtual                                                            ~AbstractPhyloBrownianProcess(void);                                                     //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractPhyloBrownianProcess*                               clone(void) const = 0;                                                                  //!< Create an independent clone
        virtual double                                                      computeLnProbability(void) = 0;
        
        // non-virtual
        void                                                                setValue(ContinuousCharacterData *v, bool f=false);                                     //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                redrawValue(void);
        void                                                                reInitialized(void);
        
    protected:
        // helper method for this and derived classes
        double                                                              computeBranchTime(size_t nodeIdx, double brlen);
        double                                                              computeSiteRate(size_t siteIdx);
        virtual void                                                        resetValue(void);
        
        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                         //!< Swap a parameter
        
        // members
        double                                                              lnProb;
        size_t                                                              numNodes;
        size_t                                                              numSites;
        const TypedDagNode<Tree>*                                           tau;
        
        // members
        const TypedDagNode< double >*                                       homogeneousClockRate;
        const TypedDagNode< RbVector< double > >*                           heterogeneousClockRates;
        const TypedDagNode< double >*                                       homogeneousSiteRate;
        const TypedDagNode< RbVector< double > >*                           heterogeneousSiteRates;
        
        
    private:
        
        // private methods
        void                                                                simulate(const TopologyNode& node, std::vector< ContinuousTaxonData > &t);
        
    };
    
}


#endif
