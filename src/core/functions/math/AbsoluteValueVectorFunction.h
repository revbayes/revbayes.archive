#ifndef AbsoluteValueVectorFunction_H
#define AbsoluteValueVectorFunction_H

#include "RbVector.h"
#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    
    /**
     * \brief Absolute value of a vector real number.
     *
     * The absolute value functons computes the value without sign.
     * Hence, the return value y = x if x >= 0 and y = -x if x < 0.
     *
     * \copyright (c) Copyright 2009-2015 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2015-08-02
     *
     */
    class AbsoluteValueVectorFunction : public TypedFunction< RbVector<double> > {
        
    public:
        AbsoluteValueVectorFunction(const TypedDagNode<RbVector<double> > *a);
        
        AbsoluteValueVectorFunction*                clone(void) const;                                                  //!< Create a clon.
        void                                        update(void);                                                       //!< Recompute the value
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<RbVector<double> >*      a;
    };
}

#endif
