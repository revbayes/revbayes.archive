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



#ifndef PomoRateMatrixFunction_H
#define PomoRateMatrixFunction_H

#include "RateMatrix_Pomo.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class PomoRateMatrixFunction : public TypedFunction<RateMatrix> {

    public:
        PomoRateMatrixFunction(const TypedDagNode< int > *ps, const TypedDagNode< RbVector<double> > *mr, const TypedDagNode< RbVector<double>  > *sc);
        PomoRateMatrixFunction(const TypedDagNode< int > *ps, const TypedDagNode< RateMatrix > *mm, const TypedDagNode< RbVector<double>  > *sc);

        virtual                                            ~PomoRateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        PomoRateMatrixFunction*                             clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members

        const TypedDagNode< int >*                          populationSize;
        const TypedDagNode< RbVector<double> >*             mutationRates;
        const TypedDagNode< RateMatrix >*                   mutationMatrix;
        const TypedDagNode< RbVector<double> >*             selectionCoefficients;
        bool                                                useMutationMatrix;
        
        // private member functions
        std::vector<double> setMutationRates(const RateMatrix& mm);

    };
    
}

#endif
