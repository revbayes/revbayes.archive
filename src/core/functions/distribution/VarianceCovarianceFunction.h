/**
 * @file
 * This file contains the declaration of the GTR rate matrix function class.
 * This class is derived from the function class and is used to
 * compute the rate matrix of a general time reversible (GTR) Markov chain.
 *
 * @brief Declaration of the GTR rate matrix function.
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



#ifndef VarianceCovarianceFunction_H
#define VarianceCovarianceFunction_H

#include "RbVector.h"
#include "MatrixRealSymmetric.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class VarianceCovarianceFunction : public TypedFunction<MatrixRealSymmetric> {
        
    public:
        VarianceCovarianceFunction(const TypedDagNode< RbVector<double> > *er, const TypedDagNode< RbVector<double> > *bf);
        virtual                                            ~VarianceCovarianceFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        VarianceCovarianceFunction*                         clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        
        const TypedDagNode< RbVector<double> >*             standardDeviations;
        const TypedDagNode< RbVector<double> >*             correlationCoefficients;
        
    };
    
}

#endif
