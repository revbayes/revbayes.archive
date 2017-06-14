#ifndef Kimura81RateMatrixFunction_H
#define Kimura81RateMatrixFunction_H

#include "RateMatrix_Kimura81.h"
#include "RbVector.h"
<<<<<<< HEAD
=======
#include "Simplex.h"
>>>>>>> development
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief Kimura81 rate matrix function.
     *
     * This function creates the Kimura81 rates matrix object by setting the exchangeability rates
     * and the base frequencies. The rate matrix takes care of the setting of the actual rates and transition probabilities.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-07-04
     *
     */
    class Kimura81RateMatrixFunction : public TypedFunction<RateGenerator> {
        
    public:
<<<<<<< HEAD
        Kimura81RateMatrixFunction(const TypedDagNode<double> *k1, const TypedDagNode<double> *k2, const TypedDagNode< RbVector<double> > *bf);
=======
        Kimura81RateMatrixFunction(const TypedDagNode<double> *k1, const TypedDagNode<double> *k2, const TypedDagNode< Simplex > *bf);
>>>>>>> development
        virtual                                            ~Kimura81RateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        Kimura81RateMatrixFunction*                         clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        
        const TypedDagNode<double>*                         kappa_1;
        const TypedDagNode<double>*                         kappa_2;
<<<<<<< HEAD
        const TypedDagNode< RbVector<double> >*             base_frequencies;
=======
        const TypedDagNode< Simplex >*                      base_frequencies;
>>>>>>> development
        
    };
    
}

#endif
