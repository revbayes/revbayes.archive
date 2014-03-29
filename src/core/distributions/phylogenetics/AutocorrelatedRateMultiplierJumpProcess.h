#ifndef AutocorrelatedRateMultiplierJumpProcess_H
#define AutocorrelatedRateMultiplierJumpProcess_H

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    
    
    /**
     * This class implements the random local clock process with branch length dependent jump probabilities.
     *
     * The random local clock process is a simple jump process where jumps can occur at a branch.
     * The probability of no jump is    p = 1 - exp( -lambda * t )        where t is the branch length.
     * If no jump occurs than the rate is 1.0. Otherwise, the rate is drawn from the procided distribution.
     * Generally, it makes sense if the distribution has mean = 1.0. 
     * These value
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-28, version 1.0
     */
    class AutocorrelatedRateMultiplierJumpProcess : public TypedDistribution< std::vector<double> > {
        
    public:
        // constructor(s)
        AutocorrelatedRateMultiplierJumpProcess(TypedDistribution<double> *d, const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* l);
        
        // public member functions
        AutocorrelatedRateMultiplierJumpProcess*                clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
                
        
    private:
        
        // private members
        TypedDistribution<double>*                              valueDistribution;
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< double >*                           lambda;
        
    };
    
}

#endif
