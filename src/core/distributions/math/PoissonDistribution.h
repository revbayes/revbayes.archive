#ifndef PoissonDistribution_H
#define PoissonDistribution_H

#include "TypedDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    /**
     * @brief Poisson distribution class.
     *
     * The Poisson distribution represents a family of distributions
     * defined on the natural numbers. The Poisson distribution has 1 parameters:
     *   lambda .. the rate
     * Instances of this class can be associated to stochastic variables.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-12, version 1.0
     *
     */
    class PoissonDistribution : public TypedDistribution<long> {
        
    public:
        PoissonDistribution(const TypedDagNode<double> *l);
        virtual                                            ~PoissonDistribution(void);                                                  //!< Virtual destructor
        
        // public member functions
        PoissonDistribution*                                clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode<double>*                         lambda;
        
    };
    
}

#endif
