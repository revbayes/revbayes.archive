/**
 * @file
 * This file contains the declaration of the F81 rate matrix function class.
 * This class is derived from the function class and is used to
 * compute the rate matrix of a Felsenstein (F81) Markov chain.
 *
 * @brief Declaration of the F81 rate matrix function.
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



#ifndef F81RateMatrixFunction_H
#define F81RateMatrixFunction_H

#include "RateMatrix_F81.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class F81RateMatrixFunction : public TypedFunction<RateMatrix> {
        
    public:
        F81RateMatrixFunction(const TypedDagNode< RbVector<double> > *bf);
        virtual                                            ~F81RateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        F81RateMatrixFunction*                              clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             baseFrequencies;
        
    };
    
}

#endif
