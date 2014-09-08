/**
 * @file
 * This file contains the declaration of the chromosomes rate matrix function class.
 * This class is derived from the function class and is used to
 * compute the rate matrix of a general time reversible (chromosomes) Markov chain.
 *
 * @brief Declaration of the chromosomes rate matrix function.
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



#ifndef ChromosomesRateMatrixFunction_H
#define ChromosomesRateMatrixFunction_H

#include "RateMatrix_Chromosomes.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class ChromosomesRateMatrixFunction : public TypedFunction<RateMatrix> {

    public:
        ChromosomesRateMatrixFunction(const TypedDagNode<int> *n, const TypedDagNode<double> *l, const TypedDagNode<double> *d, const TypedDagNode<double> *r);
        
        virtual                                            ~ChromosomesRateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        ChromosomesRateMatrixFunction*                              clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members

        const TypedDagNode<int>*                    n;
        const TypedDagNode<double>*                 lambda;
        const TypedDagNode<double>*                 delta;
        const TypedDagNode<double>*                 rho;

    };
    
}

#endif
