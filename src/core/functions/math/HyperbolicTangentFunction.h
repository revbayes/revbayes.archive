#ifndef HyperbolicTangentFunction_H
#define HyperbolicTangentFunction_H

#include "ContinuousFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    
    /**
     * \brief Hyperbolic tangent of a real number.
     *
     * Compute the hyperbolic tangent of a real number x (tanh(x) = exp(2x)-1 / (exp(2x)+1)).
     *
     * \copyright (c) Copyright 2009-2015 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2015-01-31
     *
     */
    class HyperbolicTangentFunction : public ContinuousFunction {
        
    public:
        HyperbolicTangentFunction(const TypedDagNode<double> *a);
        
        HyperbolicTangentFunction*          clone(void) const;                                                  //!< Create a clon.
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<double>*         x;
    };
}

#endif
