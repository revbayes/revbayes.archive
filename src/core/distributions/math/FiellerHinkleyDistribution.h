#ifndef FiellerHinkleyDistribution_H
#define FiellerHinkleyDistribution_H

#include "ContinuousDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class FiellerHinkleyDistribution : public ContinuousDistribution {
        
    public:
        FiellerHinkleyDistribution(const TypedDagNode<double> *m1, const TypedDagNode<double> *s1, const TypedDagNode<double> *m2, const TypedDagNode<double> *s2, const TypedDagNode<double> *r);
        virtual                                            ~FiellerHinkleyDistribution(void);                                                   //!< Virtual destructor
        
        // public member functions
        double                                              cdf(void) const;                                                                  //!< Cummulative density function
        double                                              quantile(double p) const;                                                                  //!< Cummulative density function
        double                                              getMax(void) const;
        double                                              getMin(void) const;
        FiellerHinkleyDistribution*                             clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode<double>*                         mu1;
        const TypedDagNode<double>*                         sd1;
        const TypedDagNode<double>*                         mu2;
        const TypedDagNode<double>*                         sd2;
        const TypedDagNode<double>*                         rho;
    };
    
}

#endif
