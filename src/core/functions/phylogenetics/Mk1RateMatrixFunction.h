/**
 * @file
 * This file contains the declaration of the Mk1RateMatrixFunction class.
 * This class is derived from the function class and is used to
 * create the mk1 rate matrix.
 *
 * @brief Declaration of the Mk1RateMatrixFunction.
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 */



#ifndef Mk1RateMatrixFunction_H
#define Mk1RateMatrixFunction_H

#include "RateMatrix_Mk1.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class Mk1RateMatrixFunction : public TypedFunction<RateMatrix> {
		
    public:
        Mk1RateMatrixFunction(const TypedDagNode<int> *n, const TypedDagNode<double> *l);
        
        virtual                                            ~Mk1RateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        Mk1RateMatrixFunction*                              clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
		
        const TypedDagNode<int>*                    n;
        const TypedDagNode<double>*                 lambda;

		
    };
    
}

#endif
