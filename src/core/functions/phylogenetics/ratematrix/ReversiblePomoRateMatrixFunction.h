/**
 * @file
 * This file contains the declaration of the Pomo rate matrix function class.
 * This class is derived from the function class and is used to
 * compute the rate matrix of a general time reversible (Pomo) Markov chain.
 *
 * @brief Declaration of the Pomo rate matrix function.
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



#ifndef ReversiblePomoRateMatrixFunction_H
#define ReversiblePomoRateMatrixFunction_H

#include "RateMatrix_ReversiblePomo.h"
#include "RbVector.h"
#include "MatrixReal.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {

    class ReversiblePomoRateMatrixFunction : public TypedFunction<RateGenerator> {

    public:
        // ReversiblePomoRateMatrixFunction(const TypedDagNode< int > *ps, const TypedDagNode< RateGenerator > *mm );
        ReversiblePomoRateMatrixFunction(const TypedDagNode< int > *ps, const TypedDagNode< RbVector<double> > *rho, const TypedDagNode< Simplex > *pi  );

        virtual                                             ~ReversiblePomoRateMatrixFunction(void);                                                    //!< Virtual destructor

        // public member functions
        ReversiblePomoRateMatrixFunction*                   clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);

    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters

    private:

        // members

        const TypedDagNode< int >*                          populationSize;
        // const TypedDagNode< RateGenerator >*                mutationMatrix;
        const TypedDagNode< RbVector<double> >*                   exchangeabilities;
        const TypedDagNode< Simplex >*                   equilibriumFrequencies;


        // private member functions

    };

}

#endif
