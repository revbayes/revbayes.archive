#ifndef EmpiricalQuantileFunction_H
#define EmpiricalQuantileFunction_H

#include "RbVector.h"
#include "TypedFunction.h"
#include "TypedDagNode.h"

#include <vector>

namespace RevBayesCore {
    
    
    /**
     * @brief Arithmetic EmpiricalQuantile of a vector of real numbers.
     *
     * This function computes the EmpiricalQuantile of a vector of real numbers computing the kth quantile for the vector.
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-07-04
     *
     */
    class EmpiricalQuantileFunction : public TypedFunction<double> {
        
    public:
        EmpiricalQuantileFunction(const TypedDagNode< RbVector<double> > * v, const TypedDagNode<double>* k);
        virtual                                            ~EmpiricalQuantileFunction(void);                                                       //!< Virtual destructor
        
        // public member functions
<<<<<<< HEAD
        EmpiricalQuantileFunction*                                       clone(void) const;                                                          //!< Create an independent clone
=======
        EmpiricalQuantileFunction*                          clone(void) const;                                                          //!< Create an independent clone
>>>>>>> development
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             vals;
        const TypedDagNode<double>*                         kth_quantile;
        
    };
    
}


#endif /* EmpiricalQuantileFunction_hpp */

