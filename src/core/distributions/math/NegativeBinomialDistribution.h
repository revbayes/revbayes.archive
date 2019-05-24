#ifndef NegativeBinomialDistribution_H
#define NegativeBinomialDistribution_H

#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    /**
     * @brief NegativeBinomial distribution class.
     *
     * The Negative Binomial distribution represents a family of distributions defined
     * on the natural number. The distribution represents the probability of a given number of successes before n failures  The Binomial distribution has 2 parameters:
     *   r .. the number of trials
     *   p .. the probability of success
     * Instances of this class can be associated to stochastic variables.
     *
     */
    class NegativeBinomialDistribution : public TypedDistribution<long> {
        
    public:
        NegativeBinomialDistribution(const TypedDagNode<long> *r, const TypedDagNode<double> *p);
        virtual                                            ~NegativeBinomialDistribution(void);                                             //!< Virtual destructor
        
        // public member functions
        NegativeBinomialDistribution*                       clone(void) const;                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);        //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode<long>*                           r;
        const TypedDagNode<double>*                         p;
    };
    
}

#endif
