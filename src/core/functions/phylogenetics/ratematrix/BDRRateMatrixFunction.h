/**
 * @file
 * This file contains the declaration of the BDRRateMatrixFunction class.
 * This class is derived from the function class and is used to
 * create the chromosomes rate matrix.
 *
 * @brief Declaration of the BDRRateMatrixFunction.
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 */


#ifndef BDRRateMatrixFunction_H
#define BDRRateMatrixFunction_H

#include "RateMatrix_Chromosomes.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class BDRRateMatrixFunction : public TypedFunction<RateGenerator> {

    public:
        BDRRateMatrixFunction(const TypedDagNode<int> *n, const TypedDagNode<double> *a, const TypedDagNode<double> *b, const TypedDagNode<double> *l_a, const TypedDagNode<double> *m_a, const TypedDagNode<double> *l_i, const TypedDagNode<double> *m_i, const TypedDagNode<double> *l_ia, const TypedDagNode<double> *l_ai, const TypedDagNode<double> *d_i, const TypedDagNode<double> *d_a );
        
        virtual                                     ~BDRRateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        BDRRateMatrixFunction*                      clone(void) const;                                                              //!< Create an independent clone
        void                                        update(void);
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members

        const TypedDagNode<int>*                    n;
        const TypedDagNode<double>*                 alpha;
        const TypedDagNode<double>*                 beta;
        const TypedDagNode<double>*                 lambda_a;
        const TypedDagNode<double>*                 mu_a;
        const TypedDagNode<double>*                 lambda_i;
        const TypedDagNode<double>*                 mu_i;
        const TypedDagNode<double>*                 lambda_ai;
        const TypedDagNode<double>*                 lambda_ia;
        const TypedDagNode<double>*                 denovo_i;
        const TypedDagNode<double>*                 denovo_a;


    };
    
}

#endif
