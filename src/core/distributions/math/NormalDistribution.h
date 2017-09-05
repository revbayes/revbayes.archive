#ifndef NormalDistribution_H
#define NormalDistribution_H

#include "ContinuousDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class NormalDistribution : public ContinuousDistribution {
        
    public:
        NormalDistribution(const TypedDagNode<double> *m, const TypedDagNode<double> *s, const TypedDagNode<double> *mi, const TypedDagNode<double> *ma );
        virtual                                            ~NormalDistribution(void);                                                   //!< Virtual destructor
        
        // public member functions
        double                                              cdf(void) const;                                                                  //!< Cummulative density function
        NormalDistribution*                                 clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        double                                              getMax(void) const;
        double                                              getMin(void) const;
        double                                              quantile(double p) const;                                                       //!< Qu
        void                                                redrawValue(void);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode<double>*                         mean;
        const TypedDagNode<double>*                         stDev;
        const TypedDagNode<double>*                         min;
        const TypedDagNode<double>*                         max;

    };
    
}

#endif
