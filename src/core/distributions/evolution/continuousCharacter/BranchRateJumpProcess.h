#ifndef BranchRateJumpProcess_H
#define BranchRateJumpProcess_H

#include "RbVector.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    
    /**
     * This class implements the random local clock process with branch length dependent jump probabilities.
     *
     * The random local clock process is a simple jump process where jumps can occur at a branch.
     * The probability of no jump is    
     *       p = rho + ( 1 - exp( -lambda * t ) ) * (1-rho)        
     * where t is the branch length and rho the instantaneous jump probability.
     * If rho == 0.0 then this process is only defined by the branch length and 
     * if lambda == 0 then this process is only defined by the instantaneous jump probability (rho).
     * If no jump occurs then the rate is 1.0. Otherwise, the rate is drawn from the procided distribution.
     * Generally, it makes sense if the distribution has mean = 1.0. 
     * These value of this distribution is a rate multiplier for each branch. The actual values should be computed
     * by a deterministic function that multiplies the parent rate with the branch rate multiplier for each branch.
     * See RateMultipliesPhyloFunction.h
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-28, version 1.0
     */
    class BranchRateJumpProcess : public TypedDistribution< RbVector<double> > {
        
    public:
        // constructor(s)
        BranchRateJumpProcess(TypedDistribution<double> *d, const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* l, const TypedDagNode< double >* r);
        
        // public member functions
        BranchRateJumpProcess*                                  clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        TypedDistribution<double>*                              getValueDistribution(void) const;                                                       //!< Get the distribution of the values when a jump occurred.
        void                                                    redrawValue(void);
        
        // Parameter management functions
        std::set<const DagNode*>                                getParameters(void) const;                                          //!< Return parameters
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

    private:
        
        // private members
        TypedDistribution<double>*                              valueDistribution;
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< double >*                           lambda;
        const TypedDagNode< double >*                           instantaneousJumpProbability;
        
    };
    
}

#endif
