#ifndef ExponentialFunction_H
#define ExponentialFunction_H

#include "ContinuousFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    
    /**
     * \brief Exponential function e^x.
     *
     * We compute the exponential value of x by e^x, or exp(x).
     *
     * \copyright (c) Copyright 2009-2015 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2012-08-13
     *
     */
    class ExponentialFunction : public ContinuousFunction {
        
    public:
        ExponentialFunction(const TypedDagNode<double> *a);
        
        ExponentialFunction*                clone(void) const;                                                  //!< Create a clone
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters

    private:
        const TypedDagNode<double>*         a;
    };
}

#endif
