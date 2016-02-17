#ifndef SumIntegerFunction_H
#define SumIntegerFunction_H

#include "RbVector.h"
#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    /**
     * \brief Function for computation of the sum of some numbers.
     *
     * This class is the function that computes the sum of some numbers.
     * The numbers are passed in as a DAG node whose value type is a std::vector<double>.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2014-07-27
     *
     */
    class SumIntegerFunction : public TypedFunction<int> {
        
    public:
        SumIntegerFunction(const TypedDagNode<RbVector<int> > * v);
        virtual                                            ~SumIntegerFunction(void);                                                         //!< Virtual destructor
        
        // public member functions
        SumIntegerFunction*                                 clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<RbVector<int> >*                 vals;
        
    };
    
}


#endif
