#ifndef FloorFunction_H
#define FloorFunction_H

#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    /**
     * \brief Round down to the next integer.
     *
     * The floor function rounds a real number down to the next integer number.
     *
     * \copyright (c) Copyright 2009-2015 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2012-08-13
     *
     */
    class FloorFunction : public TypedFunction<int> {
        
    public:
        FloorFunction(const TypedDagNode<double> *a);
        
        FloorFunction*                      clone(void) const;                                                  //!< Create a clon.
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<double>*         a;
    };
}

#endif
