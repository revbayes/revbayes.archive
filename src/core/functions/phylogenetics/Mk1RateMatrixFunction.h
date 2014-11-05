/**
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-06, version 1.0
 * @interface Function
 *
 * $Id$
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
