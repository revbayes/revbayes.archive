#ifndef RateMultiplierPhyloFunction_H
#define RateMultiplierPhyloFunction_H


#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "TimeTree.h"
#include "RbVector.h"

#include <vector>

namespace RevBayesCore {
    
    
    /**
     * This class implements the rate multiplier along a tree that is used, e.g., for the random local clock.
     *
     * The rate multiplier phylogenetic function will compute rates by multiplying the parents branch rate with the
     * given branch rate multiplier. If the branch multiplier is 1.0 than the child has the same rate as the parent.
     * The root will take the base rate as its parent rate.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-29, version 1.0
     */
    class RateMultiplierPhyloFunction : public TypedFunction< RbVector<double> > {
        
    public:
        RateMultiplierPhyloFunction(const TypedDagNode< TimeTree > *t, const TypedDagNode< RbVector<double> > *r, const TypedDagNode<double> *b);
        
        // public member functions
        RateMultiplierPhyloFunction*                        clone(void) const;                                                                                              //!< Create an independent clone
        void                                                update(void);                                                                                                   //!< Update
        
    protected:
        void                                                recursiveUpdate(const TopologyNode& n, double parentValue);
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                                //!< Implementation of swaping parameters
        
        
    private:
        
        // members
        const TypedDagNode< double >*                       baseRate;                                                                                                       //!< The base rate that will be applied to the root
        const TypedDagNode< RbVector<double> >*             rateMultipliers;                                                                                                //!< The branch-wise rate multipliers
        const TypedDagNode< TimeTree >*                     tau;                                                                                                            //!< The phylogeny along which the multipliers change their rate.
        
    };
    
}

#endif
