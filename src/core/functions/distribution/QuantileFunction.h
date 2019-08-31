#ifndef QuantileFunction_H
#define QuantileFunction_H

#include "ContinuousFunction.h"
#include "ContinuousDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    /**
     * @brief Function calculating a quantile from a distribution
     */
    class QuantileFunction : public ContinuousFunction {
        
    public:
        QuantileFunction(const TypedDagNode<double> *p, ContinuousDistribution *d);
        QuantileFunction(const QuantileFunction &pdf);
        virtual                            ~QuantileFunction(void);
        
        QuantileFunction*                   clone(void) const;
        void                                update(void);
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);
        
    private:
        const TypedDagNode<double>*         p; //!< quantile probability
        ContinuousDistribution*             dist; //!< distribution to calculate from
    };
}


#endif
