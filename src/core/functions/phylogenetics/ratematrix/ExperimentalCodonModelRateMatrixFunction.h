#ifndef ExperimentalCodonModelRateMatrixFunction_H
#define ExperimentalCodonModelRateMatrixFunction_H

#include "RateMatrix_ExperimentalCodonModel.h"
#include "RbVector.h"
#include "Simplex.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {

    /**
     * @brief HKY rate matrix function.
     *
     * This function creates the HKY rates matrix object by setting the transition-transversion parameter kappa
     * and the base frequencies. The rate matrix takes care of the setting of the actual rates and transition probabilities.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0.7, 2017-10-16
     *
     */
    class ExperimentalCodonModelRateMatrixFunction : public TypedFunction<RateGenerator> {

    public:
        ExperimentalCodonModelRateMatrixFunction(const TypedDagNode<double> *b, const TypedDagNode<double> *o, const TypedDagNode<double> *k, const TypedDagNode< Simplex > *aap, const TypedDagNode< Simplex > *bf);
        virtual                                            ~ExperimentalCodonModelRateMatrixFunction(void);                                                    //!< Virtual destructor

        // public member functions
        ExperimentalCodonModelRateMatrixFunction*      clone(void) const;                                                              //!< Create an independent clone
        void                                                    update(void);

    protected:
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters

    private:

        // members
        const TypedDagNode< Simplex >*                          amino_acid_preferences;
        const TypedDagNode< Simplex >*                          base_frequencies;
        const TypedDagNode<double>*                             beta;
        const TypedDagNode<double>*                             kappa;
        const TypedDagNode<double>*                             omega;

    };

}

#endif
